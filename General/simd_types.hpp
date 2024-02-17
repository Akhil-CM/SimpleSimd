// -*- C++ Header -*-
/*
==================================================
Authors: A.Mithran;
Emails: mithran@fias.uni-frankfurt.de
==================================================
*/

#include "simd_macros.hpp"
#include "simd_abi.hpp"

#include <x86intrin.h>

namespace KFP
{
namespace SIMD
{

template<typename T, ABI ABIVal>
class SimdType
{
    typedef T simd_type;
    static_assert((ABIVal == ABI::Scalar), "[Error] (SimdType): Invalid use of primary template of SimdType meant for Scalar abi.") ;
    T data_ ;
};

#if defined(__KFP_SIMD__AVX)
template<>
class SimdType<int, ABI::AVX>
{
    typedef __m256i simd_type;
    __m256i data_ ;
};

template<>
class SimdType<float, ABI::AVX>
{
    typedef __m256 simd_type;
    __m256 data_ ;
};
#elif defined(__KFP_SIMD__SSE)
template<>
class SimdType<int, ABI::SSE>
{
    typedef __m128i simd_type;
    __m128i data_ ;
};

template<>
class SimdType<float, ABI::SSE>
{
    typedef __m128 simd_type;
    __m128 data_ ;
};
#endif

}
}
