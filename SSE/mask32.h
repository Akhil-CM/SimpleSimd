// -*- C++ Header -*-
/*
==================================================
Authors: A.Mithran;
Emails: mithran@fias.uni-frankfurt.de
==================================================
*/

#ifndef SIMD_SSE_MASK32_H
#define SIMD_SSE_MASK32_H

#include "../Utils/macros.h"
#include "../Utils/tag.h"

#include <iostream>
#include <cassert>

namespace KFP {
namespace SIMD {

class Mask32_128
{
public:
    typedef bool value_type;
    typedef __m128i simd_type;
    static constexpr Tag tag{ Tag::SSE };
    static constexpr int SimdSize{ sizeof(simd_type) };
    static constexpr int SimdLen{ SimdSize / sizeof(value_type) };

    friend class Int32_128;
    friend class Float32_128;

    // ------------------------------------------------------
    // Constructors
    // ------------------------------------------------------
    // Default constructor:
    Mask32_128() {}
    // Constructor to broadcast the same value into all elements:
    Mask32_128(value_type val)
    {
        data_ = _mm_set1_epi32(-int(val));
    }
    Mask32_128(const value_type* val_ptr)
    {
        KFP_SIMD__SPEC_ALIGN(SimdSize) const int
        data[SimdLen]{
            -int(val_ptr[0]), -int(val_ptr[1]), -int(val_ptr[2]), -int(val_ptr[3])
        }; // Helper data array
        data_ = _mm_load_si128(reinterpret_cast<const simd_type*>(val_ptr));
    }
    Mask32_128(const Mask32_128& class_simd)
    {
        data_ = class_simd.data_;
    }

    // Assignment constructors:
    Mask32_128& operator=(value_type val)
    {
        data_ = _mm_set1_epi32(-int(val));
        return *this;
    }
    Mask32_128& operator=(const Mask32_128& class_simd)
    {
        data_ = class_simd.data_;
        return *this;
    }

    // ------------------------------------------------------
    // Load and Store
    // ------------------------------------------------------
    // Member function to load from array (unaligned)
    KFP_SIMD__INLINE Mask32_128& load(const value_type* val_ptr)
    {
        KFP_SIMD__SPEC_ALIGN(SimdSize) const int
        data[SimdLen]{
            -int(val_ptr[0]), -int(val_ptr[1]), -int(val_ptr[2]), -int(val_ptr[3])
        }; // Helper data array
        data_ = _mm_load_si128(reinterpret_cast<const simd_type*>(val_ptr));
        return *this;
    }
    KFP_SIMD__INLINE Mask32_128& loadFromInt(const int* val_ptr)
    {
        data_ = _mm_loadu_si128(reinterpret_cast<const simd_type*>(val_ptr));
        return *this;
    }
    // Member function to store into array (unaligned)
    KFP_SIMD__INLINE void store(value_type* val_ptr) const
    {
        KFP_SIMD__SPEC_ALIGN(SimdSize) int
        data[SimdLen]{}; // Helper data array
        _mm_store_si128(reinterpret_cast<simd_type*>(data), data_);
        val_ptr[0] = data[0];
        val_ptr[1] = data[1];
        val_ptr[2] = data[2];
        val_ptr[3] = data[3];
    }
    KFP_SIMD__INLINE void storeToInt(int* val_ptr) const
    {
        _mm_storeu_si128(reinterpret_cast<simd_type*>(val_ptr), data_);
    }

    // ------------------------------------------------------
    // Data member accessors
    // ------------------------------------------------------
    KFP_SIMD__INLINE simd_type& simd()
    {
        return data_;
    }
    KFP_SIMD__INLINE const simd_type& simd() const
    {
        return data_;
    }
    KFP_SIMD__INLINE simd_type simdf() const
    {
        return _mm_castsi128_ps(data_);
    }
    KFP_SIMD__INLINE value_type operator[](int index) const
    {
        assert((index >= 0) && ("[Error] (Mask32_128::operator[]): invalid index (" +
               std::to_string(index) + ") given. Negative")
               .data());
        assert((index < SimdLen) &&
               ("[Error] (Mask32_128::operator[]): invalid index (" + std::to_string(index) +
               ") given. Exceeds maximum")
               .data());
        KFP_SIMD__SPEC_ALIGN(SimdSize) int
        data[SimdLen]{}; // Helper array
        _mm_store_si128(reinterpret_cast<simd_type*>(data), data_);
        return data[index];
    }

    // ------------------------------------------------------
    // Print I/O
    // ------------------------------------------------------
    // TODO

    // ------------------------------------------------------
    // Basic Arithmetic
    // ------------------------------------------------------
    KFP_SIMD__INLINE int count() const
    {
        const int tmp{ _mm_movemask_ps(_mm_castsi128_ps(data_)) };
        return (tmp & 0x01) + ((tmp & 0x02) >> 1) + ((tmp & 0x04) >> 2) + ((tmp & 0x08) >> 3);
    }
    KFP_SIMD__INLINE bool isFull() const
    {
        return _mm_testc_si128(data_, _mm_set1_epi32(-1));
    }
    KFP_SIMD__INLINE bool isEmpty() const
    {
        return _mm_testz_si128(data_, data_);
    }

    friend Mask32_128 operator!(const Mask32_128& a)
    {
        Mask32_128 result;
        result.data_ = _mm_xor_si128(_mm_set1_epi32(-1), a.data_);
        return result;
    }
    friend Mask32_128 operator==(const Mask32_128& a, const Mask32_128& b)
    {
        Mask32_128 result;
        result.data_ = _mm_cmpeq_epi32(a.data_, b.data_);
        return result;
    }
    friend Mask32_128 operator!=(const Mask32_128& a, const Mask32_128& b)
    {
        Mask32_128 result;
        result.data_ = _mm_cmpeq_epi32(a.data_, b.data_);
        return not result;
    }
    friend Mask32_128 operator&&(const Mask32_128& a, const Mask32_128& b)
    {
        Mask32_128 result;
        result.data_ = _mm_and_si128(a.data_, b.data_);
        return result;
    }
    friend Mask32_128 operator||(const Mask32_128& a, const Mask32_128& b)
    {
        Mask32_128 result;
        result.data_ = _mm_or_si128(a.data_, b.data_);
        return result;
    }

private:
    KFP_SIMD__SPEC_ALIGN(SimdSize) simd_type data_;
};

} // namespace SIMD
} // namespace KFP

#endif // !SIMD_SSE_MASK32_H
