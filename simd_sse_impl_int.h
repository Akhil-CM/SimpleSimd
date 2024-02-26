// -*- C++ Header -*-
/*
==================================================
Authors: A.Mithran;
Emails: mithran@fias.uni-frankfurt.de
==================================================
*/

#ifndef SIMD_SSE_IMPL_INT_H
#define SIMD_SSE_IMPL_INT_H

#include "simd_class.h"
#include "simd_sse_type.h"
#include "simd_sse_detail_int.h"

#include <x86intrin.h>
#include <algorithm>
#include <cassert>

namespace KFP {
namespace SIMD {

template <>
inline SimdI::SimdBaseClass()
{
    data_.simd_ = Detail::constant<SimdDataI, ValueDataI>(0);
}
template <> inline SimdI::SimdBaseClass(const SimdI& class_simd)
{
    data_.simd_ = class_simd.data_.simd_;
}
// Constructor to broadcast the same value into all elements:
template <> inline SimdI::SimdBaseClass(ValueDataI val)
{
    data_.simd_ = Detail::constant<SimdDataI, ValueDataI>(val);
}
template <> inline SimdI::SimdBaseClass(ValueDataI* val)
{
    data_.simd_ = _mm_setr_epi32(val[0], val[1], val[2], val[3]);
}
template<>
template<typename T, typename std::enable_if<true, T>::type*>
inline SimdI::SimdBaseClass(SimdDataI val_simd)
{
    data_.simd_ = val_simd;
}
// ------------------------------------------------------
// Load and Store
// ------------------------------------------------------
// Load
template <> inline SimdI& SimdI::load(const ValueDataI* val_ptr)
{
    data_.simd_ = Detail::load<SimdDataI, ValueDataI>(val_ptr);
    return *this;
}
template <> inline SimdI& SimdI::load_a(const ValueDataI* val_ptr)
{
    data_.simd_ = Detail::load_a<SimdDataI, ValueDataI>(val_ptr);
    return *this;
}
template <>
inline SimdI& SimdI::load_partial(int index, const ValueDataI* val_ptr)
{
    switch (index) {
    case 0:
        data_.simd_ = _mm_setzero_si128();
        break;
    case 1:
        // data_.simd_ = _mm_loadu_si32(reinterpret_cast<const void*>(val_ptr));
        data_.simd_ = _mm_setr_epi32(val_ptr[0], 0, 0, 0);
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
template <> inline void SimdI::store(ValueDataI* val_ptr) const
{
    Detail::store<SimdDataI, ValueDataI>(data_.simd_, val_ptr);
}
template <> inline void SimdI::store_a(ValueDataI* val_ptr) const
{
    Detail::store_a<SimdDataI, ValueDataI>(data_.simd_, val_ptr);
}
template <> inline void SimdI::store_stream(ValueDataI* val_ptr) const
{
    _mm_stream_si128(reinterpret_cast<SimdDataI*>(val_ptr), data_.simd_);
}
template <>
inline void SimdI::store_partial(int index, ValueDataI* val_ptr) const
{
    if (index < 1)
        return;
    ValueDataI __KFP_SIMD__ATTR_ALIGN(__KFP_SIMD__Size_Int)
        data[__KFP_SIMD__Len_Int]{}; // Helper array
    store_a(data);
    if (index > SimdI::SimdLen) {
        index = SimdI::SimdLen;
    }
    std::copy_n(data, index, val_ptr);
}
// ------------------------------------------------------
// Gather and Scatter
// ------------------------------------------------------
template <>
inline SimdI& SimdI::gather(const SimdIndex& index,
                                const ValueDataI* val_ptr)
{
    data_.simd_ = _mm_setr_epi32( val_ptr[index[0]], val_ptr[index[1]], val_ptr[index[2]], val_ptr[index[3]]) ;
    return *this;
}
template <>
inline void SimdI::scatter(const SimdIndex& index, ValueDataI* val_ptr) const
{
    ValueDataI __KFP_SIMD__ATTR_ALIGN(__KFP_SIMD__Size_Int)
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
template <> inline SimdI& SimdI::insert(int index, ValueDataI val)
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
    const SimdDataI mask = Detail::load_a<SimdDataI, ValueDataI>(indices) ;
    data_.simd_ =
        Detail::select<SimdDataI>(mask, Detail::constant<SimdDataI, ValueDataI>(val), data_.simd_);
    return *this;
}
template <> inline ValueDataI SimdI::operator[](int index) const
{
    assert((index > -1) && ("[Error] (operator[]): invalid index (" +
                            std::to_string(index) + ") given. Negative")
                               .data());
    assert((index < SimdLen) &&
           ("[Error] (operator[]): invalid index (" + std::to_string(index) +
            ") given. Exceeds maximum")
               .data());
    return Detail::extract<ValueDataI, SimdDataI>(index, data_.simd_);
}
template <> inline SimdI& SimdI::cutoff(int index)
{
    ValueDataI __KFP_SIMD__ATTR_ALIGN(__KFP_SIMD__Size_Int)
        data[__KFP_SIMD__Len_Int]{}; // Helper array
    store_a(data);
    return load_partial(index, data);
}

inline SimdI select(const SimdMask& mask, const SimdI& a, const SimdI& b)
{
    return SimdI(
        Detail::select<SimdI::simd_type>(mask.maski(), a.simd(), b.simd()));
}

} // namespace SIMD
} // namespace KFP

#endif
