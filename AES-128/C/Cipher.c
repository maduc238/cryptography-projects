/* Copyright (C) 2022 maduc238
 *
 * \file Cipher.c
 *
 * \author Ma Duc <mavietduc@gmail.com>
 *
 * Code mã hóa AES-128
 * 
 */

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#define ROTL8(x,shift) ((uint8_t) ((x) << (shift)) | ((x) >> (8 - (shift))))
#define Nk 4
#define Nb 4
#define Nr 10

/* Giá trị tràn và modulo (x^8 + x^4 + x^3 + x + 1) */
const int overflow = 0x100, modulus = 0x11b;
/**
 * @brief Hàm nhân hai byte trong field GF(2^8)
 * 
 * @param a 
 * @param b 
 * @return uint8_t kết quả của phép nhân
 */
uint8_t Multiply(int a, int b) {
    uint8_t sum = 0;
    while (b > 0) {
        if (b & 1) sum = sum ^ a;
        b = b >> 1;
        a = a << 1;
        if (a & overflow) a = a ^ modulus;
    }
    return sum;
}

/**
 * @brief Khởi tạo S-box xuôi
 * 
 * @param sbox mảng S-box lưu thông tin của bảng
 */
void InitializeAESsbox(uint8_t sbox[256]) {
	uint8_t p = 1, q = 1;
	do {
		p = p ^ (p << 1) ^ (p & 0x80 ? 0x1B : 0);
		q ^= q << 1;
		q ^= q << 2;
		q ^= q << 4;
		q ^= q & 0x80 ? 0x09 : 0;
		uint8_t xformed = q ^ ROTL8(q, 1) ^ ROTL8(q, 2) ^ ROTL8(q, 3) ^ ROTL8(q, 4);
		sbox[p] = xformed ^ 0x63;
	} while (p != 1);

	sbox[0] = 0x63;
}

/**
 * @brief Cộng XOR hai mảng với nhau, chủ yếu được dùng với
 * key và key expansion 
 * 
 * @param data dữ liệu muốn cộng XOR vào
 * @param w thường là key
 */
void AddRoundKey(uint8_t data[16], uint8_t w[16]) {
    for (int i=0; i<16; i++) {
        data[i] ^= w[i];
    }
}

/**
 * @brief Hàm lấy kết quả từ bảng S-box
 * 
 * @param data dữ liệu muốn thay thế
 * @param sbox Bảng S-box
 */
void SubBytes(uint8_t data[16], uint8_t sbox[256]) {
    for (int i=0; i<16; i++) {
        data[i] = sbox[data[i]];
    }
}

/**
 * @brief Hàm biến đổi khi xoay trái các hàng theo thứ tự
 * tăng dần của số hàng
 * 
 * @param data dữ liệu muốn thay đổi
 */
void ShiftRows(uint8_t data[16]) {
    uint8_t temp;
    temp = data[1]; data[1] = data[5]; data[5] = data[9]; data[9] = data[13]; data[13] = temp;
    temp = data[2]; data[2] = data[10]; data[10] = temp; temp = data[6]; data[6] = data[14]; data[14] = temp;
    temp = data[15]; data[15] = data[11]; data[11] = data[7]; data[7] = data[3]; data[3] = temp;
}

/**
 * @brief Biến đổi từng cột của ma trận data, chi tiết hơn về ma trận
 * này có trong tài liệu
 * 
 * @param data dữ liệu muốn thay đổi
 */
void MixColumns(uint8_t data[16]) {
    uint8_t temp[16];
    for (int i=0; i<4; i++) {
        temp[4*i] = Multiply(2,data[4*i]) ^ Multiply(3,data[4*i+1]) ^ data[4*i+2] ^ data[4*i+3];
        temp[4*i+1] = data[4*i] ^ Multiply(2,data[4*i+1]) ^ Multiply(3,data[4*i+2]) ^ data[4*i+3];
        temp[4*i+2] = data[4*i] ^ data[4*i+1] ^ Multiply(2,data[4*i+2]) ^ Multiply(3,data[4*i+3]);
        temp[4*i+3] = Multiply(3,data[4*i]) ^ data[4*i+1] ^ data[4*i+2] ^ Multiply(2,data[4*i+3]);
    }
    for (int i=0; i<16; i++)
        data[i] = temp[i];
}

void RotWord(uint8_t data[4]) {
    uint8_t temp = data[0]; data[0] = data[1]; data[1] = data[2]; data[2] = data[3]; data[3] = temp;
}

/**
 * @brief Hàm tạo Key Expansion từ một key có sẵn
 * 
 * @param key 
 * @param sbox bảng S-box xuôi 
 * @return uint8_t* con trỏ chứa toàn bộ dữ liệu Key Expansion
 */
uint8_t *KeyExpansion(uint8_t key[16], uint8_t sbox[256]) {
    static uint8_t w[16*(Nr+1)];
    for (int i=0; i<16; i++)
        w[i] = key[i];

    uint8_t temp[4], temp2[4];
    uint8_t Rcon[4] = {0x00, 0x00, 0x00, 0x00};
    uint8_t RC[10] = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36};
    
    temp[0] = w[12]; temp[1] = w[13]; temp[2] = w[14]; temp[3] = w[15];
    int i = 0;
    while (i < 4*Nr) {
        if (i % 4 == 0) {
            // printf("\n*******************************\n");
            Rcon[0] = RC[i/4];
            // printf("Rcon[0] %02x\n",Rcon[0]);
            RotWord(temp);      // i chia het 4
            for (int i=0; i<4; i++)
                temp[i] = sbox[temp[i]] ^ Rcon[i];  // i chia het 4
        }
        temp2[0] = w[4*i]; temp2[1] = w[4*i+1]; temp2[2] = w[4*i+2]; temp2[3] = w[4*i+3];
        for (int j=0; j<4; j++) {
            temp[j] = temp[j] ^ temp2[j];
            w[4*(i+4)+j] = temp[j];
        }
        i++;
    }
    return w;
}

int main(int argc, char *argv[]) {
    /* Sửa giá trị input tại đây */
    uint8_t input[16] = {0x00,0x11,0x22,0x33,0x44,0x55,0x66,0x77,0x88,0x99,0xaa,0xbb,0xcc,0xdd,0xee,0xff};
    uint8_t key[16] = {0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a,0x0b,0x0c,0x0d,0x0e,0x0f};
    
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
    
    printf("Ciphertext\n");
    for (int i=0; i<16; i++) {
        printf("%02x ",input[i]);
    }
}