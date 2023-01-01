#include "AES128.h"

uint8_t toInt(char a){
    if (a >= 48 && a <= 57)
        return a-48;
    else
        return a-87;
}

int main(int argc, char *argv[]) {
    // uint8_t key[] = {0xDE,0x8D,0xC9,0x1B,0xC4,0xD9,0x05,0x20,0xB4,0x66,0x25,0x2A,0xF0,0x22,0x24,0xB6};
    printf("Running GiaiMa.c\n");
    uint8_t key[] = "xin chao cac ban";
    uint8_t key_temp[16];

    char buff[32];
    FILE *fp, *fp_write;

    fp = fopen("data/encrypted.data", "r");
    fp_write = fopen("data/picture.data", "w+");

    uint8_t sbox[256];      /* Dùng để tạo S-box */
    InitializeAESsbox(sbox);
    uint8_t *w;             /* Dùng để tạo Key Expansion */
    w = KeyExpansion(key, sbox);

    while(fgets(buff, 33, (FILE*)fp) != NULL) {
        sbox[0] = 0x63;
		uint8_t plaintext[16];
		for (int i=0; i<16; i++) {
			plaintext[i] = (uint8_t) (toInt(buff[2*i])*16 + toInt(buff[2*i+1]));
		}

        for (int j=0; j<16; j++)
            key_temp[j] = w[16*Nr+j];
        AddRoundKey(plaintext, key_temp);

        for (int i=Nr-1; i>=1; i--) {
            InvShiftRows(plaintext);
            InvSubBytes(plaintext, InvSBox);
            for (int j=0; j<16; j++) {
                key_temp[j] = w[16*i+j];
            }
            AddRoundKey(plaintext, key_temp);
            InvMixColumns(plaintext);
        }

        InvShiftRows(plaintext);
        InvSubBytes(plaintext, InvSBox);
        for (int j=0; j<16; j++) {
            key_temp[j] = w[j];
        }
        AddRoundKey(plaintext, key_temp);

        // Write to file
        for (int i=0; i<16; i++) {
            fprintf(fp_write,"%02x",plaintext[i]);
        }

		for (int i=0; i<16; i++) {
			plaintext[i] = 0x00;
		}
	}

    fclose(fp);
    fclose(fp_write);
}