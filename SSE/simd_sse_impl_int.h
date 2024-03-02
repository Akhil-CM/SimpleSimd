// -*- C++ Header -*-
/*
==================================================
Authors: A.Mithran;
Emails: mithran@fias.uni-frankfurt.de
==================================================
*/

#ifndef SIMD_SSE_IMPL_INT_H
#define SIMD_SSE_IMPL_INT_H

#include "../Base/simd_class.h"
#include "simd_sse_type.h"
#include "simd_sse_detail_int.h"

#include <x86intrin.h>
#include <algorithm>
#include <cassert>

namespace KFP {
namespace SIMD {

template <>
inline simd_int::SimdClassBase()
{
    data_.simd_ = Detail::constant<simd_type, value_type>(0);
}
// Constructor to broadcast the same value into all elements:
template <> inline simd_int::SimdClassBase(value_type val)
{
    data_.simd_ = Detail::constant<simd_type, value_type>(val);
}
template<>
template<typename T, typename std::enable_if<true, T>::type*>
inline simd_int::SimdClassBase(const simd_type& val_simd)
{
    data_.simd_ = val_simd;
}
template <> inline simd_int::SimdClassBase(value_type* val)
{
    data_.simd_ = Detail::load<simd_type, value_type>(val);
}
template <> inline simd_int::SimdClassBase(const simd_int& class_simd)
{
    data_.simd_ = class_simd.data_.simd_;
}
template <> inline simd_int::SimdClassBase(const simd_index& class_index)
{
    data_.simd_ = class_index.index();
}

template <>
inline simd_int& simd_int::operator=(value_type val)
{
    data_.simd_ = Detail::constant<simd_type, value_type>(val);
    return *this;
}
template<>
template<typename T, typename std::enable_if<true, T>::type*>
inline simd_int& simd_int::operator=(const simd_type& val_simd)
{
    data_.simd_ = val_simd;
    return *this;
}
template <> inline simd_int& simd_int::operator=(const simd_int& class_simd)
{
    data_.simd_ = class_simd.data_.simd_;
    return *this;
}
// ------------------------------------------------------
// Load and Store
// ------------------------------------------------------
// Load
template <> inline simd_int& simd_int::load(const value_type* val_ptr)
{
    data_.simd_ = Detail::load<simd_type, value_type>(val_ptr);
    return *this;
}
template <> inline simd_int& simd_int::load_a(const value_type* val_ptr)
{
    data_.simd_ = Detail::load_a<simd_type, value_type>(val_ptr);
    return *this;
}
template <>
inline simd_int& simd_int::load_partial(int index, const value_type* val_ptr)
{
    switch (index) {
    case 0:
        data_.simd_ = _mm_setzero_si128();
        break;
    case 1:
        // data_.simd_ = _mm_loadu_si32(val_ptr);
        data_.simd_ = _mm_cvtsi32_si128(val_ptr[0]);
        break;
    case 2:
        data_.simd_ = _mm_setr_epi32(val_ptr[0], val_ptr[1], 0, 0);
        break;
    case 3:
        data_.simd_ = _mm_setr_epi32(val_ptr[0], val_ptr[1], val_ptr[2], 0);
        break;
    case 4:
    default:
        load(val_ptr);
        break;
    }
    return *this;
}
// Store
template <> inline void simd_int::store(value_type* val_ptr) const
{
    Detail::store<simd_type, value_type>(data_.simd_, val_ptr);
}
template <> inline void simd_int::store_a(value_type* val_ptr) const
{
    Detail::store_a<simd_type, value_type>(data_.simd_, val_ptr);
}
template <> inline void simd_int::store_stream(value_type* val_ptr) const
{
    _mm_stream_si128(reinterpret_cast<simd_type*>(val_ptr), data_.simd_);
}
template <>
inline void simd_int::store_partial(int index, value_type* val_ptr) const
{
    if (index < 1)
        return;
    value_type __KFP_SIMD__ATTR_ALIGN(__KFP_SIMD__Size_Int)
        data[__KFP_SIMD__Len_Int]{}; // Helper array
    store_a(data);
    if (index > SimdLen) {
        index = SimdLen;
    }
    std::copy_n(data, index, val_ptr);
}
// ------------------------------------------------------
// Gather and Scatter
// ------------------------------------------------------
template <>
inline simd_int& simd_int::gather(const simd_index& index,
                                const value_type* val_ptr)
{
    data_.simd_ = _mm_setr_epi32( val_ptr[index[0]], val_ptr[index[1]], val_ptr[index[2]], val_ptr[index[3]]) ;
    return *this;
}
template <>
inline void simd_int::scatter(const simd_index& index, value_type* val_ptr) const
{
    value_type __KFP_SIMD__ATTR_ALIGN(__KFP_SIMD__Size_Int)
        data[__KFP_SIMD__Len_Int]{}; // Helper array
    store_a(data);
    val_ptr[index[0]] = data[0];
    val_ptr[index[1]] = data[1];
    val_ptr[index[2]] = data[2];
    val_ptr[index[3]] = data[3];
}

// ------------------------------------------------------
// Data member accessors
// ------------------------------------------------------
template <> inline simd_int& simd_int::insert(int index, value_type val)
{
    assert((index > -1) && ("[Error] (insert): invalid index (" +
                            std::to_string(index) + ") given. Negative")
                               .data());
    assert((index < SimdLen) &&
           ("[Error] (insert): invalid index (" + std::to_string(index) +
            ") given. Exceeds maximum")
               .data());
    int __KFP_SIMD__ATTR_ALIGN(__KFP_SIMD__Size_Int)
    indices[4] = { 0, 0, 0, 0 };
    indices[index] = -1;
    const simd_type mask = Detail::load_a<simd_type, value_type>(indices) ;
    data_.simd_ =
        Detail::select<simd_type>(mask, Detail::constant<simd_type, value_type>(val), data_.simd_);
    return *this;
}
template <> inline simd_int::value_type simd_int::operator[](int index) const
{
    assert((index > -1) && ("[Error] (operator[]): invalid index (" +
                            std::to_string(index) + ") given. Negative")
                               .data());
    assert((index < SimdLen) &&
           ("[Error] (operator[]): invalid index (" + std::to_string(index) +
            ") given. Exceeds maximum")
               .data());
    return Detail::extract<value_type, simd_type>(index, data_.simd_);
}
template <> inline simd_int& simd_int::cutoff(int index)
{
    value_type __KFP_SIMD__ATTR_ALIGN(__KFP_SIMD__Size_Int)
        data[__KFP_SIMD__Len_Int]{}; // Helper array
    store_a(data);
    return load_partial(index, data);
}

inline simd_int select(const simd_mask& mask, const simd_int& a, const simd_int& b)
{
    return simd_int(
        Detail::select<simd_int::simd_type>(mask.maski(), a.simd(), b.simd()));
}

template <typename F> inline simd_int apply(const simd_int& a, const F& func)
{
    simd_int::value_type __KFP_SIMD__ATTR_ALIGN(__KFP_SIMD__Size_Int)
        data[__KFP_SIMD__Len_Int]{}; // Helper array
    a.store_a(data);
    return simd_int{ _mm_setr_epi32(func(data[0]), func(data[1]), func(data[2]),
                              func(data[3])) };
}

inline simd_int signMultiply(const simd_int& a, const simd_int& b)
{
    const simd_int::simd_type& mask_sign = Detail::sign<simd_int::simd_type>(a.simd());
    return simd_int{ Detail::opXORbitwise<simd_int::simd_type>(mask_sign, b.simd()) };
}
inline simd_int signMultiply(const simd_float& a, const simd_int& b)
{
    const simd_float::simd_type& mask_sign = Detail::sign<simd_float::simd_type>(a.simd());
    return simd_int{ Detail::opXORbitwise<simd_int::simd_type>(_mm_castps_si128(mask_sign), b.simd()) };
}

} // namespace SIMD
} // namespace KFP

#endif
