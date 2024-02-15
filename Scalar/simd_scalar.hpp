// -*- C++ Header -*-
/*
==================================================
Authors: A.Mithran;
Emails: mithran@fias.uni-frankfurt.de
==================================================
*/

#pragma once

#include "../General/simd_abi.hpp"
#include "../General/simd_base.hpp"
#include <type_traits>

namespace KFP
{
namespace SIMD
{

template <typename ValueType>
class SimdBaseClass<ValueType, ValueType, ABI::Scalar>
{
    static_assert(std::is_fundamental<ValueType>::value &&
                      std::is_arithmetic<ValueType>::value,
                  "Only works for fundamental arithmetic types.");

public:
    typedef ValueType value_type;
    typedef ValueType simd_type;
    static constexpr int SimdLen{1} ;
    static constexpr int SimdSize{SimdLen * sizeof(ValueType)} ;

    static_assert((SimdLen == 1), "[Error] : KFP::SIMD Scalar type has invalid length") ;

    // Default constructor:
    SimdBaseClass() = default;
    // Constructor to broadcast the same value into all elements:
    SimdBaseClass(ValueType val)
        : data_{ val }
    {}

    // Assignment operator to convert from type __m128 used in intrinsics:
    SimdBaseClass& operator=(ValueType val)
    {
        data_ = val;
        return *this;
    }

    // Member function to load from array (unaligned)
    SimdBaseClass& load(ValueType const* p)
    {
        data_ = *p;
        return *this;
    }

    SimdBaseClass& load_a(ValueType const* p)
    {
        return load(p);
    }

    // Member function to store into array (unaligned)
    void store(ValueType* p) const
    {
        *p = data_;
    }
    // Member function storing into array, aligned by 16
    // "store_a" is faster than "store" on older Intel processors (Pentium 4, Pentium M, Core 1,
    // Merom, Wolfdale) and Atom, but not on other processors from Intel, AMD or VIA.
    // You may use store_a instead of store if you are certain that p points to an address
    // divisible by 16.
    void store_a(ValueType* p) const
    {
        store(p);
    }
    // Member function storing to aligned uncached memory (non-temporal store).
    // This may be more efficient than store_a when storing large blocks of memory if it
    // is unlikely that the data will stay in the cache until it is read again.
    // Note: Will generate runtime error if p is not aligned by 16
    void store_nt(ValueType* p) const
    {
        store(p);
    }
    // Partial load. Load n elements and set the rest to 0
    SimdBaseClass& load_partial(int, ValueType const* p)
    {
        return load(p);
    }
    // Partial store. Store n elements
    void store_partial(int, ValueType* p) const
    {
        store(p);
    }
    // cut off vector to n elements. The last 4-n elements are set to zero
    SimdBaseClass& cutoff(int n)
    {
        if (n >= 1)
            return *this;
        data_ = ValueType{};
        return *this;
    }
    // Member function to change a single element in vector
    void insert(int index, ValueType value)
    {
        if (index >= 1)
            return *this;
        data_ = value;
        return *this;
    }
    // Member function extract a single element from vector
    ValueType extract(int index) const
    {
        return data_ ;
    }
    // Extract a single element. Use store function if extracting more than one element.
    // Operator [] can only read an element, not write.
    ValueType operator[](int index) const
    {
        return extract(index);
    }
protected:
    // typename std::conditional<
    //         std::is_integral<SimdType>::value, int,
    //         typename std::conditional<std::is_same<SimdType, float>::value, float,
    //                                   double>::type>::type data_;
    ValueType data_; // Float vector
};

template <typename ValueType>
using Simd_t =  SimdBaseClass<ValueType, ValueType, ABI::Scalar> ;
using SimdF_t =  Simd_t<float> ;
using SimdI_t =  Simd_t<int> ;
using SimdD_t =  Simd_t<double> ;
using SimdB_t =  Simd_t<bool> ;

} // namespace SIMD
} // namespace KFP
