#include <stdio.h>
#include <stdlib.h>
#include <openssl/md5.h>

int add_num(int a, int b)
{
    return (a+b);
}

void test_md5 (void)
{
    int n;
    MD5_CTX c;
    char const buf[] = "The quick brown fox jumped over the lazy dog.";
    ssize_t bytes;
    unsigned char out[MD5_DIGEST_LENGTH];

    MD5_Init(&c);
    bytes=read(STDIN_FILENO, buf, 512);
    while(bytes > 0)
    {
        MD5_Update(&c, buf, bytes);
        bytes=read(STDIN_FILENO, buf, 512);
    }

    MD5_Final(out, &c);

    for(n=0; n<MD5_DIGEST_LENGTH; n++)
        printf("%02x", out[n]);
    printf("\n");

    return;
}
