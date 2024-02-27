// -*- C++ Header -*-
/*
==================================================
Authors: A.Mithran;
Emails: mithran@fias.uni-frankfurt.de
==================================================
*/

#ifndef SIMD_INDEX_H
#define SIMD_INDEX_H

#include "simd_tag.h"
#include "simd_data.h"
#include "simd_detail.h"
#include "simd_mask.h"

#include <iosfwd>
#include <type_traits>

namespace KFP {
namespace SIMD {

template <typename ValueType, Tag tag>
class SimdClassBase;
template <Tag tag> class SimdIndexBase
{
public:
    typedef typename SimdData<int, tag>::simd_type simd_typei;
    typedef typename SimdData<float, tag>::simd_type simd_typef;
    static constexpr int SimdSize{ sizeof(simd_typei) };
    static constexpr int SimdLen{ SimdSize / sizeof(int) };
    // ------------------------------------------------------
    // Constructors
    // ------------------------------------------------------
    // Default constructor:
    SimdIndexBase();
    SimdIndexBase(int val);
    SimdIndexBase(const SimdIndexBase& class_indices);
    template<typename T=void, typename std::enable_if<!(std::is_same<int, simd_typei>::value), T>::type* = nullptr>
    SimdIndexBase(const simd_typei& val_simd);
    SimdIndexBase(const simd_typef& val_simd);
    SimdIndexBase(const SimdClassBase<int, tag>& class_simd);
    SimdIndexBase(const SimdClassBase<float, tag>& class_simd);
    // SimdIndexBase(const int* val_ptr)
    // {
    //     _mm_store_si128(reinterpret_cast<simd_type*>(data_), _mm_set1_epi32(val)) ;
    //     std::copy_n(class_indices.data_, SimdLen, data_) ;
    // }

    // Assignment operator
    SimdIndexBase& operator=(int val);
    SimdIndexBase& operator=(const SimdIndexBase& class_indices);
    template<typename T=void, typename std::enable_if<!(std::is_same<int, simd_typei>::value), T>::type* = nullptr>
    SimdIndexBase& operator=(const simd_typei& val_simd);
    SimdIndexBase& operator=(const simd_typef& val_simd);

    // ------------------------------------------------------
    // Data member accessors
    // ------------------------------------------------------
    int operator[](int index) const;

    simd_typei& index()
    {
        return index_;
    }
    const simd_typei& index() const
    {
        return index_;
    }
    // ------------------------------------------------------
    // Print I/O
    // ------------------------------------------------------
    friend std::ostream& operator<<(std::ostream& stream, const SimdIndexBase& s)
    {
        Detail::print<simd_typei>(stream, s.index_);
        return stream;
    }

    // ------------------------------------------------------
    // Basic Arithmetic
    // ------------------------------------------------------
    friend SimdIndexBase operator+(const SimdIndexBase& a, const SimdIndexBase& b)
    {
        return SimdIndexBase{Detail::add<simd_typei>(a.index_, b.index_)};
    }
    friend SimdIndexBase operator+(int val, const SimdIndexBase& a)
    {
        return SimdIndexBase{Detail::add<simd_typei, int, simd_typei>(val, a.index_)};
    }
    friend SimdIndexBase operator+(const SimdIndexBase& a, int val)
    {
        return SimdIndexBase{Detail::add<simd_typei, int, simd_typei>(val, a.index_)};
    }
    friend SimdIndexBase operator-(const SimdIndexBase& a, const SimdIndexBase& b)
    {
        return SimdIndexBase{Detail::substract<simd_typei>(a.index_, b.index_)};
    }
    friend SimdIndexBase operator-(int val, const SimdIndexBase& a)
    {
        return SimdIndexBase{Detail::substract<simd_typei, int, simd_typei>(val, a.index_)};
    }
    friend SimdIndexBase operator-(const SimdIndexBase& a, int val)
    {
        return SimdIndexBase{Detail::substract<simd_typei, simd_typei, int>(a.index_, val)};
    }
    friend SimdIndexBase operator*(const SimdIndexBase& a, const SimdIndexBase& b)
    {
        return SimdIndexBase{Detail::multiply<simd_typei>(a.index_, b.index_)};
    }
    friend SimdIndexBase operator*(int val, const SimdIndexBase& a)
    {
        return SimdIndexBase{Detail::multiply<simd_typei, int, simd_typei>(val, a.index_)};
    }
    friend SimdIndexBase operator*(const SimdIndexBase& a, int val)
    {
        return SimdIndexBase{Detail::multiply<simd_typei, int, simd_typei>(val, a.index_)};
    }
    friend SimdIndexBase operator/(const SimdIndexBase& a, const SimdIndexBase& b)
    {
        return SimdIndexBase{Detail::divide<simd_typei>(a.index_, b.index_)};
    }
    friend SimdIndexBase operator/(int val, const SimdIndexBase& a)
    {
        return SimdIndexBase{Detail::divide<simd_typei, int, simd_typei>(val, a.index_)};
    }
    friend SimdIndexBase operator/(const SimdIndexBase& a, int val)
    {
        return SimdIndexBase{Detail::divide<simd_typei, simd_typei, int>(a.index_, val)};
    }

    /* Comparison */
    friend SimdMaskBase<tag> operator<(const SimdIndexBase& a, const SimdIndexBase& b)
    { // mask returned
        return SimdMaskBase<tag>{Detail::opLessThan<simd_typei>(a.index_, b.index_)};
    }
    friend SimdMaskBase<tag> operator<=(const SimdIndexBase& a, const SimdIndexBase& b)
    { // mask returned
        return SimdMaskBase<tag>{Detail::opLessThanEqual<simd_typei>(a.index_, b.index_)};
    }
    friend SimdMaskBase<tag> operator>(const SimdIndexBase& a, const SimdIndexBase& b)
    { // mask returned
        return SimdMaskBase<tag>{Detail::opGreaterThan<simd_typei>(a.index_, b.index_)};
    }
    friend SimdMaskBase<tag> operator>=(const SimdIndexBase& a, const SimdIndexBase& b)
    { // mask returned
        return SimdMaskBase<tag>{Detail::opGreaterThanEqual<simd_typei>(a.index_, b.index_)};
    }
    friend SimdMaskBase<tag> operator==(const SimdIndexBase& a, const SimdIndexBase& b)
    { // mask returned
        return SimdMaskBase<tag>{Detail::opEqual<simd_typei>(a.index_, b.index_)};
    }
    friend SimdMaskBase<tag> operator!=(const SimdIndexBase& a, const SimdIndexBase& b)
    { // mask returned
        return SimdMaskBase<tag>{Detail::opNotEqual<simd_typei>(a.index_, b.index_)};
    }
    friend SimdMaskBase<tag> operator<(const SimdIndexBase& a, int val)
    { // mask returned
        return (a < SimdIndexBase{ val });
    }
    friend SimdMaskBase<tag> operator<=(const SimdIndexBase& a, int val)
    { // mask returned
        return (a <= SimdIndexBase{ val });
    }
    friend SimdMaskBase<tag> operator>(const SimdIndexBase& a, int val)
    { // mask returned
        return (a > SimdIndexBase{ val });
    }
    friend SimdMaskBase<tag> operator>=(const SimdIndexBase& a, int val)
    { // mask returned
        return (a >= SimdIndexBase{ val });
    }
    friend SimdMaskBase<tag> operator==(const SimdIndexBase& a, int val)
    { // mask returned
        return (a == SimdIndexBase{ val });
    }
    friend SimdMaskBase<tag> operator!=(const SimdIndexBase& a, int val)
    { // mask returned
        return (a != SimdIndexBase{ val });
    }
    friend SimdMaskBase<tag> operator<(int val, const SimdIndexBase& a)
    { // mask returned
        return (SimdIndexBase{ val } < a);
    }
    friend SimdMaskBase<tag> operator<=(int val, const SimdIndexBase& a)
    { // mask returned
        return (SimdIndexBase{ val } <= a);
    }
    friend SimdMaskBase<tag> operator>(int val, const SimdIndexBase& a)
    { // mask returned
        return (SimdIndexBase{ val } > a);
    }
    friend SimdMaskBase<tag> operator>=(int val, const SimdIndexBase& a)
    { // mask returned
        return (SimdIndexBase{ val } >= a);
    }
    friend SimdMaskBase<tag> operator==(int val, const SimdIndexBase& a)
    { // mask returned
        return (SimdIndexBase{ val } == a);
    }
    friend SimdMaskBase<tag> operator!=(int val, const SimdIndexBase& a)
    { // mask returned
        return (SimdIndexBase{ val } != a);
    }

protected:
    simd_typei index_;
    // int __KFP_SIMD__ATTR_ALIGN(
    //     __KFP_SIMD__Size_Int) indices_[__KFP_SIMD__Len_Int]{}; // Indices array
    // __KFP_SIMD__SPEC_ALIGN(__KFP_SIMD__Size_Int) int indices_[__KFP_SIMD__Len_Int]{};
};

} // namespace SIMD
} // namespace KFP

#endif
