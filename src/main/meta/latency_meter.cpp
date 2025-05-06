/*
 * Copyright (C) 2025 Linux Studio Plugins Project <https://lsp-plug.in/>
 *           (C) 2025 Vladimir Sadovnikov <sadko4u@gmail.com>
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

#include <lsp-plug.in/plug-fw/meta/ports.h>
#include <lsp-plug.in/shared/meta/developers.h>
#include <private/meta/latency_meter.h>

#define LSP_PLUGINS_LATENCY_METER_VERSION_MAJOR       1
#define LSP_PLUGINS_LATENCY_METER_VERSION_MINOR       0
#define LSP_PLUGINS_LATENCY_METER_VERSION_MICRO       25

#define LSP_PLUGINS_LATENCY_METER_VERSION  \
    LSP_MODULE_VERSION( \
        LSP_PLUGINS_LATENCY_METER_VERSION_MAJOR, \
        LSP_PLUGINS_LATENCY_METER_VERSION_MINOR, \
        LSP_PLUGINS_LATENCY_METER_VERSION_MICRO  \
    )

namespace lsp
{
    namespace meta
    {
        static const int plugin_classes[]       = { C_ANALYSER, -1};
        static const int clap_features[]        = { CF_ANALYZER, CF_UTILITY, CF_MONO, -1 };

        const meta::bundle_t latency_meter_bundle =
        {
            "latency_meter",
            "Latency Meter",
            B_UTILITIES,
            "JFXTcJb4DLs",
            "A simple plugin that allows one to measure the latency of a transmission line\nby using chirp - inverse filter convolution. Latency is determined as the\nlag of the convolution peak."
        };

        static const port_t latency_meter_ports[] =
        {
            PORTS_MONO_PLUGIN,
            BYPASS,
            CONTROL("mlat", "Maximum Expected Latency", U_MSEC, latency_meter_metadata::LATENCY),
            CONTROL("pthr", "Peak Threshold", U_GAIN_AMP, latency_meter_metadata::PEAK_THRESHOLD),
            CONTROL("athr", "Absolute Threshold", U_GAIN_AMP, latency_meter_metadata::ABS_THRESHOLD),
            AMP_GAIN10("gin", "Input Gain", 1.0f),
            SWITCH("fback", "Feedback", "Feedback", 0.0f),
            AMP_GAIN10("gout", "Output Gain", 1.0f),
            TRIGGER("ttrig", "Triger Latency Measurement", "Measure start"),
            METER("l_v", "Latency Value", U_MSEC, latency_meter_metadata::MTR_LATENCY),
            METER_GAIN("ilvl", "Input Level", GAIN_AMP_P_24_DB),

            PORTS_END
        };

        const meta::plugin_t latency_meter =
        {
            "Latenzmessgerät",
            "Latency Meter",
            "Latency Meter",
            "LM1M",
            &developers::s_tronci,
            "latency_meter",
            {
                LSP_LV2_URI("latency_meter"),
                LSP_LV2UI_URI("latency_meter"),
                "abee",
                LSP_VST3_UID("lm1m    abee"),
                LSP_VST3UI_UID("lm1m    abee"),
                LSP_LADSPA_LATENCY_METER_BASE + 0,
                LSP_LADSPA_URI("latency_meter"),
                LSP_CLAP_URI("latency_meter"),
                LSP_GST_UID("latency_meter"),
            },
            LSP_PLUGINS_LATENCY_METER_VERSION,
            plugin_classes,
            clap_features,
            E_DUMP_STATE,
            latency_meter_ports,
            "util/latency_meter.xml",
            NULL,
            mono_plugin_port_groups,
            &latency_meter_bundle
        };
    } // namespace meta
} // namespace lsp
