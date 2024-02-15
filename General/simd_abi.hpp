// -*- C++ Header -*-
/*
==================================================
Authors: A.Mithran;
Emails: mithran@fias.uni-frankfurt.de
==================================================
*/

#pragma once

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

} // namespace SIMD
} // namespace KFP
