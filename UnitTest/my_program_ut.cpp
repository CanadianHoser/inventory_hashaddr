//
//  linked_lists_tests.cpp
//  HelloWorld
//
//  Created by Warren Hein on 3/10/21.
//

#include <stdio.h>
#include "CppUTest/TestHarness.h"
extern "C" {
#include "md5sum.h"
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
    md5sum_t result;
    char const buf[] = "The quick brown fox jumped over the lazy dog.";
    generate_md5_for_string(buf, sizeof(buf), &result);
    LONGS_EQUAL(0x5C6FFBDD40D9556B, result.upper);
    LONGS_EQUAL(0x73A21E63C3E0E904, result.lower);
}

// Using https://onlinehashtools.com/calculate-md5-hash for comparison
