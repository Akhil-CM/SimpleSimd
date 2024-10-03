// -*- C++ Header -*-
/*
==================================================
Authors: A.Mithran;
Emails: mithran@fias.uni-frankfurt.de
==================================================
*/

#ifndef SIMD_SSE_TYPE_H
#define SIMD_SSE_TYPE_H

#include "mask32.h"
#include "int32.h"
#include "float32.h"
#include <type_traits>

namespace KFP {
namespace SIMD {

using simd_mask = Mask32_128;

using simd_float = Float32_128;
static_assert(std::is_same<simd_float::value_type, float>::value,
              "[Error]: Invalid value type for SSE float SimdClass.");

using simd_int = Int32_128;
static_assert(std::is_same<simd_int::value_type, int>::value,
              "[Error]: Invalid value type for SSE int SimdClass.");

template<typename To, typename From,
typename std::enable_if<!std::is_same<To, From>::value>::type* = nullptr>
To type_cast(const From&)
{
    throw std::runtime_error("[Error] (KFP::SIMD::type_cast): Invalid types given");
}
template<typename To, typename From,
typename std::enable_if<std::is_same<To, From>::value>::type* = nullptr>
To type_cast(const From& a)
{
    return a;
}
template<>
Float32_128 type_cast<Float32_128, Int32_128>(const Int32_128& a)
{
    return _mm_castsi128_ps(a.simd());
}
template<>
Int32_128 type_cast<Int32_128, Float32_128>(const Float32_128& a)
{
    return _mm_castps_si128(a.simd());
}

template<typename To, typename From,
typename std::enable_if<!std::is_same<To, From>::value>::type* = nullptr>
To value_cast(const From&)
{
    throw std::runtime_error("[Error] (KFP::SIMD::value_cast): Invalid types given");
}
template<typename To, typename From,
typename std::enable_if<std::is_same<To, From>::value>::type* = nullptr>
To value_cast(const From& a)
{
    return a;
}
template<>
Float32_128 value_cast<Float32_128, Int32_128>(const Int32_128& a)
{
    return _mm_cvtepi32_ps(a.simd());
}
template<>
Int32_128 value_cast<Int32_128, Float32_128>(const Float32_128& a)
{
    return _mm_cvttps_epi32(a.simd());
}

} // namespace SIMD
} // namespace KFP

#endif // !SIMD_SSE_TYPE_H
