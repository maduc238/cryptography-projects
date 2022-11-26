/** Copyright (C) 2022 maduc238
 *
 * \file InverseCipher.c
 *
 * \author Ma Duc <mavietduc@gmail.com>
 * 
 * Code giải mã AES-128
 *
 */

#include "AES128.h"

#define ROTL8(x,shift) ((uint8_t) ((x) << (shift)) | ((x) >> (8 - (shift))))
#define Nk 4
#define Nb 4
#define Nr 10

int main(int argc, char *argv[]) {
    /* Sửa giá trị input tại đây */
    uint8_t input[16] = {0x52,0x0a,0x25,0xee,0x81,0xef,0xcf,0x2b,0x39,0x6b,0xa5,0x01,0x62,0xed,0x11,0xf8};
    // uint8_t key[16] = {0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a,0x0b,0x0c,0x0d,0x0e,0x0f};
    uint8_t key[] = "xin chao cac ban";
    printf("Ciphertext:");
    printf("\nHex: ");
    for (int i=0; i<16; i++) {
        printf("0x%02x,",input[i]);
    }
    printf("\nText: ");
    for (int i=0; i<16; i++) {
        printf("%c",input[i]);
    }
    printf("\n");

    uint8_t sbox[256];      /* Dùng để tạo S-box */
    InitializeAESsbox(sbox);
    uint8_t *w;             /* Dùng để tạo Key Expansion */
    w = KeyExpansion(key, sbox);

    for (int j=0; j<16; j++)
        key[j] = w[16*Nr+j];
    AddRoundKey(input, key);

    for (int i=Nr-1; i>=1; i--) {
        InvShiftRows(input);
        InvSubBytes(input, InvSBox);
        for (int j=0; j<16; j++) {
            key[j] = w[16*i+j];
        }
        AddRoundKey(input, key);
        InvMixColumns(input);
    }

    InvShiftRows(input);
    InvSubBytes(input, InvSBox);
    for (int j=0; j<16; j++) {
        key[j] = w[j];
    }
    AddRoundKey(input, key);

    /* Print kết quả */
    printf("\nPlaintext:");
    printf("\nHex: ");
    for (int i=0; i<16; i++) {
        printf("0x%02x,",input[i]);
    }
    printf("\nText: ");
    for (int i=0; i<16; i++) {
        printf("%c",input[i]);
    }
}