// -*- C++ Header -*-
/*
==================================================
Authors: A.Mithran;
Emails: mithran@fias.uni-frankfurt.de
==================================================
*/

#ifndef SIMD_CLASS_H
#define SIMD_CLASS_H

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

template <typename ValueType, Tag tag> class SimdClassBase
{
public:
    // static_assert(validateTag(tag), "[Error]: KFP::SIMD::SimdClassBase given invalid tag.") ;
    static_assert(
        std::is_arithmetic<ValueType>::value,
        "[Error]: KFP::SIMD::SimdClassBase only works for arithmetic types.");
    static_assert(
        std::is_same<ValueType, float>::value ||
            std::is_same<ValueType, int>::value,
        "[Error]: KFP::SIMD::SimdClassBase only works for int and float types.");

    typedef ValueType value_type;
    typedef typename SimdData<ValueType, tag>::simd_type simd_type;
    static constexpr int SimdSize{ sizeof(simd_type) };
    static constexpr int SimdLen{ SimdSize / sizeof(ValueType) };

    // ------------------------------------------------------
    // Constructors
    // ------------------------------------------------------
    // Default constructor:
    // SimdClassBase() = default ;
    SimdClassBase();
    SimdClassBase(const SimdClassBase& class_simd);
    // Constructor to broadcast the same value into all elements:
    SimdClassBase(ValueType val);
    SimdClassBase(ValueType* val);
    template <typename T = void,
              typename std::enable_if<!(tag == Tag::Scalar), T>::type* = nullptr>
    SimdClassBase(simd_type val_simd);

    // Assignment constructors:
    SimdClassBase& operator=(const SimdClassBase& class_simd);
    SimdClassBase& operator=(ValueType val);
    template <typename T = void,
              typename std::enable_if<!(tag == Tag::Scalar), T>::type* = nullptr>
    SimdClassBase& operator=(simd_type val_simd);

    // ------------------------------------------------------
    // Load and Store
    // ------------------------------------------------------
    // Member function to load from array (unaligned)
    SimdClassBase& load(const ValueType* p);
    SimdClassBase& load_a(const ValueType* p);
    // Partial load. Load n elements and set the rest to 0
    SimdClassBase& load_partial(int index, const ValueType* p);

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
    SimdClassBase& gather(const SimdIndexBase<tag>& indices, const ValueType* p);
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
    SimdClassBase& insert(int index, ValueType val);
    // Extract a single element. Use store function if extracting more than one element.
    // Operator [] can only read an element, not write.
    ValueType operator[](int index) const;
    // cut off vector to n elements. The last 4-n elements are set to zero
    SimdClassBase& cutoff(int index);

    // ------------------------------------------------------
    // Print I/O
    // ------------------------------------------------------
    friend std::ostream& operator<<(std::ostream& stream,
                                    const SimdClassBase& s)
    {
        Detail::print<simd_type>(stream, s.data_.simd_);
        return stream;
    }

    // ------------------------------------------------------
    // Basic Arithmetic
    // ------------------------------------------------------
    friend SimdClassBase operator-(const SimdClassBase& a)
    {
        return SimdClassBase{ Detail::minus<simd_type>(a.data_.simd_) };
    }
    friend SimdClassBase operator+(const SimdClassBase& a,
                                   const SimdClassBase& b)
    {
        return SimdClassBase{ Detail::add<simd_type>(a.data_.simd_,
                                                     b.data_.simd_) };
    }
    friend SimdClassBase operator-(const SimdClassBase& a,
                                   const SimdClassBase& b)
    {
        return SimdClassBase{ Detail::substract<simd_type>(a.data_.simd_,
                                                           b.data_.simd_) };
    }
    friend SimdClassBase operator*(const SimdClassBase& a,
                                   const SimdClassBase& b)
    {
        return SimdClassBase{ Detail::multiply<simd_type>(a.data_.simd_,
                                                          b.data_.simd_) };
    }
    friend SimdClassBase operator/(const SimdClassBase& a,
                                   const SimdClassBase& b)
    {
        return SimdClassBase{ Detail::divide<simd_type>(a.data_.simd_,
                                                        b.data_.simd_) };
    }

    friend SimdClassBase min(const SimdClassBase& a, const SimdClassBase& b)
    {
        return SimdClassBase{ Detail::min<simd_type>(a.data_.simd_,
                                                     b.data_.simd_) };
    }
    friend SimdClassBase max(const SimdClassBase& a, const SimdClassBase& b)
    {
        return SimdClassBase{ Detail::max<simd_type>(a.data_.simd_,
                                                     b.data_.simd_) };
    }

    friend SimdClassBase sqrt(const SimdClassBase& a)
    {
        return SimdClassBase{ Detail::sqrt<simd_type>(a.data_.simd_) };
    }

    /* Reciprocal( inverse) Square Root */
    friend SimdClassBase rsqrt(const SimdClassBase& a)
    {
        return SimdClassBase{ Detail::rsqrt<simd_type>(a.data_.simd_) };
    }

    friend SimdClassBase abs(const SimdClassBase& a)
    {
        return SimdClassBase{ Detail::abs<simd_type>(a.data_.simd_) };
    }

    friend SimdClassBase log(const SimdClassBase& a)
    {
        return SimdClassBase{ Detail::log<simd_type>(a.data_.simd_) };
    }

    friend SimdClassBase pow(const SimdClassBase& a, int exp)
    {
        return SimdClassBase{ Detail::pow<simd_type>(a.data_.simd_, exp) };
    }

    /* Logical */
    // friend SimdClassBase operator&(const SimdClassBase& a,
    //                                const SimdClassBase& b)
    // {
    //     return SimdClassBase{Detail::opLessThan<simd_type>(a.data_.simd_, b.data_.simd_)} ;
    // }
    // friend SimdClassBase operator|(const SimdClassBase& a,
    //                                const SimdClassBase& b)
    // {
    //     return SimdClassBase{Detail::opLessThanEqual<simd_type>(a.data_.simd_, b.data_.simd_)} ;
    // }
    // friend SimdClassBase operator^(const SimdClassBase& a,
    //                                const SimdClassBase& b)
    // {
    //     return SimdClassBase{Detail::opLessThanEqual<simd_type>(a.data_.simd_, b.data_.simd_)} ;
    //     return SimdClassBase(a.data_.simd_ ^ b.data_.simd_);
    // }
    // friend SimdClassBase operator!(const SimdClassBase& a)
    // {
    //     return SimdClassBase(!a.data_.simd_);
    // }
    // friend SimdClassBase operator||( const SimdClassBase &a, const SimdClassBase &b ){ // mask returned
    //   return _mm_or_ps(a, b);
    // }

    /* Comparison */
    friend SimdMaskBase<tag> operator<(const SimdClassBase& a,
                                   const SimdClassBase& b)
    { // mask returned
        return SimdMaskBase<tag>{Detail::opLessThan<simd_type>(a.data_.simd_,
                                                            b.data_.simd_)};
    }
    friend SimdMaskBase<tag> operator<=(const SimdClassBase& a,
                                    const SimdClassBase& b)
    { // mask returned
        return SimdMaskBase<tag>{Detail::opLessThanEqual<simd_type>(
            a.data_.simd_, b.data_.simd_)};
    }
    friend SimdMaskBase<tag> operator>(const SimdClassBase& a,
                                   const SimdClassBase& b)
    { // mask returned
        return SimdMaskBase<tag>{Detail::opGreaterThan<simd_type>(a.data_.simd_,
                                                               b.data_.simd_)};
    }
    friend SimdMaskBase<tag> operator>=(const SimdClassBase& a,
                                    const SimdClassBase& b)
    { // mask returned
        return SimdMaskBase<tag>{Detail::opGreaterThanEqual<simd_type>(
            a.data_.simd_, b.data_.simd_)};
    }
    friend SimdMaskBase<tag> operator==(const SimdClassBase& a,
                                    const SimdClassBase& b)
    { // mask returned
        return SimdMaskBase<tag>{Detail::opEqual<simd_type>(a.data_.simd_,
                                                         b.data_.simd_)};
    }

    friend SimdMaskBase<tag> operator!=(const SimdClassBase& a,
                                    const SimdClassBase& b)
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
