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

#ifndef PRIVATE_META_LATENCY_METER_H_
#define PRIVATE_META_LATENCY_METER_H_

#include <lsp-plug.in/plug-fw/meta/types.h>
#include <lsp-plug.in/plug-fw/const.h>


namespace lsp
{
    namespace meta
    {
        struct latency_meter_metadata
        {
            static constexpr float LATENCY_MIN          = 0.0f;       /* Min detectable latency [ms] */
            static constexpr float LATENCY_MAX          = 2000.0f;    /* Max detectable latency [ms] */
            static constexpr float LATENCY_DFL          = 1000.0f;
            static constexpr float LATENCY_STEP         = 1.0f;

            static constexpr float MTR_LATENCY_MIN      = 0.0f;       /* Min detectable latency [ms] */
            static constexpr float MTR_LATENCY_MAX      = 2000.0f;    /* Max detectable latency [ms] */
            static constexpr float MTR_LATENCY_DFL      = 0.0f;
            static constexpr float MTR_LATENCY_STEP     = 1.0f;

            static constexpr float PEAK_THRESHOLD_MIN   = GAIN_AMP_M_84_DB;
            static constexpr float PEAK_THRESHOLD_MAX   = GAIN_AMP_0_DB;
            static constexpr float PEAK_THRESHOLD_DFL   = GAIN_AMP_M_24_DB;
            static constexpr float PEAK_THRESHOLD_STEP  = 0.01f;

            static constexpr float ABS_THRESHOLD_MIN    = GAIN_AMP_M_84_DB;
            static constexpr float ABS_THRESHOLD_MAX    = GAIN_AMP_0_DB;
            static constexpr float ABS_THRESHOLD_DFL    = GAIN_AMP_M_24_DB;
            static constexpr float ABS_THRESHOLD_STEP   = 0.01f;
        };

        extern const meta::plugin_t latency_meter;
    } // namespace meta
} // namespace lsp


#endif /* PRIVATE_META_LATENCY_METER_H_ */
