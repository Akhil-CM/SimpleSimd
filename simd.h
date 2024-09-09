// -*- C++ Header -*-
/*
==================================================
Authors: A.Mithran;
Emails: mithran@fias.uni-frankfurt.de
==================================================
*/

#ifndef KFP_SIMD_H
#define KFP_SIMD_H

// Determine instruction set, and define platform-dependent functions
#include "Utils/macros.h"
#include "Utils/memory.h"

// Select appropriate header files depending on instruction set
#if defined(__KFP_SIMD__AVX)
#include "AVX/types.h"
#elif defined(__KFP_SIMD__SSE)
#include "SSE/types.h"
#else
#error "[Error] (simd.h): KFParticle SIMD Scalar not implemented."
#include "Scalar/types.h"
#endif

static_assert(
    (KFP::SIMD::simd_float::SimdSize == __KFP_SIMD__Size_Float),
    "[Error]: KFP::SIMD::simd_float given invalid size of simd type.");
static_assert(
    (KFP::SIMD::simd_float::SimdLen == __KFP_SIMD__Len_Float),
    "[Error]: KFP::SIMD::simd_float given invalid length of simd type.");

static_assert(
    (KFP::SIMD::simd_int::SimdSize == __KFP_SIMD__Size_Int),
    "[Error]: KFP::SIMD::simd_int given invalid size of simd type.");
static_assert(
    (KFP::SIMD::simd_int::SimdLen == __KFP_SIMD__Len_Int),
    "[Error]: KFP::SIMD::simd_int given invalid length of simd type.");

#endif // !KFP_SIMD_H
