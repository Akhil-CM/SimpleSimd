// -*- C++ Header -*-
/*
==================================================
Authors: A.Mithran;
Emails: mithran@fias.uni-frankfurt.de
==================================================
*/

#pragma once

#include "simd_index.hpp"
#include "simd_mask.hpp"
#include <iostream>

namespace KFP {
namespace SIMD {

namespace Detail {

template <typename T1, typename T2> void constant(T1& val_simd, T2 val);

template <typename T1, typename T2> void load(T1& val_simd, const T2* val_ptr);

template <typename T1, typename T2>
void load_a(T1& val_simd, const T2* val_ptr);

template <typename T1, typename T2> void store(const T1& val_simd, T2* val_ptr);

template <typename T1, typename T2>
void store_a(const T1& val_simd, T2* val_ptr);

template <typename T1, typename T2>
void store_stream(const T1& val_simd, T2* val_ptr);

template <typename T1, typename T2>
void load_partial(T1& val_simd, int index, const T2* val_ptr);

template <typename T1, typename T2>
void store_partial(T1& class_simd, int index, T2* val_ptr);

// ------------------------------------------------------
// Gather and Scatter
// ------------------------------------------------------
template <typename T1, typename T2>
void gather(T1& val_simd, const SimdIndex& index, const T2* val_ptr) ;

template <typename T1, typename T2>
void scatter(const T1& val_simd, const SimdIndex& index, T2* val_ptr) ;

// ------------------------------------------------------
// Individual Scalar manipulation
// ------------------------------------------------------
template <typename T> T select(const T mask, const T a, const T b);

template <typename T1, typename T2>
void insert(T1& val_simd, int index, T2 val);

template <typename T1, typename T2> T2 extract(const T1& val_simd, int index);

template <typename T> void cutoff(T& val_simd, int index);

template <typename T> void print(std::ostream& stream, T& val_simd);

// ------------------------------------------------------
// Basic Arithmetic
// ------------------------------------------------------
template <typename T> T add(const T& a, const T& b);

template <typename T> T substract(const T& a, const T& b);

template <typename T1, typename T2> T1 multiply(T2& a, T2& b);

template <typename T1, typename T2> T1 divide(T2& a, T2& b);

template <typename T> T min(const T& a, const T& b);

template <typename T> T max(const T& a, const T& b);

template <typename T> T sqrt(const T& a);
/* Reciprocal( inverse) Square Root */
template <typename T> T rsqrt(const T& a);

template <typename T> T abs(const T& a);

template <typename T> T pow(const T& a, int exp);

template <typename T> T log(const T& a);

/* Logical */
template <typename T> T opANDbitwise(const T& a, const T& b);

template <typename T> T opORbitwise(const T& a, const T& b);

template <typename T> T opXORbitwise(const T& a);

template <typename T> T opOR(const T& a, const T& b);

/* Comparison */
template <typename T> T opLessThan(const T& a, const T& b);

template <typename T> T opLessThanEqual(const T& a, const T& b);

template <typename T> T opGreaterThan(const T& a, const T& b);

template <typename T> T opGreaterThanEqual(const T& a, const T& b);

template <typename T> T opEqual(const T& a, const T& b);

} // namespace Detail

} // namespace SIMD
} // namespace KFP
