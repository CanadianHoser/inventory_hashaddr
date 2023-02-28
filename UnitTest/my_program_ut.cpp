//
//  linked_lists_tests.cpp
//  HelloWorld
//
//  Created by Warren Hein on 3/10/21.
//

#include <stdio.h>
#include "CppUTest/TestHarness.h"
extern "C" {
#include "sku_hash.h"
}

TEST_GROUP(my_test)
{
    void setup() override
    {
    }

    void teardown() override
    {
    }
};

TEST(my_test, md5_validation)
{
    sku_hash_t result;
    char const buf[] = "The quick brown fox jumped over the lazy dog.";
    generate_hash_for_sku(buf, sizeof(buf), &result);
    // Using https://onlinehashtools.com/calculate-md5-hash for comparison
    LONGS_EQUAL(0x5C6FFBDD40D9556B, result.upper);
    LONGS_EQUAL(0x73A21E63C3E0E904, result.lower);
}
