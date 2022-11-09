# Salsa20

![Spring Boot 2.0](https://img.shields.io/badge/Spring%20Boot-2.0-brightgreen.svg)
![JDK 11.1](https://img.shields.io/badge/JDK-11.1-brightgreen.svg)
![Maven](https://img.shields.io/badge/Maven-3.5.0-yellowgreen.svg)
![license](https://img.shields.io/crates/l/rustc-serialize/0.3.24.svg)

## 1. Giới thiệu

**Salsa20** là một hàm băm với input là 64 byte và output là 64 byte. Hàm băm được sử dụng trong chế độ bộ đếm như một mã dòng: **Salsa20** mã hóa khối 64 byte plaintext bằng cách băm key, số nonce, và số block và thực hiện xor với plaintext. Trước đó ta phải định nghĩa một byte là một phần tử của {0, 1, ..., 255}

Phần dưới đây sẽ nói chi tiết một vài hàm được sử dụng và cách cipher plaintext với **Salsa20** này. Bắt đầu từ ba thao tác đơn giản trên 4 **word**, sau đó thông qua hàm băm **Salsa20** và kết thúc với hàm encryption. Nghe có vẻ khó khăn :sweat_smile: nhưng các bước dưới dây sẽ nói rõ một số hàm cụ thể và có cái nhìn khái quát dễ hiểu hơn.

## 2. word

Một **word** là một phần tử trong tập { $0, 1, ..., 2^{32}-1$ }, mỗi **word** kích thước 4 byte. các **word** trong tài liệu này sẽ thường được viết bằng hexa, bao gồm cả ký tự `0x`. Ví dụ, `0xc0a8787e` = $12 · 2^{28} + 0 · 2^{24} + 10 · 2^{20} + 8 · 2^{16} + 7 · 2^{12} + 8 · 2^8 + 7 · 2^4 + 14 · 2^0 = 3232266366$

Tổng của 2 **word** $u, v$ là $u + v mod 2^{32}$. Ví dụ ` 0xc0a8787e + 0x9fd1161d = 0x60798e9b`

Xor của 2 **word** $u, v$ là $u ⊕ v$. Cụ thể, nếu $u = \sum_{i} 2^i u_i$ và $v = \sum_{i} 2^i v_i$ thì $u ⊕ v = \sum_{i} 2^i (u_i+v_i-2u_iv_i)$. Ví dụ: `0xc0a8787e ⊕ 0x9fd1161d = 0x5f796e63`

Với mỗi $c \in$ { $0, 1, 2, 3,...$ }, phép quay trái c bit (c-bit left rotation) của một **word** $u$ được ký hiệu là $u <<< c$. Hay hiểu theo cách khác, nếu $u = \sum_{i} 2^i u_i$ thì $u <<< c = \sum_{i} 2^{i+c mod 32} u_i$. Ví dụ, `0xc0a8787e <<< 5 = 0x150f0fd8`.

## 3. Hàm `quarterround`
### input và output
Nếu $y$ là một chuỗi 4 **word** thì `quarterround(y)` là một chuỗi 4 **word**
### Định nghĩa
Nếu $y = (y_0, y_1, y_2, y_3)$ thì `quarterround(y)` -> $(z_0, z_1, z_2, z_3)$. Trong đó:
- $z_1 = y_1 ⊕ ((y_0 + y_3) <<< 7)$
- $z_2 = y_2 ⊕ ((z_1 + y_0) <<< 9)$
- $z_3 = y_3 ⊕ ((z_2 + z_1) <<< 13)$
- $z_0 = y_0 ⊕ ((z_3 + z_2) <<< 18)$

### Ví dụ
```
quarterround(0x00000000, 0x00000000, 0x00000000, 0x00000000) = (0x00000000, 0x00000000, 0x00000000, 0x00000000)
quarterround(0x00000001, 0x00000000, 0x00000000, 0x00000000) = (0x08008145, 0x00000080, 0x00010200, 0x20500000)
quarterround(0x00000000, 0x00000001, 0x00000000, 0x00000000) = (0x88000100, 0x00000001, 0x00000200, 0x00402000)
quarterround(0x00000000, 0x00000000, 0x00000001, 0x00000000) = (0x80040000, 0x00000000, 0x00000001, 0x00002000)
quarterround(0x00000000, 0x00000000, 0x00000000, 0x00000001) = (0x00048044, 0x00000080, 0x00010000, 0x20100001)
quarterround(0xe7e8c006, 0xc4f9417d, 0x6479b4b2, 0x68c67137) = (0xe876d72b, 0x9361dfd5, 0xf1460244, 0x948541a3)
quarterround(0xd3917c5b, 0x55f1c407, 0x52a58a7a, 0x8f887a3b) = (0x3e2f308c, 0xd90a8f36, 0x6ab2a923, 0x2883524c)
```

## 4. Hàm `rowround`
### input và output
Nếu $y$ là một chuỗi 16 **word** thì `rowround(y)` là một chuỗi 16 **word**

$$\left(\begin{array}{cc} 
y_0 & y_1 & y_2 & y_3 \\
y_4 & y_5 & y_6 & y_7 \\
y_8 & y_9 & y_{10} & y_{11} \\
y_{12} & y_{13} & y_{14} & y_{15}
\end{array}\right)$$

### Định nghĩa
Nếu $y = (y_0, y_1, y_2,..., y_{15})$ thì `rowround(y)` -> $(z_0, z_1, z_2,..., z_{15})$. Trong đó:
- $(z_0, z_1, z_2, z_3) = quarterround(y_0, y_1, y_2, y_3)$
- $(z_5, z_6, z_7, z_4) = quarterround(y_5, y_6, y_7, y_4)$
- $(z_{10}, z_{11}, z_8, z_9) = quarterround(y_{10}, y_{11}, y_8, y_9)$
- $(z_{15}, z_{12}, z_{13}, z_{14}) = quarterround(y_{15}, y_{12}, y_{13}, y_{14})$

Nhìn ma trận bên trên sẽ thấy được quy luật của nó :new_moon_with_face:

### Ví dụ
```
rowround(0x00000001, 0x00000000, 0x00000000, 0x00000000,
        0x00000001, 0x00000000, 0x00000000, 0x00000000,
        0x00000001, 0x00000000, 0x00000000, 0x00000000,
        0x00000001, 0x00000000, 0x00000000, 0x00000000)
= (0x08008145, 0x00000080, 0x00010200, 0x20500000,
  0x20100001, 0x00048044, 0x00000080, 0x00010000,
  0x00000001, 0x00002000, 0x80040000, 0x00000000,
  0x00000001, 0x00000200, 0x00402000, 0x88000100)
  
rowround(0x08521bd6, 0x1fe88837, 0xbb2aa576, 0x3aa26365,
        0xc54c6a5b, 0x2fc74c2f, 0x6dd39cc3, 0xda0a64f6,
        0x90a2f23d, 0x067f95a6, 0x06b35f61, 0x41e4732e,
        0xe859c100, 0xea4d84b7, 0x0f619bff, 0xbc6e965a)
= (0xa890d39d, 0x65d71596, 0xe9487daa, 0xc8ca6a86,
  0x949d2192, 0x764b7754, 0xe408d9b9, 0x7a41b4d1,
  0x3402e183, 0x3c3af432, 0x50669f96, 0xd89ef0a8,
  0x0040ede5, 0xb545fbce, 0xd257ed4f, 0x1818882d)
```

## 5. Hàm `columnround`
### input và output
Nếu $x$ là một chuỗi 16 **word** thì `columnround(x)` là một chuỗi 16 **word**
### Định nghĩa
Nếu $x = (x_0, x_1, x_2,..., x_{15})$ thì `columnround(y)` -> $(y_0, y_1, y_2,..., y_{15})$. Trong đó:
- $(y_0, y_4, y_8, y_{12}) = quarterround(x_0, x_4, x_8, x_{12})$
- $(y_5, y_9, y_{13}, y_1) = quarterround(x_5, x_9, x_{13}, x_1)$
- $(y_{10}, y_{14}, y_2, y_6) = quarterround(x_{10}, x_{14}, x_2, x_6)$
- $(y_{15}, y_3, y_7, y_{11}) = quarterround(x_{15}, x_3, x_7, x_{11})$
### Ví dụ
```
columnround(0x00000001, 0x00000000, 0x00000000, 0x00000000,
        0x00000001, 0x00000000, 0x00000000, 0x00000000,
        0x00000001, 0x00000000, 0x00000000, 0x00000000,
        0x00000001, 0x00000000, 0x00000000, 0x00000000)
= (0x10090288, 0x00000000, 0x00000000, 0x00000000,
0x00000101, 0x00000000, 0x00000000, 0x00000000,
0x00020401, 0x00000000, 0x00000000, 0x00000000,
0x40a04001, 0x00000000, 0x00000000, 0x00000000)

columnround(0x08521bd6, 0x1fe88837, 0xbb2aa576, 0x3aa26365,
        0xc54c6a5b, 0x2fc74c2f, 0x6dd39cc3, 0xda0a64f6,
        0x90a2f23d, 0x067f95a6, 0x06b35f61, 0x41e4732e,
        0xe859c100, 0xea4d84b7, 0x0f619bff, 0xbc6e965a)
= (0x8c9d190a, 0xce8e4c90, 0x1ef8e9d3, 0x1326a71a,
0x90a20123, 0xead3c4f3, 0x63a091a0, 0xf0708d69,
0x789b010c, 0xd195a681, 0xeb7d5504, 0xa774135c,
0x481c2027, 0x53a8e4b5, 0x4c1f89c5, 0x3f78c9c8)
```

## 6. Hàm `doubleround`
### input và output
Nếu $x$ là một chuỗi 16 **word** thì `columnround(x)` là một chuỗi 16 **word**
### Định nghĩa
Nghe tên là biết thực hiện round 2 lần :satisfied:. Vì vậy chính xác định nghĩa hàm này là
`doubleround(x) = rowround(columnround(x))`

Còn ví dụ thì nếu 2 hàm trên chính xác rồi, không cần test nữa :sunglasses:

## 7. Hàm `littleendian`
### input và output
Với b là mỗi chuỗi 4 byte, output của littleendian(b) là một **word**
### Định nghĩa
Nếu $b = (b_0, b_1, b_2, b_3)$ thì $littleendian(b) = b_0 + 2^8b_1 + 2^{16}b_2 + 2^{24}b_3$
### Ví dụ
```
littleendian(0, 0, 0, 0) = 0x00000000
littleendian(86, 75, 30, 9) = 0x091e4b56
littleendian(255, 255, 255, 250) = 0xfaffffff
```
## 8. Hàm `Salsa20` hash
### input và output
Nếu $x$ là một chuỗi 64 byte thì `Salsa20(x)` là một chuỗi 64 byte
### Định nghĩa
$Salsa20(x) = x + doubleround^{10}(x)$, trong đó mỗi chuỗi 4 byte được xem như một word ở dạng little-endian

Chi tiết: Bắt đầu từ $x = (x[0], x[1], ..., x[63])$, ta định nghĩa
- $x_0 = littleendian(x[0], x[1], x[2], x[3]),$
- $x_1 = littleendian(x[4], x[5], x[6], x[7]),$
- $x_2 = littleendian(x[8], x[9], x[10], x[11]),$
- $...$
- $x_{15} = littleendian(x[60], x[61], x[62], x[63])$

Ta định nghĩa $(z_0, z_1, ..., z_{15}) = doubleround^{10}(x_0, x_1, ..., x_{15})$. Thì `Salsa20(x)` là concatenate (nối) của:
- $littleendian^{−1}(z_0 + x_0),$
- $littleendian^{−1}(z_1 + x_1),$
- $littleendian^{−1}(z_2 + x_2),$
- $...$
- $littleendian^{−1}(z_{15} + x_{15})$

### Ví dụ
```
Salsa20(211,159, 13,115, 76, 55, 82,183,  3,117,222, 37,191,187,234,136,
         49,237,179, 48,  1,106,178,219,175,199,166, 48, 86, 16,179,207,
         31,240, 32, 63, 15, 83, 93,161,116,147, 48,113,238, 55,204, 36,
         79,201,235, 79,  3, 81,156, 47,203, 26,244,243, 88,118,104, 54)
= (109, 42,178,168,156,240,248,238,168,196,190,203, 26,110,170,154,
    29, 29,150, 26,150, 30,235,249,190,163,251, 48, 69,144, 51, 57,
   118, 40,152,157,180, 57, 27, 94,107, 42,236, 35, 27,111,114,114,
   219,236,232,135,111,155,110, 18, 24,232, 95,158,179, 19, 48,202)
   
Salsa20( 88,118,104, 54, 79,201,235, 79,  3, 81,156, 47,203, 26,244,243,
        191,187,234,136,211,159, 13,115, 76, 55, 82,183,  3,117,222, 37,
         86, 16,179,207, 49,237,179, 48,  1,106,178,219,175,199,166, 48,
        238, 55,204, 36, 31,240, 32, 63, 15, 83, 93,161,116,147, 48,113)
= (179, 19, 48,202,219,236,232,135,111,155,110, 18, 24,232, 95,158,
    26,110,170,154,109, 42,178,168,156,240,248,238,168,196,190,203,
    69,144, 51, 57, 29, 29,150, 26,150, 30,235,249,190,163,251, 48,
    27,111,114,114,118, 40,152,157,180, 57, 27, 94,107, 42,236, 35) 
```
## 9. Hàm `Salsa20` expansion
### input và output
Với $k$ là mỗi chuỗi 32 byte hoặc 16 byte, $n$ là một chuỗi 16 byte. $Salsa20_k(n)$ là một chuỗi 64 byte
### Định nghĩa
$σ_0 = (101, 120, 112, 97), σ_1 = (110, 100, 32, 51), σ_2 = (50, 45, 98, 121), σ_3 = (116, 101, 32, 107)$ nghĩa là “expand 16-byte k”. Nếu $k_0, k_1, n$ là chuỗi 16 byte, $Salsa20_{k_0,k_1}(n) = Salsa20(σ_0, k_0, σ_1, n, σ_2, k_1, σ_3)$

$τ_0 = (101, 120, 112, 97), τ_1 = (110, 100, 32, 49), τ_2 = (54, 45, 98, 121), τ_3 = (116, 101, 32, 107)$ nghĩa là “expand 32-byte k”. Nếu $k, n$ là chuỗi 16 byte, $Salsa20_k(n) = Salsa20(τ_0, k, τ_1, n, τ_2, k, τ_3)$

![image](https://user-images.githubusercontent.com/95759699/200103995-a721b020-5e39-4fe8-8f5a-8e584159139d.png)

### Ví dụ
Nếu $k_0 = (1, 2, 3, 4, 5, ..., 16), k_1 = (201, 202, 203, 204, 205, ..., 216), n = (101, 102, 103, 104, 105, ..., 116)$

$Salsa20_{k_0,k_1}(n) =$
```
Salsa20(101,120,112, 97,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12,
         13, 14, 15, 16,110,100, 32, 51,101,102,103,104,105,106,107,108,
        109,110,111,112,113,114,115,116, 50, 45, 98,121,201,202,203,204,
        205,206,207,208,209,210,211,212,213,214,215,216,116,101, 32,107)
= ( 69, 37, 68, 39, 41, 15,107,193,255,139,122,  6,170,233,217, 98,
    89,144,182,106, 21, 51,200, 65,239, 49,222, 34,215,114, 40,126,
   104,197,  7,225,197,153, 31,  2,102, 78, 76,176, 84,245,246,184,
   177,160,133,130,  6, 72,149,119,192,195,132,236,234,103,246, 74)
```
Và $Salsa20_{k_0}(n) =$
```
Salsa20(101,120,112, 97,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12,
         13, 14, 15, 16,110,100, 32, 49,101,102,103,104,105,106,107,108,
        109,110,111,112,113,114,115,116, 54, 45, 98,121,  1,  2,  3,  4,
          5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15, 16,116,101, 32,107)
= ( 39,173, 46,248, 30,200, 82, 17, 48, 67,254,239, 37, 18, 13,247,
   241,200, 61,144, 10, 55, 50,185,  6, 47,246,253,143, 86,187,225,
   134, 85,110,246,161,163, 43,235,231, 94,171, 51,145,214,112, 29,
    14,232,  5, 16,151,140,183,141,171,  9,122,181,104,182,177,193)
```
## 10. Hàm `Salsa20` encryption
### input và output
Với $k$ là một chuỗi 32 byte hoặc 16 byte. $m$ là một chuỗi $l$ byte với $m \in$ { $0, 1, ..., 2^{70}$ }. **Salsa20 encryption** của $m$ với **nonce** $v$ dưới **key** $k$ được ký hiệu là $Salsa20_k(v) ⊕ m$ là một chuỗi $l$ byte

Để dễ hiểu hơn:
- $k$ là **secret key** của chúng ta
- $v$ là một **nonce**, một message duy nhất
- $m$ là **plaintext**
- $Salsa20_k(v) ⊕ m$ là **ciphertext** message

### Định nghĩa
$Salsa20_k(v)$ là một chuỗi $2^{70}$ byte

$Salsa20_k(v, \bar{0}), Salsa20_k(v,\bar{1}), Salsa20_k(v, \bar{2}), ..., Salsa20_k(v,\bar{2^{64} − 1})$

Trong đó $\bar{i}$ là chuỗi 8 byte duy nhất $(i_0, i_1, ..., i_7)$ sao cho $i = i_0 + 2^8i_1 + 2^{16}i_2 + ... + 2^{56}i_7$

Công thức ra **ciphertext** $Salsa20_k(v) ⊕ m$ nghĩa là cắt ngắn $Salsa20_k(v)$ có cùng chiều dài với $m$. Nói cách khác,

$Salsa20_k(v) ⊕ (m[0], m[1], ..., m[l-1]) = (c[0], c[1], ..., c[l-1])$

Khi $c[i] = m[i] ⊕ Salsa20_k(v, \bar{int(i/64)})[i mod 64]$

Để dễ hiểu hơn :dizzy_face: thì cách tạo cipher text sẽ như hình dưới

![image](https://user-images.githubusercontent.com/95759699/200104685-63bace58-3628-4240-9374-ad7cc15092fa.png)

Thế là hết :smiley:
