#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
// #include <openssl/md5.h>
#include <openssl/evp.h>
#include <arpa/inet.h>  // For inet_ntop and inet_pton

int add_num(int a, int b)
{
    return (a+b);
}

void generate_md5_for_string(char const *input_buf, ssize_t bytes)
{
    char md5buf[33];
    EVP_MD_CTX *mdctx = EVP_MD_CTX_new();
    const EVP_MD *md = EVP_md5();
    unsigned char md_value[EVP_MAX_MD_SIZE];
    unsigned int md_len, i;
    printf("input_buf: \"%s\", # bytes (without NULL terminator) = %zd\n", input_buf, bytes-1);
    EVP_DigestInit_ex(mdctx, md, NULL);
    EVP_DigestUpdate(mdctx, input_buf, bytes-1); // Ignore NULL terminator
    EVP_DigestFinal_ex(mdctx, md_value, &md_len);
    EVP_MD_CTX_free(mdctx);
    for (i = 0; i < md_len; i++) {
        snprintf(&(md5buf[i * 2]), 16 * 2, "%02x", md_value[i]);
    }
    printf("%s\n", md5buf);
}
