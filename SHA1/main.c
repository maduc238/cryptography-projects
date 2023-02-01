#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "sha1.h"

int main(int argc, char* argv[]){
    SHA1Context sha;
    int i, j, err;
    uint8_t Message_Digest[20];
    char *input = argv[1];
    printf( "Input van ban: '%s'\t\n", input);

    err = SHA1Reset(&sha);
    
    if (err){
        fprintf(stderr, "SHA1Reset Error %d.\n", err );
        return 0;
    }
    if (argc >= 3){
        sha.Print_Block_Input = (uint8_t) argv[2][0] - 48;      /* print padding input data */
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
        printf("Output: ");
        for(i = 0; i < 20 ; ++i)
            printf("%02x ", Message_Digest[i]);
        printf("\n");
    }
    return 0;
}