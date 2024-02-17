// -*- C++ Header -*-
/*
==================================================
Authors: A.Mithran;
Emails: mithran@fias.uni-frankfurt.de
==================================================
*/

#pragma once

#include "simd_macros.hpp"

namespace KFP {
namespace SIMD {

enum class ABI
{
    /// uses only fundamental types
    Scalar,
    // Entry to SSE versions
    SSE,
    /// x86 SSE + SSE2
    SSE2,
    /// x86 SSE + SSE2 + SSE3
    SSE3,
    /// x86 SSE + SSE2 + SSE3 + SSSE3
    SSSE3,
    /// x86 SSE + SSE2 + SSE3 + SSSE3 + SSE4.1
    SSE41,
    /// x86 SSE + SSE2 + SSE3 + SSSE3 + SSE4.1 + SSE4.2
    SSE42,
    /// x86 AVX
    AVX,
    /// x86 AVX + AVX2
    AVX2,
};

constexpr inline bool validABI(ABI abi)
{
    switch (abi) {
    case ABI::Scalar:
        return true;
    case ABI::SSE:
        return true;
    case ABI::SSE2:
        return true;
    case ABI::SSE3:
        return true;
    case ABI::SSSE3:
        return true;
    case ABI::SSE41:
        return true;
    case ABI::SSE42:
        return true;
    case ABI::AVX:
        return true;
    case ABI::AVX2:
        return true;
    default:
        return false;
    };
}

constexpr inline const char* getABI()
{
#if defined(__KFP_SIMD__AVX2)
    return "AVX2";
#elif defined(__KFP_SIMD__AVX)
    return "AVX";
#elif defined(__KFP_SIMD__SSE)
    #if defined(__KFP_SIMD__SSE4_2)
        return "SSE4.2";
    #elif defined(__KFP_SIMD__SSE4_1)
        return "SSE4.1";
    #elif defined(__KFP_SIMD__SSSE3)
        return "SSSE3";
    #elif defined(__KFP_SIMD__SSE3)
        return "SSE3";
    #elif defined(__KFP_SIMD__SSE2)
        return "SSE2";
    #endif
#elif defined(__KFP_SIMD__Scalar)
        return "Scalar";
#else
    return nullptr;
#endif
}

} // namespace SIMD
} // namespace KFP
