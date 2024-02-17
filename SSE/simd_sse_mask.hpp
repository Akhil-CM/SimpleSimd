// -*- C++ Header -*-
/*
==================================================
Authors: A.Mithran;
Emails: mithran@fias.uni-frankfurt.de
==================================================
*/

#pragma once

#include <x86intrin.h>
#include <algorithm>
#include <iostream>
#include <type_traits>

namespace KFP {
namespace SIMD {

class SimdMask
{
public:

    static constexpr int SimdLen = sizeof(__m128) / sizeof(float) ;
    static constexpr int SimdSize{ SimdLen * sizeof(float) };
    // ------------------------------------------------------
    // Constructors
    // ------------------------------------------------------
    // Default constructor:
    SimdMask() = default;
    SimdMask(const __m128& mask)
    {
        _mm_store_si128(reinterpret_cast<__m128i*>(bools_), _mm_castps_si128(mask)) ;
    }
    SimdMask(const __m128i& mask)
    {
        _mm_store_si128(reinterpret_cast<__m128i*>(bools_), mask) ;
    }
    SimdMask(const SimdMask& class_mask)
    {
        std::copy_n(class_mask.bools_, SimdLen, bools_) ;
    }
    SimdMask(bool val1, bool val2, bool val3, bool val4)
    {
        bools_[0] = -int(val1) ;
        bools_[1] = -int(val2) ;
        bools_[2] = -int(val3) ;
        bools_[3] = -int(val4) ;
    }
    SimdMask(bool val)
    {
        bools_[0] = -int(val) ;
        bools_[1] = -int(val) ;
        bools_[2] = -int(val) ;
        bools_[3] = -int(val) ;
    }
    SimdMask(bool* val_ptr)
    {
        bools_[0] = -int(val_ptr[0]) ;
        bools_[1] = -int(val_ptr[1]) ;
        bools_[2] = -int(val_ptr[2]) ;
        bools_[3] = -int(val_ptr[3]) ;
    }
    // Assignment operator
    SimdMask& operator=(bool val)
    {
        bools_[0] = -int(val) ;
        bools_[1] = -int(val) ;
        bools_[2] = -int(val) ;
        bools_[3] = -int(val) ;
        return *this;
    }
    SimdMask& operator=(const SimdMask& class_mask)
    {
        std::copy_n(class_mask.bools_, SimdLen, bools_) ;
        return *this;
    }

    // ------------------------------------------------------
    // Status accessors
    // ------------------------------------------------------
    int count() const
    {
        const int result = bools_[0] + bools_[1] + bools_[2] + bools_[3] ;
        return -(result) ;
    }

    bool statusAND() const
    {
        return (count() == 4) ;
    }

    bool statusOR() const
    {
        return !(count() == 0) ;
    }

    // ------------------------------------------------------
    // Data member accessors
    // ------------------------------------------------------
    int& operator[](int index)
    {
        return bools_[index];
    }

    const int& operator[](int index) const
    {
        return bools_[index];
    }

    __m128i maski() const
    {
        return _mm_load_si128(reinterpret_cast<const __m128i*>(bools_)) ;
    }

    __m128 maskf() const
    {
        return _mm_castsi128_ps(maski()) ;
    }

    __m128d maskd() const
    {
        return _mm_castsi128_pd(_mm_setr_epi32(bools_[0], bools_[0], bools_[1], bools_[1]));
    }

    int* scalar()
    {
        return bools_;
    }

    const int* scalar() const
    {
        return bools_;
    }

    // ------------------------------------------------------
    // Print I/O
    // ------------------------------------------------------
    friend std::ostream& operator<<(std::ostream& stream,
                                    const SimdMask& s)
    {
        stream.setf(std::ios::boolalpha) ;
        stream << "[" << bool(s.bools_[0]) << ", " << bool(s.bools_[1]) << ", " << bool(s.bools_[2]) << ", " << bool(s.bools_[3])
               << "]";
        stream.unsetf(std::ios::boolalpha) ;
        return stream;
    }

    /* Comparison */
    friend SimdMask operator!(const SimdMask& a)
    {
        const int* data = a.scalar() ;
        return SimdMask(data[0]+1, data[1]+1, data[2]+1, data[3]+1) ;
    }

    friend bool operator==(const SimdMask& a,
                                    const SimdMask& b)
    { // mask returned
        const int* data1 = a.scalar() ;
        const int* data2 = b.scalar() ;
        return ((data1[0] == data2[0]) && (data1[1] == data2[1]) && (data1[2] == data2[2]) && (data1[3] == data2[3])) ;
    }

    friend bool operator!=(const SimdMask& a,
                                    const SimdMask& b)
    { // mask returned
        return !(a == b) ;
    }

protected:
    // __m128i mask_ ;
    int __attribute__((aligned(16)))
    bools_[SimdLen]{0, 0, 0, 0}; // Indices array
    // alignas(16) int indices_[SimdLen]{0, 1, 2, 3} ; // Indices array
};

} // namespace SIMD
} // namespace KFP
