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

#include "xsimd/xsimd.hpp"

#include <doctest/doctest.h>

#include <algorithm>
#include <array>
#include <cstdlib>
#include <string>

#define CHECK_IMPLICATION(a, b) CHECK_UNARY(!(a) || (b))

namespace detail
{
    void check_env_flag(const char* env_var, const char* feature_name, bool actual)
    {
        if (const char* val = std::getenv(env_var))
        {
            auto msg = std::string(env_var) + " = " + val + ", " + feature_name + " = " + (actual ? "true" : "false");
            INFO(msg);
            CHECK_EQ(actual, val[0] == '1');
        }
    }
}

#define CHECK_ENV_FEATURE(env_var, feature) detail::check_env_flag(env_var, #feature, feature)

TEST_CASE("[cpu_features] arm implication chains")
{
    xsimd::arm_cpu_features cpu;

    CHECK_IMPLICATION(cpu.neon64(), cpu.neon());
    CHECK_IMPLICATION(cpu.sve(), cpu.neon64());
    CHECK_IMPLICATION(cpu.sve(), cpu.sve_size_bytes() >= (128 / 8));
    CHECK_IMPLICATION(cpu.i8mm(), cpu.neon64());
}

TEST_CASE("[cpu_features] arm features from environment")
{
    xsimd::cpu_features cpu;

    CHECK_ENV_FEATURE("XSIMD_TEST_CPU_ASSUME_NEON", cpu.neon());
    CHECK_ENV_FEATURE("XSIMD_TEST_CPU_ASSUME_NEON64", cpu.neon64());
    CHECK_ENV_FEATURE("XSIMD_TEST_CPU_ASSUME_SVE", cpu.sve());
    CHECK_ENV_FEATURE("XSIMD_TEST_CPU_ASSUME_I8MM", cpu.i8mm());
}
