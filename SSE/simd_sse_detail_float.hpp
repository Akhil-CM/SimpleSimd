// -*- C++ Header -*-
/*
==================================================
Authors: A.Mithran;
Emails: mithran@fias.uni-frankfurt.de
==================================================
*/

#ifndef SIMD_SSE_DETAIL_FLOAT_H
#define SIMD_SSE_DETAIL_FLOAT_H

#include "../Base/simd_macros.hpp"
#include "../Base/simd_detail.hpp"
#include "simd_sse_type.hpp"

#include <x86intrin.h>
#include <iostream>
#include <cmath>

namespace KFP {
namespace SIMD {

const SimdDataF TRUE_MASK_F{ _mm_castsi128_ps(_mm_set1_epi32(-1)) } ;
const SimdDataF ABS_MASK_F{ _mm_castsi128_ps(_mm_set1_epi32(0x7FFFFFFF)) } ;
const SimdDataF MINUS_MASK_F{ _mm_castsi128_ps(_mm_set1_epi32(0x80000000)) } ;

namespace Detail {

template <>
inline SimdDataF constant<SimdDataF, ValueDataF>(ValueDataF val)
{
    return _mm_set1_ps(val);
}

template <>
inline SimdDataF load<SimdDataF, ValueDataF>(const ValueDataF* val_ptr)
{
    return _mm_loadu_ps(val_ptr);
}
template <>
inline SimdDataF load_a<SimdDataF, ValueDataF>(const ValueDataF* val_ptr)
{
    return _mm_load_ps(val_ptr);
}
template <>
inline void store<SimdDataF, ValueDataF>(const SimdDataF& val_simd,
                                         ValueDataF* val_ptr)
{
    _mm_storeu_ps(val_ptr, val_simd);
}
template <>
inline void store_a<SimdDataF, ValueDataF>(const SimdDataF& val_simd,
                                         ValueDataF* val_ptr)
{
    _mm_store_ps(val_ptr, val_simd);
}
template <int N> inline ValueDataF get(const SimdDataF& a)
{
    const SimdDataF result =
        _mm_shuffle_ps(a, a, (N&3));
    return _mm_cvtss_f32(result);
}
template <>
inline ValueDataF extract<ValueDataF, SimdDataF>(int index, const SimdDataF& a)
{
#if 0
    ValueDataF __KFP_SIMD__ATTR_ALIGN(__KFP_SIMD__Size_Float)
    data[__KFP_SIMD__Len_Float]{}; // Helper array
    store_a(data);
    return data[index];
// #elif defined(__KFP_SIMD__SSE4_1)
#elif 0
    float result;
    switch (index) {
    case 0:
        _MM_EXTRACT_FLOAT(result, a, 0x00);
        break;
    case 1:
        _MM_EXTRACT_FLOAT(result, a, 0x01);
        break;
    case 2:
        _MM_EXTRACT_FLOAT(result, a, 0x02);
        break;
    case 3:
    default:
        _MM_EXTRACT_FLOAT(result, a, 0x03);
        break;
    }
    return result;
#else
    switch (index) {
    case 0:
        return get<0>(a);
    case 1:
        return get<1>(a);
    case 2:
        return get<2>(a);
    case 3:
    default:
        return get<3>(a);
    }
#endif
}

template <>
inline SimdDataF select(const SimdDataF& mask, const SimdDataF& a,
                                const SimdDataF& b)
{
#if defined(__KFP_SIMD__SSE4_1) // SSE4.1
    return _mm_blendv_ps(b, a, mask);
#else
    return _mm_or_ps(_mm_and_ps(mask, a), _mm_andnot_ps(mask, b));
#endif
}

template <>
inline void print<SimdDataF>(std::ostream& stream, const SimdDataF& val_simd)
{
    ValueDataF __KFP_SIMD__ATTR_ALIGN(__KFP_SIMD__Size_Float)
        data[__KFP_SIMD__Len_Float]{}; // Helper array
    store_a<SimdDataF, ValueDataF>(val_simd, data);
    stream << "[" << data[0] << ", " << data[1] << ", " << data[2] << ", "
           << data[3] << "]";
}

template <> inline SimdDataF minus<SimdDataF>(const SimdDataF& a)
{
    return _mm_xor_ps(a, MINUS_MASK_F);
}

template <>
inline SimdDataF add<SimdDataF>(const SimdDataF& a, const SimdDataF& b)
{
    return _mm_add_ps(a, b);
}

template <>
inline SimdDataF substract<SimdDataF>(const SimdDataF& a, const SimdDataF& b)
{
    return _mm_sub_ps(a, b);
}

template <>
inline SimdDataF multiply<SimdDataF>(const SimdDataF& a, const SimdDataF& b)
{
    return _mm_mul_ps(a, b);
}

template <>
inline SimdDataF divide<SimdDataF>(const SimdDataF& a, const SimdDataF& b)
{
    return _mm_div_ps(a, b);
}

template <>
inline SimdDataF min<SimdDataF>(const SimdDataF& a, const SimdDataF& b)
{
    return _mm_min_ps(a, b);
}

template <>
inline SimdDataF max<SimdDataF>(const SimdDataF& a, const SimdDataF& b)
{
    return _mm_max_ps(a, b);
}

template <> inline SimdDataF sqrt<SimdDataF>(const SimdDataF& a)
{
    return _mm_sqrt_ps(a);
}

template <> inline SimdDataF rsqrt<SimdDataF>(const SimdDataF& a)
{
    return _mm_rsqrt_ps(a);
}

template <> inline SimdDataF abs<SimdDataF>(const SimdDataF& a)
{
    return _mm_and_ps(a, ABS_MASK_F);
}

template <> inline SimdDataF log<SimdDataF>(const SimdDataF& a)
{
    ValueDataF __KFP_SIMD__ATTR_ALIGN(__KFP_SIMD__Size_Float)
        data[__KFP_SIMD__Len_Float]{}; // Helper array
    store<SimdDataF, ValueDataF>(a, data);
    return _mm_setr_ps(std::log(data[0]), std::log(data[1]), std::log(data[2]),
                       std::log(data[3]));
}

template <> inline SimdDataF pow<SimdDataF>(const SimdDataF& a, int exp)
{
#if 0
    std::cerr << "[Error]: SimdF_t pow not implemented\n" ;
    exit(1) ;
#else
    ValueDataF __KFP_SIMD__ATTR_ALIGN(__KFP_SIMD__Size_Float)
        data[__KFP_SIMD__Len_Float]{}; // Helper array
    store<SimdDataF, ValueDataF>(a, data);
    return _mm_setr_ps(std::pow(data[0], exp), std::pow(data[1], exp),
                       std::pow(data[2], exp), std::pow(data[3], exp));
#endif
}

template <>
inline SimdDataF opNOT<SimdDataF>(const SimdDataF& a)
{
    return _mm_xor_ps(TRUE_MASK_F, a);
}

template <>
inline SimdDataF opLessThan<SimdDataF>(const SimdDataF& a, const SimdDataF& b)
{
    return _mm_cmplt_ps(a, b);
}

template <>
inline SimdDataF opLessThanEqual<SimdDataF>(const SimdDataF& a,
                                            const SimdDataF& b)
{
    return _mm_cmple_ps(a, b);
}

template <>
inline SimdDataF opGreaterThan<SimdDataF>(const SimdDataF& a,
                                          const SimdDataF& b)
{
    return _mm_cmpgt_ps(a, b);
}

template <>
inline SimdDataF opGreaterThanEqual<SimdDataF>(const SimdDataF& a,
                                               const SimdDataF& b)
{
    return _mm_cmpge_ps(a, b);
}

template <>
inline SimdDataF opEqual<SimdDataF>(const SimdDataF& a, const SimdDataF& b)
{
    return _mm_cmpeq_ps(a, b);
}
template <>
inline SimdDataF opNotEqual<SimdDataF>(const SimdDataF& a, const SimdDataF& b)
{
    return _mm_cmpneq_ps(a, b);
}
} // namespace Detail

} // namespace SIMD
} // namespace KFP

#endif
