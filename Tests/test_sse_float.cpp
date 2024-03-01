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

    std::cout << "Print SimdF{0.0f}\n" ;
    std::cout << simd_float << "\n\n";
    std::cout << "Print SimdF{0.0f} + 5\n" ;
    std::cout << simd_float + 5 << "\n\n";
    std::cout << "Print SimdF{0.0f} + SimdF{5}\n" ;
    std::cout << (simd_float + SimdF{ 5 }) << "\n\n";
    std::cout << "Print sqrt of SimdF{0.0f} + SimdF{5}\n" ;
    std::cout << sqrt(simd_float + SimdF{ 5 }) << "\n\n";
    std::cout << "Print rsqrt of SimdF{0.0f} + SimdF{5}\n" ;
    std::cout << rsqrt(simd_float + SimdF{ 5 }) << "\n\n";
    std::cout << "Print log of SimdF{0.0f} + SimdF{5}\n" ;
    std::cout << log(simd_float + SimdF{ 5 }) << "\n\n";
    std::cout << "Print power 3 of SimdF{0.0f} + SimdF{5}\n" ;
    std::cout << pow(simd_float + SimdF{ 5 }, 3) << "\n\n";

    SimdF::value_type f1234[SimdF::SimdLen]{1.0f, 2.0f, 3.0f, 4.0f};
    std::cout << "Print load_partial of {1.0f, 2.0f, 3.0f, 4.0f}\n" ;
    std::cout << SimdF{}.load_partial(5, f1234) << "\n\n";

    std::cout << "Print load_partial of {1.0f, 2.0f, 3.0f, 4.0f} + SimdF{5.0f}\n" ;
    std::cout << SimdF{}.load_partial(5, f1234) + SimdF{5.0f} << "\n\n";

    std::cout << "Print SimdF to SimdI cast\n" ;
    std::cout << SimdF{}.load_partial(5, f1234) + SimdF{0.5f} << "\n\n";
    std::cout << SimdI{SimdF{}.load_partial(5, f1234) + SimdF{0.5f}} << "\n\n";
}
