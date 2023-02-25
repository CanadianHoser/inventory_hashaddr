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
extern void test_md5(void);
extern void test_new_md5(char const *, ssize_t);
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

TEST(my_test, md5_validation)
{
    char const buf[] = "The quick brown fox jumped over the lazy dog.";
    test_md5();
    test_new_md5(buf, sizeof(buf));
}
