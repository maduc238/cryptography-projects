# Code mô phỏng mã khối AES-128 bằng C

## Chi tiết các file
- `AES128.h` thư viện khi sử dụng với bộ mã
- `Cipher.c` code thực hiện mã hóa 128 bit plaintext
- `InverseCipher.c`code thực hiện giải mã 128 bit ciphertext

## Chạy mô phỏng
Tại hàm `main` của mỗi file mã hóa hoặc giải mã, thay đổi giá trị input đầu vào. Input text có thể dạng hex:
```C
uint8_t input[16] = {0x00,0x11,0x22,0x33,0x44,0x55,0x66,0x77,0x88,0x99,0xaa,0xbb,0xcc,0xdd,0xee,0xff};
uint8_t key[16] = {0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a,0x0b,0x0c,0x0d,0x0e,0x0f}; 
```
Hoặc dạng văn bản UTF-8:
```C
uint8_t input[] = "Ma Viet Duc 1234";
uint8_t key[] = "xin chao cac ban";
```
***Lưu ý:*** Chiều dài của input và key luôn là 16
