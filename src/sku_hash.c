#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
// #include <openssl/md5.h>
#include <openssl/evp.h>
#include <arpa/inet.h>  // For inet_ntop and inet_pton
#include "sku_hash.h"


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
    pResult->upper = ntohll(*(uint64_t*)(&md_value[0]));
    pResult->lower = ntohll(*(uint64_t*)(&md_value[8]));
}
