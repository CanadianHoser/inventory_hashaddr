#ifndef __SKU_HASH_H__
#define __SKU_HASH_H__
#include <stdint.h>
#include <arpa/inet.h>

typedef struct {
    uint64_t lower;
    uint64_t upper;
} sku_hash_t;

extern void generate_hash_for_sku(char const *input_buf, ssize_t bytes, sku_hash_t *pResult);
extern int get_ipv6_network_prefix(const char *input_buf, size_t buf_len, struct in6_addr *pPrefix);

#endif /* __SKU_HASH_H__ */
