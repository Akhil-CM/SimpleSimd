// -*- C++ Header -*-
/*
==================================================
Authors: A.Mithran;
Emails: mithran@fias.uni-frankfurt.de
==================================================
*/

#pragma once

#include "simd_sse_mask.hpp"

#include <x86intrin.h>
#include <algorithm>
#include <iostream>
#include <type_traits>

namespace KFP {
namespace SIMD {

class SimdIndex
{
public:

    static constexpr int SimdLen = sizeof(__m128) / sizeof(float) ;
    static constexpr int SimdSize{ SimdLen * sizeof(float) };
    // ------------------------------------------------------
    // Constructors
    // ------------------------------------------------------
    // Default constructor:
    SimdIndex() = default;
    SimdIndex(const SimdIndex& class_indices)
    {
        std::copy_n(class_indices.indices_, SimdLen, indices_) ;
    }
    SimdIndex(int val1, int val2, int val3, int val4)
    {
        indices_[0] = val1 ;
        indices_[1] = val2 ;
        indices_[2] = val3 ;
        indices_[3] = val4 ;
    }
    SimdIndex(int val)
    {
        indices_[0] = val ;
        indices_[1] = val ;
        indices_[2] = val ;
        indices_[3] = val ;
    }
    // Assignment operator
    SimdIndex& operator=(int val)
    {
        indices_[0] = val ;
        indices_[1] = val ;
        indices_[2] = val ;
        indices_[3] = val ;
        return *this;
    }
    SimdIndex& operator=(const SimdIndex& class_indices)
    {
        std::copy_n(class_indices.indices_, SimdLen, indices_) ;
        return *this;
    }

    // ------------------------------------------------------
    // Data member accessors
    // ------------------------------------------------------
    int* scalar()
    {
        return indices_;
    }

    const int* scalar() const
    {
        return indices_;
    }

    // ------------------------------------------------------
    // Print I/O
    // ------------------------------------------------------
    friend std::ostream& operator<<(std::ostream& stream,
                                    const SimdIndex& s)
    {
        stream << "[" << s.indices_[0] << ", " << s.indices_[1] << ", " << s.indices_[2] << ", " << s.indices_[3]
               << "]";
        return stream;
    }

    // ------------------------------------------------------
    // Basic Arithmetic
    // ------------------------------------------------------
    friend SimdIndex operator+(const SimdIndex& a,
                                   const SimdIndex& b)
    {
        const int* indices1 = a.indices_ ;
        const int* indices2 = b.indices_ ;
        return SimdIndex(indices1[0] + indices2[0], indices1[1] + indices2[1], indices1[2] + indices2[2], indices1[3] + indices2[3]) ;
    }
    friend SimdIndex operator+(int val,
                                   const SimdIndex& a)
    {
        const int* indices = a.indices_ ;
        return SimdIndex(indices[0] + val, indices[1] + val, indices[2] + val, indices[3] + val) ;
    }
    friend SimdIndex operator+(const SimdIndex& a, int val)
    {
        const int* indices = a.indices_ ;
        return SimdIndex(indices[0] + val, indices[1] + val, indices[2] + val, indices[3] + val) ;
    }
    friend SimdIndex operator-(const SimdIndex& a,
                                   const SimdIndex& b)
    {
        const int* indices1 = a.indices_ ;
        const int* indices2 = b.indices_ ;
        return SimdIndex(indices1[0] - indices2[0], indices1[1] - indices2[1], indices1[2] - indices2[2], indices1[3] - indices2[3]) ;
    }
    friend SimdIndex operator-(int val,
                                   const SimdIndex& a)
    {
        const int* indices = a.indices_ ;
        return SimdIndex(val - indices[0], val - indices[1], val - indices[2], val - indices[3]) ;
    }
    friend SimdIndex operator-(const SimdIndex& a, int val)
    {
        const int* indices = a.indices_ ;
        return SimdIndex(indices[0] - val, indices[1] - val, indices[2] - val, indices[3] - val) ;
    }
    friend SimdIndex operator*(const SimdIndex& a,
                                   const SimdIndex& b)
    {
        const int* indices1 = a.indices_ ;
        const int* indices2 = b.indices_ ;
        return SimdIndex(indices1[0] * indices2[0], indices1[1] * indices2[1], indices1[2] * indices2[2], indices1[3] * indices2[3]) ;
    }
    friend SimdIndex operator*(int val,
                                   const SimdIndex& a)
    {
        const int* indices = a.indices_ ;
        return SimdIndex(indices[0] * val, indices[1] * val, indices[2] * val, indices[3] * val) ;
    }
    friend SimdIndex operator*(const SimdIndex& a, int val)
    {
        const int* indices = a.indices_ ;
        return SimdIndex(indices[0] * val, indices[1] * val, indices[2] * val, indices[3] * val) ;
    }
    friend SimdIndex operator/(const SimdIndex& a,
                                   const SimdIndex& b)
    {
        const int* indices1 = a.indices_ ;
        const int* indices2 = b.indices_ ;
        return SimdIndex(indices1[0] / indices2[0], indices1[1] / indices2[1], indices1[2] / indices2[2], indices1[3] / indices2[3]) ;
    }
    friend SimdIndex operator/(int val,
                                   const SimdIndex& a)
    {
        const int* indices = a.indices_ ;
        return SimdIndex(val / indices[0], val / indices[1], val / indices[2], val / indices[3]) ;
    }
    friend SimdIndex operator/(const SimdIndex& a, int val)
    {
        const int* indices = a.indices_ ;
        return SimdIndex(indices[0] / val, indices[1] / val, indices[2] / val, indices[3] / val) ;
    }

