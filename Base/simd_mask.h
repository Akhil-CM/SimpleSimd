// -*- C++ Header -*-
/*
==================================================
Authors: A.Mithran;
Emails: mithran@fias.uni-frankfurt.de
==================================================
*/

#ifndef SIMD_MASK_H
#define SIMD_MASK_H

#include "simd_tag.h"
#include "simd_data.h"
#include "simd_detail.h"

#include <iosfwd>

namespace KFP {
namespace SIMD {

template <Tag tag> class SimdMaskBase
{
public:
    typedef typename SimdData<int, tag>::simd_type simd_typei;
    typedef typename SimdData<float, tag>::simd_type simd_typef;
    static constexpr int SimdSize{ sizeof(simd_typei) };
    static constexpr int SimdLen{ SimdSize / sizeof(int) };
    // ------------------------------------------------------
    // Constructors
    // ------------------------------------------------------
    // Default constructor:
    SimdMaskBase();
    SimdMaskBase(bool val);
    SimdMaskBase(const bool* val_ptr);
    SimdMaskBase(const simd_typei& mask);
    SimdMaskBase(const simd_typef& mask);
    SimdMaskBase(const SimdMaskBase& class_mask);

    // Assignment operators
    SimdMaskBase& operator=(bool val);
    SimdMaskBase& operator=(const simd_typei& mask);
    SimdMaskBase& operator=(const simd_typef& mask);
    SimdMaskBase& operator=(const SimdMaskBase& class_mask);

    // ------------------------------------------------------
    // Status accessors
    // ------------------------------------------------------
    int count() const;
    bool AND() const;
    bool OR() const;

    // ------------------------------------------------------
    // Data member accessors
    // ------------------------------------------------------
    bool operator[](int index) const;

    simd_typei& maski()
    {
        return mask_;
    }
    const simd_typei& maski() const
    {
        return mask_;
    }
    simd_typef maskf() const;

    // ------------------------------------------------------
    // Print I/O
    // ------------------------------------------------------
    friend std::ostream& operator<<(std::ostream& stream, const SimdMaskBase& s)
    {
        Detail::print<SimdMaskBase>(stream, s);
        return stream;
    }
    /* Comparison */
    friend SimdMaskBase operator!(const SimdMaskBase& a)
    {
        return SimdMaskBase{ Detail::opNOT<simd_typei>(a.mask_) };
    }
    friend bool operator==(const SimdMaskBase& a, const SimdMaskBase& b)
    { // mask returned
        return Detail::opEqual<bool, simd_typei, simd_typei>(a.mask_, b.mask_);
    }
    friend bool operator!=(const SimdMaskBase& a, const SimdMaskBase& b)
    { // mask returned
        return Detail::opNotEqual<bool, simd_typei, simd_typei>(a.mask_, b.mask_);
    }
    friend SimdMaskBase operator&&(const SimdMaskBase& a, const SimdMaskBase& b)
    { // mask returned
        return SimdMaskBase{ Detail::opANDbitwise<simd_typei>(a.mask_, b.mask_) };
    }
    friend SimdMaskBase operator||(const SimdMaskBase& a, const SimdMaskBase& b)
    { // mask returned
        return SimdMaskBase{ Detail::opORbitwise<simd_typei>(a.mask_, b.mask_) };
    }

protected:
    simd_typei mask_;
    // int __KFP_SIMD__ATTR_ALIGN(
    //     __KFP_SIMD__Size_Int) bools_[__KFP_SIMD__Len_Int]{}; // Indices array
    // __KFP_SIMD__SPEC_ALIGN(__KFP_SIMD__Size_Int) int bools_[__KFP_SIMD__Len_Int]{};
};

} // namespace SIMD
} // namespace KFP

#endif
