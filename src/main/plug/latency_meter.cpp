/*
 * Copyright (C) 2023 Linux Studio Plugins Project <https://lsp-plug.in/>
 *           (C) 2023 Vladimir Sadovnikov <sadko4u@gmail.com>
 *
 * This file is part of lsp-plugins-latency-meter
 * Created on: 3 авг. 2021 г.
 *
 * lsp-plugins-latency-meter is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * lsp-plugins-latency-meter is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with lsp-plugins-latency-meter. If not, see <https://www.gnu.org/licenses/>.
 */

#include <private/plugins/latency_meter.h>
#include <lsp-plug.in/common/alloc.h>
#include <lsp-plug.in/common/debug.h>
#include <lsp-plug.in/dsp/dsp.h>

#define TMP_BUF_SIZE        1024U

namespace lsp
{
    namespace plugins
    {
        static plug::IPort *TRACE_PORT(plug::IPort *p)
        {
            lsp_trace("  port id=%s", (p)->metadata()->id);
            return p;
        }

        //---------------------------------------------------------------------
        // Plugin factory
        static const meta::plugin_t *plugins[] =
        {
            &meta::latency_meter
        };

        static plug::Module *plugin_factory(const meta::plugin_t *meta)
        {
            return new latency_meter(meta);
        }

        static plug::Factory factory(plugin_factory, plugins, 1);

        //---------------------------------------------------------------------
        latency_meter::latency_meter(const meta::plugin_t *metadata): plug::Module(metadata)
        {
            bBypass         = true;
            bTrigger        = false;
            bFeedback       = false;
            fInGain         = 0.0f;
            fOutGain        = 0.0f;

            vBuffer         = NULL;
            pData           = NULL;

            pIn             = NULL;
            pOut            = NULL;
            pBypass         = NULL;
            pMaxLatency     = NULL;
            pPeakThreshold  = NULL;
            pAbsThreshold   = NULL;
            pInputGain      = NULL;
            pOutputGain     = NULL;
            pTrigger        = NULL;
            pLatencyScreen  = NULL;
            pLevel          = NULL;
            pFeedback       = NULL;
        }

        latency_meter::~latency_meter()
        {
            do_destroy();
        }

        void latency_meter::destroy()
        {
            plug::Module::destroy();
            do_destroy();
        }

        void latency_meter::do_destroy()
        {
            if (pData != NULL)
            {
                delete [] pData;
                pData = NULL;
            }
            vBuffer     = NULL;
        }

        void latency_meter::init(plug::IWrapper *wrapper, plug::IPort **ports)
        {
            plug::Module::init(wrapper, ports);

            size_t samples  = TMP_BUF_SIZE;
            pData           = new uint8_t[samples * sizeof(float) + DEFAULT_ALIGN];

            uint8_t *ptr    = align_ptr(pData, DEFAULT_ALIGN);
            vBuffer         = reinterpret_cast<float *>(ptr);
            ptr            += TMP_BUF_SIZE * sizeof(float);

            lsp_assert(reinterpret_cast<uint8_t *>(ptr) <= &pData[samples * sizeof(float) + DEFAULT_ALIGN]);

            size_t port_id = 0;
            pIn             = TRACE_PORT(ports[port_id++]);
            pOut            = TRACE_PORT(ports[port_id++]);
            pBypass         = TRACE_PORT(ports[port_id++]);
            pMaxLatency     = TRACE_PORT(ports[port_id++]);
            pPeakThreshold  = TRACE_PORT(ports[port_id++]);
            pAbsThreshold   = TRACE_PORT(ports[port_id++]);
            pInputGain      = TRACE_PORT(ports[port_id++]);
            pFeedback       = TRACE_PORT(ports[port_id++]);
            pOutputGain     = TRACE_PORT(ports[port_id++]);
            pTrigger        = TRACE_PORT(ports[port_id++]);
            pLatencyScreen  = TRACE_PORT(ports[port_id++]);
            pLevel          = TRACE_PORT(ports[port_id++]);

            sLatencyDetector.init();

            sLatencyDetector.set_delay_ratio(0.5f);
            sLatencyDetector.set_duration(0.050f);
            sLatencyDetector.set_op_fading(0.030f);
            sLatencyDetector.set_op_pause(0.025f);
        }

        void latency_meter::update_sample_rate(long sr)
        {
            sLatencyDetector.set_sample_rate(sr);
            sBypass.init(sr);
        }

        void latency_meter::process(size_t samples)
        {
            float *in = pIn->buffer<float>();
            if (in == NULL)
                return;

            // Calculate the level
            pLevel->set_value(dsp::abs_max(in, samples));

            float *out = pOut->buffer<float>();
            if (out == NULL)
                return;

            while (samples > 0)
            {
                size_t to_do        = lsp_min(samples, TMP_BUF_SIZE);

                dsp::mul_k3(vBuffer, in, fInGain, to_do);

                sLatencyDetector.process_in(vBuffer, vBuffer, to_do);
                if (!bFeedback)
                    dsp::fill_zero(vBuffer, to_do);
                sLatencyDetector.process_out(vBuffer, vBuffer, to_do);

                dsp::mul_k2(vBuffer, fOutGain, to_do);
                sBypass.process(out, in, vBuffer, to_do);

                in                 += to_do;
                out                += to_do;
                samples            -= to_do;
            }

            if (sLatencyDetector.latency_detected())
                pLatencyScreen->set_value(sLatencyDetector.get_latency_seconds() * 1000.0f); // * 1000.0f to show ms instead of s
        }

        void latency_meter::update_settings()
        {
            bBypass = pBypass->value() >= 0.5f;
            sBypass.set_bypass(bBypass);

            bTrigger    = pTrigger->value() >= 0.5f;
            bFeedback   = pFeedback->value() >= 0.5f;

            if (bTrigger)
            {
                sLatencyDetector.start_capture();
                pLatencyScreen->set_value(0.0f); // Showing 0 if no latency was detected. Can we have something like ----.--- instead?
            }

            sLatencyDetector.set_ip_detection(pMaxLatency->value() * 0.001f);
            sLatencyDetector.set_peak_threshold(pPeakThreshold->value());
            sLatencyDetector.set_abs_threshold(pAbsThreshold->value());
            fInGain     = pInputGain->value();
            fOutGain    = pOutputGain->value();

            if (sLatencyDetector.needs_update())
                sLatencyDetector.update_settings();
        }

        void latency_meter::dump(dspu::IStateDumper *v) const
        {
            plug::Module::dump(v);

            v->write_object("sLatencyDetector", &sLatencyDetector);
            v->write_object("sBypass", &sBypass);

            v->write("bBypass", bBypass);
            v->write("bTrigger", bTrigger);
            v->write("bFeedback", bFeedback);
            v->write("fInGain", fInGain);
            v->write("fOutGain", fOutGain);

            v->write("vBuffer", vBuffer);
            v->write("pData", pData);

            v->write("pIn", pIn);
            v->write("pOut", pOut);
            v->write("pBypass", pBypass);
            v->write("pMaxLatency", pMaxLatency);
            v->write("pPeakThreshold", pPeakThreshold);
            v->write("pAbsThreshold", pAbsThreshold);
            v->write("pInputGain", pInputGain);
            v->write("pFeedback", pFeedback);
            v->write("pOutputGain", pOutputGain);
            v->write("pTrigger", pTrigger);
            v->write("pLatencyScreen", pLatencyScreen);
            v->write("pLevel", pLevel);
        }
    } /* namespace plugins */
} /* namespace lsp */


