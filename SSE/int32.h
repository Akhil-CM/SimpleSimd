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
#include "constants.h"

#include <cstdint>
#include <string>
#include <cassert>

namespace KFP {
namespace SIMD {

KFP_SIMD_INLINE __m128i select_(const __m128i& mask, const __m128i& a,
                                    const __m128i& b) {
    return _mm_blendv_epi8(b, a, mask);
}

class Int32_128
{
public:
    typedef std::int32_t value_type;
    typedef __m128i simd_type;
    static constexpr Tag tag{ Tag::SSE };

    // ------------------------------------------------------
    // Constructors
    // ------------------------------------------------------
    // Default constructor:
    Int32_128() {
        m_data = _mm_setzero_si128();
    }
    Int32_128(UninitializeTag) {}
    // Constructor to broadcast the same value into all elements:
    Int32_128(std::int32_t val)
    {
        m_data = _mm_set1_epi32(val);
    }
    Int32_128(__m128i val_simd)
    {
        m_data = val_simd;
    }
    Int32_128(const std::int32_t* val_ptr)
    {
        m_data = _mm_loadu_si128(reinterpret_cast<const __m128i*>(val_ptr));
    }
    Int32_128(const Int32_128& class_simd) = default;

    // Assignment constructors:
    Int32_128& operator=(std::int32_t val)
    {
        m_data = _mm_set1_epi32(val);
        return *this;
    }
    Int32_128& operator=(const __m128i& val_simd)
    {
        m_data = val_simd;
        return *this;
    }
    Int32_128& operator=(const Int32_128& class_simd) = default;

    // ------------------------------------------------------
    // Factory methods
    // ------------------------------------------------------
    KFP_SIMD_INLINE static Int32_128 indicesSequence(std::int32_t start)
    {
        return Int32_128{
            _mm_add_epi32(_mm_setr_epi32(0, 1, 2, 3), _mm_set1_epi32(start))
        };
    }

    // ------------------------------------------------------
    // Load and Store
    // ------------------------------------------------------
    // Member function to load from array (unaligned)
    KFP_SIMD_INLINE Int32_128& loadUnaligned(const std::int32_t* val_ptr)
    {
        m_data = _mm_loadu_si128(reinterpret_cast<const __m128i*>(val_ptr));
        return *this;
    }
    // Member function to load from array (aligned)
    KFP_SIMD_INLINE Int32_128& load(const std::int32_t* val_ptr)
    {
        m_data = _mm_load_si128(reinterpret_cast<const __m128i*>(val_ptr));
        return *this;
    }
    // Member function to store into array (unaligned)
    KFP_SIMD_INLINE void storeUnaligned(std::int32_t* val_ptr) const
    {
        _mm_storeu_si128(reinterpret_cast<__m128i*>(val_ptr), m_data);
    }
    // Member function storing into array (aligned)
    KFP_SIMD_INLINE void store(std::int32_t* val_ptr) const
    {
        _mm_store_si128(reinterpret_cast<__m128i*>(val_ptr), m_data);
    }

    // ------------------------------------------------------
    // Gather and Scatter
    // ------------------------------------------------------
    KFP_SIMD_INLINE Int32_128& gather(const std::int32_t* val_ptr, const Int32_128& index)
    {
        alignas(SimdSize) std::int32_t
        indices[SimdLen]{}; // Helper indices array
        index.store(indices);
        m_data = _mm_setr_epi32(
            val_ptr[indices[0]], val_ptr[indices[1]],
            val_ptr[indices[2]], val_ptr[indices[3]]
        );
        return *this;
    }
    KFP_SIMD_INLINE void scatter(std::int32_t* val_ptr, const Int32_128& index) const
    {
        alignas(SimdSize) std::int32_t
        data[SimdLen]{}; // Helper data array
        store(data);

        alignas(SimdSize) int
        indices[SimdLen]{}; // Helper indices array
        index.store(indices);

        val_ptr[indices[0]] = data[0];
        val_ptr[indices[1]] = data[1];
        val_ptr[indices[2]] = data[2];
        val_ptr[indices[3]] = data[3];
    }

    // ------------------------------------------------------
    // Data member accessors
    // ------------------------------------------------------
    KFP_SIMD_INLINE __m128i& simd()
    {
        return m_data;
    }
    KFP_SIMD_INLINE const __m128i& simd() const
    {
        return m_data;
    }
    template <int N>
    KFP_SIMD_INLINE std::int32_t get() const {
        static_assert(N < 0,
        "[Error] (Int32_128::get): Negative value of index to access N given");
        static_assert(N < SimdLen,
        "[Error] (Int32_128::get): Invalid value of index to access N given");
        return _mm_extract_epi32(m_data, N);
    }
    KFP_SIMD_INLINE std::int32_t operator[](std::size_t index) const
    {
        assert((index >= 0) && ("[Error] (Int32_128::operator[]): invalid index (" +
               std::to_string(index) + ") given. Negative")
               .data());
        assert((index < SimdLen) &&
               ("[Error] (Int32_128::operator[]): invalid index (" + std::to_string(index) +
               ") given. Exceeds maximum")
               .data());
        alignas(SimdSize) std::int32_t
        data[SimdLen]{}; // Helper array
        store(data);
        return data[index];
    }

