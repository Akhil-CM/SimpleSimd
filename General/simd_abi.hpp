// -*- C++ Header -*-
/*
==================================================
Authors: A.Mithran;
Emails: mithran@fias.uni-frankfurt.de
==================================================
*/

#pragma once

namespace KFP
{
namespace SIMD
{

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

} // namespace SIMD
} // namespace KFP
