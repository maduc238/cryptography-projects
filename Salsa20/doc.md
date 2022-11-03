# Salsa20

## 1. Giới thiệu

**Salsa20** là một hàm băm với input là 64 byte và output là 64 byte. Hàm băm được sử dụng trong chế độ bộ đếm như một mã dòng: **Salsa20** mã hóa khối 64 byte plaintext bằng cách băm key, số nonce, và số block và thực hiện xor với plaintext. Trước đó ta phải định nghĩa một byte là một phần tử của {0, 1, ..., 255}

Phần dưới đây sẽ nói chi tiết vầ Salsa20, bắt đầu từ ba thao tác đơn giản trên các từ 4 byte, sau đó thông qua hàm băm **Salsa20** và kết thúc với hàm encryption. Nghe có vẻ khó khăn :sweat_smile: nhưng các bước dưới dây sẽ nói rõ một số hàm cụ thể và có cái nhìn khái quát dễ hiểu hơn.

## 2. word

Một **word** là một phần tử trong tập { $0, 1, ..., 2^{32}-1$ }. các **word** trong tài liệu này sẽ thường được viết bằng hexa, bao gồm cả ký tự `0x`. Ví dụ, `0xc0a8787e` = $12 · 2^{28} + 0  2^{24} + 10 · 2^{20} + 8 · 2^{16} + 7 · 2^{12} + 8 · 2^8 + 7 · 2^4 + 14 · 2^0 = 3232266366$

Tổng của 2 **word** $u, v$ là $u + v mod 2^{32}$. Ví dụ ` 0xc0a8787e + 0x9fd1161d = 0x60798e9b`

Xor của 2 **word** $u, v$ là $u ⊕ v$. Cụ thể, nếu $u = \sum_{i} 2^i u_i$ và $v = \sum_{i} 2^i v_i$ thì $u ⊕ v = \sum_{i} 2^i (u_i+v_i-2u_iv_i)$. Ví dụ: `0xc0a8787e ⊕ 0x9fd1161d = 0x5f796e63`

Với mỗi $c \in$ { $0, 1, 2, 3,...$ }, phép quay trái bit của một **word** $u$ được ký hiệu là $u <<< c$. Hay hiểu theo cách khác, nếu $u = \sum_{i} 2^i u_i$ thì $u <<< c = \sum_{i} 2^{i+c mod 32} u_i$. Ví dụ, `0xc0a8787e <<< 5 = 0x150f0fd8`.

## 3. Hàm `quarterround`
### input và output
Nếu $y$ là một chuỗi 4 **word** thì `quarterround(y)` là một chuỗi 4 **word**
### Định nghĩa
Nếu $y = (y_0, y_1, y_2, y_3)$ thì `quarterround(y)` -> $(z_0, z_1, z_2, z_3)$. Trong đó:
- $z_1 = y_1 ⊕ ((y_0 + y_3) <<< 7)$
- $z_2 = y_2 ⊕ ((z_1 + y_0) <<< 9)$
- $z_3 = y_3 ⊕ ((z_2 + z_1) <<< 13)$
- $z_0 = y_0 ⊕ ((z_3 + z_2) <<< 18)$
Nghe vài bước đầu có vẻ khó khăn nhưng làm nhiều sẽ quen thôi :grinning:
### Ví dụ
```
quarterround(0x00000000; 0x00000000; 0x00000000; 0x00000000) = (0x00000000; 0x00000000; 0x00000000; 0x00000000)
quarterround(0x00000001; 0x00000000; 0x00000000; 0x00000000) = (0x08008145; 0x00000080; 0x00010200; 0x20500000)
quarterround(0x00000000; 0x00000001; 0x00000000; 0x00000000) = (0x88000100; 0x00000001; 0x00000200; 0x00402000)
quarterround(0x00000000; 0x00000000; 0x00000001; 0x00000000) = (0x80040000; 0x00000000; 0x00000001; 0x00002000)
quarterround(0x00000000; 0x00000000; 0x00000000; 0x00000001) = (0x00048044; 0x00000080; 0x00010000; 0x20100001)
quarterround(0xe7e8c006; 0xc4f9417d; 0x6479b4b2; 0x68c67137) = (0xe876d72b; 0x9361dfd5; 0xf1460244; 0x948541a3)
quarterround(0xd3917c5b; 0x55f1c407; 0x52a58a7a; 0x8f887a3b) = (0x3e2f308c; 0xd90a8f36; 0x6ab2a923; 0x2883524c)
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
### Ví dụ
```
rowround(0x00000001; 0x00000000; 0x00000000; 0x00000000;
        0x00000001; 0x00000000; 0x00000000; 0x00000000;
        0x00000001; 0x00000000; 0x00000000; 0x00000000;
        0x00000001; 0x00000000; 0x00000000; 0x00000000)
= (0x08008145; 0x00000080; 0x00010200; 0x20500000;
  0x20100001; 0x00048044; 0x00000080; 0x00010000;
  0x00000001; 0x00002000; 0x80040000; 0x00000000;
  0x00000001; 0x00000200; 0x00402000; 0x88000100)
  
rowround(0x08521bd6; 0x1fe88837; 0xbb2aa576; 0x3aa26365;
        0xc54c6a5b; 0x2fc74c2f; 0x6dd39cc3; 0xda0a64f6;
        0x90a2f23d; 0x067f95a6; 0x06b35f61; 0x41e4732e;
        0xe859c100; 0xea4d84b7; 0x0f619bff; 0xbc6e965a)
= (0xa890d39d; 0x65d71596; 0xe9487daa; 0xc8ca6a86;
  0x949d2192; 0x764b7754; 0xe408d9b9; 0x7a41b4d1;
  0x3402e183; 0x3c3af432; 0x50669f96; 0xd89ef0a8;
  0x0040ede5; 0xb545fbce; 0xd257ed4f; 0x1818882d)
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
columnround(0x00000001; 0x00000000; 0x00000000; 0x00000000;
        0x00000001; 0x00000000; 0x00000000; 0x00000000;
        0x00000001; 0x00000000; 0x00000000; 0x00000000;
        0x00000001; 0x00000000; 0x00000000; 0x00000000)
= (0x10090288; 0x00000000; 0x00000000; 0x00000000;
0x00000101; 0x00000000; 0x00000000; 0x00000000;
0x00020401; 0x00000000; 0x00000000; 0x00000000;
0x40a04001; 0x00000000; 0x00000000; 0x00000000)

columnround(0x08521bd6; 0x1fe88837; 0xbb2aa576; 0x3aa26365;
        0xc54c6a5b; 0x2fc74c2f; 0x6dd39cc3; 0xda0a64f6;
        0x90a2f23d; 0x067f95a6; 0x06b35f61; 0x41e4732e;
        0xe859c100; 0xea4d84b7; 0x0f619bff; 0xbc6e965a)
= (0x8c9d190a; 0xce8e4c90; 0x1ef8e9d3; 0x1326a71a;
0x90a20123; 0xead3c4f3; 0x63a091a0; 0xf0708d69;
0x789b010c; 0xd195a681; 0xeb7d5504; 0xa774135c;
0x481c2027; 0x53a8e4b5; 0x4c1f89c5; 0x3f78c9c8)
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
