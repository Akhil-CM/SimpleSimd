// -*- C++ Header -*-
/*
==================================================
Authors: A.Mithran;
Emails: mithran@fias.uni-frankfurt.de
==================================================
*/

#pragma once

#include "simd_detect.hpp"

#if defined(__KFP_SIMD__SSE)
#include "../SSE/simd_sse_index.hpp"
#elif defined(__KFP_SIMD__AVX)
#include "../AVX/simd_avx_index.hpp"
#else
#endif
