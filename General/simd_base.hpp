// -*- C++ Header -*-
/*
==================================================
Authors: A.Mithran;
Emails: mithran@fias.uni-frankfurt.de
==================================================
*/

#pragma once

#include "simd_abi.hpp"
#include "simd_detail.hpp"
#include "simd_mask.hpp"
#include "simd_index.hpp"
#include <iostream>
#include <type_traits>

namespace KFP {
namespace SIMD {

template <typename ValueType, typename SimdType, ABI ABIVal>
class SimdBaseClass;

template <typename ValueType, typename SimdType, ABI ABIVal> class SimdBaseClass
{
    static_assert(validABI(ABIVal), "[Error]: KFP::SIMD given invalid ABI tag.") ;
    static_assert(
        std::is_fundamental<ValueType>::value &&
            std::is_arithmetic<ValueType>::value,
        "[Error] : KFP::SIMD Only works for fundamental arithmetic types.");
    // using SimdType = typename std::conditional<
    //         std::is_integral<ValueType>::value, __m128i,
    //         typename std::conditional<std::is_same<ValueType, float>::value, __m128,
    //                                   __m128d>::type>::type ;
    // static_assert((SimdLen == 4),
    //               "[Error] : KFP::SIMD SSE type has invalid length");

public:
    typedef ValueType value_type;
    typedef SimdType simd_type;
    static constexpr int SimdLen{ sizeof(SimdType) / sizeof(ValueType) };
    static constexpr int SimdSize{ SimdLen * sizeof(ValueType) };
    static constexpr ABI SimdABI()
    { return ABIVal ; }

    // ------------------------------------------------------
    // Constructors
    // ------------------------------------------------------
    // Default constructor:
    SimdBaseClass() = default;
    SimdBaseClass(const SimdBaseClass& class_simd)
    {
        dataSimd_ = class_simd.dataSimd_;
    }

    // Constructor to broadcast the same value into all elements:
    SimdBaseClass(ValueType val)
    {
        Detail::constant<SimdType, ValueType>(dataSimd_, val);
    }
    SimdBaseClass(SimdType val_simd)
    {
        dataSimd_ = val_simd;
    }

    // Assignment operator to convert from type __m128 used in intrinsics:
    SimdBaseClass& operator=(ValueType val)
    {
        Detail::constant<SimdType, ValueType>(dataSimd_, val);
        return *this;
    }
    SimdBaseClass& operator=(SimdType val_simd)
    {
        dataSimd_ = val_simd;
        return *this;
    }

    // ------------------------------------------------------
    // Load and Store
    // ------------------------------------------------------
    // Member function to load from array (unaligned)
    SimdBaseClass& load(const ValueType* p)
    {
        Detail::load<SimdType, ValueType>(dataSimd_, p);
        return *this;
    }

    SimdBaseClass& load_a(const ValueType* p)
    {
        Detail::load_a<SimdType, ValueType>(dataSimd_, p);
        return *this;
    }

    // Member function to store into array (unaligned)
    void store(ValueType* p) const
    {
        Detail::store<SimdType, ValueType>(dataSimd_, p);
    }

    // Member function storing into array, aligned by 16
    // "store_a" is faster than "store" on older Intel processors (Pentium 4, Pentium M, Core 1,
    // Merom, Wolfdale) and Atom, but not on other processors from Intel, AMD or VIA.
    // You may use store_a instead of store if you are certain that p points to an address
    // divisible by 16.
    void store_a(ValueType* p) const
    {
        Detail::store_a<SimdType, ValueType>(dataSimd_, p);
    }

    // Member function storing to aligned uncached memory (non-temporal store).
    // This may be more efficient than store_a when storing large blocks of memory if it
    // is unlikely that the data will stay in the cache until it is read again.
    // Note: Will generate runtime error if p is not aligned by 16
    void store_stream(ValueType* p) const
    {
        Detail::store_stream<SimdType, ValueType>(dataSimd_, p);
    }

    // Partial load. Load n elements and set the rest to 0
    SimdBaseClass& load_partial(int index, const ValueType* p)
    {
        Detail::load_partial<SimdType, ValueType>(dataSimd_, index, p);
        return *this;
    }

    // Partial store. Store n elements
    void store_partial(int index, ValueType* p) const
    {
        Detail::store_partial<SimdBaseClass, ValueType>(*this, index, p);
    }

    // ------------------------------------------------------
    // Gather and Scatter
    // ------------------------------------------------------
    SimdBaseClass& gather(const SimdIndex& indices, const ValueType* p)
    {
        Detail::gather<SimdType, ValueType>(dataSimd_, indices, p);
        return *this;
    }

    template<typename T>
    SimdBaseClass& scatter(SimdBaseClass<int, T, ABIVal> indices, ValueType* p)
    {
        Detail::scatter<SimdType, SimdBaseClass<int, T, ABIVal>, ValueType>(dataSimd_, indices, p);
        return *this;
    }

    // ------------------------------------------------------
    // Data member accessors
    // ------------------------------------------------------
    ValueType* storeScalar()
    {
        store(dataScalar_);
        return dataScalar_;
    }

    ValueType* scalar()
    {
        return dataScalar_;
    }

    SimdType& simd()
    {
        return dataSimd_;
    }

    const ValueType* scalar() const
    {
        return dataScalar_;
    }

    const SimdType& simd() const
    {
        return dataSimd_;
    }

    // ------------------------------------------------------
    // Individual Scalar manipulation
    // ------------------------------------------------------
    // Member function to change a single element in vector
    SimdBaseClass& insert(int index, ValueType val)
    {
        if (index < 0) {
            throw "[Error] (insert): invalid index (" + std::to_string(index) +
                ") given. Negative";
        }
        if (index >= SimdLen) {
            throw "[Error] (insert): invalid index (" + std::to_string(index) +
                ") given. Exceeds maximum";
        }
        Detail::insert<SimdType, ValueType>(dataSimd_, index, val);
        return *this;
    }

    // Extract a single element. Use store function if extracting more than one element.
    // Operator [] can only read an element, not write.
    ValueType operator[](int index) const
    {
        if (index < 0) {
            throw "[Error] (extract): invalid index (" + std::to_string(index) +
                ") given. Negative.";
        }
        if (index >= SimdLen) {
            // index = SimdLen - 1;
            throw "[Error] (extract): Invalid index (" + std::to_string(index) +
                ") given. Exceeds maximum";
        }
        return Detail::extract<SimdBaseClass, ValueType>(*this, index);
    }

    // cut off vector to n elements. The last 4-n elements are set to zero
    SimdBaseClass& cutoff(int index)
    {
        if (index >= SimdLen)
            return *this;
        Detail::cutoff<SimdType>(dataSimd_, index);
        return *this;
    }

    // ------------------------------------------------------
    // Print I/O
    // ------------------------------------------------------
    friend std::ostream& operator<<(std::ostream& stream,
                                    const SimdBaseClass& s)
    {
        Detail::print<SimdBaseClass>(stream, s);
        return stream;
    }

    // ------------------------------------------------------
    // Basic Arithmetic
    // ------------------------------------------------------
    friend SimdBaseClass operator+(const SimdBaseClass& a,
                                   const SimdBaseClass& b)
    {
        return Detail::add<SimdType>(a.dataSimd_, b.dataSimd_);
    }
    friend SimdBaseClass operator-(const SimdBaseClass& a,
                                   const SimdBaseClass& b)
    {
        return Detail::substract<SimdType>(a.dataSimd_, b.dataSimd_);
    }
    friend SimdBaseClass operator*(SimdBaseClass& a,
                                   SimdBaseClass& b)
    {
        return Detail::multiply<SimdType, SimdBaseClass>(a, b);
    }
    friend SimdBaseClass operator/(SimdBaseClass& a,
                                   SimdBaseClass& b)
    {
        return Detail::divide<SimdType, SimdBaseClass>(a, b);
    }

    friend SimdBaseClass min(const SimdBaseClass& a, const SimdBaseClass& b)
    {
        return Detail::min<SimdType>(a.dataSimd_, b.dataSimd_);
    }
    friend SimdBaseClass max(const SimdBaseClass& a, const SimdBaseClass& b)
    {
        return Detail::max<SimdType>(a.dataSimd_, b.dataSimd_);
    }

    friend SimdBaseClass sqrt(const SimdBaseClass& a)
    {
        return Detail::sqrt<SimdType, SimdBaseClass>(a);
    }

    /* Reciprocal( inverse) Square Root */
    friend SimdBaseClass rsqrt(const SimdBaseClass& a)
    {
        return Detail::rsqrt<SimdType, SimdBaseClass>(a);
    }

    friend SimdBaseClass abs(const SimdBaseClass& a)
    {
        return Detail::abs<SimdType>(a.dataSimd_);
    }

    /* Logical */
    friend SimdBaseClass operator&(const SimdBaseClass& a,
                                   const SimdBaseClass& b)
    {
        return Detail::opANDbitwise<SimdType>(a.dataSimd_, b.dataSimd_);
    }
    friend SimdBaseClass operator|(const SimdBaseClass& a,
                                   const SimdBaseClass& b)
    {
        return Detail::opORbitwise<SimdType>(a.dataSimd_, b.dataSimd_);
    }
    friend SimdBaseClass operator^(const SimdBaseClass& a,
                                   const SimdBaseClass& b)
    {
        return Detail::opXORbitwise<SimdType>(a.dataSimd_, b.dataSimd_);
    }
    friend SimdBaseClass operator!(const SimdBaseClass& a)
    {
    }
    // friend SimdBaseClass operator||( const SimdBaseClass &a, const SimdBaseClass &b ){ // mask returned
    //   return _mm_or_ps(a, b);
    // }

    /* Comparison */
    friend SimdMask operator<(const SimdBaseClass& a,
                                   const SimdBaseClass& b)
    { // mask returned
        const SimdType mask = Detail::opLessThan<SimdType>(a.dataSimd_, b.dataSimd_);
        return SimdMask(mask) ;
    }
    friend SimdMask operator<=(const SimdBaseClass& a,
                                    const SimdBaseClass& b)
    { // mask returned
        const SimdType mask = Detail::opLessThanEqual<SimdType>(a.dataSimd_, b.dataSimd_);
        return SimdMask(mask) ;
    }
    friend SimdMask operator>(const SimdBaseClass& a,
                                   const SimdBaseClass& b)
    { // mask returned
        const SimdType mask = Detail::opGreaterThan<SimdType>(a.dataSimd_, b.dataSimd_);
        return SimdMask(mask) ;
    }
    friend SimdMask operator>=(const SimdBaseClass& a,
                                    const SimdBaseClass& b)
    { // mask returned
        const SimdType mask = Detail::opGreaterThanEqual<SimdType>(a.dataSimd_, b.dataSimd_);
        return SimdMask(mask) ;
    }
    friend SimdMask operator==(const SimdBaseClass& a,
                                    const SimdBaseClass& b)
    { // mask returned
        const SimdType mask = Detail::opEqual<SimdType>(a.dataSimd_, b.dataSimd_);
        return SimdMask(mask) ;
    }

protected:
    SimdType dataSimd_;
    ValueType __attribute__((aligned(16)))
    dataScalar_[SimdLen]{}; // Helper array
    // alignas(16) ValueType dataScalar_[SimdLen]{} ; // Helper array
};

} // namespace SIMD
} // namespace KFP
