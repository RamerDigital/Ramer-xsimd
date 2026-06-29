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

#ifndef XSIMD_ALL_REGISTERS_HPP
#define XSIMD_ALL_REGISTERS_HPP

#include "./xsimd_i8mm_neon64_register.hpp"
#include "./xsimd_neon64_register.hpp"
#include "./xsimd_neon_register.hpp"

#if XSIMD_WITH_EMULATED
#include "./xsimd_emulated_register.hpp"
#endif

#endif
