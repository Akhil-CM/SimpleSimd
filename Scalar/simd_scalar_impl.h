// -*- C++ Header -*-
/*
==================================================
Authors: A.Mithran;
Emails: mithran@fias.uni-frankfurt.de
==================================================
*/

#ifndef SIMD_SCALAR_IMPL_H
#define SIMD_SCALAR_IMPL_H

#include "../Base/simd_class.h"
#include "simd_scalar_type.h"
#include "simd_scalar_detail.h"

#include <cassert>

namespace KFP {
namespace SIMD {

template<typename ValueType, Tag tag>
SimdClassBase<ValueType, tag>::SimdClassBase()
{
    data_.simd_ = ValueType{ 0 };
}
template<typename ValueType, Tag tag>
SimdClassBase<ValueType, tag>::SimdClassBase(const SimdClassBase& class_simd)
{
    data_.simd_ = class_simd.data_.simd_;
}
template<typename ValueType, Tag tag>
SimdClassBase<ValueType, tag>::SimdClassBase(ValueType val)
{
    data_.simd_ = val;
}

template<typename ValueType, Tag tag>
SimdClassBase<ValueType, tag>& SimdClassBase<ValueType, tag>::operator=(const SimdClassBase& class_simd)
{
    data_.simd_ = class_simd.data_.simd_;
    return *this;
}
template<typename ValueType, Tag tag>
SimdClassBase<ValueType, tag>& SimdClassBase<ValueType, tag>::operator=(ValueType val)
{
    data_.simd_ = val;
    return *this;
}

template<typename ValueType, Tag tag>
SimdClassBase<ValueType, tag>& SimdClassBase<ValueType, tag>::load(const ValueType* p)
{
    data_.simd_ = *p;
    return *this;
}
template<typename ValueType, Tag tag>
SimdClassBase<ValueType, tag>& SimdClassBase<ValueType, tag>::load_a(const ValueType* p)
{
    data_.simd_ = *p;
    return *this;
}
template<typename ValueType, Tag tag>
SimdClassBase<ValueType, tag>& SimdClassBase<ValueType, tag>::load_partial(int index, const ValueType* p)
{
    assert((index > 0) && ("[Error] (load_partial): invalid index (" +
                           std::to_string(index) + ") given. Not postive.")
                              .data());
    if (index < 1)
        return *this;
    data_.simd_ = *p;
    return *this;
}

template<typename ValueType, Tag tag>
void SimdClassBase<ValueType, tag>::store(ValueType* p) const
{
    *p = data_.simd_;
}
template<typename ValueType, Tag tag>
void SimdClassBase<ValueType, tag>::store_a(ValueType* p) const
{
    *p = data_.simd_;
}
template<typename ValueType, Tag tag>
void SimdClassBase<ValueType, tag>::store_stream(ValueType* p) const
{
    *p = data_.simd_;
}
template<typename ValueType, Tag tag>
void SimdClassBase<ValueType, tag>::store_partial(int index, ValueType* p) const
{
    assert((index > 0) && ("[Error] (store_partial): invalid index (" +
                           std::to_string(index) + ") given. Not positive.")
                              .data());
    if (index < 1)
        return;
    *p = data_.simd_;
}

template<typename ValueType, Tag tag>
SimdClassBase<ValueType, tag>& SimdClassBase<ValueType, tag>::gather(const SimdIndexBase<tag>& indices, const ValueType* p)
{
    data_.simd_ = p[indices[0]];
    return *this;
}
template<typename ValueType, Tag tag>
void SimdClassBase<ValueType, tag>::scatter(const SimdIndexBase<tag>& indices, ValueType* p) const
{
    p[indices[0]] = data_.simd_;
}

template<typename ValueType, Tag tag>
SimdClassBase<ValueType, tag>& SimdClassBase<ValueType, tag>::insert(int index, ValueType val)
{
    assert((index == 0) &&
           ("[Error] (insert): invalid index (" + std::to_string(index) +
            ") given. Non-zero index not allowed.")
               .data());
    if (index != 0)
        return *this;
    data_.simd_ = val;
    return *this;
}

template<typename ValueType, Tag tag>
ValueType SimdClassBase<ValueType, tag>::operator[](int index) const
{
    assert((index == 0) &&
           ("[Error] (operator[]): invalid index (" +
            std::to_string(index) + ") given. Non-zero index not allowed.")
               .data());
    if (index != 0) {
        std::cerr
            << "[Error] (operator[]): invalid element access. Non zero index for Scalar simd type.\n";
        exit(1);
    }
    return data_.simd_;
}
template<typename ValueType, Tag tag>
SimdClassBase<ValueType, tag>& SimdClassBase<ValueType, tag>::cutoff(int index)
{
    assert((index > -1) && ("[Error] (cutoff): invalid index (" +
                            std::to_string(index) + ") given. Negative")
                               .data());
    if (index == 0){
        data_.simd_ = ValueType{ 0 };
    }
    return *this;
}

inline SimdF select(const SimdMask& mask, const SimdF& a, const SimdF& b)
{
    return SimdF{(mask.AND() ? a : b)};
}

inline SimdI select(const SimdMask& mask, const SimdI& a, const SimdI& b)
{
    return SimdI{(mask.AND() ? a : b)};
}

} // namespace SIMD
} // namespace KFP

#endif
