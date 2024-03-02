// -*- C++ Header -*-
/*
==================================================
Authors: A.Mithran;
Emails: mithran@fias.uni-frankfurt.de
==================================================
*/

#ifndef SIMD_SSE_DETAIL_MASK_H
#define SIMD_SSE_DETAIL_MASK_H

#include "../Base/simd_macros.h"
#include "../Base/simd_detail.h"
#include "../Base/simd_mask.h"
#include "simd_sse_detail_int.h"
#include "simd_sse_type.h"

#include <x86intrin.h>
#include <iostream>

namespace KFP {
namespace SIMD {

namespace Detail {

template <>
inline void print<simd_mask>(std::ostream& stream, const simd_mask& class_simd)
{
    ValueDataI __KFP_SIMD__ATTR_ALIGN(__KFP_SIMD__Size_Int)
        data[__KFP_SIMD__Len_Int]{}; // Helper array
    store_a<SimdDataI, ValueDataI>(class_simd.maski(), data);
    stream << "[" << std::boolalpha;
    for(int idx{0} ; idx < (simd_mask::SimdLen-1) ; ++idx){
        stream << static_cast<bool>(data[idx]) << ", ";
    }
    stream << static_cast<bool>(data[(simd_mask::SimdLen-1)]) << std::noboolalpha << "]";
}

template <>
inline SimdDataI opAND<SimdDataI>(const SimdDataI& a,
                                                 const SimdDataI& b)
{
    return Detail::opANDbitwise<SimdDataI>(a,b);
}
template <>
inline SimdDataI opOR<SimdDataI>(const SimdDataI& a,
                                                 const SimdDataI& b)
{
    return Detail::opORbitwise<SimdDataI>(a,b);
}

template <>
inline SimdDataI opNOT<SimdDataI>(const SimdDataI& a)
{
    const SimdDataI mask_true = _mm_set1_epi32(-1)  ;
    return _mm_xor_si128(mask_true, a);
}
template <>
inline bool opEqual<bool, SimdDataI, SimdDataI>(const SimdDataI& a,
                                              const SimdDataI& b)
{
    return ( sign<ValueDataI, SimdDataI>(a) == sign<ValueDataI, SimdDataI>(b) );
}
template <>
inline bool opNotEqual<bool, SimdDataI, SimdDataI>(const SimdDataI& a,
                                                 const SimdDataI& b)
{
    return ( sign<ValueDataI, SimdDataI>(a) != sign<ValueDataI, SimdDataI>(b) );
}

} // namespace Detail

} // namespace SIMD
} // namespace KFP

#endif
