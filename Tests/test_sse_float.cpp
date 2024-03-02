// -*- C++ -*-
#include "../simd.h"
#include <iostream>

using KFP::SIMD::simd_mask;
using KFP::SIMD::simd_index;
using KFP::SIMD::simd_float;
using KFP::SIMD::simd_int;

int main()
{
    simd_mask mask;
    simd_index index;
    simd_float simd_f{ 0.0f };

    std::cout << "Print Simd Mask\n" ;
    std::cout << mask << "\n\n";
    std::cout << "Print Simd Mask(simd_index < 5)\n" ;
    std::cout << simd_mask{index < 5} << "\n\n";

    std::cout << "Print Simd Index\n" ;
    std::cout << index << "\n\n";
    std::cout << "Print simd_index + 5\n" ;
    std::cout << index + 5 << "\n\n";
    std::cout << "Print simd_index + simd_index{5}\n" ;
    std::cout << (index + simd_index{ 5 }) << "\n\n";

    std::cout << "Print simd_float{0.0f}\n" ;
    std::cout << simd_f << "\n\n";
    std::cout << "Print simd_float{0.0f} + 5\n" ;
    std::cout << simd_f + 5 << "\n\n";
    std::cout << "Print simd_float{0.0f} + simd_float{5}\n" ;
    std::cout << (simd_f + simd_float{ 5 }) << "\n\n";
    std::cout << "Print sqrt of simd_float{0.0f} + simd_float{5}\n" ;
    std::cout << sqrt(simd_f + simd_float{ 5 }) << "\n\n";
    std::cout << "Print rsqrt of simd_float{0.0f} + simd_float{5}\n" ;
    std::cout << rsqrt(simd_f + simd_float{ 5 }) << "\n\n";
    std::cout << "Print log of simd_float{0.0f} + simd_float{5}\n" ;
    std::cout << log(simd_f + simd_float{ 5 }) << "\n\n";
    std::cout << "Print power 3 of simd_float{0.0f} + simd_float{5}\n" ;
    std::cout << pow(simd_f + simd_float{ 5 }, 3) << "\n\n";

    simd_float::value_type f1234[simd_float::SimdLen]{1.0f, 2.0f, 3.0f, 4.0f};
    std::cout << "Print load_partial of {1.0f, 2.0f, 3.0f, 4.0f}\n" ;
    std::cout << simd_float{}.load_partial(5, f1234) << "\n\n";

    std::cout << "Print load_partial of {1.0f, 2.0f, 3.0f, 4.0f} + simd_float{5.0f}\n" ;
    std::cout << simd_float{}.load_partial(5, f1234) + simd_float{5.0f} << "\n\n";

    std::cout << "Print simd_float to SimdI cast\n" ;
    std::cout << simd_float{}.load_partial(5, f1234) + simd_float{0.5f} << "\n\n";
    std::cout << simd_int{simd_float{}.load_partial(5, f1234) + simd_float{0.5f}} << "\n\n";
}
