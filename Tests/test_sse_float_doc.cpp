// -*- C++ -*-
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include "../simd.hpp"
#include <iostream>

using KFP::SIMD::SimdMask;
using KFP::SIMD::SimdIndex;
using KFP::SIMD::SimdF;

TEST_CASE("Testing SimdMask") {
    SimdMask mask;
    SUBCASE("Testing default state") {
        CHECK(mask == SimdMask{false});
        CHECK(mask.AND() == false);
        CHECK(mask.OR() == false);
        CHECK(mask.count() == 0);
    }

    SimdMask mask_true{true};
    SUBCASE("Testing constant constructor") {
        CHECK(mask_true.AND() == true);
        CHECK(mask_true.OR() == true);
        CHECK(mask_true.count() == 4);
    }

    bool masks[]{true, false, false, false};
    SimdMask mask_1true3false{masks};
    SUBCASE("Testing variable constructor") {
        CHECK(mask_1true3false.AND() == false);
        CHECK(mask_1true3false.OR() == true);
        CHECK(mask_1true3false.count() == 1);
    }
}
TEST_CASE("Testing methods") {
    float constants[]{1.0f, 1.0f, 1.0f, 1.0f};
    float f1234[]{1.0f, 2.0f, 3.0f, 4.0f};
    const SimdF val_const = KFP::SIMD::SimdF(constants) ;
    const SimdF val_1234 = KFP::SIMD::SimdF(f1234) ;
    SUBCASE("Testing the single value constructor") {
        CHECK((val_const == SimdF(1.0f)).AND() );
    }
    SUBCASE("Testing insert") {
        const SimdF val_zero = SimdF{0.0f} ;
        float f1000[]{1.0f, 0.0f, 0.0f, 0.0f};
        float f0200[]{0.0f, 2.0f, 0.0f, 0.0f};
        float f0030[]{0.0f, 0.0f, 3.0f, 0.0f};
        float f0004[]{0.0f, 0.0f, 0.0f, 4.0f};
        const SimdF val_1000 =  KFP::SIMD::SimdF(f1000);
        const SimdF val_0200 =  KFP::SIMD::SimdF(f0200);
        const SimdF val_0030 =  KFP::SIMD::SimdF(f0030);
        const SimdF val_0004 =  KFP::SIMD::SimdF(f0004);
        CHECK((SimdF{0.0f}.insert(0, 1.0f) == val_1000).AND()) ;
        CHECK((SimdF{0.0f}.insert(1, 2.0f) == val_0200).AND());
        CHECK((SimdF{0.0f}.insert(2, 3.0f) == val_0030).AND());
        CHECK((SimdF{0.0f}.insert(3, 4.0f) == val_0004).AND());
        SUBCASE("Testing addition version 1") {
            CHECK((val_1234 == (val_1000 + val_0200 + val_0030 + val_1000)).count() == 2);
            CHECK((val_1234 == (val_1000 + val_0200 + val_0030 + val_0004)).AND());
        }
    }
}
