#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "sha1.h"

int main(int argc, char* argv[]){
    SHA1Context sha;
    int i, j, err;
    uint8_t Message_Digest[20];
    char *input = argv[1];
    printf( "Input: '%s'\n", input);

    err = SHA1Reset(&sha);
    if (err){
        fprintf(stderr, "SHA1Reset Error %d.\n", err );
        return 0;
    }
    err = SHA1Input(&sha, (const unsigned char *) input, strlen(input));
    if (err) {
        fprintf(stderr, "SHA1Input Error %d.\n", err );
        return 0;
    }
    err = SHA1Result(&sha, Message_Digest);
    if (err) {
        fprintf(stderr,
        "SHA1Result Error %d, không thể tính được message digest.\n", err );
    }
    else {
        printf("\t");
        for(i = 0; i < 20 ; ++i)
            printf("%02x ", Message_Digest[i]);
        printf("\n");
    }
    return 0;
}
