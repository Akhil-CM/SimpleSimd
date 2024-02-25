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

template<> inline SimdIndex::SimdIndexBase()
{
    index_ = _mm_setr_epi32(0, 1, 2, 3);
}
template<> inline SimdIndex::SimdIndexBase(int val)
{
    index_ = Detail::constant<SimdDataI, ValueDataI>(val);
}
template<> inline SimdIndex::SimdIndexBase(const SimdIndex& class_indices)
{
    index_ = class_indices.index_;
}
template<>
template<typename T, typename std::enable_if<!(std::is_same<int, SimdDataI>::value), T>::type*>
inline SimdIndex::SimdIndexBase(const SimdDataI& val_simd)
{
    index_ = val_simd;
}
template<> inline SimdIndex::SimdIndexBase(const SimdDataF& val_simd)
{
    index_ = _mm_cvtps_epi32(val_simd);
}

template<> inline SimdIndex& SimdIndex::operator=(int val)
{
    index_ = Detail::constant<SimdDataI, ValueDataI>(val);
    return *this;
}
template<> inline SimdIndex& SimdIndex::operator=(const SimdIndex& class_indices)
{
    index_ = class_indices.index_;
    return *this;
}
template<>
template<typename T, typename std::enable_if<!(std::is_same<int, SimdDataI>::value), T>::type*>
inline SimdIndex& SimdIndex::operator=(const SimdDataI& val_simd)
{
    index_ = val_simd;
    return *this;
}
template<> inline SimdIndex& SimdIndex::operator=(const SimdDataF& val_simd)
{
    index_ = _mm_cvtps_epi32(val_simd);
    return *this;
}

template<> inline ValueDataI SimdIndex::operator[](int index) const
{
    assert((index > -1) && ("[Error] (operator[]): invalid index (" +
                            std::to_string(index) + ") given. Negative")
                               .data());
    assert((index < SimdLen) &&
           ("[Error] (operator[]): invalid index (" + std::to_string(index) +
            ") given. Exceeds maximum")
               .data());
    return Detail::extract<ValueDataI, SimdDataI>(index, index_);
}

} // namespace SIMD
} // namespace KFP

#endif
