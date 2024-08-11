// -*- C++ Header -*-
/*
==================================================
Authors: A.Mithran;
Emails: mithran@fias.uni-frankfurt.de
==================================================
*/

#ifndef SIMD_AVX_DETAIL_INT_H
#define SIMD_AVX_DETAIL_INT_H

#include "../Base/simd_macros.h"
#include "../Base/simd_detail.h"
#include "simd_avx_type.h"

#include <cmath>
#include <iostream>

namespace KFP {
namespace SIMD {

namespace Detail {

enum class MASK { ABS, MINUS, TRUE, INF };
template <MASK mask> inline SimdDataI getMask() {
  switch (mask) {
  case MASK::ABS:
    return _mm256_set1_epi32(0x7FFFFFFF);
  case MASK::MINUS:
    return _mm256_set1_epi32(0x80000000);
  case MASK::TRUE:
    return _mm256_set1_epi32(-1);
  case MASK::INF:
    return _mm256_set1_epi32(0x7F800000);
  }
}

// ------------------------------------------------------
// General
// ------------------------------------------------------
template <>
inline SimdDataI type_cast<SimdDataI, SimdDataF>(const SimdDataF &val_simd) {
  return _mm256_castps_si256(val_simd);
}
template <>
inline SimdDataI type_cast<SimdDataI>(const SimdDataI &val_simd) {
  return val_simd;
}
template <>
inline SimdDataI value_cast<SimdDataI, SimdDataF>(const SimdDataF &val_simd) {
  return _mm256_cvttps_epi32(val_simd);
}
template <>
inline SimdDataI value_cast<SimdDataI>(const SimdDataI &val_simd) {
  return val_simd;
}
template <> inline SimdDataI constant<SimdDataI, ValueDataI>(ValueDataI val) {
  return _mm256_set1_epi32(val);
}

// ------------------------------------------------------
// Load and Store
// ------------------------------------------------------
template <>
inline SimdDataI load<SimdDataI, ValueDataI>(const ValueDataI *val_ptr) {
  return _mm256_loadu_si256(reinterpret_cast<const SimdDataI *>(val_ptr));
}
template <>
inline SimdDataI load_a<SimdDataI, ValueDataI>(const ValueDataI *val_ptr) {
  return _mm256_load_si256(reinterpret_cast<const SimdDataI *>(val_ptr));
}
template <>
inline void store<SimdDataI, ValueDataI>(const SimdDataI &val_simd,
                                         ValueDataI *val_ptr) {
  _mm256_storeu_si256(reinterpret_cast<SimdDataI *>(val_ptr), val_simd);
}
template <>
inline void store_a<SimdDataI, ValueDataI>(const SimdDataI &val_simd,
                                           ValueDataI *val_ptr) {
  _mm256_store_si256(reinterpret_cast<SimdDataI *>(val_ptr), val_simd);
}

// ------------------------------------------------------
// Logical bitwise
// ------------------------------------------------------
template <>
inline SimdDataI ANDBits<SimdDataI>(const SimdDataI &a, const SimdDataI &b) {
  return _mm256_and_si256(a, b);
}
template <>
inline SimdDataI ORBits<SimdDataI>(const SimdDataI &a, const SimdDataI &b) {
  return _mm256_or_si256(a, b);
}
template <>
inline SimdDataI XORBits<SimdDataI>(const SimdDataI &a, const SimdDataI &b) {
  return _mm256_xor_si256(a, b);
}
template <>
inline SimdDataI NOTBits<SimdDataI>(const SimdDataI& a)
{
    return XORBits<SimdDataI>(getMask<MASK::TRUE>(), a);
}

// ------------------------------------------------------
// Comparison
// ------------------------------------------------------
template <>
inline SimdDataI equal<SimdDataI>(const SimdDataI &a, const SimdDataI &b) {
  return _mm256_cmpeq_epi32(a, b);
}
template <>
inline SimdDataI notEqual<SimdDataI>(const SimdDataI &a, const SimdDataI &b) {
  return XORBits<SimdDataI>(equal<SimdDataI>(a, b), getMask<MASK::TRUE>());
}
template <>
inline SimdDataI lessThan<SimdDataI>(const SimdDataI &a, const SimdDataI &b) {
  return _mm256_cmpgt_epi32(b, a);
}
template <>
inline SimdDataI greaterThan<SimdDataI>(const SimdDataI &a, const SimdDataI &b) {
  return _mm256_cmpgt_epi32(a, b);
}
template <>
inline SimdDataI lessThanEqual<SimdDataI>(const SimdDataI &a, const SimdDataI &b) {
  return XORBits<SimdDataI>(greaterThan<SimdDataI>(a, b), getMask<MASK::TRUE>());
}
template <>
inline SimdDataI greaterThanEqual<SimdDataI>(const SimdDataI &a, const SimdDataI &b) {
  return XORBits<SimdDataI>(lessThan<SimdDataI>(a, b), getMask<MASK::TRUE>());
}

// ------------------------------------------------------
// Manipulate bits
// ------------------------------------------------------
template <> inline SimdDataI shiftLBits<SimdDataI>(const SimdDataI &a, int b) {
  return _mm256_slli_epi32(a, b);
}
template <> inline SimdDataI shiftRBits<SimdDataI>(const SimdDataI &a, int b) {
  return _mm256_srai_epi32(a, b);
}

// ------------------------------------------------------
// Logical lanewise
// ------------------------------------------------------
template <>
inline SimdDataI ANDLanes<SimdDataI>(const SimdDataI& a, const SimdDataI& b) {
    return ANDBits<SimdDataI>(a, b);
}

template <>
inline SimdDataI ORLanes<SimdDataI>(const SimdDataI& a, const SimdDataI& b) {
    return ORBits<SimdDataI>(a, b);
}

// ------------------------------------------------------
// Manipulate lanes
// ------------------------------------------------------
template <>
inline SimdDataI select<SimdDataI>(const SimdDataI &mask, const SimdDataI &a, const SimdDataI &b) {
#if defined(__KFP_SIMD__AVX) //TODO
  return _mm256_blendv_epi8(b, a, mask);
#else
  return ORBits<SimdDataI>(ANDBits<SimdDataI>(mask, a), _mm256_andnot_si256(mask, b));
#endif
}
template <int N> inline ValueDataI get(const SimdDataI &a) {
  const SimdDataI result = _mm256_permutevar8x32_epi32(a, _mm256_set1_epi32(N));
  return _mm256_extract_epi32(result, 0);
}
template <>
inline ValueDataI extract<ValueDataI, SimdDataI>(int index, const SimdDataI &val_simd) {
#if 0
    ValueDataI __KFP_SIMD__ALIGN_V1(__KFP_SIMD__Size_Int)
    data[__KFP_SIMD__Len_Int]{}; // Helper array
    store_a<SimdDataI, ValueDataI>(val_simd, data);
    return data[index];
#elif defined(__KFP_SIMD__AVX)
  switch (index) {
  case 0:
    return _mm256_extract_epi32(val_simd, 0);
  case 1:
    return _mm256_extract_epi32(val_simd, 1);
  case 2:
    return _mm256_extract_epi32(val_simd, 2);
  case 3:
    return _mm256_extract_epi32(val_simd, 3);
  case 4:
    return _mm256_extract_epi32(val_simd, 4);
  case 5:
    return _mm256_extract_epi32(val_simd, 5);
  case 6:
    return _mm256_extract_epi32(val_simd, 6);
  case 7:
  default:
    return _mm256_extract_epi32(val_simd, 7);
  }
#else
  switch (index) {
  case 0:
    return get<0>(val_simd);
  case 1:
    return get<1>(val_simd);
  case 2:
    return get<2>(val_simd);
  case 3:
    return get<3>(val_simd);
  }
#endif
}
template <>
inline void insert<SimdDataI, ValueDataI>(SimdDataI &val_simd, int index, const ValueDataI &val) {
  __m256i indices = _mm256_setzero_si256();
  indices = _mm256_insert_epi32(indices, -1, index);
  const SimdDataI mask = _mm256_cmpeq_epi32(indices, _mm256_set1_epi32(-1));
  val_simd = select<SimdDataI>(mask, _mm256_set1_epi32(val), val_simd);
}

// Robin: I'm not sure about these two functions. Additionally, SSE only has one function for shiftLLanes. I put another one below.
// template <>
// inline SimdDataI shiftLLanes<SimdDataI>(int n, const SimdDataI &val_simd) {
//   constexpr int value_size_bytes = sizeof(int);
//   switch (n) {
//   case 0:
//     return val_simd;
//   case 1:
//     return _mm256_permutevar8x32_epi32(val_simd, _mm256_setr_epi32(1, 2, 3, 4, 5, 6, 7, 0));
//   case 2:
//     return _mm256_permutevar8x32_epi32(val_simd, _mm256_setr_epi32(2, 3, 4, 5, 6, 7, 0, 1));
//   case 3:
//     return _mm256_permutevar8x32_epi32(val_simd, _mm256_setr_epi32(3, 4, 5, 6, 7, 0, 1, 2));
//   default:
//     return _mm256_set1_epi32(0);
//   }
// }
// template <>
// inline SimdDataI shiftLLanes<SimdDataI>(int n, const SimdDataI &val_simd) {
//   constexpr int value_size_bytes = sizeof(int) * 8;
//   __m256i indices = _mm256_setr_epi32(0, 1, 2, 3, 4, 5, 6, 7); //TODO Initial indices
//   indices = _mm256_add_epi32(indices, _mm256_set1_epi32(n)); // Add n to each index
//   return _mm256_permutevar8x32_epi32(val_simd, indices); // Permute based on indices
// }

// Robin:
template <>
inline __m256i shiftLLanes<__m256i>(int n, const __m256i &val_simd) {
  constexpr int value_size_bytes = sizeof(int);
  constexpr int lane_count = 8; // AVX has 8 lanes for 32-bit integers
  constexpr int total_size_bytes = lane_count * value_size_bytes;

  if (n < 0 || n >= lane_count) {
    return _mm256_setzero_si256();
  }

  switch (n) {
  case 0:
    return val_simd;
  case 1:
    return _mm256_srli_si256(val_simd, value_size_bytes);
  case 2:
    return _mm256_srli_si256(val_simd, 2 * value_size_bytes);
  case 3:
    return _mm256_srli_si256(val_simd, 3 * value_size_bytes);
  case 4:
    return _mm256_srli_si256(val_simd, 4 * value_size_bytes);
  case 5:
    return _mm256_srli_si256(val_simd, 5 * value_size_bytes);
  case 6:
    return _mm256_srli_si256(val_simd, 6 * value_size_bytes);
  case 7:
    return _mm256_srli_si256(val_simd, 7 * value_size_bytes);
  default:
    return _mm256_setzero_si256();
  }
}

// Robin: I also don't know about this one. I put another one below.
// template <> inline SimdDataI rotate<SimdDataI>(int n, const SimdDataI &val_simd) {
// #if defined(__KFP_SIMD__AVX)
//   constexpr int value_size_bytes = sizeof(int);
//   __m256i tmp1, tmp2;
//   switch (n) {
//   case 0:
//     return val_simd;
//   case 1:
//     tmp1 = _mm256_permute2x128_si256(val_simd, val_simd, 0x03);
//     tmp2 = _mm256_alignr_epi8(val_simd, tmp1, value_size_bytes);
//     return tmp2;
//   case 2:
//     return _mm256_permute4x64_epi64(val_simd, _MM_SHUFFLE(2, 1, 0, 3));
//   case 3:
//     tmp1 = _mm256_permute2x128_si256(val_simd, val_simd, 0x03);
//     tmp2 = _mm256_alignr_epi8(val_simd, tmp1, 3 * value_size_bytes);
//     return tmp2;
//   }
//   return _mm256_setzero_si256();
// #else
//   switch (n) {
//   case 0:
//     return val_simd;
//   case 1:
//     return ORBits<SimdDataI>(shiftLLanes<SimdDataI>(1, val_simd),
//                              shiftRLanes<SimdDataI>(3, val_simd));
//   case 2:
//     return ORBits<SimdDataI>(shiftLLanes<SimdDataI>(2, val_simd),
//                              shiftRLanes<SimdDataI>(2, val_simd));
//   case 3:
//     return ORBits<SimdDataI>(shiftLLanes<SimdDataI>(3, val_simd),
//                              shiftRLanes<SimdDataI>(1, val_simd));
//   }
//   return constant<SimdDataI, ValueDataI>(0);
// #endif
// }

// Robin:
template <>
inline __m256i rotate<__m256i>(int n, const __m256i &val_simd) {
  constexpr int value_size_bytes = sizeof(int);
  constexpr int lane_count = 8;

  if (n < 0 || n >= lane_count) {
    return _mm256_setzero_si256();
  }

  switch (n) {
  case 0:
    return val_simd;
  case 1:
    return _mm256_or_si256(_mm256_srli_si256(val_simd, 4), _mm256_slli_si256(val_simd, 28));
  case 2:
    return _mm256_or_si256(_mm256_srli_si256(val_simd, 8), _mm256_slli_si256(val_simd, 24));
  case 3:
    return _mm256_or_si256(_mm256_srli_si256(val_simd, 12), _mm256_slli_si256(val_simd, 20));
  case 4:
    return _mm256_or_si256(_mm256_srli_si256(val_simd, 16), _mm256_slli_si256(val_simd, 16));
  case 5:
    return _mm256_or_si256(_mm256_srli_si256(val_simd, 20), _mm256_slli_si256(val_simd, 12));
  case 6:
    return _mm256_or_si256(_mm256_srli_si256(val_simd, 24), _mm256_slli_si256(val_simd, 8));
  case 7:
    return _mm256_or_si256(_mm256_srli_si256(val_simd, 28), _mm256_slli_si256(val_simd, 4));
  default:
    return _mm256_setzero_si256();
  }
}

// ------------------------------------------------------
// Basic Arithmetic
// ------------------------------------------------------
template <>
inline SimdDataI add<SimdDataI>(const SimdDataI &a, const SimdDataI &b) {
  return _mm256_add_epi32(a, b);
}

template <>
inline SimdDataI substract<SimdDataI>(const SimdDataI &a, const SimdDataI &b) {
  return _mm256_sub_epi32(a, b);
}

template <>
inline SimdDataI multiply<SimdDataI>(const SimdDataI &a, const SimdDataI &b) {
#if defined(__KFP_SIMD__AVX) // TODO
  return _mm256_mullo_epi32(a, b);
#else
  ValueDataI __KFP_SIMD__ATTR_ALIGN(__KFP_SIMD__Size_Int)
      data1[__KFP_SIMD__Len_Int]{}; // Helper array
  ValueDataI __KFP_SIMD__ATTR_ALIGN(__KFP_SIMD__Size_Int)
      data2[__KFP_SIMD__Len_Int]{}; // Helper array
  store_a<SimdDataI, ValueDataI>(a, data1);
  store_a<SimdDataI, ValueDataI>(b, data2);
  const ValueDataI __KFP_SIMD__ATTR_ALIGN(__KFP_SIMD__Size_Int)
      data[__KFP_SIMD__Len_Int]{data1[0] * data2[0], data1[1] * data2[1],
                                data1[2] * data2[2], data1[3] * data2[3],
                                data1[4] * data2[4], data1[5] * data2[5],
                                data1[6] * data2[6], data1[7] * data2[7]};
  return load_a<SimdDataI, ValueDataI>(data);
#endif
}
template <>
inline SimdDataI divide<SimdDataI>(const SimdDataI &a, const SimdDataI &b) {
  ValueDataI __KFP_SIMD__ATTR_ALIGN(__KFP_SIMD__Size_Int)
      data1[__KFP_SIMD__Len_Int]{}; // Helper array
  ValueDataI __KFP_SIMD__ATTR_ALIGN(__KFP_SIMD__Size_Int)
      data2[__KFP_SIMD__Len_Int]{}; // Helper array
  store_a<SimdDataI, ValueDataI>(a, data1);
  store_a<SimdDataI, ValueDataI>(b, data2);
  const ValueDataI __KFP_SIMD__ATTR_ALIGN(__KFP_SIMD__Size_Int)
      data[__KFP_SIMD__Len_Int]{data1[0] / data2[0], data1[1] / data2[1],
                                data1[2] / data2[2], data1[3] / data2[3],
                                data1[4] / data2[4], data1[5] / data2[5],
                                data1[6] / data2[6], data1[7] / data2[7]};
  return load_a<SimdDataI, ValueDataI>(data);
}
template <> inline SimdDataI minus<SimdDataI>(const SimdDataI &a) {
  return substract<SimdDataI>(_mm256_setzero_si256(), a);
}

template <>
inline SimdDataI min<SimdDataI>(const SimdDataI &a, const SimdDataI &b) {
#if defined(__KFP_SIMD__AVX) //TODO
  return _mm256_min_epi32(a, b);
#else
  const SimdDataI mask = greaterThan<SimdDataI>(a, b);
  return select<SimdDataI>(mask, b, a);
#endif
}
template <>
inline SimdDataI max<SimdDataI>(const SimdDataI &a, const SimdDataI &b) {
#if defined(__KFP_SIMD__AVX)
  return _mm256_max_epi32(a, b);
#else
  const SimdDataI mask = greaterThan<SimdDataI>(a, b);
  return select<SimdDataI>(mask, a, b);
#endif
}

template <> inline SimdDataI sqrt<SimdDataI>(const SimdDataI &a) {
  // Note: There's no direct AVX2 equivalent for integer sqrt, so we use the same approach as SSE
  return _mm256_cvtps_epi32(_mm256_sqrt_ps(_mm256_cvtepi32_ps(a)));
}

template <> inline SimdDataI rsqrt<SimdDataI>(const SimdDataI &a) {
  // Note: There's no direct AVX2 equivalent for integer rsqrt, so we use the same approach as SSE
  return _mm256_cvtps_epi32(_mm256_rsqrt_ps(_mm256_cvtepi32_ps(a)));
}

template <> inline SimdDataI abs<SimdDataI>(const SimdDataI &a) {
#if defined(__KFP_SIMD__AVX2)
  return _mm256_abs_epi32(a);
#else
  return ANDBits<SimdDataI>(a, getMask<MASK::ABS>());
#endif
}
template <> inline SimdDataI log<SimdDataI>(const SimdDataI &a) {
  ValueDataI __KFP_SIMD__ATTR_ALIGN(__KFP_SIMD__Size_Int)
      data[__KFP_SIMD__Len_Int]{}; // Helper array //TODO
  store_a<SimdDataI, ValueDataI>(a, data);
  // Note: Direct AVX2 log operation on integers is not available, using SSE approach
  return _mm256_setr_epi32(std::log(data[0]), std::log(data[1]), std::log(data[2]),
                           std::log(data[3]), std::log(data[4]), std::log(data[5]),
                           std::log(data[6]), std::log(data[7]));
}
template <> inline SimdDataI pow<SimdDataI>(const SimdDataI &a, int exp) {
  ValueDataI __KFP_SIMD__ATTR_ALIGN(__KFP_SIMD__Size_Int)
      data[__KFP_SIMD__Len_Int]{}; // Helper array
  store_a<SimdDataI, ValueDataI>(a, data);
  // Note: Direct AVX2 pow operation on integers is not available, using SSE approach
  return _mm256_setr_epi32(std::pow(data[0], exp), std::pow(data[1], exp),
                           std::pow(data[2], exp), std::pow(data[3], exp),
                           std::pow(data[4], exp), std::pow(data[5], exp),
                           std::pow(data[6], exp), std::pow(data[7], exp));
}
template <> inline int sign<int, SimdDataI>(const SimdDataI &a) {
  // Note: _mm256_movemask_ps is not directly available, using _mm_movemask_ps on parts
  return (_mm_movemask_ps(_mm_castsi128_ps(_mm256_extractf128_si256(a, 0))) |
          (_mm_movemask_ps(_mm_castsi128_ps(_mm256_extractf128_si256(a, 1))) << 4));
}
template <> inline SimdDataI sign<SimdDataI>(const SimdDataI &a) {
  // Note: Using AVX2 operation for sign, as direct AVX equivalent is not available
  return ANDBits<SimdDataI>(getMask<MASK::MINUS>(), a);
}

template <>
inline bool equal<bool, SimdDataI, SimdDataI>(const SimdDataI& a, const SimdDataI& b) {
    return (_mm256_movemask_ps(_mm256_castsi256_ps(a)) == _mm256_movemask_ps(_mm256_castsi256_ps(b)));
}

template <>
inline bool notEqual<bool, SimdDataI, SimdDataI>(const SimdDataI& a, const SimdDataI& b) {
    return (_mm256_movemask_ps(_mm256_castsi256_ps(a)) != _mm256_movemask_ps(_mm256_castsi256_ps(b)));
}

template <>
inline void print<SimdDataI>(std::ostream &stream, const SimdDataI &val_simd) {
    ValueDataI __KFP_SIMD__ATTR_ALIGN(__KFP_SIMD__Size_Int)
        data[__KFP_SIMD__Len_Int]{};
    store_a<SimdDataI, ValueDataI>(val_simd, data);
    stream << "[" << data[0] << ", " << data[1] << ", " << data[2] << ", "
           << data[3] << ", " << data[4] << ", " << data[5] << ", "
           << data[6] << ", " << data[7] << "]";
}

} // namespace Detail

} // namespace SIMD
} // namespace KFP

#endif // !SIMD_SSE_DETAIL_INT_H
