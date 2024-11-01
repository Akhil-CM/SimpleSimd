// -*- C++ -*-
// compile as follows:
// g++ -o float32.out -std=c++11 -msse4 -Wall -Wextra -pedantic -Wsign-conversion float32.cpp
// Note: either use ".out" or ".exe" for the compiled binary so that it is skipped from git vcs

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "../../simd.h"
#include "../utils.hpp"
#include "../doctest.h"

#include <limits>

using KFP::SIMD::Float32_128;
using KFP::SIMD::SimdSize;
using KFP::SIMD::SimdLen;

static_assert(std::numeric_limits<float>::is_iec559, "IEEE 754 required");

// Test cases
TEST_CASE("Float32_128 Addition") {
    Float32_128 a{1.0f}, b{2.0f};
    Float32_128 result = a + b;

    float expected[SimdLen] = {3.0f, 3.0f, 3.0f, 3.0f};
    alignas(SimdSize) float actual[SimdLen];
    result.store(actual);

    CHECK(testFloatAll(expected, actual, SimdLen));
}

TEST_CASE("Float32_128 Subtraction") {
    Float32_128 a{5.0f}, b{3.0f};
    Float32_128 result = a - b;

    float expected[SimdLen] = {2.0f, 2.0f, 2.0f, 2.0f};
    alignas(SimdSize) float actual[SimdLen];
    result.store(actual);

    CHECK(testFloatAll(expected, actual, SimdLen));
}

TEST_CASE("Float32_128 Multiplication") {
    Float32_128 a{3.0f}, b{4.0f};
    Float32_128 result = a * b;

    float expected[SimdLen] = {12.0f, 12.0f, 12.0f, 12.0f};
    alignas(SimdSize) float actual[SimdLen];
    result.store(actual);

    CHECK(testFloatAll(expected, actual, SimdLen));
}

TEST_CASE("Float32_128 Division") {
    Float32_128 a{8.0f}, b{2.0f};
    Float32_128 result = a / b;

    float expected[SimdLen] = {4.0f, 4.0f, 4.0f, 4.0f};
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
    Float32_128 a{std::numeric_limits<float>::max()}, b{std::numeric_limits<float>::max()};
    Float32_128 result = a - b;

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

TEST_CASE("Float32_128 combining various Corner Cases") {

    float min = std::numeric_limits<float>::min();
    float max = std::numeric_limits<float>::max();
    float min_denorm = std::numeric_limits<float>::denorm_min();
    float max_denorm = (1.f - std::pow(2.f, -23)) * std::pow(2.f, -126);
    float inf = std::numeric_limits<float>::infinity();
    float qnan = std::numeric_limits<float>::quiet_NaN();

    float special_cases_base[] = {
        0.f, 1.f, 12345678.f, max, min, max_denorm, min_denorm, max/2, inf, qnan,
    };
    const std::size_t num_special_cases_base = sizeof(special_cases_base)/sizeof(float);
    std::vector<float> special_cases;
    special_cases.reserve(num_special_cases_base);
    for (std::size_t idx{0}; idx != num_special_cases_base; ++idx) {
        addTestCase(special_cases_base[idx], special_cases);
    }

    SUBCASE("Float32_128 Addition Corner Cases") {
        for(uint32_t i = 0; i != special_cases.size(); ++i)
        {
            const float value1 = special_cases[i];

            for(uint32_t j = 0; j != special_cases.size(); ++j)
            {
                const float value2 = special_cases[j];

                Float32_128 a{value1}, b{value2};
                Float32_128 result = a + b;
                float expected = value1 + value2;
                alignas(SimdSize) float actual[SimdLen];
                result.store(actual);

                CHECK(testFloatAll(expected, actual, SimdLen));
            }
        }
    }

    SUBCASE("Float32_128 Subtraction Corner Cases") {
        for(uint32_t i = 0; i != special_cases.size(); ++i)
        {
            const float value1 = special_cases[i];

            for(uint32_t j = 0; j != special_cases.size(); ++j)
            {
                const float value2 = special_cases[j];

                Float32_128 a{value1}, b{value2};
                Float32_128 result = a - b;
                float expected = value1 - value2;
                alignas(SimdSize) float actual[SimdLen];
                result.store(actual);

                CHECK(testFloatAll(expected, actual, SimdLen));
            }
        }
    }

    SUBCASE("Float32_128 Multiplication Corner Cases") {
        for(uint32_t i = 0; i != special_cases.size(); ++i)
        {
            const float value1 = special_cases[i];

            for(uint32_t j = 0; j != special_cases.size(); ++j)
            {
                const float value2 = special_cases[j];

                Float32_128 a{value1}, b{value2};
                Float32_128 result = a * b;
                float expected = value1 * value2;
                alignas(SimdSize) float actual[SimdLen];
                result.store(actual);

                CHECK(testFloatAll(expected, actual, SimdLen));
            }
        }
    }

    SUBCASE("Float32_128 Division Corner Cases") {
        for(uint32_t i = 0; i != special_cases.size(); ++i)
        {
            const float value1 = special_cases[i];

            for(uint32_t j = 0; j != special_cases.size(); ++j)
            {
                const float value2 = special_cases[j];

                Float32_128 a{value1}, b{value2};
                Float32_128 result = a / b;
                float expected = value1 / value2;
                alignas(SimdSize) float actual[SimdLen];
                result.store(actual);

                CHECK(testFloatAll(expected, actual, SimdLen));
            }
        }
    }
}
