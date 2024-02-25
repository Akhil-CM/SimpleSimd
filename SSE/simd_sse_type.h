// -*- C++ Header -*-
/*
==================================================
Authors: A.Mithran;
Emails: mithran@fias.uni-frankfurt.de
==================================================
*/

#ifndef SIMD_SSE_TYPE_H
#define SIMD_SSE_TYPE_H

#include "../Base/simd_data.h"
#include "../Base/simd_mask.h"
#include "../Base/simd_index.h"
#include "../Base/simd_class.h"

#include <type_traits>

namespace KFP {
namespace SIMD {

using SimdDataI = SimdData<int, Tag::SSE>::simd_type;
using SimdDataF = SimdData<float, Tag::SSE>::simd_type;

using ValueDataI = int;
using ValueDataF = float;

using SimdMask = SimdMaskBase<Tag::SSE>;
using SimdIndex = SimdIndexBase<Tag::SSE>;

using SimdF = SimdBaseClass<float, Tag::SSE>;
static_assert(std::is_same<SimdDataF, __m128>::value,
              "[Error]: Invalid simd type for SSE float SimdClass.");
static_assert(std::is_same<ValueDataF, float>::value,
              "[Error]: Invalid value type for SSE float SimdClass.");

using SimdI = SimdBaseClass<int, Tag::SSE>;
static_assert(std::is_same<SimdDataI, __m128i>::value,
              "[Error]: Invalid simd type for SSE int SimdClass.");
static_assert(std::is_same<ValueDataI, int>::value,
              "[Error]: Invalid value type for SSE int SimdClass.");

} // namespace SIMD
} // namespace KFP

#endif
