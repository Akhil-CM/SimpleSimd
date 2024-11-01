// -*- C++ Header -*-
#include <cmath>
#include <limits>
#include <vector>

inline bool testFloatAll(const float* arr1, const float* arr2, int size) {
    for (int i = 0; i < size; ++i) {
        if (std::isnan(arr1[i]) and std::isnan(arr2[i])) {
            continue; // Both are NaN, treat them as equal
        } else if (std::isinf(arr1[i]) and std::isinf(arr2[i]) and arr1[i] == arr2[i]) {
            continue; // Both are infinities of the same sign, treat them as equal
        } else if (arr1[i] != arr2[i]) {
            return false; // Mismatch for finite values or infinities of different signs
        }
    }
    return true;
}

inline bool testFloatAll(const float expected, const float* arr, int size) {
    for (int i = 0; i < size; ++i) {
        if (std::isnan(expected) and std::isnan(arr[i])) {
            continue; // Both are NaN, treat them as equal
        } else if (std::isinf(expected) and std::isinf(arr[i]) and expected == arr[i]) {
            continue; // Both are infinities of the same sign, treat them as equal
        } else if (expected != arr[i]) {
            return false; // Mismatch for finite values or infinities of different signs
        }
    }
    return true;
}

inline bool testFloatINF(const float* arr1, const float* arr2, int size) {
    for (int i = 0; i < size; ++i) {
        if (std::isinf(arr1[i]) and std::isinf(arr2[i]) and arr1[i] == arr2[i]) {
            continue; // Both are infinities of the same sign, treat them as equal
        } else {
            return false; // Mismatch for finite values or infinities of different signs
        }
    }
    return true;
}

inline bool testFloatNaN(const float* arr1, const float* arr2, int size) {
    for (int i = 0; i < size; ++i) {
        if (std::isnan(arr1[i]) and std::isnan(arr2[i])) {
            continue; // Both are NaN, treat them as equal
        } else {
            return false; // Mismatch for finite values or infinities of different signs
        }
    }
    return true;
}

inline void addTestCase(const float a, std::vector<float>& v)
{
    v.push_back(a);
    v.push_back(-a);

    if(std::isnormal(a))
    {
        const float a1 = a * (1.f + std::numeric_limits<float>::epsilon());
        v.push_back(a1);
        v.push_back(-a1);
        const float a2 = a * (1.f - std::numeric_limits<float>::epsilon());
        v.push_back(a2);
        v.push_back(-a2);
    }
}
