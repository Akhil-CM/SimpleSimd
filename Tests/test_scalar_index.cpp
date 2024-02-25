// -*- C++ -*-
#include "../simd.hpp"
#include <iostream>

using KFP::SIMD::SimdIndex;

int main()
{
    {
        SimdIndex simd_index;
        std::cout << "Print Simd Index\n" ;
        std::cout << simd_index << "\n\n";
        std::cout << "Print Simd Index op+ 5\n" ;
        std::cout << simd_index + 5 << "\n\n";
        std::cout << "Print Simd Index seq\n" ;
        std::cout << SimdIndex(3) << "\n\n";
    }

    {
        SimdIndex simd_index{5};
        std::cout << "Print Simd Index\n" ;
        std::cout << simd_index << "\n\n";
        std::cout << "Print Simd Index op/ 5\n" ;
        std::cout << simd_index / 5 << "\n\n";
        std::cout << "Print Simd Index seq\n" ;
        std::cout << SimdIndex(5) << "\n\n";
    }

}
