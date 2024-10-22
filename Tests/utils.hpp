// -*- C++ Header -*-
#include <cmath>

bool inline testFloatAll(const float* arr1, const float* arr2, int size) {
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

bool inline testFloatINF(const float* arr1, const float* arr2, int size) {
    for (int i = 0; i < size; ++i) {
        if (std::isinf(arr1[i]) and std::isinf(arr2[i]) and arr1[i] == arr2[i]) {
            continue; // Both are infinities of the same sign, treat them as equal
        } else {
            return false; // Mismatch for finite values or infinities of different signs
        }
    }
    return true;
}

bool inline testFloatNaN(const float* arr1, const float* arr2, int size) {
    for (int i = 0; i < size; ++i) {
        if (std::isnan(arr1[i]) and std::isnan(arr2[i])) {
            continue; // Both are NaN, treat them as equal
        } else {
            return false; // Mismatch for finite values or infinities of different signs
        }
    }
    return true;
}
