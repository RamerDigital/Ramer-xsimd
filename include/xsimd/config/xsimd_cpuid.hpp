/***************************************************************************
 * Copyright (c) Johan Mabille, Sylvain Corlay, Wolf Vollprecht and         *
 * Martin Renou                                                             *
 * Copyright (c) QuantStack                                                 *
 * Copyright (c) Serge Guelton                                              *
 *                                                                          *
 * Distributed under the terms of the BSD 3-Clause License.                 *
 *                                                                          *
 * The full license is in the file LICENSE, distributed with this software. *
 ****************************************************************************/

#ifndef XSIMD_CPUID_HPP
#define XSIMD_CPUID_HPP

#include "../types/xsimd_all_registers.hpp"
#include "./xsimd_cpu_features.hpp"
#include "./xsimd_macros.hpp"

namespace xsimd
{
    namespace detail
    {
        struct supported_arch
        {
            // ARM-only features for Apple Silicon
            unsigned neon = 1;
            unsigned neon64 = 1;
            unsigned i8mm_neon64 = 0;

            XSIMD_INLINE bool has(::xsimd::neon) const { return this->neon; }
            XSIMD_INLINE bool has(::xsimd::neon64) const { return this->neon64; }
            XSIMD_INLINE bool has(::xsimd::i8mm<::xsimd::neon64>) const { return this->i8mm_neon64; }

            // Obsolete architectures mapped to false
            template <typename Arch>
            XSIMD_INLINE bool has(Arch) const noexcept { return false; }

            XSIMD_INLINE supported_arch() noexcept
            {
                const auto cpu = xsimd::cpu_features();
                neon = cpu.neon();
                neon64 = cpu.neon64();
#if defined(__ARM_FEATURE_MATMUL_INT8)
                i8mm_neon64 = cpu.neon64() && cpu.i8mm();
#else
                i8mm_neon64 = 0;
#endif
            }
        };
    } // namespace detail

    XSIMD_INLINE detail::supported_arch available_architectures() noexcept
    {
        static detail::supported_arch supported;
        return supported;
    }
}

#endif
