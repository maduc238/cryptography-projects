#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#define ROTL8(x,shift) ((uint8_t) ((x) << (shift)) | ((x) >> (8 - (shift))))
#define Nk 4
#define Nb 4
#define Nr 10

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

void RotWord(uint8_t data[4]) {
    uint8_t temp = data[0]; data[0] = data[1]; data[1] = data[2]; data[2] = data[3]; data[3] = temp;
}

void PrintWord(uint8_t data[4]) {
    printf("%02x %02x %02x %02x",data[0],data[1],data[2],data[3]);
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
            printf("\n*********** Vong %d *************************************************\n", i/4+1);
            Rcon[0] = RC[i/4];
            printf("Word %d = ", i+3); PrintWord(temp); printf("\n");
            RotWord(temp);      // i chia het 4
            printf("Sau khi qua RotWord: Word %d = ", i+3); PrintWord(temp); printf("\n");
            for (int i=0; i<4; i++)
                temp[i] = sbox[temp[i]];
            printf("Sau khi qua SubWord: Word %d = ", i+3); PrintWord(temp); printf("\n");
            for (int i=0; i<4; i++)
                temp[i] ^= Rcon[i];
            printf("Rcon[] = 2^%d 00 00 00 = ",i/4); PrintWord(Rcon); printf("\n");
            printf("Sau khi XOR voi Rcon: Word %d = ", i+3); PrintWord(temp); printf("\n");
            printf("Word %d XOR voi Word %d da bien doi de ra Word %d = ", i, i+3, i+4);
        }
        else {
            printf("Word %d XOR voi Word %d de ra Word %d = ", i, i+3, i+4);
        }
        temp2[0] = w[4*i]; temp2[1] = w[4*i+1]; temp2[2] = w[4*i+2]; temp2[3] = w[4*i+3];
        for (int j=0; j<4; j++) {
            temp[j] = temp[j] ^ temp2[j];
            w[4*(i+4)+j] = temp[j];
        }
        PrintWord(temp); printf("\n");
        if ((i+1) % 4 == 0) {
                printf("Vay Key cho vong thu %d la: || ", i/4+1);
                for (int c = 4*(i+1); c<4*(i+5); c++) {
                    printf("%02x ", w[c]);
                }
                printf("||\n");
            }
        i++;
    }
    return w;
}


int main(int argc, char *argv[]) {
    // uint8_t key[16] = {0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a,0x0b,0x0c,0x0d,0x0e,0x0f};
    uint8_t key[16] = {0x0f, 0x15, 0x71, 0xc9, 0x47, 0xd9, 0xe8, 0x59, 0x0c, 0xb7, 0xad, 0xd6, 0xaf, 0x7f, 0x67, 0x98};
    for (int i=0; i<4; i++) {
        printf("Word %d = ", i);
        printf("%02x %02x %02x %02x\n",key[4*i],key[4*i+1],key[4*i+2],key[4*i+3]);
    }
    uint8_t sbox[256];      /* Dùng để tạo S-box */
    InitializeAESsbox(sbox);
    uint8_t *w;             /* Dùng để tạo Key Expansion */
    w = KeyExpansion(key, sbox);
}
