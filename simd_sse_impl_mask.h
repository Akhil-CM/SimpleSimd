// -*- C++ Header -*-
/*
==================================================
Authors: A.Mithran;
Emails: mithran@fias.uni-frankfurt.de
==================================================
*/

#ifndef SIMD_SSE_IMPL_MASK_H
#define SIMD_SSE_IMPL_MASK_H

#include "simd_mask.h"
#include "simd_sse_detail_mask.h"
#include "simd_sse_type.h"

#include <x86intrin.h>

namespace KFP {
namespace SIMD {

template <> inline SimdMask::SimdMaskBase()
{
    mask_ = Detail::constant<SimdDataI, ValueDataI>(0) ;
}
template <> inline SimdMask::SimdMaskBase(bool val)
{
    mask_ = Detail::constant<SimdDataI, ValueDataI>(-int(val)) ;
}
template <> inline SimdMask::SimdMaskBase(const bool* val_ptr)
{
    ValueDataI __KFP_SIMD__ATTR_ALIGN(__KFP_SIMD__Size_Int)
        data[__KFP_SIMD__Len_Int]{}; // Helper array
    data[0] = -int(val_ptr[0]);
    data[1] = -int(val_ptr[1]);
    data[2] = -int(val_ptr[2]);
    data[3] = -int(val_ptr[3]);
    mask_ = Detail::load_a<SimdDataI, ValueDataI>(data) ;
}
template <> inline SimdMask::SimdMaskBase(const simd_typei& mask)
{
    mask_ = Detail::opNOT<SimdDataI>( _mm_cmpeq_epi32(mask, _mm_setzero_si128()) );
}
template <> inline SimdMask::SimdMaskBase(const simd_typef& mask)
{
    mask_ = Detail::opNOT<SimdDataI>( _mm_castps_si128(_mm_cmpeq_ps(mask, _mm_setzero_ps())) );
}
template <> inline SimdMask::SimdMaskBase(const SimdMaskBase& class_mask)
{
    mask_ = class_mask.mask_ ;
}

template <> inline SimdMask& SimdMask::operator=(bool val)
{
    mask_ = Detail::constant<SimdDataI, ValueDataI>(-int(val)) ;
    return *this ;
}
template <> inline SimdMask& SimdMask::operator=(const simd_typei& mask)
{
    mask_ = Detail::opNOT<SimdDataI>( _mm_cmpeq_epi32(mask, _mm_setzero_si128()) );
    return *this ;
}
template <> inline SimdMask& SimdMask::operator=(const simd_typef& mask)
{
    mask_ = Detail::opNOT<SimdDataI>( _mm_castps_si128(_mm_cmpeq_ps(mask, _mm_setzero_ps())) );
    return *this ;
}
template <> inline SimdMask& SimdMask::operator=(const SimdMaskBase& class_mask)
{
    mask_ = class_mask.mask_ ;
    return *this ;
}

template <> inline int SimdMask::count() const
{
    ValueDataI __KFP_SIMD__ATTR_ALIGN(__KFP_SIMD__Size_Int)
        data[__KFP_SIMD__Len_Int]{}; // Helper array
    Detail::store_a(mask_, data);
    return -(data[0] + data[1] + data[2] + data[3]) ;
}
template <> inline bool SimdMask::AND() const
{
    return static_cast<bool>(count() == 4) ;
}
template <> inline bool SimdMask::OR() const
{
    return static_cast<bool>(count() != 0) ;
}

template <> inline bool SimdMask::operator[](int index) const
{
    return static_cast<bool>(Detail::extract<ValueDataI, SimdDataI>(index, mask_));
}
template <> inline SimdDataF SimdMask::maskf() const
{
    return _mm_castsi128_ps(mask_);
}

} // namespace SIMD
} // namespace KFP

#endif
