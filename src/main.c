#include <stdlib.h>
#include <stdio.h>
#include <arpa/inet.h>
#include "asset_tracker.h"

void usage() {
    printf("Usage: hashaddr <ipv6addr> <sku>\n"
           "     ipv6addr formats: a:b:c:d:e:f:g:h\n"
           "                       a::h\n"
           "                       a:b::h\n"
           "     sku: arbitrary string without spaces\n");
}


int main(int argc, char *argv[]) 
{
    int rc = 0;
    char result[80];
    struct in6_addr addr;
    if (argc < 2) {
        usage();
        exit(0);
    }
    rc = generate_address_for_sku(argv[1], argv[2], &addr);
    if (rc == 1) {
        usage();
        exit(-1);
    } else {
        inet_ntop(AF_INET6, &addr, result, sizeof(result));
        printf("%s\n", result);
    }

	exit(0);
}
