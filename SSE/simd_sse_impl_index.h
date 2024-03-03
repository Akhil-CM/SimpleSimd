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
    index_ = Detail::constant<simd_typei, simd_int::value_type>(val);
}
template<> inline simd_index::SimdIndexBase(int* val_ptr)
{
    index_ = Detail::load<simd_typei, int>(val_ptr);
}
template<> inline simd_index::SimdIndexBase(const simd_index& class_indices)
{
    index_ = class_indices.index_;
}
template<>
template<typename T, typename std::enable_if<true, T>::type*>
inline simd_index::SimdIndexBase(const simd_typei& val_simd)
{
    index_ = val_simd;
}
template<> inline simd_index::SimdIndexBase(const simd_typef& val_simd)
{
    index_ = Detail::cast<simd_typei, simd_typef>(val_simd);
}
template<>
inline simd_index::SimdIndexBase(const simd_int& class_simd)
{
    index_ = class_simd.simd();
}
template<> inline simd_index::SimdIndexBase(const simd_float& class_simd)
{
    index_ = Detail::cast<simd_typei, simd_typef>(class_simd.simd());
}

template<> inline simd_index& simd_index::operator=(int val)
{
    index_ = Detail::constant<simd_typei, simd_int::value_type>(val);
    return *this;
}
template<> inline simd_index& simd_index::operator=(const simd_index& class_indices)
{
    index_ = class_indices.index_;
    return *this;
}
template<>
template<typename T, typename std::enable_if<true, T>::type*>
inline simd_index& simd_index::operator=(const simd_typei& val_simd)
{
    index_ = val_simd;
    return *this;
}
template<> inline simd_index& simd_index::operator=(const simd_typef& val_simd)
{
    index_ = Detail::cast<simd_typei, simd_typef>(val_simd);
    return *this;
}

// ------------------------------------------------------
// Load and Store
// ------------------------------------------------------
// Load
template <> inline simd_index& simd_index::load(const int* val_ptr)
{
    index_ = Detail::load<simd_typei, int>(val_ptr);
    return *this;
}
template <> inline simd_index& simd_index::load_a(const int* val_ptr)
{
    index_ = Detail::load_a<simd_typei, int>(val_ptr);
    return *this;
}
// Store
template <> inline void simd_index::store(int* val_ptr) const
{
    Detail::store<simd_typei, int>(index_, val_ptr);
}
template <> inline void simd_index::store_a(int* val_ptr) const
{
    Detail::store_a<simd_typei, int>(index_, val_ptr);
}
template <> inline void simd_index::store_stream(int* val_ptr) const
{
    _mm_stream_si128(reinterpret_cast<simd_typei*>(val_ptr), index_);
}
template <> inline simd_index& simd_index::insert(int index, int val)
{
    int __KFP_SIMD__ATTR_ALIGN(__KFP_SIMD__Size_Int)
    indices[4] = { 0, 0, 0, 0 };
    indices[index] = -1;
    const simd_typei mask = Detail::load_a<simd_typei, int>(indices) ;
    index_ =
        Detail::select<simd_typei>(mask, Detail::constant<simd_typei, int>(val), index_);
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
