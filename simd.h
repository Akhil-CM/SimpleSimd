// -*- C++ Header -*-
/*
==================================================
Authors: A.Mithran;
Emails: mithran@fias.uni-frankfurt.de
==================================================
*/

#pragma once

// Determine instruction set, and define platform-dependent functions
#include "Base/simd_macros.h"

// Select appropriate header files depending on instruction set
#if defined(__KFP_SIMD__AVX)
#include "AVX/simd_avx.h"
#elif defined(__KFP_SIMD__SSE)
#include "SSE/simd_sse.h"
#else
#include "Scalar/simd_scalar.h"
#endif
