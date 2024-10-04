// -*- C++ Header -*-
/*
==================================================
Authors: A.Mithran;
Emails: mithran@fias.uni-frankfurt.de
==================================================
*/

#ifndef SIMD_SSE_FLOAT32_H
#define SIMD_SSE_FLOAT32_H

#include "../Utils/macros.h"
#include "../Utils/tag.h"
#include "int32.h"
#include "mask32.h"
#include "constants.h"

#include <cstdint>
#include <string>
#include <cassert>

namespace KFP {
namespace SIMD {

class Float32_128
{
public:
    typedef float value_type;
    typedef __m128 simd_type;
    static constexpr Tag tag{ Tag::SSE };

    // ------------------------------------------------------
    // Constructors
    // ------------------------------------------------------
    // Default constructor:
    Float32_128() {
        m_data = _mm_setzero_ps();
    }
    Float32_128(UninitializeTag) {}
    // Constructor to broadcast the same value into all elements:
    Float32_128(float val)
    {
        m_data = _mm_set1_ps(val);
    }
    Float32_128(const __m128& val_simd)
    {
        m_data = val_simd;
    }
    Float32_128(const float* val_ptr)
    {
        m_data = _mm_loadu_ps(val_ptr);
    }
    Float32_128(const Float32_128& class_simd) = default;

    // Assignment constructors:
    Float32_128& operator=(float val)
    {
        m_data = _mm_set1_ps(val);
        return *this;
    }
    Float32_128& operator=(const __m128& val_simd)
    {
        m_data = val_simd;
        return *this;
    }
    Float32_128& operator=(const Float32_128& class_simd) = default;

    // ------------------------------------------------------
    // Load and Store
    // ------------------------------------------------------
    // Member function to load from array (unaligned)
    KFP_SIMD_INLINE Float32_128& loadUnaligned(const float* val_ptr)
    {
        m_data = _mm_loadu_ps(val_ptr);
        return *this;
    }
    // Member function to load from array (aligned)
    KFP_SIMD_INLINE Float32_128& load(const float* val_ptr)
    {
        m_data = _mm_load_ps(val_ptr);
        return *this;
    }
    // Member function to store into array (unaligned)
    KFP_SIMD_INLINE void storeUnaligned(float* val_ptr) const
    {
        _mm_storeu_ps(val_ptr, m_data);
    }
    // Member function storing into array (aligned)
    KFP_SIMD_INLINE void store(float* val_ptr) const
    {
        _mm_store_ps(val_ptr, m_data);
    }

