#ifndef __SKU_HASH_H__
#define __SKU_HASH_H__
#include <stdint.h>
#include <stdbool.h>
#include <arpa/inet.h>

typedef struct {
    uint64_t lower;
    uint64_t upper;
} sku_hash_t;

#define MAX_SKU_STRING_LENGTH 128   /* Including NULL terminator */
#define SUCCESS 1
#define NULL_PTR -1
#ifndef HTONLL
#define HTONLL(x) ((1==htonl(1)) ? (x) : (((uint64_t)htonl((x) & 0xFFFFFFFFUL)) << 32) | htonl((uint32_t)((x) >> 32)))
#define NTOHLL(x) ((1==ntohl(1)) ? (x) : (((uint64_t)ntohl((x) & 0xFFFFFFFFUL)) << 32) | ntohl((uint32_t)((x) >> 32)))
#endif

extern void set_logging(bool logging_on);
extern void generate_hash_for_sku(char const *input_buf, ssize_t bytes, sku_hash_t *pResult);
extern int get_ipv6_network_prefix(const char *input_buf, struct in6_addr *pPrefix);
int generate_address_for_sku(const char *network_buf, const char *sku_buf, struct in6_addr *pIpv6_addr);


#endif /* __SKU_HASH_H__ */
