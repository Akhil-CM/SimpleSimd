// -*- C++ Header -*-
/*
==================================================
Authors: A.Mithran;
Emails: mithran@fias.uni-frankfurt.de
==================================================
*/

#ifndef SIMD_BASE_H
#define SIMD_BASE_H

#include "simd_macros.h"
#include "simd_tag.h"
#include "simd_data.h"
#include "simd_mask.h"
#include "simd_index.h"
#include "simd_detail.h"

#include <iosfwd>
#include <type_traits>

namespace KFP {
namespace SIMD {

template <typename ValueType, Tag tag> class SimdBaseClass
{
public:
    // static_assert(validateTag(tag), "[Error]: KFP::SIMD::SimdBaseClass given invalid tag.") ;
    static_assert(
        std::is_arithmetic<ValueType>::value,
        "[Error]: KFP::SIMD::SimdBaseClass only works for arithmetic types.");
    static_assert(
        std::is_same<ValueType, float>::value ||
            std::is_same<ValueType, int>::value,
        "[Error]: KFP::SIMD::SimdBaseClass only works for int and float types.");

    typedef ValueType value_type;
    typedef typename SimdData<ValueType, tag>::simd_type simd_type;
    static constexpr int SimdSize{ sizeof(simd_type) };
    static constexpr int SimdLen{ SimdSize / sizeof(ValueType) };
    static_assert(
        (SimdSize == __KFP_SIMD__Size_Float),
        "[Error]: KFP::SIMD::SimdBaseClass given invalid size of simd type.");
    static_assert(
        (SimdLen == __KFP_SIMD__Len_Float),
        "[Error]: KFP::SIMD::SimdBaseClass given invalid size of simd type.");

    // ------------------------------------------------------
    // Constructors
    // ------------------------------------------------------
    // Default constructor:
    // SimdBaseClass() = default ;
    SimdBaseClass();
    SimdBaseClass(const SimdBaseClass& class_simd);
    // Constructor to broadcast the same value into all elements:
    SimdBaseClass(ValueType val);
    SimdBaseClass(ValueType* val);
    template <typename T = void,
              typename std::enable_if<!(tag == Tag::Scalar), T>::type* = nullptr>
    SimdBaseClass(simd_type val_simd);

    // Assignment constructors:
    SimdBaseClass& operator=(const SimdBaseClass& class_simd);
    SimdBaseClass& operator=(ValueType val);
    template <typename T = void,
              typename std::enable_if<!(tag == Tag::Scalar), T>::type* = nullptr>
    SimdBaseClass& operator=(simd_type val_simd);

    // ------------------------------------------------------
    // Load and Store
    // ------------------------------------------------------
    // Member function to load from array (unaligned)
    SimdBaseClass& load(const ValueType* p);
    SimdBaseClass& load_a(const ValueType* p);
    // Partial load. Load n elements and set the rest to 0
    SimdBaseClass& load_partial(int index, const ValueType* p);

    // Member function to store into array (unaligned)
    void store(ValueType* p) const;
    // Member function storing into array, aligned by 16
    // "store_a" is faster than "store" on older Intel processors (Pentium 4, Pentium M, Core 1,
    // Merom, Wolfdale) and Atom, but not on other processors from Intel, AMD or VIA.
    // You may use store_a instead of store if you are certain that p points to an address
    // divisible by 16.
    void store_a(ValueType* p) const;
    // Member function storing to aligned uncached memory (non-temporal store).
    // This may be more efficient than store_a when storing large blocks of memory if it
    // is unlikely that the data will stay in the cache until it is read again.
    // Note: Will generate runtime error if p is not aligned by 16
    void store_stream(ValueType* p) const;
    // Partial store. Store n elements
    void store_partial(int index, ValueType* p) const;

    // ------------------------------------------------------
    // Gather and Scatter
    // ------------------------------------------------------
    SimdBaseClass& gather(const SimdIndexBase<tag>& indices, const ValueType* p);
    void scatter(const SimdIndexBase<tag>& indices, ValueType* p) const;

    // ------------------------------------------------------
    // Data member accessors
    // ------------------------------------------------------
    simd_type& simd()
    {
        return data_.simd_;
    }
    const simd_type& simd() const
    {
        return data_.simd_;
    }

    // ------------------------------------------------------
    // Individual Scalar manipulation
    // ------------------------------------------------------
    // Member function to change a single element in vector
    SimdBaseClass& insert(int index, ValueType val);
    // Extract a single element. Use store function if extracting more than one element.
    // Operator [] can only read an element, not write.
    ValueType operator[](int index) const;
    // cut off vector to n elements. The last 4-n elements are set to zero
    SimdBaseClass& cutoff(int index);

    // ------------------------------------------------------
    // Print I/O
    // ------------------------------------------------------
    friend std::ostream& operator<<(std::ostream& stream,
                                    const SimdBaseClass& s)
    {
        Detail::print<simd_type>(stream, s.data_.simd_);
        return stream;
    }

    // ------------------------------------------------------
    // Basic Arithmetic
    // ------------------------------------------------------
    friend SimdBaseClass operator-(const SimdBaseClass& a)
    {
        return SimdBaseClass{ Detail::minus<simd_type>(a.data_.simd_) };
    }
    friend SimdBaseClass operator+(const SimdBaseClass& a,
                                   const SimdBaseClass& b)
    {
        return SimdBaseClass{ Detail::add<simd_type>(a.data_.simd_,
                                                     b.data_.simd_) };
    }
    friend SimdBaseClass operator-(const SimdBaseClass& a,
                                   const SimdBaseClass& b)
    {
        return SimdBaseClass{ Detail::substract<simd_type>(a.data_.simd_,
                                                           b.data_.simd_) };
    }
    friend SimdBaseClass operator*(const SimdBaseClass& a,
                                   const SimdBaseClass& b)
    {
        return SimdBaseClass{ Detail::multiply<simd_type>(a.data_.simd_,
                                                          b.data_.simd_) };
    }
    friend SimdBaseClass operator/(const SimdBaseClass& a,
                                   const SimdBaseClass& b)
    {
        return SimdBaseClass{ Detail::divide<simd_type>(a.data_.simd_,
                                                        b.data_.simd_) };
    }

    friend SimdBaseClass min(const SimdBaseClass& a, const SimdBaseClass& b)
    {
        return SimdBaseClass{ Detail::min<simd_type>(a.data_.simd_,
                                                     b.data_.simd_) };
    }
    friend SimdBaseClass max(const SimdBaseClass& a, const SimdBaseClass& b)
    {
        return SimdBaseClass{ Detail::max<simd_type>(a.data_.simd_,
                                                     b.data_.simd_) };
    }

    friend SimdBaseClass sqrt(const SimdBaseClass& a)
    {
        return SimdBaseClass{ Detail::sqrt<simd_type>(a.data_.simd_) };
    }

    /* Reciprocal( inverse) Square Root */
    friend SimdBaseClass rsqrt(const SimdBaseClass& a)
    {
        return SimdBaseClass{ Detail::rsqrt<simd_type>(a.data_.simd_) };
    }

    friend SimdBaseClass abs(const SimdBaseClass& a)
    {
        return SimdBaseClass{ Detail::abs<simd_type>(a.data_.simd_) };
    }

    friend SimdBaseClass log(const SimdBaseClass& a)
    {
        return SimdBaseClass{ Detail::log<simd_type>(a.data_.simd_) };
    }

    friend SimdBaseClass pow(const SimdBaseClass& a, int exp)
    {
        return SimdBaseClass{ Detail::pow<simd_type>(a.data_.simd_, exp) };
    }

    /* Logical */
    // friend SimdBaseClass operator&(const SimdBaseClass& a,
    //                                const SimdBaseClass& b)
    // {
    //     return SimdBaseClass{Detail::opLessThan<simd_type>(a.data_.simd_, b.data_.simd_)} ;
    // }
    // friend SimdBaseClass operator|(const SimdBaseClass& a,
    //                                const SimdBaseClass& b)
    // {
    //     return SimdBaseClass{Detail::opLessThanEqual<simd_type>(a.data_.simd_, b.data_.simd_)} ;
    // }
    // friend SimdBaseClass operator^(const SimdBaseClass& a,
    //                                const SimdBaseClass& b)
    // {
    //     return SimdBaseClass{Detail::opLessThanEqual<simd_type>(a.data_.simd_, b.data_.simd_)} ;
    //     return SimdBaseClass(a.data_.simd_ ^ b.data_.simd_);
    // }
    // friend SimdBaseClass operator!(const SimdBaseClass& a)
    // {
    //     return SimdBaseClass(!a.data_.simd_);
    // }
    // friend SimdBaseClass operator||( const SimdBaseClass &a, const SimdBaseClass &b ){ // mask returned
    //   return _mm_or_ps(a, b);
    // }

    /* Comparison */
    friend SimdMaskBase<tag> operator<(const SimdBaseClass& a,
                                   const SimdBaseClass& b)
    { // mask returned
        return SimdMaskBase<tag>{Detail::opLessThan<simd_type>(a.data_.simd_,
                                                            b.data_.simd_)};
    }
    friend SimdMaskBase<tag> operator<=(const SimdBaseClass& a,
                                    const SimdBaseClass& b)
    { // mask returned
        return SimdMaskBase<tag>{Detail::opLessThanEqual<simd_type>(
            a.data_.simd_, b.data_.simd_)};
    }
    friend SimdMaskBase<tag> operator>(const SimdBaseClass& a,
                                   const SimdBaseClass& b)
    { // mask returned
        return SimdMaskBase<tag>{Detail::opGreaterThan<simd_type>(a.data_.simd_,
                                                               b.data_.simd_)};
    }
    friend SimdMaskBase<tag> operator>=(const SimdBaseClass& a,
                                    const SimdBaseClass& b)
    { // mask returned
        return SimdMaskBase<tag>{Detail::opGreaterThanEqual<simd_type>(
            a.data_.simd_, b.data_.simd_)};
    }
    friend SimdMaskBase<tag> operator==(const SimdBaseClass& a,
                                    const SimdBaseClass& b)
    { // mask returned
        return SimdMaskBase<tag>{Detail::opEqual<simd_type>(a.data_.simd_,
                                                         b.data_.simd_)};
    }

    friend SimdMaskBase<tag> operator!=(const SimdBaseClass& a,
                                    const SimdBaseClass& b)
    { // mask returned
        return SimdMaskBase<tag>{Detail::opNotEqual<simd_type>(a.data_.simd_,
                                                            b.data_.simd_)};
    }

protected:
    SimdData<ValueType, tag> data_;
};

} // namespace SIMD
} // namespace KFP

#endif // !SIMD_BASE_H
