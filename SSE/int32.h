// -*- C++ Header -*-
/*
==================================================
Authors: A.Mithran;
Emails: mithran@fias.uni-frankfurt.de
==================================================
*/

#ifndef SIMD_SSE_INT32_H
#define SIMD_SSE_INT32_H

#include "../Utils/macros.h"
#include "../Utils/tag.h"
#include "mask32.h"

#include <string>
#include <cassert>

namespace KFP {
namespace SIMD {

KFP_SIMD__INLINE __m128i select_(const __m128i& mask, const __m128i& a,
                                    const __m128i& b) {
    return _mm_blendv_epi8(b, a, mask);
}

class Int32_128
{
public:
    typedef int value_type;
    typedef __m128i simd_type;
    static constexpr Tag tag{ Tag::SSE };
    static constexpr int SimdSize{ sizeof(simd_type) };
    static constexpr int SimdLen{ SimdSize / sizeof(value_type) };

    // ------------------------------------------------------
    // Constructors
    // ------------------------------------------------------
    // Default constructor:
    Int32_128() {}
    // Constructor to broadcast the same value into all elements:
    Int32_128(value_type val)
    {
        data_ = _mm_set1_epi32(val);
    }
    Int32_128(simd_type val_simd)
    {
        data_ = val_simd;
    }
    Int32_128(const value_type* val_ptr)
    {
        data_ = _mm_loadu_si128(reinterpret_cast<const simd_type*>(val_ptr));
    }
    Int32_128(const Int32_128& class_simd)
    {
        data_ = class_simd.data_;
    }

    // Assignment constructors:
    Int32_128& operator=(value_type val)
    {
        data_ = _mm_set1_epi32(val);
        return *this;
    }
    Int32_128& operator=(const simd_type& val_simd)
    {
        data_ = val_simd;
        return *this;
    }
    Int32_128& operator=(const Int32_128& class_simd)
    {
        data_ = class_simd.data_;
        return *this;
    }

    // ------------------------------------------------------
    // Factory methods
    // ------------------------------------------------------
    KFP_SIMD__INLINE static Int32_128 indicesSequence(value_type start)
    {
        KFP_SIMD__SPEC_ALIGN(SimdSize) constexpr value_type
        data[SimdLen]{0, 1, 2, 3}; // Helper data array
        return Int32_128{}.load_a(data) + start;
    }

    // ------------------------------------------------------
    // Load and Store
    // ------------------------------------------------------
    // Member function to load from array (unaligned)
    KFP_SIMD__INLINE Int32_128& load(const value_type* val_ptr)
    {
        data_ = _mm_loadu_si128(reinterpret_cast<const simd_type*>(val_ptr));
        return *this;
    }
    // Member function to load from array (aligned)
    KFP_SIMD__INLINE Int32_128& load_a(const value_type* val_ptr)
    {
        data_ = _mm_load_si128(reinterpret_cast<const simd_type*>(val_ptr));
        return *this;
    }
    // Member function to store into array (unaligned)
    KFP_SIMD__INLINE void store(value_type* val_ptr) const
    {
        _mm_storeu_si128(reinterpret_cast<simd_type*>(val_ptr), data_);
    }
    // Member function storing into array (aligned)
    KFP_SIMD__INLINE void store_a(value_type* val_ptr) const
    {
        _mm_store_si128(reinterpret_cast<simd_type*>(val_ptr), data_);
    }

