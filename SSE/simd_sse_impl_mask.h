// -*- C++ Header -*-
/*
==================================================
Authors: A.Mithran;
Emails: mithran@fias.uni-frankfurt.de
==================================================
*/

#ifndef SIMD_SSE_IMPL_MASK_H
#define SIMD_SSE_IMPL_MASK_H

#include "../Base/simd_mask.h"
#include "simd_sse_detail_mask.h"
#include "simd_sse_type.h"

#include <x86intrin.h>

namespace KFP {
namespace SIMD {

template <> inline simd_mask::SimdMaskBase()
{
    mask_ = Detail::constant<simd_int::simd_type, simd_int::value_type>(0) ;
}
template <> inline simd_mask::SimdMaskBase(bool val)
{
    mask_ = Detail::constant<simd_int::simd_type, simd_int::value_type>(-int(val)) ;
}
template <> inline simd_mask::SimdMaskBase(const bool* val_ptr)
{
    simd_int::value_type __KFP_SIMD__ATTR_ALIGN(__KFP_SIMD__Size_Int)
        data[__KFP_SIMD__Len_Int]{}; // Helper array
    data[0] = -int(val_ptr[0]);
    data[1] = -int(val_ptr[1]);
    data[2] = -int(val_ptr[2]);
    data[3] = -int(val_ptr[3]);
    mask_ = Detail::load_a<simd_int::simd_type, simd_int::value_type>(data) ;
}
template <> inline simd_mask::SimdMaskBase(const simd_typei& mask)
{
    mask_ = Detail::opNOT<simd_int::simd_type>( _mm_cmpeq_epi32(mask, _mm_setzero_si128()) );
}
template <> inline simd_mask::SimdMaskBase(const simd_typef& mask)
{
    mask_ = Detail::opNOT<simd_int::simd_type>( _mm_castps_si128(_mm_cmpeq_ps(mask, _mm_setzero_ps())) );
}
template <> inline simd_mask::SimdMaskBase(const simd_mask& class_mask)
{
    mask_ = class_mask.mask_ ;
}

template <> inline simd_mask& simd_mask::operator=(bool val)
{
    mask_ = Detail::constant<simd_int::simd_type, simd_int::value_type>(-int(val)) ;
    return *this ;
}
template <> inline simd_mask& simd_mask::operator=(const simd_typei& mask)
{
    mask_ = Detail::opNOT<simd_int::simd_type>( _mm_cmpeq_epi32(mask, _mm_setzero_si128()) );
    return *this ;
}
template <> inline simd_mask& simd_mask::operator=(const simd_typef& mask)
{
    mask_ = Detail::opNOT<simd_int::simd_type>( _mm_castps_si128(_mm_cmpeq_ps(mask, _mm_setzero_ps())) );
    return *this ;
}
template <> inline simd_mask& simd_mask::operator=(const simd_mask& class_mask)
{
    mask_ = class_mask.mask_ ;
    return *this ;
}

template <> inline int simd_mask::count() const
{
#if 1
    const int tmp{ Detail::sign<simd_int::value_type, simd_int::simd_type>(mask_) };
    return (tmp & 0x01) + ((tmp & 0x02) >> 1) + ((tmp & 0x04) >> 2) + ((tmp & 0x08) >> 3);
#else
    simd_int::value_type __KFP_SIMD__ATTR_ALIGN(__KFP_SIMD__Size_Int)
        data[__KFP_SIMD__Len_Int]{}; // Helper array
    Detail::store_a(mask_, data);
    return -(data[0] + data[1] + data[2] + data[3]) ;
#endif
}
template <> inline bool simd_mask::AND() const
{
    return static_cast<bool>(count() == 4) ;
}
template <> inline bool simd_mask::OR() const
{
    return static_cast<bool>(count() != 0) ;
}

template <> inline bool simd_mask::operator[](int index) const
{
    return static_cast<bool>(Detail::extract<simd_int::value_type, simd_int::simd_type>(index, mask_));
}
template <> inline simd_float::simd_type simd_mask::maskf() const
{
    return _mm_castsi128_ps(mask_);
}

} // namespace SIMD
} // namespace KFP

#endif
