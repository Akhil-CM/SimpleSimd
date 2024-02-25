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

template <> inline SimdF::SimdBaseClass()
{
    data_.simd_ = Detail::constant<SimdDataF, ValueDataF>(0.0f);
}
template <> inline SimdF::SimdBaseClass(const SimdF& class_simd)
{
    data_.simd_ = class_simd.data_.simd_;
}
// Constructor to broadcast the same value into all elements:
template <> inline SimdF::SimdBaseClass(ValueDataF val)
{
    data_.simd_ = Detail::constant<SimdDataF, ValueDataF>(val);
}
template <> inline SimdF::SimdBaseClass(ValueDataF* val)
{
    data_.simd_ = _mm_setr_ps(val[0], val[1], val[2], val[3]);
}
template <>
template <typename T, typename std::enable_if<true, T>::type*>
inline SimdF::SimdBaseClass(SimdDataF val_simd)
{
    data_.simd_ = val_simd;
}
// ------------------------------------------------------
// Load and Store
// ------------------------------------------------------
// Load
template <> inline SimdF& SimdF::load(const ValueDataF* val_ptr)
{
    data_.simd_ = Detail::load<SimdDataF, ValueDataF>(val_ptr);
    return *this;
}
template <> inline SimdF& SimdF::load_a(const ValueDataF* val_ptr)
{
    data_.simd_ = Detail::load_a<SimdDataF, ValueDataF>(val_ptr);
    return *this;
}
template <>
inline SimdF& SimdF::load_partial(int index, const ValueDataF* val_ptr)
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
template <> inline void SimdF::store(ValueDataF* val_ptr) const
{
    Detail::store<SimdDataF, ValueDataF>(data_.simd_, val_ptr);
}
template <> inline void SimdF::store_a(ValueDataF* val_ptr) const
{
    Detail::store_a<SimdDataF, ValueDataF>(data_.simd_, val_ptr);
}
template <> inline void SimdF::store_stream(ValueDataF* val_ptr) const
{
    _mm_stream_ps(val_ptr, data_.simd_);
}
template <>
inline void SimdF::store_partial(int index, ValueDataF* val_ptr) const
{
    if (index < 1)
        return;
    ValueDataF __KFP_SIMD__ATTR_ALIGN(__KFP_SIMD__Size_Float)
        data[__KFP_SIMD__Len_Float]{}; // Helper array
    store_a(data);
    if (index > SimdF::SimdLen) {
        index = SimdF::SimdLen;
    }
    std::copy_n(data, index, val_ptr);
}
// ------------------------------------------------------
// Gather and Scatter
// ------------------------------------------------------
template <>
inline SimdF& SimdF::gather(const SimdIndex& index, const ValueDataF* val_ptr)
{
    ValueDataI __KFP_SIMD__ATTR_ALIGN(__KFP_SIMD__Size_Int)
        indices[__KFP_SIMD__Len_Int]{}; // Helper array
    Detail::store_a<SimdDataI, ValueDataI>(index.index(), indices);
    data_.simd_ = _mm_setr_ps(val_ptr[indices[0]], val_ptr[indices[1]],
                              val_ptr[indices[2]], val_ptr[indices[3]]);
    return *this;
}
template <>
inline void SimdF::scatter(const SimdIndex& index, ValueDataF* val_ptr) const
{
    ValueDataF __KFP_SIMD__ATTR_ALIGN(__KFP_SIMD__Size_Float)
        data[__KFP_SIMD__Len_Float]{}; // Helper array
    store_a(data);
    ValueDataI __KFP_SIMD__ATTR_ALIGN(__KFP_SIMD__Size_Int)
        indices[__KFP_SIMD__Len_Int]{}; // Helper array
    Detail::store_a<SimdDataI, ValueDataI>(index.index(), indices);
    val_ptr[indices[0]] = data[0];
    val_ptr[indices[1]] = data[1];
    val_ptr[indices[2]] = data[2];
    val_ptr[indices[3]] = data[3];
}

// ------------------------------------------------------
// Data member accessors
// ------------------------------------------------------
template <> inline SimdF& SimdF::insert(int index, ValueDataF val)
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
    const SimdDataF mask =
        _mm_castsi128_ps(Detail::load_a<SimdDataI, ValueDataI>(indices));
    data_.simd_ = Detail::select<SimdDataF>(
        mask, Detail::constant<SimdDataF, ValueDataF>(val), data_.simd_);
#endif
    return *this;
}
template <> inline ValueDataF SimdF::operator[](int index) const
{
    assert((index > -1) && ("[Error] (operator[]): invalid index (" +
                            std::to_string(index) + ") given. Negative")
                               .data());
    assert((index < SimdLen) &&
           ("[Error] (operator[]): invalid index (" + std::to_string(index) +
            ") given. Exceeds maximum")
               .data());
    return Detail::extract<ValueDataF, SimdDataF>(index, data_.simd_);
}
template <> inline SimdF& SimdF::cutoff(int index)
{
    ValueDataF __KFP_SIMD__ATTR_ALIGN(__KFP_SIMD__Size_Float)
        data[__KFP_SIMD__Len_Float]{}; // Helper array
    store_a(data);
    return load_partial(index, data);
}

inline SimdF select(const SimdMask& mask, const SimdF& a, const SimdF& b)
{
    return SimdF(
        Detail::select<SimdF::simd_type>(mask.maskf(), a.simd(), b.simd()));
}

} // namespace SIMD
} // namespace KFP

#endif
