// -*- C++ Header -*-
/*
==================================================
Authors: A.Mithran;
Emails: mithran@fias.uni-frankfurt.de
==================================================
*/

#ifndef SIMD_SSE_FLOAT32_H
#define SIMD_SSE_FLOAT32_H

#include "../Utils/macros.h"
#include "../Utils/tag.h"

#include <iostream>
#include <x86intrin.h>
#include <cmath>
#include <string>
#include <cassert>

namespace KFP {
namespace SIMD {

__KFP_SIMD__INLINE __m128 select(const __m128& mask, const __m128& a,
                                    const __m128& b);

class Float32_128
{
public:
    typedef float value_type;
    typedef __m128 simd_type;
    static constexpr Tag tag{ Tag::SSE };
    static constexpr int SimdSize{ sizeof(simd_type) };
    static constexpr int SimdLen{ SimdSize / sizeof(value_type) };

protected:
    simd_type data_;
};

} // namespace SIMD
} // namespace KFP

#endif // !SIMD_SSE_FLOAT32_H
