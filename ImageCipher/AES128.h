/** Copyright (C) 2022 maduc238
 *
 * \file AES128.h
 *
 * \author Ma Duc <mavietduc@gmail.com>
 *
 * Library functions cho AES-128
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

uint8_t InvSBox[] = {
    0x52, 0x09, 0x6a, 0xd5, 0x30, 0x36, 0xa5, 0x38, 0xbf, 0x40, 0xa3, 0x9e, 0x81, 0xf3, 0xd7, 0xfb, 
    0x7c, 0xe3, 0x39, 0x82, 0x9b, 0x2f, 0xff, 0x87, 0x34, 0x8e, 0x43, 0x44, 0xc4, 0xde, 0xe9, 0xcb, 
    0x54, 0x7b, 0x94, 0x32, 0xa6, 0xc2, 0x23, 0x3d, 0xee, 0x4c, 0x95, 0x0b, 0x42, 0xfa, 0xc3, 0x4e, 
    0x08, 0x2e, 0xa1, 0x66, 0x28, 0xd9, 0x24, 0xb2, 0x76, 0x5b, 0xa2, 0x49, 0x6d, 0x8b, 0xd1, 0x25, 
    0x72, 0xf8, 0xf6, 0x64, 0x86, 0x68, 0x98, 0x16, 0xd4, 0xa4, 0x5c, 0xcc, 0x5d, 0x65, 0xb6, 0x92, 
    0x6c, 0x70, 0x48, 0x50, 0xfd, 0xed, 0xb9, 0xda, 0x5e, 0x15, 0x46, 0x57, 0xa7, 0x8d, 0x9d, 0x84, 
    0x90, 0xd8, 0xab, 0x00, 0x8c, 0xbc, 0xd3, 0x0a, 0xf7, 0xe4, 0x58, 0x05, 0xb8, 0xb3, 0x45, 0x06, 
    0xd0, 0x2c, 0x1e, 0x8f, 0xca, 0x3f, 0x0f, 0x02, 0xc1, 0xaf, 0xbd, 0x03, 0x01, 0x13, 0x8a, 0x6b, 
    0x3a, 0x91, 0x11, 0x41, 0x4f, 0x67, 0xdc, 0xea, 0x97, 0xf2, 0xcf, 0xce, 0xf0, 0xb4, 0xe6, 0x73, 
    0x96, 0xac, 0x74, 0x22, 0xe7, 0xad, 0x35, 0x85, 0xe2, 0xf9, 0x37, 0xe8, 0x1c, 0x75, 0xdf, 0x6e, 
    0x47, 0xf1, 0x1a, 0x71, 0x1d, 0x29, 0xc5, 0x89, 0x6f, 0xb7, 0x62, 0x0e, 0xaa, 0x18, 0xbe, 0x1b, 
    0xfc, 0x56, 0x3e, 0x4b, 0xc6, 0xd2, 0x79, 0x20, 0x9a, 0xdb, 0xc0, 0xfe, 0x78, 0xcd, 0x5a, 0xf4, 
    0x1f, 0xdd, 0xa8, 0x33, 0x88, 0x07, 0xc7, 0x31, 0xb1, 0x12, 0x10, 0x59, 0x27, 0x80, 0xec, 0x5f, 
    0x60, 0x51, 0x7f, 0xa9, 0x19, 0xb5, 0x4a, 0x0d, 0x2d, 0xe5, 0x7a, 0x9f, 0x93, 0xc9, 0x9c, 0xef, 
    0xa0, 0xe0, 0x3b, 0x4d, 0xae, 0x2a, 0xf5, 0xb0, 0xc8, 0xeb, 0xbb, 0x3c, 0x83, 0x53, 0x99, 0x61, 
    0x17, 0x2b, 0x04, 0x7e, 0xba, 0x77, 0xd6, 0x26, 0xe1, 0x69, 0x14, 0x63, 0x55, 0x21, 0x0c, 0x7d
};

void RotWord(uint8_t data[4]) {
    uint8_t temp = data[0]; data[0] = data[1]; data[1] = data[2]; data[2] = data[3]; data[3] = temp;
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
 * @brief Hàm lấy kết quả từ bảng S-box
 * 
 * @param data dữ liệu muốn thay thế
 * @param sbox Bảng S-box ngược
 */
void InvSubBytes(uint8_t data[16], uint8_t sbox[256]) {
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
void ShiftRows(uint8_t data[]) {
    uint8_t temp;
    temp = data[1]; data[1] = data[5]; data[5] = data[9]; data[9] = data[13]; data[13] = temp;
    temp = data[2]; data[2] = data[10]; data[10] = temp; temp = data[6]; data[6] = data[14]; data[14] = temp;
    temp = data[15]; data[15] = data[11]; data[11] = data[7]; data[7] = data[3]; data[3] = temp;
}

/**
 * @brief Hàm biến đổi khi xoay phải các hàng theo thứ tự
 * tăng dần của số hàng
 * 
 * @param data dữ liệu muốn thay đổi
 */
void InvShiftRows(uint8_t data[16]) {
    uint8_t temp;
    temp = data[13]; data[13] = data[9]; data[9] = data[5]; data[5] = data[1]; data[1] = temp;
    temp = data[2]; data[2] = data[10]; data[10] = temp; temp = data[6]; data[6] = data[14]; data[14] = temp;
    temp = data[3]; data[3] = data[7]; data[7] = data[11]; data[11] = data[15]; data[15] = temp;
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

/**
 * @brief Biến đổi từng cột của ma trận data, hàm ngược của MixColumns()
 * 
 * @param data dữ liệu muốn thay đổi
 */
void InvMixColumns(uint8_t data[16]) {
    uint8_t temp[16];
    for (int i=0; i<4; i++) {
        temp[4*i] =   Multiply(0xe,data[4*i]) ^ Multiply(0xb,data[4*i+1]) ^ Multiply(0xd,data[4*i+2]) ^ Multiply(0x9,data[4*i+3]);
        temp[4*i+1] = Multiply(0x9,data[4*i]) ^ Multiply(0xe,data[4*i+1]) ^ Multiply(0xb,data[4*i+2]) ^ Multiply(0xd,data[4*i+3]);
        temp[4*i+2] = Multiply(0xd,data[4*i]) ^ Multiply(0x9,data[4*i+1]) ^ Multiply(0xe,data[4*i+2]) ^ Multiply(0xb,data[4*i+3]);
        temp[4*i+3] = Multiply(0xb,data[4*i]) ^ Multiply(0xd,data[4*i+1]) ^ Multiply(0x9,data[4*i+2]) ^ Multiply(0xe,data[4*i+3]);
    }
    for (int i=0; i<16; i++)
        data[i] = temp[i];
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
            Rcon[0] = RC[i/4];
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