// -*- C++ -*-
#include "../simd.hpp"
#include <iostream>

using KFP::SIMD::SimdMask;
using KFP::SIMD::SimdIndex;
using KFP::SIMD::SimdF_t;
using KFP::SIMD::SimdI_t;

int main()
{
    SimdMask simd_mask{ false };
    SimdIndex simd_index;
    SimdF_t simd_float{ 0.0f };
    SimdI_t simd_int{ 0 };

    std::cout << simd_mask << '\n';
    std::cout << SimdMask{simd_index < 5} << '\n';

    std::cout << simd_index << '\n';
    std::cout << simd_index + 5 << '\n';
    std::cout << (simd_index + SimdIndex{ 5 }) << '\n';

    std::cout << simd_int << '\n';
    std::cout << simd_int + 5 << '\n';
    std::cout << (simd_int + SimdI_t{ 5 }) << '\n';

    std::cout << simd_float << '\n';
    std::cout << simd_float + 5 << '\n';
    std::cout << (simd_float + SimdF_t{ 5 }) << '\n';
}
