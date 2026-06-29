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

#ifndef XSIMD_CPU_FEATURES_HPP
#define XSIMD_CPU_FEATURES_HPP

#include "./xsimd_cpu_features_arm.hpp"

namespace xsimd
{
    /**
     * Cross-platform CPU feature detection class.
     * Specialized for Apple Silicon (ARM-only).
     */
    class cpu_features : public arm_cpu_features
    {
    public:
        // Mock obsolete architectures features to false
        constexpr bool sse2() const noexcept { return false; }
        constexpr bool sse3() const noexcept { return false; }
        constexpr bool ssse3() const noexcept { return false; }
        constexpr bool sse4_1() const noexcept { return false; }
        constexpr bool sse4_2() const noexcept { return false; }
        constexpr bool fma3() const noexcept { return false; }
        constexpr bool fma4() const noexcept { return false; }
        constexpr bool avx() const noexcept { return false; }
        constexpr bool avx2() const noexcept { return false; }
        constexpr bool avxvnni() const noexcept { return false; }
        constexpr bool avx512f() const noexcept { return false; }
        constexpr bool avx512cd() const noexcept { return false; }
        constexpr bool avx512vl() const noexcept { return false; }
        constexpr bool avx512dq() const noexcept { return false; }
        constexpr bool avx512bw() const noexcept { return false; }
        constexpr bool avx512er() const noexcept { return false; }
        constexpr bool avx512pf() const noexcept { return false; }
        constexpr bool avx512ifma() const noexcept { return false; }
        constexpr bool avx512vbmi() const noexcept { return false; }
        constexpr bool avx512vbmi2() const noexcept { return false; }
        constexpr bool avx512vnni_bw() const noexcept { return false; }
        constexpr bool sve() const noexcept { return false; }
        constexpr bool rvv() const noexcept { return false; }
        constexpr bool wasm() const noexcept { return false; }
        constexpr bool vsx() const noexcept { return false; }
        constexpr bool vxe() const noexcept { return false; }
        constexpr std::size_t rvv_size_bytes() const noexcept { return 0; }
        constexpr std::size_t sve_size_bytes() const noexcept { return 0; }
    };
}

#endif