    // ------------------------------------------------------
    // Print I/O
    // ------------------------------------------------------
    // TODO

    // ------------------------------------------------------
    // Data lanes manipulation
    // ------------------------------------------------------
    KFP_SIMD_INLINE friend Int32_128 select(const Mask32_128& mask, const Int32_128& a,
                                       const Int32_128& b) {
        return select_(mask.m_data, a.m_data, b.m_data);
    }

    KFP_SIMD_INLINE Int32_128 sign() const
    {
        return Int32_128{
            _mm_and_si128(_mm_set1_epi32(0x80000000), m_data)
        };
    }
    template<int N>
    KFP_SIMD_INLINE Int32_128 shiftLeft()
    {
        constexpr int num_shift_bytes = N < 0 ? (-N)*4 : N*4;
        return _mm_bsrli_si128(m_data, num_shift_bytes);
    }
    template<int N>
    KFP_SIMD_INLINE Int32_128 shiftRight()
    {
        constexpr int num_shift_bytes = N < 0 ? (-N)*4 : N*4;
        return _mm_bslli_si128(m_data, num_shift_bytes);
    }

    // ------------------------------------------------------
    // Basic Arithmetic
    // ------------------------------------------------------
    friend Int32_128 operator-(const Int32_128& a)
    {
        return Int32_128{ _mm_sub_epi32(_mm_setzero_si128(), a.m_data) };
    }
    friend Int32_128 operator+(const Int32_128& a,
                                   const Int32_128& b)
    {
        return Int32_128{ _mm_add_epi32(a.m_data, b.m_data) };
    }
    Int32_128& operator+=(const Int32_128& a)
    {
        *this = *this + a;
        return *this;
    }
    friend Int32_128 operator-(const Int32_128& a,
                                   const Int32_128& b)
    {
        return Int32_128{ _mm_sub_epi32(a.m_data, b.m_data) };
    }
    Int32_128& operator-=(const Int32_128& a)
    {
        *this = *this - a;
        return *this;
    }
    friend Int32_128 operator*(const Int32_128& a,
                                   const Int32_128& b)
    {
        return _mm_mullo_epi32(a.m_data, b.m_data);
    }
    Int32_128& operator*=(const Int32_128& a)
    {
        *this = *this * a;
        return *this;
    }
    Int32_128 operator<<(int n)
    {
        return _mm_slli_epi32(m_data, n);
    }
    Int32_128 operator>>(int n)
    {
        return _mm_srai_epi32(m_data, n);
    }
    friend Int32_128 operator&(const Int32_128& a,
                                   const Int32_128& b)
    {
        return _mm_and_si128(a.m_data, b.m_data);
    }
    friend Int32_128 operator|(const Int32_128& a,
                                   const Int32_128& b)
    {
        return _mm_or_si128(a.m_data, b.m_data);
    }
    friend Int32_128 operator^(const Int32_128& a,
                                   const Int32_128& b)
    {
        return _mm_xor_si128(a.m_data, b.m_data);
    }

    // Comparison (mask returned)
    friend Mask32_128 operator<(const Int32_128& a,
                                   const Int32_128& b)
    {
        Mask32_128 result;
        result.m_data = _mm_cmplt_epi32(a.m_data, b.m_data);
        return result;
    }
    friend Mask32_128 operator<=(const Int32_128& a,
                                    const Int32_128& b)
    {
        Mask32_128 result;
        result.m_data = _mm_cmpeq_epi32(_mm_min_epi32(a.m_data, b.m_data), a.m_data);
        return result;
    }
    friend Mask32_128 operator>(const Int32_128& a,
                                   const Int32_128& b)
    {
        Mask32_128 result;
        result.m_data = _mm_cmpgt_epi32(a.m_data, b.m_data);
        return result;
    }
    friend Mask32_128 operator>=(const Int32_128& a,
                                    const Int32_128& b)
    {
        Mask32_128 result;
        result.m_data = _mm_cmpeq_epi32(_mm_min_epi32(b.m_data, a.m_data), b.m_data);
        return result;
    }
    friend Mask32_128 operator==(const Int32_128& a,
                                    const Int32_128& b)
    {
        Mask32_128 result;
        result.m_data = _mm_cmpeq_epi32(a.m_data, b.m_data);
        return result;
    }
    friend Mask32_128 operator!=(const Int32_128& a,
                                    const Int32_128& b)
    {
        Mask32_128 result;
        result.m_data = _mm_cmpeq_epi32(a.m_data, b.m_data);
        return not result;
    }

    KFP_SIMD_INLINE friend Int32_128 min(const Int32_128& a, const Int32_128& b)
    {
        return _mm_min_epi32(a.m_data, b.m_data);
    }
    KFP_SIMD_INLINE friend Int32_128 max(const Int32_128& a, const Int32_128& b)
    {
        return _mm_max_epi32(a.m_data, b.m_data);
    }
    KFP_SIMD_INLINE friend Int32_128 abs(const Int32_128& a)
    {
        return _mm_abs_epi32(a.m_data);
    }

private:
    alignas(SimdSize) __m128i m_data;
};

} // namespace SIMD
} // namespace KFP

#endif // !SIMD_SSE_INT32_H