    // ------------------------------------------------------
    // Gather and Scatter
    // ------------------------------------------------------
    KFP_SIMD_INLINE Float32_128& gather(const float* val_ptr, const Int32_128& index)
    {
        alignas(SimdSize) std::int32_t
        indices[SimdLen]{}; // Helper indices array
        index.store(indices);
        m_data = _mm_setr_ps(
            val_ptr[indices[0]], val_ptr[indices[1]],
            val_ptr[indices[2]], val_ptr[indices[3]]
        );
        return *this;
    }
    KFP_SIMD_INLINE void scatter(float* val_ptr, const Int32_128& index) const
    {
        alignas(SimdSize) float
        data[SimdLen]{}; // Helper data array
        store(data);

        alignas(SimdSize) std::int32_t
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
    KFP_SIMD_INLINE __m128& simd()
    {
        return m_data;
    }
    KFP_SIMD_INLINE const __m128& simd() const
    {
        return m_data;
    }
    template <int N>
    KFP_SIMD_INLINE float get() const {
        static_assert(N >= 0,
        "[Error] (Float32_128::get): Invalid value of index N. Negative");
        static_assert(N < SimdLen,
        "[Error] (Float32_128::get): Invalid value of index N. Out of range.");
        return _mm_extract_ps(m_data, N);
    }
    KFP_SIMD_INLINE float operator[](std::size_t index) const
    {
        assert((index >= 0) && ("[Error] (Float32_128::operator[]): Invalid index (" +
               std::to_string(index) + ") given. Negative")
               .c_str());
        assert((index < SimdLen) &&
               ("[Error] (Float32_128::operator[]): Invalid index (" + std::to_string(index) +
               ") given. Out of range.")
               .c_str());
        alignas(SimdSize) float
        data[SimdLen]{}; // Helper array
        store(data);
        return data[index];
    }

    // ------------------------------------------------------
    // Data lanes manipulation
    // ------------------------------------------------------
    KFP_SIMD_INLINE friend Float32_128 select(const Mask32_128& mask, const Float32_128& a,
                                       const Float32_128& b) {
        return _mm_blendv_ps(b.m_data, a.m_data, mask.simdf());
    }

    KFP_SIMD_INLINE Float32_128 sign() const
    {
        return Float32_128{
            _mm_and_ps(_mm_castsi128_ps(_mm_set1_epi32(0x80000000)), m_data)
        };
    }
    template<int N>
    KFP_SIMD_INLINE Float32_128 shiftLeft() const
    {
        constexpr int num_shift_bytes = N < 0 ? (-N)*4 : N*4;
        return _mm_castsi128_ps(
            _mm_bsrli_si128(_mm_castps_si128(m_data), num_shift_bytes)
        );
    }
    template<int N>
    KFP_SIMD_INLINE Float32_128 shiftRight() const
    {
        constexpr int num_shift_bytes = N < 0 ? (-N)*4 : N*4;
        return _mm_castsi128_ps(
            _mm_bslli_si128(_mm_castps_si128(m_data), num_shift_bytes)
        );
    }

    // ------------------------------------------------------
    // Basic Arithmetic
    // ------------------------------------------------------
    friend Float32_128 operator-(const Float32_128& a)
    {
        return Float32_128{ _mm_sub_ps(_mm_setzero_ps(), a.m_data) };
    }
    friend Float32_128 operator+(const Float32_128& a,
                                   const Float32_128& b)
    {
        return Float32_128{
            _mm_add_ps(a.m_data, b.m_data)
        };
    }
    Float32_128& operator+=(const Float32_128& a)
    {
        *this = *this + a;
        return *this;
    }
    friend Float32_128 operator-(const Float32_128& a,
                                   const Float32_128& b)
    {
        return Float32_128{
            _mm_sub_ps(a.m_data, b.m_data)
        };
    }
    Float32_128& operator-=(const Float32_128& a)
    {
        *this = *this - a;
        return *this;
    }
    friend Float32_128 operator*(const Float32_128& a,
                                   const Float32_128& b)
    {
        return Float32_128{
            _mm_mul_ps(a.m_data, b.m_data)
        };
    }
    Float32_128& operator*=(const Float32_128& a)
    {
        *this = *this * a;
        return *this;
    }
    friend Float32_128 operator&(const Float32_128& a,
                                   const Float32_128& b)
    {
        return _mm_and_ps(a.m_data, b.m_data);
    }
    friend Float32_128 operator|(const Float32_128& a,
                                   const Float32_128& b)
    {
        return _mm_or_ps(a.m_data, b.m_data);
    }
    friend Float32_128 operator^(const Float32_128& a,
                                   const Float32_128& b)
    {
        return _mm_xor_ps(a.m_data, b.m_data);
    }

    // Comparison (mask returned)
    friend Mask32_128 operator<(const Float32_128& a,
                                   const Float32_128& b)
    {
        Mask32_128 result{UninitializeTag{}};
        result.m_data = _mm_castps_si128(
            _mm_cmplt_ps(a.m_data, b.m_data)
        );
        return result;
    }
    friend Mask32_128 operator<=(const Float32_128& a,
                                    const Float32_128& b)
    {
        Mask32_128 result{UninitializeTag{}};
        result.m_data = _mm_castps_si128(
            _mm_cmple_ps(a.m_data, b.m_data)
        );
        return result;
    }
    friend Mask32_128 operator>(const Float32_128& a,
                                   const Float32_128& b)
    {
        Mask32_128 result{UninitializeTag{}};
        result.m_data = _mm_castps_si128(
            _mm_cmpgt_ps(a.m_data, b.m_data)
        );
        return result;
    }
    friend Mask32_128 operator>=(const Float32_128& a,
                                    const Float32_128& b)
    {
        Mask32_128 result{UninitializeTag{}};
        result.m_data = _mm_castps_si128(
            _mm_cmpge_ps(a.m_data, b.m_data)
        );
        return result;
    }
    friend Mask32_128 operator==(const Float32_128& a,
                                    const Float32_128& b)
    {
        Mask32_128 result{UninitializeTag{}};
        result.m_data = _mm_castps_si128(
            _mm_cmpeq_ps(a.m_data, b.m_data)
        );
        return result;
    }
    friend Mask32_128 operator!=(const Float32_128& a,
                                    const Float32_128& b)
    {
        Mask32_128 result{UninitializeTag{}};
        result.m_data = _mm_castps_si128(
            _mm_cmpneq_ps(a.m_data, b.m_data)
        );
        return result;
    }

    KFP_SIMD_INLINE friend Float32_128 min(const Float32_128& a, const Float32_128& b)
    {
        return _mm_min_ps(a.m_data, b.m_data);
    }
    KFP_SIMD_INLINE friend Float32_128 max(const Float32_128& a, const Float32_128& b)
    {
        return _mm_max_ps(a.m_data, b.m_data);
    }
    KFP_SIMD_INLINE friend Float32_128 abs(const Float32_128& a)
    {
        return _mm_and_ps(a.m_data, _mm_castsi128_ps(_mm_set1_epi32(0x7FFFFFFF)));
    }

private:
    alignas(SimdSize) __m128 m_data;
};

} // namespace SIMD
} // namespace KFP

#endif // !SIMD_SSE_FLOAT32_H
