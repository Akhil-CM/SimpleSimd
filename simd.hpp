// -*- C++ Header -*-
/*
==================================================
Authors: A.Mithran;
Emails: mithran@fias.uni-frankfurt.de
==================================================
*/

#pragma once

// Determine instruction set, and define platform-dependent functions
#include "General/simd_detect.hpp"

// Select appropriate header files depending on instruction set
#if defined(__KFP_SIMD__AVX)
#include "AVX/simd_avx.hpp"
#elif defined(__KFP_SIMD__SSE)
#include "SSE/simd_sse.hpp"
#else
#include "Scalar/simd_scalar.hpp"
#endif

#define ConvertToFloat(x) _mm_cvtepi32_ps(x)
#define NotEqual(x,y) _mm_cmpneq_ps(x,y)
#ifdef SSE41
#define Floor(x) _mm_floor_ps(x)
#endif
#ifndef SSE41
#define BlendV(x,y,z) Or(AndNot(z,x), And(z,y))   //if we don't have sse4
#else
#define BlendV(x,y,z) _mm_blendv_ps(x,y,z)
#endif
