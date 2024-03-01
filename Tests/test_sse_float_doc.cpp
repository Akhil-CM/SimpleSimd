// -*- C++ -*-
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include "../simd.h"
#include <iostream>

using KFP::SIMD::simd_mask;
using KFP::SIMD::simd_index;
using KFP::SIMD::simd_float;

TEST_CASE("Testing simd_mask") {
    simd_mask mask;
    SUBCASE("Testing default state") {
        CHECK(mask == simd_mask{false});
        CHECK(mask.AND() == false);
        CHECK(mask.OR() == false);
        CHECK(mask.count() == 0);
    }

    simd_mask mask_true{true};
    SUBCASE("Testing constant constructor") {
        CHECK(mask_true.AND() == true);
        CHECK(mask_true.OR() == true);
        CHECK(mask_true.count() == 4);
    }

    bool masks[]{true, false, false, false};
    simd_mask mask_1true3false{masks};
    SUBCASE("Testing variable constructor") {
        CHECK(mask_1true3false.AND() == false);
        CHECK(mask_1true3false.OR() == true);
        CHECK(mask_1true3false.count() == 1);
    }
}
TEST_CASE("Testing methods") {
    float constants[]{1.0f, 1.0f, 1.0f, 1.0f};
    float f1234[]{1.0f, 2.0f, 3.0f, 4.0f};
    const simd_float val_const = simd_float(constants) ;
    const simd_float val_1234 = simd_float(f1234) ;
    SUBCASE("Testing the single value constructor") {
        CHECK((val_const == simd_float(1.0f)).AND() );
    }
    SUBCASE("Testing insert") {
        const simd_float val_zero = simd_float{0.0f} ;
        float f1000[]{1.0f, 0.0f, 0.0f, 0.0f};
        float f0200[]{0.0f, 2.0f, 0.0f, 0.0f};
        float f0030[]{0.0f, 0.0f, 3.0f, 0.0f};
        float f0004[]{0.0f, 0.0f, 0.0f, 4.0f};
        const simd_float val_1000 = simd_float(f1000);
        const simd_float val_0200 = simd_float(f0200);
        const simd_float val_0030 = simd_float(f0030);
        const simd_float val_0004 = simd_float(f0004);
        CHECK((simd_float{0.0f}.insert(0, 1.0f) == val_1000).AND()) ;
        CHECK((simd_float{0.0f}.insert(1, 2.0f) == val_0200).AND());
        CHECK((simd_float{0.0f}.insert(2, 3.0f) == val_0030).AND());
        CHECK((simd_float{0.0f}.insert(3, 4.0f) == val_0004).AND());
        SUBCASE("Testing addition version 1") {
            CHECK((val_1234 == (val_1000 + val_0200 + val_0030 + val_1000)).count() == 2);
            CHECK((val_1234 == (val_1000 + val_0200 + val_0030 + val_0004)).AND());
        }
    }
}
