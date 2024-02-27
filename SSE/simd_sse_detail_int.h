// -*- C++ Header -*-
/*
==================================================
Authors: A.Mithran;
Emails: mithran@fias.uni-frankfurt.de
==================================================
*/

#ifndef SIMD_SSE_DETAIL_INT_H
#define SIMD_SSE_DETAIL_INT_H

#include "../Base/simd_macros.h"
#include "../Base/simd_detail.h"
#include "simd_sse_type.h"

#include <x86intrin.h>
#include <iostream>
#include <cmath>

namespace KFP {
namespace SIMD {

namespace Detail {

template <>
inline SimdDataI constant<SimdDataI, ValueDataI>(ValueDataI val)
{
    return _mm_set1_epi32(val);
}

template <>
inline SimdDataI load<SimdDataI, ValueDataI>(const ValueDataI* val_ptr)
{
    return _mm_loadu_si128(reinterpret_cast<const SimdDataI*>(val_ptr));
}
template <>
inline SimdDataI load_a<SimdDataI, ValueDataI>(const ValueDataI* val_ptr)
{
    return _mm_load_si128(reinterpret_cast<const SimdDataI*>(val_ptr));
}
template <>
inline void store<SimdDataI, ValueDataI>(const SimdDataI& val_simd,
                                         ValueDataI* val_ptr)
{
    _mm_storeu_si128(reinterpret_cast<SimdDataI*>(val_ptr), val_simd);
}
template <>
inline void store_a<SimdDataI, ValueDataI>(const SimdDataI& val_simd,
                                         ValueDataI* val_ptr)
{
    _mm_store_si128(reinterpret_cast<SimdDataI*>(val_ptr), val_simd);
}
template <int N> inline ValueDataI get(const SimdDataI& a)
{
    const SimdDataI result = _mm_shuffle_epi32(a, (N&3)) ;
    return _mm_cvtsi128_si32(result);
}
template <>
inline ValueDataI extract<ValueDataI, SimdDataI>(int index, const SimdDataI& a)
{
#if 0
    ValueDataI __KFP_SIMD__ALIGN_V1(__KFP_SIMD__Size_Int)
    data[__KFP_SIMD__Len_Int]{}; // Helper array
    store_a(data);
    return data[index];
#elif defined(__KFP_SIMD__SSE4_1)
    switch (index) {
    case 0:
        return _mm_extract_epi32(a, 0x00) ;
    case 1:
        return _mm_extract_epi32(a, 0x01) ;
    case 2:
        return _mm_extract_epi32(a, 0x02) ;
    case 3:
    default:
        return _mm_extract_epi32(a, 0x03) ;
    }
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
inline SimdDataI select<SimdDataI>(const SimdDataI& mask, const SimdDataI& a,
                        const SimdDataI& b)
{
#if defined(__KFP_SIMD__SSE4_1) // SSE4.1
    return _mm_blendv_epi8(b, a, mask);
#else
    return _mm_or_si128(_mm_and_si128(mask, a), _mm_andnot_si128(mask, b));
#endif
}
template <>
inline void print<SimdDataI>(std::ostream& stream, const SimdDataI& val_simd)
{
    ValueDataI __KFP_SIMD__ATTR_ALIGN(__KFP_SIMD__Size_Int)
        data[__KFP_SIMD__Len_Int]{}; // Helper array
    store_a<SimdDataI, ValueDataI>(val_simd, data);
    stream << "[" << data[0] << ", " << data[1] << ", " << data[2] << ", "
           << data[3] << "]";
}

template <>
inline int sign<int, SimdDataI>(const SimdDataI& a)
{
    return _mm_movemask_ps(_mm_castsi128_ps(a));
}

template <> inline SimdDataI minus<SimdDataI>(const SimdDataI& a)
{
    const SimdDataI mask_minus{ _mm_set1_epi32(0x80000000) } ;
    return _mm_xor_si128(a, mask_minus);
}

template <>
inline SimdDataI add<SimdDataI>(const SimdDataI& a, const SimdDataI& b)
{
    return _mm_add_epi32(a, b);
}

template <>
inline SimdDataI substract<SimdDataI>(const SimdDataI& a, const SimdDataI& b)
{
    return _mm_sub_epi32(a, b);
}

template <>
inline SimdDataI multiply<SimdDataI>(const SimdDataI& a, const SimdDataI& b)
{
#if defined(__KFP_SIMD__SSE4_1) // SSE4.1
    return _mm_mullo_epi32(a, b);
#else
    ValueDataI __KFP_SIMD__ATTR_ALIGN(__KFP_SIMD__Size_Int)
        data1[__KFP_SIMD__Len_Int]{}; // Helper array
    ValueDataI __KFP_SIMD__ATTR_ALIGN(__KFP_SIMD__Size_Int)
        data2[__KFP_SIMD__Len_Int]{}; // Helper array
    store_a<SimdDataI, ValueDataI>(a, data1);
    store_a<SimdDataI, ValueDataI>(b, data2);
    const ValueDataI  __KFP_SIMD__ATTR_ALIGN(__KFP_SIMD__Size_Int)
    data[SimdI::SimdLen]{ data1[0] * data2[0],
                                             data1[1] * data2[1],
                                             data1[2] * data2[2],
                                             data1[3] * data2[3] };
    return load_a<SimdDataI, ValueDataI>(data);
#endif
}

template <>
inline SimdDataI divide<SimdDataI>(const SimdDataI& a, const SimdDataI& b)
{
    ValueDataI __KFP_SIMD__ATTR_ALIGN(__KFP_SIMD__Size_Int)
        data1[__KFP_SIMD__Len_Int]{}; // Helper array
    ValueDataI __KFP_SIMD__ATTR_ALIGN(__KFP_SIMD__Size_Int)
        data2[__KFP_SIMD__Len_Int]{}; // Helper array
    store_a<SimdDataI, ValueDataI>(a, data1);
    store_a<SimdDataI, ValueDataI>(b, data2);
    const ValueDataI __KFP_SIMD__ATTR_ALIGN(__KFP_SIMD__Size_Int)
    data[SimdI::SimdLen]{ data1[0] / data2[0],
                                             data1[1] / data2[1],
                                             data1[2] / data2[2],
                                             data1[3] / data2[3] };
    return load_a<SimdDataI, ValueDataI>(data);
}

template <>
inline SimdDataI min<SimdDataI>(const SimdDataI& a, const SimdDataI& b)
{
#if defined(__KFP_SIMD__SSE4_1) // SSE4.1
    return _mm_min_epi32(a, b);
#else
    const SimdDataI mask = _mm_cmpgt_epi32(a, b);
    return select<SimdDataI>(mask, b, a);
#endif
}

template <>
inline SimdDataI max<SimdDataI>(const SimdDataI& a, const SimdDataI& b)
{
#if defined(__KFP_SIMD__SSE4_1) // SSE4.1
    return _mm_max_epi32(a, b);
#else
    const SimdDataI mask = _mm_cmpgt_epi32(a, b);
    return select<SimdDataI>(mask, a, b);
#endif
}

template <> inline SimdDataI sqrt<SimdDataI>(const SimdDataI& a)
{
#if 0
    ValueDataI __KFP_SIMD__ATTR_ALIGN(__KFP_SIMD__Size_Int)
        data[__KFP_SIMD__Len_Int]{}; // Helper array
    store_a<SimdDataI, ValueDataI>(a, data);
    return _mm_setr_epi32(std::sqrt(data[0]), std::sqrt(data[1]),
                          std::sqrt(data[2]), std::sqrt(data[3]));
#endif
    return _mm_cvtps_epi32(_mm_sqrt_ps(_mm_cvtepi32_ps(a)));
}

template <> inline SimdDataI rsqrt<SimdDataI>(const SimdDataI& a)
{
    return _mm_cvtps_epi32(_mm_rsqrt_ps(_mm_cvtepi32_ps(a)));
}

template <> inline SimdDataI abs<SimdDataI>(const SimdDataI& a)
{
#if defined(__KFP_SIMD__SSSE3) // SSSE3
    return _mm_abs_epi32(a);
#else
    const SimdDataI mask_abs{ _mm_set1_epi32(0x7FFFFFFF) } ;
    return _mm_and_si128(a, mask_abs);
#endif
}

template <> inline SimdDataI log<SimdDataI>(const SimdDataI& a)
{
    ValueDataI __KFP_SIMD__ATTR_ALIGN(__KFP_SIMD__Size_Int)
        data[__KFP_SIMD__Len_Int]{}; // Helper array
    store_a<SimdDataI, ValueDataI>(a, data);
    return _mm_setr_epi32(std::log(data[0]), std::log(data[1]),
                          std::log(data[2]), std::log(data[3]));
}

template <> inline SimdDataI pow<SimdDataI>(const SimdDataI& a, int exp)
{
    ValueDataI __KFP_SIMD__ATTR_ALIGN(__KFP_SIMD__Size_Int)
        data[__KFP_SIMD__Len_Int]{}; // Helper array
    store_a<SimdDataI, ValueDataI>(a, data);
    return _mm_setr_epi32(std::pow(data[0], exp), std::pow(data[1], exp),
                          std::pow(data[2], exp), std::pow(data[3], exp));
}

template <>
inline SimdDataI opNOT<SimdDataI>(const SimdDataI& a)
{
    const SimdDataI mask_true{ _mm_set1_epi32(-1) } ;
    return _mm_xor_si128(mask_true, a);
}

template <>
inline SimdDataI opLessThan<SimdDataI>(const SimdDataI& a, const SimdDataI& b)
{
    return _mm_cmplt_epi32(a, b);
}

template <>
inline SimdDataI opLessThanEqual<SimdDataI>(const SimdDataI& a,
                                            const SimdDataI& b)
{
#if defined(__KFP_SIMD__SSE4_1) // SSE4.1
    return _mm_cmpeq_epi32(_mm_min_epi32(a, b), a);
#else
    const SimdDataI mask_true{ _mm_set1_epi32(-1) } ;
    return _mm_xor_si128(_mm_cmpgt_epi32(a, b), mask_true);
#endif
}

template <>
inline SimdDataI opGreaterThan<SimdDataI>(const SimdDataI& a,
                                          const SimdDataI& b)
{
    return _mm_cmpgt_epi32(a, b);
}

template <>
inline SimdDataI opGreaterThanEqual<SimdDataI>(const SimdDataI& a,
                                               const SimdDataI& b)
{
#if defined(__KFP_SIMD__SSE4_1) // SSE4.1
    return _mm_cmpeq_epi32(_mm_min_epi32(b, a), b);
#else
    const SimdDataI mask_true{ _mm_set1_epi32(-1) } ;
    return _mm_xor_si128(_mm_cmplt_epi32(a, b), mask_true);
#endif
}

template <>
inline SimdDataI opEqual<SimdDataI>(const SimdDataI& a, const SimdDataI& b)
{
    return _mm_cmpeq_epi32(a, b);
}
template <>
inline SimdDataI opNotEqual<SimdDataI>(const SimdDataI& a, const SimdDataI& b)
{
    const SimdDataI mask_true{ _mm_set1_epi32(-1) } ;
    return _mm_xor_si128(_mm_cmpeq_epi32(a, b), mask_true);
}
} // namespace Detail

} // namespace SIMD
} // namespace KFP

#endif
