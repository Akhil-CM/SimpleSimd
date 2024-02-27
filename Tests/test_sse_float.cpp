// -*- C++ -*-
#include "../simd.h"
#include <iostream>

using KFP::SIMD::SimdMask;
using KFP::SIMD::SimdIndex;
using KFP::SIMD::SimdF;
using KFP::SIMD::SimdI;

int main()
{
    SimdMask simd_mask;
    SimdIndex simd_index;
    SimdF simd_float{ 0.0f };
    SimdI simd_int{ 0 };

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

    std::cout << "Print SimdI{0}\n" ;
    std::cout << simd_int << "\n\n";
    std::cout << "Print SimdI{0} + 5\n" ;
    std::cout << simd_int + 5 << "\n\n";
    std::cout << "Print SimdI{0} + SimdI{5}\n" ;
    std::cout << (simd_int + SimdI{ 5 }) << "\n\n";

    std::cout << "Print SimdF{0.0f}\n" ;
    std::cout << simd_float << "\n\n";
    std::cout << "Print SimdF{0.0f} + 5\n" ;
    std::cout << simd_float + 5 << "\n\n";
    std::cout << "Print SimdF{0.0f} + SimdF{5}\n" ;
    std::cout << (simd_float + SimdF{ 5 }) << "\n\n";
    std::cout << "Print sqrt of SimdF{0.0f} + SimdF{5}\n" ;
    std::cout << sqrt(simd_float + SimdF{ 5 }) << "\n\n";
    std::cout << "Print sqrt of SimdI{0} + SimdI{5}\n" ;
    std::cout << sqrt(simd_int + SimdI{ 5 }) << "\n\n";
    std::cout << "Print rsqrt of SimdF{0.0f} + SimdF{5}\n" ;
    std::cout << rsqrt(simd_float + SimdF{ 5 }) << "\n\n";
    std::cout << "Print rsqrt of SimdI{0} + SimdI{5}\n" ;
    std::cout << rsqrt(simd_int + SimdI{ 5 }) << "\n\n";
    std::cout << "Print log of SimdF{0.0f} + SimdF{5}\n" ;
    std::cout << log(simd_float + SimdF{ 5 }) << "\n\n";
    std::cout << "Print log of SimdI{0} + SimdI{5}\n" ;
    std::cout << log(simd_int + SimdI{ 5 }) << "\n\n";
    std::cout << "Print power 3 of SimdF{0.0f} + SimdF{5}\n" ;
    std::cout << pow(simd_float + SimdF{ 5 }, 3) << "\n\n";
    std::cout << "Print power 3 of SimdI{0} + SimdI{5}\n" ;
    std::cout << pow(simd_int + SimdI{ 5 }, 3) << "\n\n";
}
