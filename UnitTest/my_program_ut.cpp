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

TEST(my_test, checkTestHarness)
{
    CHECK_EQUAL(2, add_num(1,1));
}
