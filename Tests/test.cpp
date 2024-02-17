// -*- C++ -*-
#include "../simd.hpp"
#include <iostream>

using KFP::SIMD::SimdMask;
using KFP::SIMD::SimdIndex;
using KFP::SIMD::SimdF_t;
using KFP::SIMD::SimdI_t;

int main()
{
    SimdMask simd_mask;
    SimdIndex simd_index;
    SimdF_t simd_float{ 0.0f };
    SimdI_t simd_int{ 0 };

    std::cout << "Print Simd Mask\n" ;
    std::cout << simd_mask << "\n\n";
    std::cout << "Print Simd Mask(SimdIndex < 5)\n" ;
    std::cout << SimdMask{simd_index < 5} << "\n\n";

    std::cout << "Print Simd Mask(SimdIndex < 5)\n" ;
    std::cout << simd_index << "\n\n";
    std::cout << "Print SimdIndex + 5\n" ;
    std::cout << simd_index + 5 << "\n\n";
    std::cout << "Print SimdIndex + SimdIndex{5}\n" ;
    std::cout << (simd_index + SimdIndex{ 5 }) << "\n\n";

    std::cout << "Print SimdI_t{0}\n" ;
    std::cout << simd_int << "\n\n";
    std::cout << "Print SimdI_t{0} + 5\n" ;
    std::cout << simd_int + 5 << "\n\n";
    std::cout << "Print SimdI_t{0} + SimdI_t{5}\n" ;
    std::cout << (simd_int + SimdI_t{ 5 }) << "\n\n";

    std::cout << "Print SimdF_t{0.0f}\n" ;
    std::cout << simd_float << "\n\n";
    std::cout << "Print SimdF_t{0.0f} + 5\n" ;
    std::cout << simd_float + 5 << "\n\n";
    std::cout << "Print SimdF_t{0.0f} + SimdF_t{5}\n" ;
    std::cout << (simd_float + SimdF_t{ 5 }) << "\n\n";
    std::cout << "Print sqrt of SimdF_t{0.0f} + SimdF_t{5}\n" ;
    std::cout << sqrt(simd_float + SimdF_t{ 5 }) << "\n\n";
    std::cout << "Print sqrt of SimdI_t{0} + SimdI_t{5}\n" ;
    std::cout << sqrt(simd_int + SimdI_t{ 5 }) << "\n\n";
    std::cout << "Print rsqrt of SimdF_t{0.0f} + SimdF_t{5}\n" ;
    std::cout << rsqrt(simd_float + SimdF_t{ 5 }) << "\n\n";
    std::cout << "Print rsqrt of SimdI_t{0} + SimdI_t{5}\n" ;
    std::cout << rsqrt(simd_int + SimdI_t{ 5 }) << "\n\n";
    std::cout << "Print log of SimdF_t{0.0f} + SimdF_t{5}\n" ;
    std::cout << log(simd_float + SimdF_t{ 5 }) << "\n\n";
    std::cout << "Print log of SimdI_t{0} + SimdI_t{5}\n" ;
    std::cout << log(simd_int + SimdI_t{ 5 }) << "\n\n";
}
