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

TEST_GROUP(asset_tracker)
{
    void setup() override
    {
        set_logging(false);
    }

    void teardown() override
    {
    }
};

TEST(asset_tracker, md5_validation)
{
    sku_hash_t result;
    char const buf[] = "The quick brown fox jumped over the lazy dog.";
    generate_hash_for_sku(buf, sizeof(buf), &result);
    // Using https://onlinehashtools.com/calculate-md5-hash for comparison
    LONGS_EQUAL(0x5C6FFBDD40D9556B, result.upper);
    LONGS_EQUAL(0x73A21E63C3E0E904, result.lower);
}

TEST(asset_tracker, example_validation) 
{
    sku_hash_t result;
    char const buf[] = "ABCDefgh0123";
    generate_hash_for_sku(buf, sizeof(buf), &result);
    // Using https://onlinehashtools.com/calculate-md5-hash for comparison
    uint64_t w1 = 0x44a938a9acb8e7f4ll;
    uint64_t w2 = 0x5ebc6634e3734c6cll;
    LONGS_EQUAL(w1, result.upper);
    LONGS_EQUAL(w2, result.lower);
}

TEST(asset_tracker, confirmLowOrderIsLowerXoredWithUpper)
{

}

TEST(asset_tracker, canGetLanPrefix)
{
    struct in6_addr prefix;
    char const buf[] = "2001:a:b:c:1:2:3:4";
    (void) get_ipv6_network_prefix(buf, &prefix);
    LONGS_EQUAL(0x2001000a, NTOHL(*(uint32_t *)&prefix.s6_addr[0]));
    LONGS_EQUAL(0x000b000c, NTOHL(*(uint32_t *)&prefix.s6_addr[4]));
    LONGS_EQUAL(0x00000000, NTOHL(*(uint32_t *)&prefix.s6_addr[8]));
    LONGS_EQUAL(0x00000000, NTOHL(*(uint32_t *)&prefix.s6_addr[12]));
}

TEST(asset_tracker, getIpv6PrefixWithBadNetworkReturnsZero)
{
    struct in6_addr prefix;
    char const buf[] = "2001:a:b:c:1:2:3:yada";
    LONGS_EQUAL(0, get_ipv6_network_prefix(buf, &prefix));
}

TEST(asset_tracker, getIpv6PrefixWithGoodNetworkReturnsOne)
{
    struct in6_addr prefix;
    char const buf[] = "2001:a:b:c:1:2:3:4";
    LONGS_EQUAL(1, get_ipv6_network_prefix(buf, &prefix));
}

TEST(asset_tracker, invalidLanResultsInZeroPrefix)
{
    struct in6_addr prefix;
    char const buf[] = "2001:a:b:c:1:2:3:yada";
    (void) get_ipv6_network_prefix(buf, &prefix);
    LONGS_EQUAL(0x00000000, NTOHL(*(uint32_t *)&prefix.s6_addr[0]));
    LONGS_EQUAL(0x00000000, NTOHL(*(uint32_t *)&prefix.s6_addr[4]));
    LONGS_EQUAL(0x00000000, NTOHL(*(uint32_t *)&prefix.s6_addr[8]));
    LONGS_EQUAL(0x00000000, NTOHL(*(uint32_t *)&prefix.s6_addr[12]));
}

TEST(asset_tracker, partialNetworkGeneratesPrefix)
{
    struct in6_addr prefix;
    char const buf[] = "2001:a::";
    (void) get_ipv6_network_prefix(buf, &prefix);
    LONGS_EQUAL(0x2001000a, NTOHL(*(uint32_t *)&prefix.s6_addr[0]));
    LONGS_EQUAL(0x00000000, NTOHL(*(uint32_t *)&prefix.s6_addr[4]));
    LONGS_EQUAL(0x00000000, NTOHL(*(uint32_t *)&prefix.s6_addr[8]));
    LONGS_EQUAL(0x00000000, NTOHL(*(uint32_t *)&prefix.s6_addr[12]));
}

TEST(asset_tracker, checkLower64bitsEndianness)
{
    struct in6_addr prefix;
    char const buf[] = "2001:a:b:c:1:2:3:4";
    (void) inet_pton(AF_INET6, buf, &prefix);
    LONGS_EQUAL(0x0001000200030004, NTOHLL(*(uint64_t *)&prefix.s6_addr[8]));
}

TEST(asset_tracker, addressFromSkuAndNetworkIsGenerated)
{
    char const network[] = "2001:a:b:c::";
    char const sku[] = "ABCDefgh0123";
    char result[80];
    struct in6_addr addr;

    (void) generate_address_for_sku(network, sku, &addr);
    LONGS_EQUAL(htons(0x1a15), *(uint16_t*)&addr.s6_addr[8]);
    LONGS_EQUAL(htons(0x5e9d), *(uint16_t*)&addr.s6_addr[10]);
    LONGS_EQUAL(htons(0x4fcb), *(uint16_t*)&addr.s6_addr[12]);
    LONGS_EQUAL(htons(0xab98), *(uint16_t*)&addr.s6_addr[14]);
    inet_ntop(AF_INET6, &addr, result, sizeof(result));
    // printf("Resulting ipv6 address: %s\n", result);
    STRCMP_NOCASE_EQUAL("2001:a:b:c:1a15:5e9d:4fcb:ab98", result);
}

TEST(asset_tracker, addressFromNullSkuIsGenerated)
{
    char const network[] = "2001:a:b:c::";
    char const sku[] = "";
    struct in6_addr addr;

    LONGS_EQUAL(SUCCESS, generate_address_for_sku(network, sku, &addr));
}

TEST(asset_tracker, addressFromNullNetworkIsNotGenerated)
{
    char const network[] = "";
    char const sku[] = "ABCDdeadBeef";
    struct in6_addr addr;

    CHECK_TRUE(SUCCESS != generate_address_for_sku(network, sku, &addr));
}

TEST(asset_tracker, nullParameterCausesFailure)
{
    char const network[] = "2001:a:b:c::";
    char const sku[] = "ABCDefgh0123";

    LONGS_EQUAL(NULL_PTR, generate_address_for_sku(network, sku, nullptr));
}
