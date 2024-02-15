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
    // Data member accessors
    // ------------------------------------------------------
    void setMask()
    {
        mask_ = _mm_load_si128(reinterpret_cast<const __m128i*>(bools_)) ;
    }

    __m128i maski()
    {
        setMask() ;
        return mask_ ;
    }

    __m128i maskf()
    {
        return _mm_castsi128_ps(maski()) ;
    }

    __m128d maskd()
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

protected:
    int __attribute__((aligned(16)))
    bools_[SimdLen]{0, 0, 0, 0}; // Indices array
    __m128i mask_ ;
    // alignas(16) int indices_[SimdLen]{0, 1, 2, 3} ; // Indices array
};

} // namespace SIMD
} // namespace KFP
