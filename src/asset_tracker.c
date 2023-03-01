#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
// #include <openssl/md5.h>
#include <openssl/evp.h>
#include <arpa/inet.h>  // For inet_ntop and inet_pton
#include <string.h>
#include <stdbool.h>
#include "asset_tracker.h"

bool log_errors = true;

void set_logging(bool logging_on) 
{
    log_errors = logging_on;
}

// Generate the hash for the SKU, which is an MD5 of the input 
// input_buf: source string
// bytes: source string length, including NULL terminator
// pResult: resulting container for the md5sum
void generate_hash_for_sku(char const *input_buf, ssize_t bytes, sku_hash_t *pResult)
{
    EVP_MD_CTX *mdctx = EVP_MD_CTX_new();
    const EVP_MD *md = EVP_md5();
    unsigned char md_value[EVP_MAX_MD_SIZE];
    unsigned int md_len;
    EVP_DigestInit_ex(mdctx, md, NULL);
    EVP_DigestUpdate(mdctx, input_buf, bytes-1); // Ignore NULL terminator
    EVP_DigestFinal_ex(mdctx, md_value, &md_len);
    EVP_MD_CTX_free(mdctx);
    // Data is in big-endian format, md5 is a 16 byte (128bit) result
    pResult->upper = NTOHLL(*(uint64_t*)(&md_value[0]));
    pResult->lower = NTOHLL(*(uint64_t*)(&md_value[8]));
}

// Return the upper 64 bits of a 128 bit ipv6 address
// input buffer: NULL-terminated string containing ipv6 address
// pPrefix: resulting structure containing the 64 bit network prefix in the upper 64 bits.
// Returns: 1 if prefix can be calculated, 0 on translation error
int get_ipv6_network_prefix(const char *input_buf, struct in6_addr *pPrefix)
{
    int rc = 0;
    bzero(pPrefix, sizeof(struct in6_addr));
    if ((rc = inet_pton(AF_INET6, input_buf, pPrefix)) != 1)
    {
        if (log_errors)
            fprintf(stderr, "Check inputs, could not validate network %s, rc = %d\n", input_buf, rc);
        return rc;
    }
    // Clear out the low order address
    bzero(&pPrefix->s6_addr[8], sizeof(uint64_t));
    return rc;
}

// TODO: Put these into a structure with more memory safe components
int generate_address_for_sku(const char *network_buf, const char *sku_buf, struct in6_addr *pIpv6_addr)
{
    int rc = 0;
    sku_hash_t hash = {0};
    uint64_t low_addr_host = 0, low_addr = 0;

    if (!(network_buf && sku_buf && pIpv6_addr)) {
        fprintf(stderr, "%s: NULL check failed\n", __FUNCTION__);
        return -1;
    }

    if ((rc = get_ipv6_network_prefix(network_buf, pIpv6_addr)) < 0) {
        return rc;
    }

    generate_hash_for_sku(sku_buf, strnlen(sku_buf, MAX_SKU_STRING_LENGTH)+1, &hash);
    // Hash values are in host order
    low_addr_host = hash.upper ^ hash.lower;
    low_addr = HTONLL(low_addr_host);
    memcpy(&pIpv6_addr->s6_addr[8], &low_addr, sizeof(low_addr));
    return 0;
}
