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

int main(int argc, char *argv[]) {
    /* Sửa giá trị input tại đây */
    // uint8_t input[16] = {0x00,0x11,0x22,0x33,0x44,0x55,0x66,0x77,0x88,0x99,0xaa,0xbb,0xcc,0xdd,0xee,0xff};
    uint8_t input[] = "Ma Viet Duc 1234";
    // uint8_t key[16] = {0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a,0x0b,0x0c,0x0d,0x0e,0x0f};
    uint8_t key[] = "xin chao cac ban";
    
    printf("Plaintext:");
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
        printf("0x%02x,",input[i]);
    }
    printf("\nText: ");
    for (int i=0; i<16; i++) {
        printf("%c",input[i]);
    }
}