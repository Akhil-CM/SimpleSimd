// -*- C++ Header -*-
/*
==================================================
Authors: A.Mithran;
Emails: mithran@fias.uni-frankfurt.de
==================================================
*/

#pragma once

#include "../General/simd_detect.hpp"
#include "../General/simd_base.hpp"
#include "../General/simd_abi.hpp"
#include "../General/simd_detail.hpp"
#include <x86intrin.h>
#include <algorithm>
#include <cmath>

namespace KFP {
namespace SIMD {

using SimdI_t = SimdBaseClass<int, __m128i, ABI::SSE>;
namespace Detail {

typedef __m128i SimdTypeI;
typedef int ValueTypeI;

inline SimdI_t SimdI(ValueTypeI val1, ValueTypeI val2, ValueTypeI val3, ValueTypeI val4)
{
    return SimdI_t(_mm_setr_epi32(val1, val2, val3, val4)) ;
}

template <>
inline void constant<SimdTypeI, ValueTypeI>(SimdTypeI& val_simd, ValueTypeI val)
{
    val_simd = _mm_set1_epi32(val);
}

template <>
inline void load<SimdTypeI, ValueTypeI>(SimdTypeI& val_simd,
                                      const ValueTypeI* val_ptr)
{
    val_simd = _mm_loadu_si128(reinterpret_cast<const SimdTypeI*>(val_ptr));
}

template <>
inline void load_a<SimdTypeI, ValueTypeI>(SimdTypeI& val_simd,
                                        const ValueTypeI* val_ptr)
{
    val_simd = _mm_load_si128(reinterpret_cast<const SimdTypeI*>(val_ptr));
}

template <>
inline void store<SimdTypeI, ValueTypeI>(const SimdTypeI& val_simd, ValueTypeI* val_ptr)
{
    _mm_storeu_si128(reinterpret_cast<SimdTypeI*>(val_ptr), val_simd);
}

template <>
inline void store_a<SimdTypeI, ValueTypeI>(const SimdTypeI& val_simd, ValueTypeI* val_ptr)
{
    _mm_store_si128(reinterpret_cast<SimdTypeI*>(val_ptr), val_simd);
}

template <>
inline void store_stream<SimdTypeI, ValueTypeI>(const SimdTypeI& val_simd,
                                          ValueTypeI* val_ptr)
{
    _mm_stream_si128(reinterpret_cast<SimdTypeI*>(val_ptr), val_simd);
}

template <>
inline void load_partial<SimdTypeI, ValueTypeI>(SimdTypeI& val_simd, int index,
                                              const ValueTypeI* val_ptr)
{
    switch (index) {
    case 0:
        val_simd = _mm_setzero_si128();
        break;
    case 1:
        val_simd = _mm_loadu_si32(val_ptr);
        break;
    case 2:
        val_simd = _mm_setr_epi32(val_ptr[0], val_ptr[1], 0.f, 0.f);
        break;
    case 3:
        val_simd = _mm_setr_epi32(val_ptr[0], val_ptr[1], val_ptr[2], 0.f);
        break;
    case 4:
        load(val_simd, val_ptr);
        break;
    default:
        break;
    }
}

template <>
inline void store_partial<SimdI_t, ValueTypeI>(SimdI_t& class_simd, int index,
                                              ValueTypeI* val_ptr)
{
    if (index < 1)
        return;
    const ValueTypeI* data = class_simd.storeScalar();
    if (index > SimdI_t::SimdLen) {
        index = SimdI_t::SimdLen;
    }
    std::copy(data, data + index, val_ptr);
}

// ------------------------------------------------------
// Gather and Scatter
// ------------------------------------------------------
template <>
inline void gather<SimdTypeI, ValueTypeI>(SimdTypeI& val_simd, const SimdIndex& index, const ValueTypeI* val_ptr)
{

}

template <>
inline void scatter<SimdTypeI, ValueTypeI>(const SimdTypeI& val_simd, const SimdIndex& index, ValueTypeI* val_ptr)
{

}

template <>
inline SimdTypeI select(const SimdTypeI& mask, const SimdTypeI& a, const SimdTypeI& b) {
#if defined(__KFP_SIMD__SSE4_1)   // SSE4.1
    return _mm_blendv_epi8(b, a, mask);
#else
    return _mm_or_si128(
        _mm_and_si128(mask, a),
        _mm_andnot_si128(mask, b));
#endif
}

template<>
inline void insert<SimdTypeI, ValueTypeI>(SimdTypeI& val_simd, int index, ValueTypeI val)
{
    int32_t indices[4] = {0,0,0,0} ;
    indices[index] = -1 ;
    const SimdTypeI mask = _mm_loadu_si128(reinterpret_cast<const SimdTypeI*>(indices)) ;
    val_simd = select(mask, _mm_set1_epi32(val), val_simd) ;
}

template <int N>
inline ValueTypeI get(const SimdTypeI& val_simd)
{
    if (N >= SimdI_t::SimdLen) {
        throw "[Error] (extract): invalid index given to extract from __m128 type." ;
    }
    if (N < 0) {
        throw "[Error] (extract): invalid index given to extract from __m128 type." ;
    }
    const SimdTypeI result = _mm_shuffle_epi32(val_simd, _MM_SHUFFLE(0,0,0,N)) ;
    return _mm_cvtsi128_si32(result);
}

template <>
inline ValueTypeI extract<SimdI_t, ValueTypeI>(const SimdI_t& class_simd, const int index)
{
#if 0
    const ValueTypeI* data = class_simd.storeScalar();
    if (index >= SimdI_t::SimdLen) {
        index = SimdI_t::SimdLen - 1;
    }
    return data[index];
#elif defined(__KFP_SIMD__SSE4_1)
    const SimdTypeI val_simd = class_simd.simd() ;
    switch (index) {
    case 0:
        return _mm_extract_epi32(val_simd, 0x00) ;
    case 1:
        return _mm_extract_epi32(val_simd, 0x01) ;
    case 2:
        return _mm_extract_epi32(val_simd, 0x02) ;
    case 3:
    default:
        return _mm_extract_epi32(val_simd, 0x03) ;
    }
#else
    const SimdTypeI val_simd = class_simd.simd() ;
    SimdTypeI result = _mm_setzero_si128() ;
    switch (index) {
    case 0:
        result = _mm_shuffle_epi32(val_simd, _MM_SHUFFLE(0,0,0,0)) ;
        break;
    case 1:
        result = _mm_shuffle_epi32(val_simd, _MM_SHUFFLE(0,0,0,1)) ;
        break;
    case 2:
        result = _mm_shuffle_epi32(val_simd, _MM_SHUFFLE(0,0,0,2)) ;
        break;
    case 3:
    default:
        result = _mm_shuffle_epi32(val_simd, _MM_SHUFFLE(0,0,0,3)) ;
        break;
    }
    return _mm_cvtsi128_si32(result);
#endif
}

template<>
inline void cutoff<SimdI_t>(SimdI_t& class_simd, int index)
{
    SimdTypeI& val_simd = class_simd.simd() ;
    const ValueTypeI* scalar = class_simd.storeScalar() ;
    load_partial<SimdTypeI, ValueTypeI>(val_simd, index, scalar) ;
}

template<>
inline void print<SimdI_t>(std::ostream& stream, SimdI_t class_simd)
{
    const ValueTypeI* val_scalar = class_simd.storeScalar() ;
    stream << "[" << val_scalar[0] << ", " << val_scalar[1] << ", " << val_scalar[2] << ", " << val_scalar[3]
           << "]";
}

template<>
inline SimdTypeI add<SimdTypeI>(const SimdTypeI& a, const SimdTypeI& b)
{
    return _mm_add_epi32(a,b) ;
}

template<>
inline SimdTypeI substract<SimdTypeI>(const SimdTypeI& a, const SimdTypeI& b)
{
    return _mm_sub_epi32(a,b) ;
}

template<>
inline SimdTypeI multiply<SimdTypeI, SimdI_t>(SimdI_t& a, SimdI_t& b)
{
#if defined(__KFP_SIMD__SSE4_1)   // SSE4.1
    return _mm_mullo_epi32(a.simd(), b.simd()) ;
#else
    const ValueTypeI* data1 = a.storeScalar() ;
    const ValueTypeI* data2 = b.storeScalar() ;
    const ValueTypeI data[SimdI_t::SimdLen]{ data1[0]*data2[0], data1[1]*data2[1], data1[2]*data2[2], data1[3]*data2[3] } ;
    SimdTypeI result ;
    load<SimdTypeI, ValueTypeI>(result, data) ;
    return result ;
#endif
}

template<>
inline SimdTypeI divide<SimdTypeI, SimdI_t>(SimdI_t& a, SimdI_t& b)
{
    const ValueTypeI* data1 = a.storeScalar() ;
    const ValueTypeI* data2 = b.storeScalar() ;
    const ValueTypeI data[SimdI_t::SimdLen]{ data1[0]/data2[0], data1[1]/data2[1], data1[2]/data2[2], data1[3]/data2[3] } ;
    SimdTypeI result ;
    load<SimdTypeI, ValueTypeI>(result, data) ;
    return result ;
}

template<>
inline SimdTypeI min<SimdTypeI>(const SimdTypeI& a, const SimdTypeI& b)
{
#if defined(__KFP_SIMD__SSE4_1)   // SSE4.1
    return _mm_min_epi32(a, b) ;
#else
    const SimdTypeI mask = _mm_cmpgt_epi32(a, b) ;
    return select(mask, b, a) ;
#endif
}

template<>
inline SimdTypeI max<SimdTypeI>(const SimdTypeI& a, const SimdTypeI& b)
{
#if defined(__KFP_SIMD__SSE4_1)   // SSE4.1
    return _mm_max_epi32(a, b) ;
#else
    const SimdTypeI mask = _mm_cmpgt_epi32(a, b) ;
    return select(mask, a, b) ;
#endif
}

template<>
inline SimdTypeI sqrt<SimdTypeI, SimdI_t>(SimdI_t class_simd)
{
    ValueTypeI* data = class_simd.storeScalar();
    return _mm_setr_epi32(std::sqrt(data[0]), std::sqrt(data[1]), std::sqrt(data[2]), std::sqrt(data[3])) ;
}

template<>
inline SimdTypeI rsqrt<SimdTypeI, SimdI_t>(SimdI_t class_simd)
{
    return _mm_cvtps_epi32( _mm_rsqrt_ps(_mm_cvtepi32_ps(class_simd.simd())) ) ;
}

template<>
inline SimdTypeI abs<SimdTypeI>(const SimdTypeI& a)
{
#if defined(__KFP_SIMD__SSSE3)   // SSSE3
    return _mm_abs_epi32(a) ;
#else
    const SimdTypeI mask = _mm_set1_epi32(0x7FFFFFFF);
    return _mm_and_si128(a, mask);
#endif
}

template<>
inline SimdTypeI log<SimdTypeI, SimdI_t>(SimdI_t class_simd)
{
    ValueTypeI* data = class_simd.storeScalar();
    return _mm_setr_epi32(std::log(data[0]), std::log(data[1]), std::log(data[2]), std::log(data[3])) ;
}

template<>
inline SimdTypeI pow<SimdTypeI, SimdI_t>(SimdI_t class_simd, int exp)
{
#if 0
    std::cerr << "[Error]: SimdI_t pow not implemented\n" ;
    exit(1) ;
#else
    ValueTypeI* data = class_simd.storeScalar();
    return _mm_setr_epi32(std::pow(data[0], exp), std::pow(data[1], exp), std::pow(data[2], exp), std::pow(data[3], exp)) ;
#endif
}

template<>
inline SimdTypeI opLessThan<SimdTypeI>(const SimdTypeI& a, const SimdTypeI& b)
{
    return _mm_cmplt_epi32(a, b) ;
}

template<>
inline SimdTypeI opLessThanEqual<SimdTypeI>(const SimdTypeI& a, const SimdTypeI& b)
{
#if defined(__KFP_SIMD__SSE4_1)   // SSE4.1
    return _mm_cmpeq_epi32(_mm_min_epi32(a, b), a) ;
#else
    return _mm_xor_si128(_mm_cmpgt_epi32(a, b), _mm_set1_epi32(-1)) ;
#endif
}

template<>
inline SimdTypeI opGreaterThan<SimdTypeI>(const SimdTypeI& a, const SimdTypeI& b)
{
    return _mm_cmpgt_epi32(a, b);
}

template<>
inline SimdTypeI opGreaterThanEqual<SimdTypeI>(const SimdTypeI& a, const SimdTypeI& b)
{
#if defined(__KFP_SIMD__SSE4_1)   // SSE4.1
    return _mm_cmpeq_epi32(_mm_min_epi32(b, a), b) ;
#else
    return _mm_xor_si128(_mm_cmplt_epi32(a, b), _mm_set1_epi32(-1)) ;
#endif
}

template<>
inline SimdTypeI opEqual<SimdTypeI>(const SimdTypeI& a, const SimdTypeI& b)
{
    return _mm_cmpeq_epi32(a, b);
}

} // namespace Detail

inline SimdI_t select(const SimdMask& mask, const SimdI_t& a, const SimdI_t& b)
{
    return SimdI_t(Detail::select<SimdI_t::simd_type>(mask.maski(), a.simd(), b.simd())) ;
}

} // namespace SIMD
} // namespace KFP
