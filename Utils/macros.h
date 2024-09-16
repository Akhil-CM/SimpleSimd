// -*- C++ Header -*-
/*
==================================================
Authors: A.Mithran, P. Kisel, I. Kisel
Emails: mithran@fias.uni-frankfurt.de
==================================================
*/

#ifndef SIMD_MACROS_H
#define SIMD_MACROS_H

#include "simd_detect.h"

#define KFP_SIMD__INLINE inline __attribute__((always_inline))
#define KFP_SIMD__INLINE_FORCE inline __attribute__((always_inline))
#define KFP_SIMD__ATTR_ALIGN(x) __attribute__((aligned(x)))
#define KFP_SIMD__SPEC_ALIGN(x) alignas(x)

#define KFP_SIMD__AVX_Size_Int 32
#define KFP_SIMD__AVX_Len_Int 8
#define KFP_SIMD__AVX_Size_Float 32
#define KFP_SIMD__AVX_Len_Float 8

#define KFP_SIMD__SSE_Size_Int 16
#define KFP_SIMD__SSE_Len_Int 4
#define KFP_SIMD__SSE_Size_Float 16
#define KFP_SIMD__SSE_Len_Float 4

#define KFP_SIMD__Scalar_Size_Int 4
#define KFP_SIMD__Scalar_Len_Int 1
#define KFP_SIMD__Scalar_Size_Float 4
#define KFP_SIMD__Scalar_Len_Float 1

#if defined(KFP_SIMD__AVX)
#define KFP_SIMD__Size_Int KFP_SIMD__AVX_Size_Int
#define KFP_SIMD__Len_Int KFP_SIMD__AVX_Len_Int
#define KFP_SIMD__Size_Float KFP_SIMD__AVX_Size_Float
#define KFP_SIMD__Len_Float KFP_SIMD__AVX_Len_Float
#elif defined(KFP_SIMD__SSE)
#define KFP_SIMD__Size_Int KFP_SIMD__SSE_Size_Int
#define KFP_SIMD__Len_Int KFP_SIMD__SSE_Len_Int
#define KFP_SIMD__Size_Float KFP_SIMD__SSE_Size_Float
#define KFP_SIMD__Len_Float KFP_SIMD__SSE_Len_Float
#elif defined(KFP_SIMD__Scalar)
#define KFP_SIMD__Size_Int KFP_SIMD__Scalar_Size_Int
#define KFP_SIMD__Len_Int KFP_SIMD__Scalar_Len_Int
#define KFP_SIMD__Size_Float KFP_SIMD__Scalar_Size_Float
#define KFP_SIMD__Len_Float KFP_SIMD__Scalar_Len_Float
#else
#error \
    "[Error] (simd_macros.hpp): Invalid KFParticle SIMD implementation value was selected."
#endif

#endif // !SIMD_MACROS_H
