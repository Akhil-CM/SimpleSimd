// -*- C++ Header -*-
/*
==================================================
Authors: A.Mithran;
Emails: mithran@fias.uni-frankfurt.de
==================================================
*/

#ifndef SIMD_SSE_DETAIL_INDEX_H
#define SIMD_SSE_DETAIL_INDEX_H

#include "../Base/simd_macros.h"
#include "../Base/simd_detail.h"
#include "simd_sse_type.h"
#include "simd_sse_detail_int.h"

namespace KFP {
namespace SIMD {

namespace Detail {

template<> inline SimdDataI add<SimdDataI, ValueDataI, SimdDataI>(const ValueDataI& val, const SimdDataI& a)
{
    return Detail::add<SimdDataI>(Detail::constant<SimdDataI, ValueDataI>(val), a);
}

template<> inline SimdDataI substract<SimdDataI, ValueDataI, SimdDataI>(const ValueDataI& val, const SimdDataI& a)
{
    return Detail::substract<SimdDataI>(Detail::constant<SimdDataI, ValueDataI>(val), a);
}
template<> inline SimdDataI substract<SimdDataI, SimdIndex, ValueDataI>(const SimdIndex& a, const ValueDataI& val)
{
    return Detail::substract<SimdDataI>(a, Detail::constant<SimdDataI, ValueDataI>(val));
}

template<> inline SimdDataI multiply<SimdDataI, ValueDataI, SimdIndex>(const ValueDataI& val, const SimdIndex& a)
{
    return Detail::multiply<SimdDataI>(Detail::constant<SimdDataI, ValueDataI>(val), a);
}

template<> inline SimdDataI divide<SimdDataI, ValueDataI, SimdIndex>(const ValueDataI& val, const SimdIndex& a)
{
    return Detail::divide<SimdDataI>(Detail::constant<SimdDataI, ValueDataI>(val), a);
}
template<> inline SimdDataI divide<SimdDataI, SimdIndex, ValueDataI>(const SimdIndex& a, const ValueDataI& val)
{
    return Detail::divide<SimdDataI>(a, Detail::constant<SimdDataI, ValueDataI>(val));
}

} // namespace Detail

} // namespace SIMD
} // namespace KFP

#endif
