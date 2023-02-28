#ifndef __MD5SUM_H__
#define __MD5SUM_H__
#include <stdint.h>

typedef struct {
    uint64_t lower;
    uint64_t upper;
} md5sum_t;

extern void generate_md5_for_string(char const *input_buf, ssize_t bytes, md5sum_t *pResult);




#endif /* __MD5SUM_H__ */
