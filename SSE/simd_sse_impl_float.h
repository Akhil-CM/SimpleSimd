// -*- C++ Header -*-
/*
==================================================
Authors: A.Mithran;
Emails: mithran@fias.uni-frankfurt.de
==================================================
*/

#ifndef SIMD_SSE_IMPL_FLOAT_H
#define SIMD_SSE_IMPL_FLOAT_H

#include "../Base/simd_class.h"
#include "simd_sse_type.h"
#include "simd_sse_detail.h"

#include <x86intrin.h>
#include <algorithm>
#include <cassert>

namespace KFP {
namespace SIMD {

template <> inline simd_float::SimdClassBase()
{
    data_.simd_ = Detail::constant<simd_float::simd_type, simd_float::value_type>(0.0f);
}
// Constructor to broadcast the same value into all elements:
template <> inline simd_float::SimdClassBase(value_type val)
{
    data_.simd_ = Detail::constant<simd_type, value_type>(val);
}
template <>
template <typename T, typename std::enable_if<true, T>::type*>
inline simd_float::SimdClassBase(const simd_type& val_simd)
{
    data_.simd_ = val_simd;
}
template <> inline simd_float::SimdClassBase(value_type* val)
{
    data_.simd_ = Detail::load<simd_type, value_type>(val);
}
template <> inline simd_float::SimdClassBase(const simd_float& class_simd)
{
    data_.simd_ = class_simd.data_.simd_;
}
template <> inline simd_float::SimdClassBase(const simd_index& class_index)
{
    data_.simd_ = Detail::cast<simd_type, simd_int::simd_type>(class_index.index());
}

template <>
inline simd_float& simd_float::operator=(value_type val)
{
    data_.simd_ = Detail::constant<simd_type, value_type>(val);
    return *this;
}
template <>
template <typename T, typename std::enable_if<true, T>::type*>
inline simd_float& simd_float::operator=(const simd_type& val_simd)
{
    data_.simd_ = val_simd;
    return *this;
}
template <> inline simd_float& simd_float::operator=(const simd_float& class_simd)
{
    data_.simd_ = class_simd.data_.simd_;
    return *this;
}
// ------------------------------------------------------
// Load and Store
// ------------------------------------------------------
// Load
template <> inline simd_float& simd_float::load(const value_type* val_ptr)
{
    data_.simd_ = Detail::load<simd_type, value_type>(val_ptr);
    return *this;
}
template <> inline simd_float& simd_float::load_a(const value_type* val_ptr)
{
    data_.simd_ = Detail::load_a<simd_type, value_type>(val_ptr);
    return *this;
}
template <>
inline simd_float& simd_float::load_partial(int index, const value_type* val_ptr)
{
    switch (index) {
    case 0:
        data_.simd_ = _mm_setzero_ps();
        break;
    case 1:
        data_.simd_ = _mm_load_ss(val_ptr);
        break;
    case 2:
        data_.simd_ = _mm_setr_ps(val_ptr[0], val_ptr[1], 0.f, 0.f);
        break;
    case 3:
        data_.simd_ = _mm_setr_ps(val_ptr[0], val_ptr[1], val_ptr[2], 0.f);
        break;
    case 4:
    default:
        load(val_ptr);
        break;
    }
    return *this;
}
// Store
template <> inline void simd_float::store(value_type* val_ptr) const
{
    Detail::store<simd_type, value_type>(data_.simd_, val_ptr);
}
template <> inline void simd_float::store_a(value_type* val_ptr) const
{
    Detail::store_a<simd_type, value_type>(data_.simd_, val_ptr);
}
template <> inline void simd_float::store_stream(value_type* val_ptr) const
{
    _mm_stream_ps(val_ptr, data_.simd_);
}
template <>
inline void simd_float::store_partial(int index, value_type* val_ptr) const
{
    if (index < 1)
        return;
    value_type __KFP_SIMD__ATTR_ALIGN(__KFP_SIMD__Size_Float)
        data[__KFP_SIMD__Len_Float]{}; // Helper array
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
inline simd_float& simd_float::gather(const simd_index& index, const value_type* val_ptr)
{
    simd_int::value_type __KFP_SIMD__ATTR_ALIGN(__KFP_SIMD__Size_Int)
        indices[__KFP_SIMD__Len_Int]{}; // Helper array
    Detail::store_a<simd_int::simd_type, simd_int::value_type>(index.index(), indices);
    data_.simd_ = _mm_setr_ps(val_ptr[indices[0]], val_ptr[indices[1]],
                              val_ptr[indices[2]], val_ptr[indices[3]]);
    return *this;
}
template <>
inline void simd_float::scatter(const simd_index& index, value_type* val_ptr) const
{
    value_type __KFP_SIMD__ATTR_ALIGN(__KFP_SIMD__Size_Float)
        data[__KFP_SIMD__Len_Float]{}; // Helper array
    store_a(data);
    simd_int::value_type __KFP_SIMD__ATTR_ALIGN(__KFP_SIMD__Size_Int)
        indices[__KFP_SIMD__Len_Int]{}; // Helper array
    Detail::store_a<simd_int::simd_type, simd_int::value_type>(index.index(), indices);
    val_ptr[indices[0]] = data[0];
    val_ptr[indices[1]] = data[1];
    val_ptr[indices[2]] = data[2];
    val_ptr[indices[3]] = data[3];
}

// ------------------------------------------------------
// Data member accessors
// ------------------------------------------------------
template <> inline simd_float& simd_float::insert(int index, value_type val)
{
    assert((index > -1) && ("[Error] (insert): invalid index (" +
                            std::to_string(index) + ") given. Negative")
                               .data());
    assert((index < SimdLen) &&
           ("[Error] (insert): invalid index (" + std::to_string(index) +
            ") given. Exceeds maximum")
               .data());
#if defined(__KFP_SIMD__SSE4_1) // SSE4.1
    switch (index) {
    case 0:
        data_.simd_ = _mm_insert_ps(data_.simd_, _mm_set_ss(val), 0 << 4);
        break;
    case 1:
        data_.simd_ = _mm_insert_ps(data_.simd_, _mm_set_ss(val), 1 << 4);
        break;
    case 2:
        data_.simd_ = _mm_insert_ps(data_.simd_, _mm_set_ss(val), 2 << 4);
        break;
    case 3:
    default:
        data_.simd_ = _mm_insert_ps(data_.simd_, _mm_set_ss(val), 3 << 4);
        break;
    }
#else
    int __KFP_SIMD__ATTR_ALIGN(__KFP_SIMD__Size_Int)
        indices[__KFP_SIMD__Len_Int] = { 0, 0, 0, 0 };
    indices[index] = -1;
    const simd_type mask =
        _mm_castsi128_ps(Detail::load_a<simd_int::simd_type, simd_int::value_type>(indices));
    data_.simd_ = Detail::select<simd_type>(
        mask, Detail::constant<simd_type, value_type>(val), data_.simd_);
#endif
    return *this;
}
template <> inline simd_float::value_type simd_float::operator[](int index) const
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
template <> inline simd_float& simd_float::cutoff(int index)
{
    value_type __KFP_SIMD__ATTR_ALIGN(__KFP_SIMD__Size_Float)
        data[__KFP_SIMD__Len_Float]{}; // Helper array
    store_a(data);
    return load_partial(index, data);
}

inline simd_float select(const simd_mask& mask, const simd_float& a, const simd_float& b)
{
    return simd_float(
        Detail::select<simd_float::simd_type>(mask.maskf(), a.simd(), b.simd()));
}

template <typename F> inline simd_float apply(const simd_float& a, const F& func)
{
    simd_float::value_type __KFP_SIMD__ATTR_ALIGN(__KFP_SIMD__Size_Float)
        data[__KFP_SIMD__Len_Float]{}; // Helper array
    a.store_a(data);
    return simd_float{ _mm_setr_ps(func(data[0]), func(data[1]), func(data[2]),
                              func(data[3])) };
}

inline simd_float signMultiply(const simd_int& a, const simd_float& b)
{
    const simd_int::simd_type& mask_minus = Detail::sign<simd_int::simd_type>(a.simd());
    return simd_float{ Detail::opXORbitwise<simd_float::simd_type>(_mm_castsi128_ps(mask_minus), b.simd()) };
}
inline simd_float signMultiply(const simd_float& a, const simd_float& b)
{
    const simd_float::simd_type& mask_minus = Detail::sign<simd_float::simd_type>(a.simd());
    return simd_float{ Detail::opXORbitwise<simd_float::simd_type>(mask_minus, b.simd()) };
}

inline simd_float round(const simd_float& a)
{
#if defined(__KFP_SIMD__SSE4_1) // SSE4.1
    return simd_float{ _mm_round_ps(a.simd(), _MM_FROUND_NINT) };
#elif 0
    simd_float::value_type __KFP_SIMD__ATTR_ALIGN(__KFP_SIMD__Size_Float)
        data[__KFP_SIMD__Len_Float]{}; // Helper array
    a.store_a(data);
    return simd_float{ _mm_setr_ps(std::round(data[0]), std::round(data[1]),
                              std::round(data[2]), std::round(data[3])) };
#else
    simd_int::simd_type tmp = _mm_cvtps_epi32(a.simd()); // convert to integer
    return simd_float{ Detail::cast<simd_float::simd_type, simd_int::simd_type>(tmp) }; // convert back to float
#endif
}

inline simd_mask isInf(const simd_float& a)
{
    return simd_mask{ _mm_cmpeq_ps(a.simd(), _mm_castsi128_ps(Detail::getMask<Detail::MASK::INF>())) };
}

inline simd_mask isFinite(const simd_float& a)
{
    const simd_float::simd_type mask_inf = _mm_castsi128_ps(Detail::getMask<Detail::MASK::INF>()) ;
    return simd_mask{ _mm_cmpneq_ps(_mm_and_ps(a.simd(), mask_inf), mask_inf) };
}

inline simd_mask isNaN(const simd_float& a)
{
    return _mm_cmpunord_ps(a.simd(), a.simd());
}

} // namespace SIMD
} // namespace KFP

#endif
