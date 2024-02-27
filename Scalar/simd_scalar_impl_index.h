// -*- C++ Header -*-
/*
==================================================
Authors: A.Mithran;
Emails: mithran@fias.uni-frankfurt.de
==================================================
*/

#ifndef SIMD_SCALAR_IMPL_INDEX_H
#define SIMD_SCALAR_IMPL_INDEX_H

#include "../Base/simd_index.h"
#include "simd_scalar_type.h"
#include "simd_scalar_detail_index.h"

#include <iostream>
#include <cassert>

namespace KFP {
namespace SIMD {

template<> inline SimdIndex::SimdIndexBase()
{
    index_ = 0;
}
template<> inline SimdIndex::SimdIndexBase(int val)
{
    index_ = val;
}
template<> inline SimdIndex::SimdIndexBase(const SimdIndex& class_indices)
{
    index_ = class_indices.index_;
}
template<> inline SimdIndex::SimdIndexBase(const SimdDataF& val_simd)
{
    index_ = int(val_simd);
}
template<>
inline SimdIndex::SimdIndexBase(const SimdI& class_simd)
{
    index_ = class_simd.simd();
}
template<> inline SimdIndex::SimdIndexBase(const SimdF& class_simd)
{
    index_ = static_cast<int>(class_simd.simd());
}

template<> inline SimdIndex& SimdIndex::operator=(int val)
{
    index_ = val;
    return *this;
}
template<> inline SimdIndex& SimdIndex::operator=(const SimdIndex& class_indices)
{
    index_ = class_indices.index_;
    return *this;
}
template<> inline SimdIndex& SimdIndex::operator=(const SimdDataF& val_simd)
{
    index_ = int(val_simd);
    return *this;
}

template<> inline ValueDataI SimdIndex::operator[](int index) const
{
    assert((index == 0) && ("[Error] (operator[]): invalid index (" +
                            std::to_string(index) + ") given. Only zero is allowed.")
                               .data());
    return index_;
}

} // namespace SIMD
} // namespace KFP

#endif
