// -*- C++ Header -*-
/*
==================================================
Authors: A.Mithran;
Emails: mithran@fias.uni-frankfurt.de
==================================================
*/

#ifndef SIMD_DETAIL_H
#define SIMD_DETAIL_H

#include <iosfwd>

namespace KFP {
namespace SIMD {

namespace Detail {

// ------------------------------------------------------
// General
// ------------------------------------------------------
template<typename T1, typename T2>
T1 cast(const T2& val_simd);

template <typename T1, typename T2> T1 constant(T2 val);

template <typename T1, typename T2> T1 load(const T2* val_ptr);

template <typename T1, typename T2> T1 load_a(const T2* val_ptr);

template <typename T1, typename T2>
void store(const T1& val_simd, T2* val_ptr);

template <typename T1, typename T2>
void store_a(const T1& val_simd, T2* val_ptr);

template <typename T1, typename T2>
T1 extract(int index, const T2& val_simd);

template <typename T1, typename T2 = T1, typename T3 = T1, typename T4 = T1> T1 select(const T2& mask, const T3& a, const T4& b);

template <typename T1, typename T2=T1>
T1 rotated(int index, const T2& val_simd);

template <typename T> void print(std::ostream& stream, const T& val_simd);

// ------------------------------------------------------
// Basic Arithmetic
// ------------------------------------------------------
template <typename T1, typename T2 = T1> T1 minus(const T2& a);

template <typename T1, typename T2 = T1, typename T3 = T1>
T1 add(const T2& a, const T3& b);
template <typename T1, typename T2 = T1, typename T3 = T1>
T1 substract(const T2& a, const T3& b);
template <typename T1, typename T2 = T1, typename T3 = T1>
T1 multiply(const T2& a, const T3& b);
template <typename T1, typename T2 = T1, typename T3 = T1>
T1 divide(const T2& a, const T3& b);

template <typename T1, typename T2 = T1, typename T3 = T1>
T1 min(const T2& a, const T3& b);
template <typename T1, typename T2 = T1, typename T3 = T1>
T1 max(const T2& a, const T3& b);

template <typename T1, typename T2 = T1> T1 sqrt(const T2& a);

/* Reciprocal( inverse) Square Root */
template <typename T1, typename T2 = T1> T1 rsqrt(const T2& a);

template <typename T1, typename T2 = T1> T1 abs(const T2& a);

template <typename T1, typename T2 = T1> T1 log(const T2& a);

template <typename T1, typename T2 = T1> T1 pow(const T2& a, int exp);

/* Logical */
template <typename T1, typename T2 = T1>
T1 opShiftLeft(const T2& a, int b);
template <typename T1, typename T2 = T1>
T1 opShiftRight(const T2& a, int b);
template <typename T1, typename T2 = T1> T1 opNOT(const T2& a);
template <typename T1, typename T2 = T1, typename T3 = T1>
T1 opANDbitwise(const T2& a, const T3& b);
template <typename T1, typename T2 = T1, typename T3 = T1>
T1 opORbitwise(const T2& a, const T3& b);
template <typename T1, typename T2 = T1, typename T3 = T1>
T1 opXORbitwise(const T2& a, const T3& b);
template <typename T1, typename T2 = T1, typename T3 = T1>
T1 opAND(const T2& a, const T3& b);
template <typename T1, typename T2 = T1, typename T3 = T1>
T1 opOR(const T2& a, const T3& b);

/* Comparison */
template <typename T1, typename T2 = T1, typename T3 = T1>
T1 opEqual(const T2& a, const T3& b);
template <typename T1, typename T2 = T1, typename T3 = T1>
T1 opNotEqual(const T2& a, const T3& b);
template <typename T1, typename T2 = T1, typename T3 = T1>
T1 opLessThan(const T2& a, const T3& b);
template <typename T1, typename T2 = T1, typename T3 = T1>
T1 opLessThanEqual(const T2& a, const T3& b);
template <typename T1, typename T2 = T1, typename T3 = T1>
T1 opGreaterThan(const T2& a, const T3& b);
template <typename T1, typename T2 = T1, typename T3 = T1>
T1 opGreaterThanEqual(const T2& a, const T3& b);

template <typename T1, typename T2 = T1> T1 sign(const T2& a);

} // namespace Detail

} // namespace SIMD
} // namespace KFP

#endif
