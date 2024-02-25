// -*- C++ Header -*-
/*
==================================================
Authors: A.Mithran;
Emails: mithran@fias.uni-frankfurt.de
==================================================
*/

#pragma once

// Determine instruction set, and define platform-dependent functions
#include "Base/simd_macros.hpp"

// Select appropriate header files depending on instruction set
#if defined(__KFP_SIMD__AVX)
#include "AVX/simd_avx.hpp"
#elif defined(__KFP_SIMD__SSE)
#include "SSE/simd_sse.hpp"
#else
#include "Scalar/simd_scalar.hpp"
#endif
