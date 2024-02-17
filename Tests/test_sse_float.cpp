// -*- C++ -*-
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include "../simd.hpp"
#include <iostream>

using KFP::SIMD::SimdMask;
using KFP::SIMD::SimdIndex;
using KFP::SIMD::SimdF_t;

TEST_CASE("Testing SimdMask") {
    SimdMask mask;
    SUBCASE("Testing default state") {
        CHECK(mask == SimdMask{false});
        CHECK(mask.statusAND() == false);
        CHECK(mask.statusOR() == false);
        CHECK(mask.count() == 0);
    }

    SimdMask mask_true{true};
    SUBCASE("Testing constant constructor") {
        CHECK(mask_true.statusAND() == true);
        CHECK(mask_true.statusOR() == true);
        CHECK(mask_true.count() == 4);
    }

    SimdMask mask_1true3false{true, false, false, false};
    SUBCASE("Testing variable constructor") {
        CHECK(mask_1true3false.statusAND() == false);
        CHECK(mask_1true3false.statusOR() == true);
        CHECK(mask_1true3false.count() == 1);
    }
}
TEST_CASE("Testing methods") {
    const SimdF_t val_const = KFP::SIMD::SimdF(1.0f, 1.0f, 1.0f, 1.0f) ;
    const SimdF_t val_1234 = KFP::SIMD::SimdF(1.0f, 2.0f, 3.0f, 4.0f) ;
    SUBCASE("Testing the single value constructor") {
        CHECK((val_const == SimdF_t(1.0f)).statusAND() );
    }
    SUBCASE("Testing insert") {
        const SimdF_t val_zero = SimdF_t{0.0f} ;
        const SimdF_t val_1000 =  KFP::SIMD::SimdF(1.0f, 0.0f, 0.0f, 0.0f);
        const SimdF_t val_0200 =  KFP::SIMD::SimdF(0.0f, 2.0f, 0.0f, 0.0f);
        const SimdF_t val_0030 =  KFP::SIMD::SimdF(0.0f, 0.0f, 3.0f, 0.0f);
        const SimdF_t val_0004 =  KFP::SIMD::SimdF(0.0f, 0.0f, 0.0f, 4.0f);
        CHECK((SimdF_t{0.0f}.insert(0, 1.0f) == val_1000).statusAND()) ;
        CHECK((SimdF_t{0.0f}.insert(1, 2.0f) == val_0200).statusAND());
        CHECK((SimdF_t{0.0f}.insert(2, 3.0f) == val_0030).statusAND());
        CHECK((SimdF_t{0.0f}.insert(3, 4.0f) == val_0004).statusAND());
        SUBCASE("Testing addition version 1") {
            CHECK((val_1234 == (val_1000 + val_0200 + val_0030 + val_1000)).count() == 2);
            CHECK((val_1234 == (val_1000 + val_0200 + val_0030 + val_0004)).statusAND());
        }
    }
}
