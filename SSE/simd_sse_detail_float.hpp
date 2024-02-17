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

using SimdF_t = SimdBaseClass<float, __m128, ABI::SSE>;

inline SimdF_t SimdF(SimdF_t::value_type val1, SimdF_t::value_type val2, SimdF_t::value_type val3, SimdF_t::value_type val4)
{
    return SimdF_t(_mm_setr_ps(val1, val2, val3, val4)) ;
}

namespace Detail {

typedef __m128 SimdTypeF;
typedef float ValueTypeF;

template <>
inline void constant<SimdTypeF, ValueTypeF>(SimdTypeF& val_simd, ValueTypeF val)
{
    val_simd = _mm_set1_ps(val);
}

template <>
inline void load<SimdTypeF, ValueTypeF>(SimdTypeF& val_simd,
                                      const ValueTypeF* val_ptr)
{
    val_simd = _mm_loadu_ps(val_ptr);
}

template <>
inline void load_a<SimdTypeF, ValueTypeF>(SimdTypeF& val_simd,
                                        const ValueTypeF* val_ptr)
{
    val_simd = _mm_load_ps(val_ptr);
}

template <>
inline void store<SimdTypeF, ValueTypeF>(const SimdTypeF& val_simd, ValueTypeF* val_ptr)
{
    _mm_storeu_ps(val_ptr, val_simd);
}

template <>
inline void store_a<SimdTypeF, ValueTypeF>(const SimdTypeF& val_simd, ValueTypeF* val_ptr)
{
    _mm_store_ps(val_ptr, val_simd);
}

template <>
inline void store_stream<SimdTypeF, ValueTypeF>(const SimdTypeF& val_simd,
                                          ValueTypeF* val_ptr)
{
    _mm_stream_ps(val_ptr, val_simd);
}

template <>
inline void load_partial<SimdTypeF, ValueTypeF>(SimdTypeF& val_simd, int index,
                                              const ValueTypeF* val_ptr)
{
    switch (index) {
    case 0:
        val_simd = _mm_setzero_ps();
        break;
    case 1:
        val_simd = _mm_load_ss(val_ptr);
        break;
    case 2:
        val_simd = _mm_setr_ps(val_ptr[0], val_ptr[1], 0.f, 0.f);
        break;
    case 3:
        val_simd = _mm_setr_ps(val_ptr[0], val_ptr[1], val_ptr[2], 0.f);
        break;
    case 4:
        load(val_simd, val_ptr);
        break;
    default:
        break;
    }
}

template <>
inline void store_partial<SimdF_t, ValueTypeF>(SimdF_t& class_simd, int index,
                                              ValueTypeF* val_ptr)
{
    if (index < 1)
        return;
    const ValueTypeF* data = class_simd.storeScalar();
    if (index > SimdF_t::SimdLen) {
        index = SimdF_t::SimdLen;
    }
    std::copy(data, data + index, val_ptr);
}

// ------------------------------------------------------
// Gather and Scatter
// ------------------------------------------------------
template <>
inline void gather<SimdF_t, ValueTypeF>(SimdF_t& class_simd, const SimdIndex& index, const ValueTypeF* val_ptr)
{
    const int* indices = index.scalar() ;
    class_simd.simd() = _mm_setr_ps( val_ptr[indices[0]], val_ptr[indices[1]], val_ptr[indices[2]], val_ptr[indices[3]]) ;
}

template <>
inline void scatter<SimdF_t, ValueTypeF>(SimdF_t& class_simd, const SimdIndex& index, ValueTypeF* val_ptr)
{
    const ValueTypeF* data = class_simd.storeScalar() ;
    const int* indices = index.scalar() ;
    val_ptr[indices[0]] = data[0] ;
    val_ptr[indices[1]] = data[1] ;
    val_ptr[indices[2]] = data[2] ;
    val_ptr[indices[3]] = data[3] ;
}

template <>
inline SimdTypeF select(const SimdTypeF& mask, const SimdTypeF& a, const SimdTypeF& b) {
#if defined(__KFP_SIMD__SSE4_1)   // SSE4.1
    return _mm_blendv_ps(b, a, mask);
#else
    return _mm_or_ps(
        _mm_and_ps(mask, a),
        _mm_andnot_ps(mask, b));
#endif
}

template<>
inline void insert<SimdTypeF, ValueTypeF>(SimdTypeF& val_simd, int index, ValueTypeF val)
{
#if defined(__KFP_SIMD__SSE4_1)   // SSE4.1
    switch (index) {
    case 0:
        val_simd = _mm_insert_ps(val_simd, _mm_set_ss(val), 0 << 4);  break;
    case 1:
        val_simd = _mm_insert_ps(val_simd, _mm_set_ss(val), 1 << 4);  break;
    case 2:
        val_simd = _mm_insert_ps(val_simd, _mm_set_ss(val), 2 << 4);  break;
    case 3:
    default:
        val_simd = _mm_insert_ps(val_simd, _mm_set_ss(val), 3 << 4);  break;
    }
#else
    int32_t indices[4] = {0,0,0,0} ;
    indices[index] = -1 ;
    const SimdTypeF mask = _mm_loadu_ps(reinterpret_cast<float*>(indices)) ;
    val_simd = select(mask, _mm_set1_ps(val), val_simd) ;
#endif
}

template <int N>
inline ValueTypeF get(const SimdTypeF& val_simd)
{
    if (N >= SimdF_t::SimdLen) {
        throw "[Error] (get): invalid index given to extract from __m128 type." ;
    }
    if (N < 0) {
        throw "[Error] (get): invalid index given to extract from __m128 type." ;
    }
    const SimdTypeF result = _mm_shuffle_ps(val_simd, val_simd, _MM_SHUFFLE(0,0,0,N)) ;
    return _mm_cvtss_f32(result);
}

template <>
inline ValueTypeF extract<SimdF_t, ValueTypeF>(const SimdF_t& class_simd, const int index)
{
#if 0
    const ValueTypeF* data = class_simd.storeScalar();
    if (index >= SimdI_t::SimdLen) {
        index = SimdI_t::SimdLen - 1;
    }
    return data[index];
// #elif defined(__KFP_SIMD__SSE4_1)
#elif 0
    const SimdTypeF val_simd = class_simd.simd() ;
    float result ;
    switch (index) {
    case 0:
        _MM_EXTRACT_FLOAT(result, val_simd, 0x00) ;
        break ;
    case 1:
        _MM_EXTRACT_FLOAT(result, val_simd, 0x01) ;
        break ;
    case 2:
        _MM_EXTRACT_FLOAT(result, val_simd, 0x02) ;
        break ;
    case 3:
    default:
        _MM_EXTRACT_FLOAT(result, val_simd, 0x03) ;
        break ;
    }
    return result ;
#else
    const SimdTypeF val_simd = class_simd.simd() ;
    SimdTypeF result = _mm_setzero_ps() ;
    switch (index) {
    case 0:
        result = _mm_shuffle_ps(val_simd, val_simd, _MM_SHUFFLE(0,0,0,0)) ;
        break;
    case 1:
        result = _mm_shuffle_ps(val_simd, val_simd, _MM_SHUFFLE(0,0,0,1)) ;
        break;
    case 2:
        result = _mm_shuffle_ps(val_simd, val_simd, _MM_SHUFFLE(0,0,0,2)) ;
        break;
    case 3:
    default:
        result = _mm_shuffle_ps(val_simd, val_simd, _MM_SHUFFLE(0,0,0,3)) ;
        break;
    }
    return _mm_cvtss_f32(result);
#endif
}

template<>
inline void cutoff<SimdF_t>(SimdF_t& class_simd, int index)
{
    SimdTypeF& val_simd = class_simd.simd() ;
    const ValueTypeF* scalar = class_simd.storeScalar() ;
    load_partial<SimdTypeF, ValueTypeF>(val_simd, index, scalar) ;
}

template<>
inline void print<SimdF_t>(std::ostream& stream, SimdF_t class_simd)
{
    const ValueTypeF* val_scalar = class_simd.storeScalar() ;
    stream << "[" << val_scalar[0] << ", " << val_scalar[1] << ", " << val_scalar[2] << ", " << val_scalar[3]
           << "]";
}

template<>
inline SimdTypeF minus<SimdTypeF>(const SimdTypeF& a)
{
    return _mm_xor_ps(a, _mm_castsi128_ps(_mm_set1_epi32(0x80000000)));
}

template<>
inline SimdTypeF add<SimdTypeF>(const SimdTypeF& a, const SimdTypeF& b)
{
    return _mm_add_ps(a,b) ;
}

template<>
inline SimdTypeF substract<SimdTypeF>(const SimdTypeF& a, const SimdTypeF& b)
{
    return _mm_sub_ps(a,b) ;
}

template<>
inline SimdTypeF multiply<SimdTypeF, SimdF_t>(SimdF_t& a, SimdF_t& b)
{
    return _mm_mul_ps(a.simd(), b.simd()) ;
}

template<>
inline SimdTypeF divide<SimdTypeF, SimdF_t>(SimdF_t& a, SimdF_t& b)
{
    return _mm_div_ps(a.simd(), b.simd()) ;
}

template<>
inline SimdTypeF min<SimdTypeF>(const SimdTypeF& a, const SimdTypeF& b)
{
    return _mm_min_ps(a,b) ;
}

template<>
inline SimdTypeF max<SimdTypeF>(const SimdTypeF& a, const SimdTypeF& b)
{
    return _mm_max_ps(a,b) ;
}

template<>
inline SimdTypeF sqrt<SimdTypeF, SimdF_t>(SimdF_t class_simd)
{
    return _mm_sqrt_ps(class_simd.simd()) ;
}

template<>
inline SimdTypeF rsqrt<SimdTypeF, SimdF_t>(SimdF_t class_simd)
{
    return _mm_rsqrt_ps(class_simd.simd());
}

template<>
inline SimdTypeF abs<SimdTypeF>(const SimdTypeF& a)
{
    const __m128 mask = _mm_castsi128_ps(_mm_set1_epi32(0x7FFFFFFF));
    return _mm_and_ps(a, mask);
}

template<>
inline SimdTypeF log<SimdTypeF, SimdF_t>(SimdF_t class_simd)
{
    ValueTypeF* data = class_simd.storeScalar();
    return _mm_setr_ps(std::log(data[0]), std::log(data[1]), std::log(data[2]), std::log(data[3])) ;
}

template<>
inline SimdTypeF pow<SimdTypeF, SimdF_t>(SimdF_t class_simd, int exp)
{
#if 0
    std::cerr << "[Error]: SimdF_t pow not implemented\n" ;
    exit(1) ;
#else
    ValueTypeF* data = class_simd.storeScalar();
    return _mm_setr_ps(std::pow(data[0], exp), std::pow(data[1], exp), std::pow(data[2], exp), std::pow(data[3], exp)) ;
#endif
}

template<>
inline SimdTypeF opLessThan<SimdTypeF>(const SimdTypeF& a, const SimdTypeF& b)
{
    return _mm_cmplt_ps(a, b);
}

template<>
inline SimdTypeF opLessThanEqual<SimdTypeF>(const SimdTypeF& a, const SimdTypeF& b)
{
    return _mm_cmple_ps(a, b);
}

template<>
inline SimdTypeF opGreaterThan<SimdTypeF>(const SimdTypeF& a, const SimdTypeF& b)
{
    return _mm_cmpgt_ps(a, b);
}

template<>
inline SimdTypeF opGreaterThanEqual<SimdTypeF>(const SimdTypeF& a, const SimdTypeF& b)
{
    return _mm_cmpge_ps(a, b);
}

template<>
inline SimdTypeF opEqual<SimdTypeF>(const SimdTypeF& a, const SimdTypeF& b)
{
    return _mm_cmpeq_ps(a, b);
}

} // namespace Detail

inline SimdF_t select(const SimdMask& mask, const SimdF_t& a, const SimdF_t& b)
{
    return SimdF_t(Detail::select<SimdF_t::simd_type>(mask.maskf(), a.simd(), b.simd())) ;
}

} // namespace SIMD
} // namespace KFP
