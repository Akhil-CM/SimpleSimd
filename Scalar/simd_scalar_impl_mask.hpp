// -*- C++ Header -*-
/*
==================================================
Authors: A.Mithran;
Emails: mithran@fias.uni-frankfurt.de
==================================================
*/

#ifndef SIMD_SCALAR_IMPL_MASK_H
#define SIMD_SCALAR_IMPL_MASK_H

#include "../Base/simd_mask.hpp"
#include "simd_scalar_detail_mask.hpp"
#include "simd_scalar_type.hpp"

#include <cmath>

namespace KFP {
namespace SIMD {

template <> inline SimdMask::SimdMaskBase()
{
    mask_ = 0 ;
}
template <> inline SimdMask::SimdMaskBase(bool val)
{
    mask_ = -int(val) ;
}
template <> inline SimdMask::SimdMaskBase(const bool* val_ptr)
{
    mask_ = -int(val_ptr[0]) ;
}
template <> inline SimdMask::SimdMaskBase(const simd_typei& mask)
{
    mask_ = -int(static_cast<bool>(mask)) ;
}
template <> inline SimdMask::SimdMaskBase(const simd_typef& mask)
{
    mask_ = -int(static_cast<bool>(mask)) ;
}
template <> inline SimdMask::SimdMaskBase(const SimdMaskBase& class_mask)
{
    mask_ = class_mask.mask_ ;
}

template <> inline SimdMask& SimdMask::operator=(bool val)
{
    mask_ = -int(val) ;
    return *this ;
}
template <> inline SimdMask& SimdMask::operator=(const simd_typei& mask)
{
    mask_ = -int(static_cast<bool>(mask)) ;
    return *this ;
}
template <> inline SimdMask& SimdMask::operator=(const simd_typef& mask)
{
    mask_ = -int(static_cast<bool>(mask)) ;
    return *this ;
}
template <> inline SimdMask& SimdMask::operator=(const SimdMaskBase& class_mask)
{
    mask_ = class_mask.mask_ ;
    return *this ;
}

template <> inline int SimdMask::count() const
{
    return -(mask_) ;
}
template <> inline bool SimdMask::AND() const
{
    return static_cast<bool>(mask_) ;
}
template <> inline bool SimdMask::OR() const
{
    return static_cast<bool>(mask_) ;
}

template <> inline bool SimdMask::operator[](int) const
{
    return mask_;
}
template <> inline SimdDataF SimdMask::maskf() const
{
    return static_cast<SimdDataF>(mask_);
}

} // namespace SIMD
} // namespace KFP

#endif
