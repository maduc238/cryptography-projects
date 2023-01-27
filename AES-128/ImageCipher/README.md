# Mã hóa ảnh bằng mã AES-128

![image](https://user-images.githubusercontent.com/95759699/210175373-9f5337e1-31e1-4fae-a815-394b2ab64d97.png)

Chạy bằng C (để mã hóa và giải mã AES) và Python (hiển thị hình ảnh)

## Requirements

Hệ điều hành: Chạy tốt trên Ubuntu, Windows

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
Nếu không chạy được thì phải cấp thêm quyền cho file bằng cách `chmod +x ./Cipher`

Ngoài ra để xem chi tiết hướng dẫn:
```
./Cipher -h
```
Để xem các lệnh khác khi nhập file ảnh, key hoặc chế độ mã hóa khác. Ví dụ như khi sử dụng ở chế độ ECB và CBC

![image](https://user-images.githubusercontent.com/95759699/210236804-e71cc633-5191-43aa-9e2e-16d9f84fda46.png)

![image](https://user-images.githubusercontent.com/95759699/210236838-b6b5f2f4-f2e6-49ee-82a1-cfc8028b1f8d.png)

Tương tự với mã hóa là code giải mã (bắt buộc phải mã hóa đúng ảnh đó trước)
```
./InverseCipher
```
