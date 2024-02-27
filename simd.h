// -*- C++ Header -*-
/*
==================================================
Authors: A.Mithran;
Emails: mithran@fias.uni-frankfurt.de
==================================================
*/

#ifndef SIMD_SIMD_H
#define SIMD_SIMD_H

// Determine instruction set, and define platform-dependent functions
#include "simd_macros.h"

// Select appropriate header files depending on instruction set
#if defined(__KFP_SIMD__AVX)
#include "simd_avx.h"
#elif defined(__KFP_SIMD__SSE)
#include "simd_sse.h"
#else
#include "simd_scalar.h"
#endif

static_assert(
    (KFP::SIMD::SimdF::SimdSize == __KFP_SIMD__Size_Float),
    "[Error]: KFP::SIMD::SimdF given invalid size of simd type.");
static_assert(
    (KFP::SIMD::SimdF::SimdLen == __KFP_SIMD__Len_Float),
    "[Error]: KFP::SIMD::SimdF given invalid length of simd type.");

static_assert(
    (KFP::SIMD::SimdI::SimdSize == __KFP_SIMD__Size_Int),
    "[Error]: KFP::SIMD::SimdI given invalid size of simd type.");
static_assert(
    (KFP::SIMD::SimdI::SimdLen == __KFP_SIMD__Len_Int),
    "[Error]: KFP::SIMD::SimdI given invalid length of simd type.");

#endif
