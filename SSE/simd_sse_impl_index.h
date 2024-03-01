// -*- C++ Header -*-
/*
==================================================
Authors: A.Mithran;
Emails: mithran@fias.uni-frankfurt.de
==================================================
*/

#ifndef SIMD_SSE_IMPL_INDEX_H
#define SIMD_SSE_IMPL_INDEX_H

#include "../Base/simd_index.h"
#include "simd_sse_type.h"
#include "simd_sse_detail.h"

#include <x86intrin.h>
#include <iostream>
#include <cassert>

namespace KFP {
namespace SIMD {

template<> inline simd_index::SimdIndexBase()
{
    index_ = _mm_setr_epi32(0, 1, 2, 3);
}
template<> inline simd_index::SimdIndexBase(int val)
{
    index_ = Detail::constant<simd_int::simd_type, simd_int::value_type>(val);
}
template<> inline simd_index::SimdIndexBase(const simd_index& class_indices)
{
    index_ = class_indices.index_;
}
template<>
template<typename T, typename std::enable_if<true, T>::type*>
inline simd_index::SimdIndexBase(const simd_int::simd_type& val_simd)
{
    index_ = val_simd;
}
template<> inline simd_index::SimdIndexBase(const simd_float::simd_type& val_simd)
{
    index_ = Detail::cast<simd_int::simd_type, simd_float::simd_type>(val_simd);
}
template<>
inline simd_index::SimdIndexBase(const simd_int& class_simd)
{
    index_ = class_simd.simd();
}
template<> inline simd_index::SimdIndexBase(const simd_float& class_simd)
{
    index_ = Detail::cast<simd_int::simd_type, simd_float::simd_type>(class_simd.simd());
}

template<> inline simd_index& simd_index::operator=(int val)
{
    index_ = Detail::constant<simd_int::simd_type, simd_int::value_type>(val);
    return *this;
}
template<> inline simd_index& simd_index::operator=(const simd_index& class_indices)
{
    index_ = class_indices.index_;
    return *this;
}
template<>
template<typename T, typename std::enable_if<!(std::is_same<int, simd_int::simd_type>::value), T>::type*>
inline simd_index& simd_index::operator=(const simd_int::simd_type& val_simd)
{
    index_ = val_simd;
    return *this;
}
template<> inline simd_index& simd_index::operator=(const simd_float::simd_type& val_simd)
{
    index_ = Detail::cast<simd_int::simd_type, simd_float::simd_type>(val_simd);
    return *this;
}

template<> inline simd_int::value_type simd_index::operator[](int index) const
{
    assert((index > -1) && ("[Error] (operator[]): invalid index (" +
                            std::to_string(index) + ") given. Negative")
                               .data());
    assert((index < SimdLen) &&
           ("[Error] (operator[]): invalid index (" + std::to_string(index) +
            ") given. Exceeds maximum")
               .data());
    return Detail::extract<simd_int::value_type, simd_int::simd_type>(index, index_);
}

inline simd_index select(const simd_mask& mask, const simd_index& a, const simd_index& b)
{
    return simd_index{
        Detail::select<simd_int::simd_type>(mask.maski(), a.index(), b.index())};
}
} // namespace SIMD
} // namespace KFP

#endif
