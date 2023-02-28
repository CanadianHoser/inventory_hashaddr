//
//  linked_lists_tests.cpp
//  HelloWorld
//
//  Created by Warren Hein on 3/10/21.
//

#include <stdio.h>
#include "CppUTest/TestHarness.h"
#include <arpa/inet.h>
extern "C" {
#include "asset_tracker.h"
}

TEST_GROUP(my_test)
{
    void setup() override
    {
        set_logging(false);
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

TEST(my_test, canGetLanPrefix)
{
    struct in6_addr prefix;
    char const buf[] = "2001:a:b:c:1:2:3:4";
    (void) get_ipv6_network_prefix(buf, sizeof(buf), &prefix);
    LONGS_EQUAL(0x2001000a, ntohl(prefix.__u6_addr.__u6_addr32[0]));
    LONGS_EQUAL(0x000b000c, ntohl(prefix.__u6_addr.__u6_addr32[1]));
    LONGS_EQUAL(0x00000000, ntohl(prefix.__u6_addr.__u6_addr32[2]));
    LONGS_EQUAL(0x00000000, ntohl(prefix.__u6_addr.__u6_addr32[3]));
}

TEST(my_test, invalidLanResultsInNegativeReturn)
{
    struct in6_addr prefix;
    char const buf[] = "2001:a:b:c:1:2:3:yada";
    LONGS_EQUAL(-1, get_ipv6_network_prefix(buf, sizeof(buf), &prefix));
}

TEST(my_test, invalidLanResultsInZeroPrefix)
{
    struct in6_addr prefix;
    char const buf[] = "2001:a:b:c:1:2:3:yada";
    LONGS_EQUAL(-1, get_ipv6_network_prefix(buf, sizeof(buf), &prefix));
    LONGS_EQUAL(0x00000000, ntohl(prefix.__u6_addr.__u6_addr32[0]));
    LONGS_EQUAL(0x00000000, ntohl(prefix.__u6_addr.__u6_addr32[1]));
    LONGS_EQUAL(0x00000000, ntohl(prefix.__u6_addr.__u6_addr32[2]));
    LONGS_EQUAL(0x00000000, ntohl(prefix.__u6_addr.__u6_addr32[3]));
}

TEST(my_test, partialNetworkGeneratesPrefix)
{
    struct in6_addr prefix;
    char const buf[] = "2001:a::";
    (void) get_ipv6_network_prefix(buf, sizeof(buf), &prefix);
    LONGS_EQUAL(0x2001000a, ntohl(prefix.__u6_addr.__u6_addr32[0]));
    LONGS_EQUAL(0x00000000, ntohl(prefix.__u6_addr.__u6_addr32[1]));
    LONGS_EQUAL(0x00000000, ntohl(prefix.__u6_addr.__u6_addr32[2]));
    LONGS_EQUAL(0x00000000, ntohl(prefix.__u6_addr.__u6_addr32[3]));
}
