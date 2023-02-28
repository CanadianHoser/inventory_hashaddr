#ifndef __SKU_HASH_H__
#define __SKU_HASH_H__
#include <stdint.h>

typedef struct {
    uint64_t lower;
    uint64_t upper;
} sku_hash_t;

extern void generate_hash_for_sku(char const *input_buf, ssize_t bytes, sku_hash_t *pResult);




#endif /* __SKU_HASH_H__ */
