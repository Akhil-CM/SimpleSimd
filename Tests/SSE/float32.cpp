// -*- C++ -*-
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "../../simd.h"
#include "../utils.hpp"
#include "../doctest.h"

#include <limits>
#include <iostream>

using KFP::SIMD::Float32_128;
using KFP::SIMD::SimdSize;
using KFP::SIMD::SimdLen;

static_assert(std::numeric_limits<float>::is_iec559, "IEEE 754 required");

// Test cases
TEST_CASE("Float32_128 Addition") {
    Float32_128 a{1.0f}, b{2.0f};
    Float32_128 result = a + b;

    constexpr float
    expected[SimdLen] = {3.0f, 3.0f, 3.0f, 3.0f};
    alignas(SimdSize) float actual[SimdLen];
    result.store(actual);

    CHECK(testFloatAll(expected, actual, SimdLen));
}

TEST_CASE("Float32_128 Subtraction") {
    Float32_128 a{5.0f}, b{3.0f};
    Float32_128 result = a - b;

    constexpr float expected[SimdLen] = {2.0f, 2.0f, 2.0f, 2.0f};
    alignas(SimdSize) float actual[SimdLen];
    result.store(actual);

    CHECK(testFloatAll(expected, actual, SimdLen));
}

TEST_CASE("Float32_128 Multiplication") {
    Float32_128 a{3.0f}, b{4.0f};
    Float32_128 result = a * b;

    constexpr float expected[SimdLen] = {12.0f, 12.0f, 12.0f, 12.0f};
    alignas(SimdSize) float actual[SimdLen];
    result.store(actual);

    CHECK(testFloatAll(expected, actual, SimdLen));
}

TEST_CASE("Float32_128 Division") {
    Float32_128 a{8.0f}, b{2.0f};
    Float32_128 result = a / b;

    constexpr float expected[SimdLen] = {4.0f, 4.0f, 4.0f, 4.0f};
    alignas(SimdSize) float actual[SimdLen];
    result.store(actual);

    CHECK(testFloatAll(expected, actual, SimdLen));
}

TEST_CASE("Float32_128 Infinity") {
    Float32_128 a{1.0f}, b{0.0f};
    Float32_128 result = a / b;  // Should result in +inf

    float expected[SimdLen] = {std::numeric_limits<float>::infinity(),
                         std::numeric_limits<float>::infinity(),
                         std::numeric_limits<float>::infinity(),
                         std::numeric_limits<float>::infinity()};
    alignas(SimdSize) float actual[SimdLen];
    result.store(actual);

    CHECK(testFloatAll(expected, actual, SimdLen));
}

TEST_CASE("Float32_128 Negative Infinity") {
    Float32_128 a{-1.0f}, b{0.0f};
    Float32_128 result = a / b;  // Should result in -inf

    float expected[SimdLen] = {std::numeric_limits<float>::infinity() * -1,
                         std::numeric_limits<float>::infinity() * -1,
                         std::numeric_limits<float>::infinity() * -1,
                         std::numeric_limits<float>::infinity() * -1};
    alignas(SimdSize) float actual[SimdLen];
    result.store(actual);

    CHECK(testFloatAll(expected, actual, SimdLen));
}

TEST_CASE("Float32_128 NaN (0.0 divided by 0.0)") {
    Float32_128 a{0.0f}, b{0.0f};
    Float32_128 result = a / b;

    float expected[SimdLen] = {0.0f/0.0f, 0.0f/0.0f, 0.0f/0.0f, 0.0f/0.0f};
    alignas(SimdSize) float actual[SimdLen];
    result.store(actual);

    CHECK(testFloatAll(expected, actual, SimdLen));
}

TEST_CASE("Float32_128 MaxValue - MaxValue") {
    Float32_128 a{-std::numeric_limits<float>::max()}, b{std::numeric_limits<float>::max()};
    Float32_128 result = a + b;

    float expected[SimdLen] = {0.0f, 0.0f, 0.0f, 0.0f};
    alignas(SimdSize) float actual[SimdLen];
    result.store(actual);

    CHECK(testFloatAll(expected, actual, SimdLen));
}

TEST_CASE("Float32_128 MaxValueDividedByTwo") {
    Float32_128 a{std::numeric_limits<float>::max() / 2}, b{std::numeric_limits<float>::max() / 2};
    Float32_128 result = a + b;

    float expected[SimdLen] = {std::numeric_limits<float>::max(),
                         std::numeric_limits<float>::max(),
                         std::numeric_limits<float>::max(),
                         std::numeric_limits<float>::max()};
    alignas(SimdSize) float actual[SimdLen];
    result.store(actual);

    CHECK(testFloatAll(expected, actual, SimdLen));
}

TEST_CASE("Float32_128 PositiveZero") {
    Float32_128 a{0.0f}, b{0.0f};
    Float32_128 result = a + b;

    float expected[SimdLen] = {0.0f, 0.0f, 0.0f, 0.0f};
    alignas(SimdSize) float actual[SimdLen];
    result.store(actual);

    CHECK(testFloatAll(expected, actual, SimdLen));
}

TEST_CASE("Float32_128 NegativeZero") {
    Float32_128 a{-0.0f}, b{0.0f};
    Float32_128 result = a + b;

    float expected[SimdLen] = {0.0f, 0.0f, 0.0f, 0.0f};
    alignas(SimdSize) float actual[SimdLen];
    result.store(actual);

    CHECK(testFloatAll(expected, actual, SimdLen));
}
