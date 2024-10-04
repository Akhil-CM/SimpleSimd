// -*- C++ Header -*-
/*
==================================================
Authors: A.Mithran;
Emails: mithran@fias.uni-frankfurt.de
==================================================
*/

#ifndef SIMD_SSE_MASK32_H
#define SIMD_SSE_MASK32_H

#include "../Utils/macros.h"
#include "../Utils/tag.h"

#include <iostream>
#include <x86intrin.h>
#include <cmath>
#include <string>
#include <cassert>

namespace KFP {
namespace SIMD {

class Mask32_128
{
public:
    typedef bool value_type;
    typedef __m128 simd_type;
    static constexpr Tag tag{ Tag::SSE };
    static constexpr int SimdSize{ sizeof(simd_type) };
    static constexpr int SimdLen{ SimdSize / sizeof(int) };

protected:
    simd_type data_;
};

} // namespace SIMD
} // namespace KFP

#endif // !SIMD_SSE_MASK32_H
