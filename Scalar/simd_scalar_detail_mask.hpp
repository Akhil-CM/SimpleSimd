// -*- C++ Header -*-
/*
==================================================
Authors: A.Mithran;
Emails: mithran@fias.uni-frankfurt.de
==================================================
*/

#ifndef SIMD_SCALAR_DETAIL_MASK_H
#define SIMD_SCALAR_DETAIL_MASK_H

#include "../Base/simd_detail.hpp"
#include "../Base/simd_mask.hpp"
#include "simd_scalar_detail.hpp"
#include "simd_scalar_type.hpp"

#include <iostream>

namespace KFP {
namespace SIMD {

namespace Detail {

template <>
inline void print<SimdMask>(std::ostream& stream, const SimdMask& class_simd)
{
    stream << std::boolalpha << "[" << static_cast<bool>(class_simd.maski()) << "]" << std::noboolalpha;
}

template <> inline SimdDataI opNOT<SimdDataI>(const SimdDataI& a)
{
    return static_cast<SimdDataI>(a + 1);
}

template <>
inline bool opEqual<bool, SimdDataI, SimdDataI>(const SimdDataI& a,
                                              const SimdDataI& b)
{
    return (a == b);
}

template <>
inline bool opNotEqual<bool, SimdDataI, SimdDataI>(const SimdDataI& a,
                                                 const SimdDataI& b)
{
    return (a != b);
}

} // namespace Detail

} // namespace SIMD
} // namespace KFP

#endif
