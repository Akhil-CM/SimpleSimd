// -*- C++ -*-
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include "../simd.h"
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
}
TEST_CASE("Testing methods") {
    const SimdF val_const = KFP::SIMD::SimdF(1.0f) ;
    const SimdF val_10 = KFP::SIMD::SimdF(10.0f) ;
    SUBCASE("Testing the single value constructor") {
        CHECK((val_const == SimdF(1.0f)).AND() );
    }
    SUBCASE("Testing insert") {
        const SimdF val_zero = SimdF{0.0f} ;
        const SimdF val_1 =  KFP::SIMD::SimdF(1.0f);
        const SimdF val_2 =  KFP::SIMD::SimdF(2.0f);
        const SimdF val_3 =  KFP::SIMD::SimdF(3.0f);
        const SimdF val_4 =  KFP::SIMD::SimdF(4.0f);
        SUBCASE("Testing addition version 1") {
            CHECK((val_10 == (val_1 + val_2 + val_3 + val_1)).count() == 2);
            CHECK((val_10 == (val_1 + val_2 + val_3 + val_4)).AND());
        }
        // SUBCASE("Testing insert") {
        //     CHECK((SimdF{0.0f}.insert(0, 1.0f) == val_1).AND()) ;
        //     CHECK((SimdF{0.0f}.insert(1, 2.0f) == val_2).AND());
        //     CHECK((SimdF{0.0f}.insert(2, 3.0f) == val_3).AND());
        //     CHECK((SimdF{0.0f}.insert(3, 4.0f) == val_4).AND());
        // }
    }
}
