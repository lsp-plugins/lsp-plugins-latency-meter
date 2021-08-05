/*
 * Copyright (C) 2021 Linux Studio Plugins Project <https://lsp-plug.in/>
 *           (C) 2021 Vladimir Sadovnikov <sadko4u@gmail.com>
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

#ifndef PRIVATE_PLUGINS_LATENCY_METER_H_
#define PRIVATE_PLUGINS_LATENCY_METER_H_

#include <lsp-plug.in/plug-fw/plug.h>
#include <lsp-plug.in/dsp-units/ctl/Bypass.h>
#include <lsp-plug.in/dsp-units/util/LatencyDetector.h>
#include <private/meta/latency_meter.h>

namespace lsp
{
    namespace plugins
    {
        /**
         * Latency Meter Plugin Series
         */
        class latency_meter: public plug::Module
        {
            protected:
                dspu::LatencyDetector   sLatencyDetector;
                dspu::Bypass            sBypass;
                bool                    bBypass;
                bool                    bTrigger;
                bool                    bFeedback;
                float                   fInGain;
                float                   fOutGain;

                float                  *vBuffer;
                uint8_t                *pData;

                plug::IPort            *pIn;
                plug::IPort            *pOut;
                plug::IPort            *pBypass;
                plug::IPort            *pMaxLatency;
                plug::IPort            *pPeakThreshold;
                plug::IPort            *pAbsThreshold;
                plug::IPort            *pInputGain;
                plug::IPort            *pFeedback;
                plug::IPort            *pOutputGain;
                plug::IPort            *pTrigger;
                plug::IPort            *pLatencyScreen;
                plug::IPort            *pLevel;

            public:
                explicit latency_meter(const meta::plugin_t *metadata);
                virtual ~latency_meter();

                virtual void        init(plug::IWrapper *wrapper);
                virtual void        destroy();

            public:
                virtual void        process(size_t samples);
                virtual void        update_settings();
                virtual void        update_sample_rate(long sr);
                virtual void        dump(dspu::IStateDumper *v) const;
        };
    } // namespace plugins
} // namespace lsp

#endif /* PRIVATE_PLUGINS_LATENCY_METER_H_ */
