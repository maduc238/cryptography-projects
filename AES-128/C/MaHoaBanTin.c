/** Copyright (C) 2022 maduc238
 *
 * \file Cipher.c
 *
 * \author Ma Duc <mavietduc@gmail.com>
 *
 * Code mã hóa AES-128
 * 
 */

#include "AES128.h"

void Cipher(uint8_t input[], uint8_t key[], uint8_t sbox[]) {
    printf("Plaintext:");
    printf("\nHex: ");
    for (int i=0; i<16; i++) {
        printf("%02x",input[i]);
    }
    printf("\nText: ");
    for (int i=0; i<16; i++) {
        printf("%c",input[i]);
    }
    printf("\n");
    
    // uint8_t sbox[256];      /* Dùng để tạo S-box */
    // InitializeAESsbox(sbox);
    uint8_t *w;             /* Dùng để tạo Key Expansion */
    w = KeyExpansion(key, sbox);

    AddRoundKey(input, key);
    for (int i=1; i<=Nr-1; i++) {
        SubBytes(input, sbox);
        ShiftRows(input);
        MixColumns(input);
        for (int j=0; j<16; j++) {
            key[j] = w[16*i+j];
        }
        AddRoundKey(input, key);
    }
    SubBytes(input, sbox);
    ShiftRows(input);
    for (int j=0; j<16; j++)
        key[j] = w[16*Nr+j];
    AddRoundKey(input, key);
    
    /* Print kết quả */
    printf("\nCiphertext:");
    printf("\nHex: ");
    for (int i=0; i<16; i++) {
        printf("%02x",input[i]);
    }
    printf("\nText: ");
    for (int i=0; i<16; i++) {
        printf("%c",input[i]);
    }

	printf("\n*****************************************************\n\n");
}

int main(int argc, char *argv[]) {
    uint8_t key[] = {0xDE,0x8D,0xC9,0x1B,0xC4,0xD9,0x05,0x20,0xB4,0x66,0x25,0x2A,0xF0,0x22,0x24,0xB6};

	FILE *fp;
   	char buff[16];

   	fp = fopen("E:/Download/AES-128/cryptography-projects-main/AES-128/C/plaintext.txt", "r");

	uint8_t sbox[256];      /* Dùng để tạo S-box */
    InitializeAESsbox(sbox);

	while(fgets(buff, 17, (FILE*)fp) != NULL) {
		uint8_t plaintext[16];
		for (int i=0; i<16; i++) {
			plaintext[i] = (uint8_t)buff[i];
		}
		Cipher(buff, key, sbox);
		for (int i=0; i<16; i++) {
			buff[i] = 0x00;
		}
	}
}