    /* Comparison */
    friend SimdMask operator<(const SimdIndex& a,
                                   const SimdIndex& b)
    { // mask returned
        const int* indices1 = a.indices_ ;
        const int* indices2 = b.indices_ ;
        return SimdMask(
            (indices1[0] < indices2[0]),
            (indices1[1] < indices2[1]),
            (indices1[2] < indices2[2]),
            (indices1[3] < indices2[3])
        );
    }
    friend SimdMask operator<=(const SimdIndex& a,
                                   const SimdIndex& b)
    { // mask returned
        const int* indices1 = a.indices_ ;
        const int* indices2 = b.indices_ ;
        return SimdMask(
            (indices1[0] <= indices2[0]),
            (indices1[1] <= indices2[1]),
            (indices1[2] <= indices2[2]),
            (indices1[3] <= indices2[3])
        );
    }
    friend SimdMask operator>(const SimdIndex& a,
                                   const SimdIndex& b)
    { // mask returned
        const int* indices1 = a.indices_ ;
        const int* indices2 = b.indices_ ;
        return SimdMask(
            (indices1[0] > indices2[0]),
            (indices1[1] > indices2[1]),
            (indices1[2] > indices2[2]),
            (indices1[3] > indices2[3])
        );
    }
    friend SimdMask operator>=(const SimdIndex& a,
                                   const SimdIndex& b)
    { // mask returned
        const int* indices1 = a.indices_ ;
        const int* indices2 = b.indices_ ;
        return SimdMask(
            (indices1[0] >= indices2[0]),
            (indices1[1] >= indices2[1]),
            (indices1[2] >= indices2[2]),
            (indices1[3] >= indices2[3])
        );
    }
    friend SimdMask operator==(const SimdIndex& a,
                                   const SimdIndex& b)
    { // mask returned
        const int* indices1 = a.indices_ ;
        const int* indices2 = b.indices_ ;
        return SimdMask(
            (indices1[0] == indices2[0]),
            (indices1[1] == indices2[1]),
            (indices1[2] == indices2[2]),
            (indices1[3] == indices2[3])
        );
    }
    friend SimdMask operator<(const SimdIndex& a,
                                   int val)
    { // mask returned
        const int* indices = a.indices_ ;
        return SimdMask(
            (indices[0] < val),
            (indices[1] < val),
            (indices[2] < val),
            (indices[3] < val)
        );
    }
    friend SimdMask operator<=(const SimdIndex& a,
                                   int val)
    { // mask returned
        const int* indices = a.indices_ ;
        return SimdMask(
            (indices[0] <= val),
            (indices[1] <= val),
            (indices[2] <= val),
            (indices[3] <= val)
        );
    }
    friend SimdMask operator>(const SimdIndex& a,
                                   int val)
    { // mask returned
        const int* indices = a.indices_ ;
        return SimdMask(
            (indices[0] > val),
            (indices[1] > val),
            (indices[2] > val),
            (indices[3] > val)
        );
    }
    friend SimdMask operator>=(const SimdIndex& a,
                                   int val)
    { // mask returned
        const int* indices = a.indices_ ;
        return SimdMask(
            (indices[0] >= val),
            (indices[1] >= val),
            (indices[2] >= val),
            (indices[3] >= val)
        );
    }
    friend SimdMask operator==(const SimdIndex& a,
                                    int val)
    { // mask returned
        const int* indices = a.indices_ ;
        return SimdMask(
            (indices[0] == val),
            (indices[1] == val),
            (indices[2] == val),
            (indices[3] == val)
        );
    }
    friend SimdMask operator<(int val, const SimdIndex& a)
    { // mask returned
        const int* indices = a.indices_ ;
        return SimdMask(
            (val < indices[0]),
            (val < indices[1]),
            (val < indices[2]),
            (val < indices[3])
        );
    }
    friend SimdMask operator<=(int val, const SimdIndex& a)
    { // mask returned
        const int* indices = a.indices_ ;
        return SimdMask(
            (val <= indices[0]),
            (val <= indices[1]),
            (val <= indices[2]),
            (val <= indices[3])
        );
    }
    friend SimdMask operator>(int val, const SimdIndex& a)
    { // mask returned
        const int* indices = a.indices_ ;
        return SimdMask(
            (val > indices[0]),
            (val > indices[1]),
            (val > indices[2]),
            (val > indices[3])
        );
    }
    friend SimdMask operator>=(int val, const SimdIndex& a)
    { // mask returned
        const int* indices = a.indices_ ;
        return SimdMask(
            (val >= indices[0]),
            (val >= indices[1]),
            (val >= indices[2]),
            (val >= indices[3])
        );
    }
    friend SimdMask operator==(int val, const SimdIndex& a)
    { // mask returned
        const int* indices = a.indices_ ;
        return SimdMask(
            (val == indices[0]),
            (val == indices[1]),
            (val == indices[2]),
            (val == indices[3])
        );
    }

protected:
    int __attribute__((aligned(16)))
    indices_[SimdLen]{0, 1, 2, 3}; // Indices array
    // alignas(16) int indices_[SimdLen]{0, 1, 2, 3} ; // Indices array
};

} // namespace SIMD
} // namespace KFP
