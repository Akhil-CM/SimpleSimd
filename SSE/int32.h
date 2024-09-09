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

#include <iostream>
#include <x86intrin.h>
#include <cmath>
#include <string>
#include <cassert>

namespace KFP {
namespace SIMD {

__KFP_SIMD__INLINE __m128i select(const __m128i& mask, const __m128i& a,
                                    const __m128i& b) {
#if defined(__KFP_SIMD__SSE4_1) // SSE4.1
    return _mm_blendv_epi8(b, a, mask);
#else
    return _mm_or_si128(_mm_and_si128(mask, a), _mm_andnot_si128(mask, b));
#endif
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
    Int32_128()
    {
        data_ = _mm_setzero_si128();
    }
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
    static Int32_128 iota(value_type start)
    {
        return Int32_128{
            _mm_setr_epi32(start, start + 1, start + 2, start + 3)
        };
    }
    static Int32_128 seq(value_type start)
    {
        return iota(start);
    }

    // ------------------------------------------------------
    // Load and Store
    // ------------------------------------------------------
    // Member function to load from array (unaligned)
    Int32_128& load(const value_type* val_ptr)
    {
        data_ = _mm_loadu_si128(reinterpret_cast<const simd_type*>(val_ptr));
        return *this;
    }
    // Member function to load from array (aligned)
    Int32_128& load_a(const value_type* val_ptr)
    {
        data_ = _mm_load_si128(reinterpret_cast<const simd_type*>(val_ptr));
        return *this;
    }
    // Member function to store into array (unaligned)
    void store(value_type* val_ptr) const
    {
        _mm_storeu_si128(reinterpret_cast<simd_type*>(val_ptr), data_);
    }
    // Member function storing into array (aligned)
    void store_a(value_type* val_ptr) const
    {
        _mm_store_si128(reinterpret_cast<simd_type*>(val_ptr), data_);
    }

    // ------------------------------------------------------
    // Gather and Scatter
    // ------------------------------------------------------
    Int32_128& gather(const value_type* val_ptr, const Int32_128& index)
    {
        __KFP_SIMD__SPEC_ALIGN(SimdSize) value_type
        indices[SimdLen]{}; // Helper indices array
        index.store_a(indices);
        data_ = _mm_setr_epi32(
            val_ptr[indices[0]], val_ptr[indices[1]],
            val_ptr[indices[2]], val_ptr[indices[3]]
        );
        return *this;
    }
    void scatter(value_type* val_ptr, const Int32_128& index) const
    {
        __KFP_SIMD__SPEC_ALIGN(SimdSize) value_type
        data[SimdLen]{}; // Helper data array
        store_a(data);

        __KFP_SIMD__SPEC_ALIGN(SimdSize) int
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
    simd_type& simd()
    {
        return data_;
    }
    const simd_type& simd() const
    {
        return data_;
    }
    template <int N>
    __KFP_SIMD__INLINE_FORCE value_type get(simd_type a) const {
        const simd_type result = _mm_shuffle_epi32(a, N);
        return _mm_cvtsi128_si32(result);
    }
    value_type operator[](int index) const
    {
        value_type a = get<0>(data_);
        assert((index > -1) && ("[Error] (Int32_128::operator[]): invalid index (" +
               std::to_string(index) + ") given. Negative")
               .data());
        assert((index < SimdLen) &&
               ("[Error] (Int32_128::operator[]): invalid index (" + std::to_string(index) +
               ") given. Exceeds maximum")
               .data());
#if 0
        __KFP_SIMD__SPEC_ALIGN(SimdSize) value_type
        data[SimdLen]{}; // Helper array
        store_a(data);
        return data[index];
#elif defined(__KFP_SIMD__SSE4_1)
        switch (index) {
            case 0:
                return _mm_extract_epi32(data_, 0x00);
            case 1:
                return _mm_extract_epi32(data_, 0x01);
            case 2:
                return _mm_extract_epi32(data_, 0x02);
            case 3:
            default:
                return _mm_extract_epi32(data_, 0x03);
        }
#else
        switch (index) {
            case 0:
                return get<0>(data_);
            case 1:
                return get<1>(data_);
            case 2:
                return get<2>(data_);
            case 3:
            default:
                return get<3>(data_);
        }
#endif
    }

    // ------------------------------------------------------
    // Data elements manipulation
    // ------------------------------------------------------
    friend __KFP_SIMD__INLINE Int32_128 select(const Int32_128& mask, const Int32_128& a,
                                       const Int32_128& b) {
        return select(mask.data_, a.data_, b.data_);
    }

    Int32_128& insert(int index, value_type val)
    {
        __KFP_SIMD__SPEC_ALIGN(SimdSize) int
        indices[SimdLen]{}; // Helper indices array
        indices[index] = -1;
        data_ = select(_mm_load_si128(reinterpret_cast<const simd_type*>(indices)),
                       _mm_set1_epi32(val), data_);
        return *this;
    }
    Int32_128 sign() const
    {
        return Int32_128{
            _mm_and_si128(_mm_set1_epi32(0x80000000), data_)
        };
    }

    // ------------------------------------------------------
    // Print I/O
    // ------------------------------------------------------
    friend std::ostream& operator<<(std::ostream& stream,
                                    const Int32_128& a)
    {
        __KFP_SIMD__SPEC_ALIGN(SimdSize) value_type
        data[SimdLen]{}; // Helper data array
        a.store_a(data);
        stream << "[" << data[0] << ", " << data[1] << ", " << data[2] << ", "
             << data[3] << "]";
        return stream;
    }

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
#if defined(__KFP_SIMD__SSE4_1) // SSE4.1
        return _mm_mullo_epi32(a.data_, b.data_);
#else

        __KFP_SIMD__SPEC_ALIGN(SimdSize) value_type
        data1[SimdLen]{}; // Helper data array
        a.store_a(data1);

        __KFP_SIMD__SPEC_ALIGN(SimdSize) value_type
        data2[SimdLen]{}; // Helper data array
        b.store_a(data2);

        __KFP_SIMD__SPEC_ALIGN(SimdSize) const value_type
        data[SimdLen]{data1[0] * data2[0], data1[1] * data2[1],
            data1[2] * data2[2], data1[3] * data2[3]};
        return Int32_128{ _mm_load_si128(reinterpret_cast<const simd_type*>(data)) };
#endif
    }
    friend Int32_128& operator*=(Int32_128& a,
                                   const Int32_128& b)
    {
        a = a * b;
        return a;
    }
    friend Int32_128 operator/(const Int32_128& a,
                                   const Int32_128& b)
    {
        __KFP_SIMD__SPEC_ALIGN(SimdSize) value_type
        data1[SimdLen]{}; // Helper data array
        a.store_a(data1);
        __KFP_SIMD__SPEC_ALIGN(SimdSize) value_type
        data2[SimdLen]{}; // Helper data array
        b.store_a(data2);
        __KFP_SIMD__SPEC_ALIGN(SimdSize) const value_type
        data[SimdLen]{data1[0] / data2[0], data1[1] / data2[1],
            data1[2] / data2[2], data1[3] / data2[3]};
        return Int32_128{ _mm_load_si128(reinterpret_cast<const simd_type*>(data)) };
    }
    friend Int32_128& operator/=(Int32_128& a,
                                   const Int32_128& b)
    {
        a = a / b;
        return a;
    }

    friend Int32_128 min(const Int32_128& a, const Int32_128& b)
    {
#if defined(__KFP_SIMD__SSE4_1) // SSE4.1
        return _mm_min_epi32(a.data_, b.data_);
#else
        const simd_type mask = _mm_cmpgt_epi32(a.data_, b.data_);
        return select(mask, b.data_, a.data_);
#endif
    }
    friend Int32_128 max(const Int32_128& a, const Int32_128& b)
    {
#if defined(__KFP_SIMD__SSE4_1) // SSE4.1
        return _mm_max_epi32(a.data_, b.data_);
#else
        const simd_type mask = _mm_cmpgt_epi32(a.data_, b.data_);
        return select(mask, a.data_, b.data_);
#endif
    }

    friend Int32_128 abs(const Int32_128& a)
    {
#if defined(__KFP_SIMD__SSSE3) // SSSE3
        return _mm_abs_epi32(a.data_);
#else
        return _mm_and_si128(a.data_, _mm_set1_epi32(0x7FFFFFFF));
#endif
    }
    friend Int32_128 pow(const Int32_128& a, int exp)
    {
        __KFP_SIMD__SPEC_ALIGN(SimdSize) value_type
        data[SimdLen]{}; // Helper data array
        a.store_a(data);
        return _mm_setr_epi32(std::pow(data[0], exp), std::pow(data[1], exp),
                              std::pow(data[2], exp), std::pow(data[3], exp));
    }

protected:
    simd_type data_;
};

} // namespace SIMD
} // namespace KFP

#endif // !SIMD_SSE_INT32_H
