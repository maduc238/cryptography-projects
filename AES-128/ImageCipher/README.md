# Mã hóa ảnh bằng mã AES-128

Chạy bằng C (để mã hóa và giải mã AES) và Python (hiển thị hình ảnh)

## Requirements

Hệ điều hành: Chạy tốt trên Ubuntu, Window có thể hơi khác :)

Với C: Chỉ cần có gcc

Với Python:
- numpy
- matplotlib
- argparse

## Hướng dẫn chạy

Các file ảnh phải để dưới dạng `.jpg`

Mã hóa AES-128 file ảnh: Lệnh cơ bản để chạy

```
./Cipher
```
Ngoài ra để xem chi tiết hướng dẫn:
```
./Cipher -h
```
Để xem các lệnh khác khi nhập file ảnh, key hoặc chế độ mã hóa khác

Tương tự với mã hóa là code giải mã
```
./InverseCipher
```
