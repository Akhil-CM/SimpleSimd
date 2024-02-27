// -*- C++ -*-
#include "../simd.h"
#include <iostream>

using KFP::SIMD::SimdMask;

int main()
{
    {
        SimdMask simd_mask;
        std::cout << "Print Simd Mask\n" ;
        std::cout << simd_mask << "\n\n";
        std::cout << "Print Simd Mask count\n" ;
        std::cout << simd_mask.count() << "\n\n";
        std::cout << "Print Simd Mask AND\n" ;
        std::cout << simd_mask.AND() << "\n\n";
        std::cout << "Print Simd Mask OR\n" ;
        std::cout << simd_mask.OR() << "\n\n";
        std::cout << "Print Simd Mask == Simd Mask{false}\n" ;
        std::cout << (simd_mask == SimdMask{false}) << "\n\n";
        std::cout << "Print !Simd Mask == Simd Mask{true}\n" ;
        std::cout << ((!simd_mask) == SimdMask{true}) << "\n\n";
        std::cout << "Print !Simd Mask == Simd Mask{false}\n" ;
        std::cout << ((!simd_mask) == SimdMask{}) << "\n\n";
    }

    {
        SimdMask simd_mask{true};
        std::cout << "Print Simd Mask\n" ;
        std::cout << simd_mask << "\n\n";
        std::cout << "Print Simd Mask count\n" ;
        std::cout << simd_mask.count() << "\n\n";
        std::cout << "Print Simd Mask AND\n" ;
        std::cout << simd_mask.AND() << "\n\n";
        std::cout << "Print Simd Mask OR\n" ;
        std::cout << simd_mask.OR() << "\n\n";
    }

}
