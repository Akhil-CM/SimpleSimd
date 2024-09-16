// -*- C++ Header -*-
/*
==================================================
Authors: A.Mithran;
Emails: mithran@fias.uni-frankfurt.de
==================================================
*/

#ifndef SIMD_DETECT_H
#define SIMD_DETECT_H

#include <x86intrin.h>

#ifndef KFP_SIMD

#if defined(__AVX2__)
#define KFP_SIMD__AVX2 1
#elif defined(__AVX__)
#define KFP_SIMD__AVX 1
#else
    #if defined(__SSE4_2__)
    #define KFP_SIMD__SSE4_2 1
    #elif defined(__SSE4_1__)
    #define KFP_SIMD__SSE4_1 1
    #elif defined(__SSSE3__)
    #define KFP_SIMD__SSSE3 1
    #elif defined(__SSE3__)
    #define KFP_SIMD__SSE3 1
    #elif defined(__SSE2__)
    #define KFP_SIMD__SSE2 1
    #else
    #define KFP_SIMD__Scalar 1
    #endif
#endif

#else // KFP_SIMD

#if KFP_SIMD > 6 // AVX2 supersedes SSE
#define KFP_SIMD__AVX2 1
#elif KFP_SIMD > 5 // AVX supersedes SSE
#define KFP_SIMD__AVX 1
#elif KFP_SIMD > 4
#define KFP_SIMD__SSE4_2 1
#elif KFP_SIMD > 3
#define KFP_SIMD__SSE4_1 1
#elif KFP_SIMD > 2
#define KFP_SIMD__SSSE3 1
#elif KFP_SIMD > 1
#define KFP_SIMD__SSE3 1
#elif KFP_SIMD > 0
#define KFP_SIMD__SSE2 1
#else
#define KFP_SIMD__Scalar 1
#endif
#undef KFP_SIMD
#endif // KFP_SIMD

#if defined(KFP_SIMD__AVX2)
#define KFP_SIMD__AVX 1
#endif

#if defined(KFP_SIMD__AVX)
#define KFP_SIMD__SSE4_2 1
#endif

#if defined(KFP_SIMD__SSE4_2)
#define KFP_SIMD__SSE4_1 1
#endif

#if defined(KFP_SIMD__SSE4_1)
#define KFP_SIMD__SSSE3 1
#endif

#if defined(KFP_SIMD__SSSE3)
#define KFP_SIMD__SSE3 1
#endif

#if defined(KFP_SIMD__SSE3)
#define KFP_SIMD__SSE2 1
#endif

#if defined(KFP_SIMD__SSE2)
#define KFP_SIMD__SSE 1
#endif

#if !defined(KFP_SIMD__Scalar) && !defined(KFP_SIMD__SSE) && !defined(KFP_SIMD__AVX)
#error \
    "[Error] (simd_detect.hpp): Invalid KFParticle SIMD implementation value was selected."
#endif
#if defined(KFP_SIMD__SSE) && !defined(KFP_SIMD__SSE2)
#error "[Error] (simd_detect.hpp): KFParticle SIMD implementation value SSE selected without SSE2."
#endif

#endif // !SIMD_DETECT_H