    // ------------------------------------------------------
    // Gather and Scatter
    // ------------------------------------------------------
    KFP_SIMD__INLINE Int32_128& gather(const value_type* val_ptr, const Int32_128& index)
    {
        KFP_SIMD__SPEC_ALIGN(SimdSize) value_type
        indices[SimdLen]{}; // Helper indices array
        index.store_a(indices);
        data_ = _mm_setr_epi32(
            val_ptr[indices[0]], val_ptr[indices[1]],
            val_ptr[indices[2]], val_ptr[indices[3]]
        );
        return *this;
    }
    KFP_SIMD__INLINE void scatter(value_type* val_ptr, const Int32_128& index) const
    {
        KFP_SIMD__SPEC_ALIGN(SimdSize) value_type
        data[SimdLen]{}; // Helper data array
        store_a(data);

        KFP_SIMD__SPEC_ALIGN(SimdSize) int
        indices[SimdLen]{}; // Helper indices array
        index.store_a(indices);

        val_ptr[indices[0]] = data[0];
        val_ptr[indices[1]] = data[1];
        val_ptr[indices[2]] = data[2];
        val_ptr[indices[3]] = data[3];
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
    template <int N>
    KFP_SIMD__INLINE value_type get(simd_type a) const {
        static_assert(N < SimdLen,
        "[Error] (Int32_128::get): Invalid value of index to access N given");
#if 1
        return _mm_extract_epi32(data_, N);
#else
        const simd_type result = _mm_shuffle_epi32(a, N);
        return _mm_cvtsi128_si32(result);
#endif
    }
    KFP_SIMD__INLINE value_type operator[](int index) const
    {
        assert((index >= 0) && ("[Error] (Int32_128::operator[]): invalid index (" +
               std::to_string(index) + ") given. Negative")
               .data());
        assert((index < SimdLen) &&
               ("[Error] (Int32_128::operator[]): invalid index (" + std::to_string(index) +
               ") given. Exceeds maximum")
               .data());
        KFP_SIMD__SPEC_ALIGN(SimdSize) value_type
        data[SimdLen]{}; // Helper array
        store_a(data);
        return data[index];
    }

    // ------------------------------------------------------
    // Print I/O
    // ------------------------------------------------------
    // TODO

    // ------------------------------------------------------
    // Data lanes manipulation
    // ------------------------------------------------------
    KFP_SIMD__INLINE friend Int32_128 select(const Mask32_128& mask, const Int32_128& a,
                                       const Int32_128& b) {
        return select_(mask.data_, a.data_, b.data_);
    }

    KFP_SIMD__INLINE Int32_128 sign() const
    {
        return Int32_128{
            _mm_and_si128(_mm_set1_epi32(0x80000000), data_)
        };
    }
    KFP_SIMD__INLINE Int32_128& shiftLeft(int n)
    {
        constexpr int value_size_bytes = sizeof(int);
        switch (n) {
            case 0:
                return *this;
            case 1:
                data_ = _mm_bsrli_si128(data_, value_size_bytes);
            case 2:
                data_ = _mm_bsrli_si128(data_, 2 * value_size_bytes);
            case 3:
                data_ = _mm_bsrli_si128(data_, 3 * value_size_bytes);
            default:
                data_ = _mm_set1_epi32(0);
        }
        return *this;
    }
    KFP_SIMD__INLINE Int32_128& shiftRight(int n)
    {
        constexpr int value_size_bytes = sizeof(int);
        switch (n) {
            case 0:
                return *this;
            case 1:
                data_ = _mm_bslli_si128(data_, value_size_bytes);
            case 2:
                data_ = _mm_bslli_si128(data_, 2 * value_size_bytes);
            case 3:
                data_ = _mm_bslli_si128(data_, 3 * value_size_bytes);
            default:
                data_ = _mm_set1_epi32(0);
        }
        return *this;
    }

    // ------------------------------------------------------
    // Print I/O
    // ------------------------------------------------------

    // ------------------------------------------------------
    // Basic Arithmetic
    // ------------------------------------------------------
    friend Int32_128 operator-(const Int32_128& a)
    {
        return Int32_128{ _mm_sub_epi32(_mm_setzero_si128(), a.data_) };
    }
    friend Int32_128 operator+(const Int32_128& a,
                                   const Int32_128& b)
    {
        return Int32_128{ _mm_add_epi32(a.data_, b.data_) };
    }
    friend Int32_128& operator+=(Int32_128& a,
                                   const Int32_128& b)
    {
        a = a + b;
        return a;
    }
    friend Int32_128 operator-(const Int32_128& a,
                                   const Int32_128& b)
    {
        return Int32_128{ _mm_sub_epi32(a.data_, b.data_) };
    }
    friend Int32_128& operator-=(Int32_128& a,
                                   const Int32_128& b)
    {
        a = a - b;
        return a;
    }
    friend Int32_128 operator*(const Int32_128& a,
                                   const Int32_128& b)
    {
        return _mm_mullo_epi32(a.data_, b.data_);
    }
    friend Int32_128& operator*=(Int32_128& a,
                                   const Int32_128& b)
    {
        a = a * b;
        return a;
    }
    friend Int32_128 operator<<(const Int32_128& a, int n)
    {
        return _mm_slli_epi32(a.data_, n);
    }
    friend Int32_128 operator>>(const Int32_128& a, int n)
    {
        return _mm_sra_epi32(a.data_, _mm_cvtsi32_si128(n));
    }
    friend Int32_128 operator&(const Int32_128& a,
                                   const Int32_128& b)
    {
        return _mm_and_si128(a.data_, b.data_);
    }
    friend Int32_128 operator|(const Int32_128& a,
                                   const Int32_128& b)
    {
        return _mm_or_si128(a.data_, b.data_);
    }
    friend Int32_128 operator^(const Int32_128& a,
                                   const Int32_128& b)
    {
        return _mm_xor_si128(a.data_, b.data_);
    }

    // Comparison (mask returned)
    friend Mask32_128 operator<(const Int32_128& a,
                                   const Int32_128& b)
    {
        Mask32_128 result;
        result.data_ = _mm_cmplt_epi32(a.data_, b.data_);
        return result;
    }
    friend Mask32_128 operator<=(const Int32_128& a,
                                    const Int32_128& b)
    {
        Mask32_128 result;
        result.data_ = _mm_cmpeq_epi32(_mm_min_epi32(a.data_, b.data_), a.data_);
        return result;
    }
    friend Mask32_128 operator>(const Int32_128& a,
                                   const Int32_128& b)
    {
        Mask32_128 result;
        result.data_ = _mm_cmpgt_epi32(a.data_, b.data_);
        return result;
    }
    friend Mask32_128 operator>=(const Int32_128& a,
                                    const Int32_128& b)
    {
        Mask32_128 result;
        result.data_ = _mm_cmpeq_epi32(_mm_min_epi32(b.data_, a.data_), b.data_);
        return result;
    }
    friend Mask32_128 operator==(const Int32_128& a,
                                    const Int32_128& b)
    {
        Mask32_128 result;
        result.data_ = _mm_cmpeq_epi32(a.data_, b.data_);
        return result;
    }
    friend Mask32_128 operator!=(const Int32_128& a,
                                    const Int32_128& b)
    {
        Mask32_128 result;
        result.data_ = _mm_cmpeq_epi32(a.data_, b.data_);
        return not result;
    }

    KFP_SIMD__INLINE friend Int32_128 min(const Int32_128& a, const Int32_128& b)
    {
        return _mm_min_epi32(a.data_, b.data_);
    }
    KFP_SIMD__INLINE friend Int32_128 max(const Int32_128& a, const Int32_128& b)
    {
        return _mm_max_epi32(a.data_, b.data_);
    }
    KFP_SIMD__INLINE friend Int32_128 abs(const Int32_128& a)
    {
        return _mm_abs_epi32(a.data_);
    }

private:
    KFP_SIMD__SPEC_ALIGN(SimdSize) simd_type data_;
};

} // namespace SIMD
} // namespace KFP

#endif // !SIMD_SSE_INT32_H
