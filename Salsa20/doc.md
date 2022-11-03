# Salsa20

## 1. Giới thiệu

Salsa20 là một hàm băm với input là 64 byte và output là 64 byte. Hàm băm được sử dụng trong chế độ bộ đếm như một mã dòng: Salsa20 mã hóa khối 64 byte plaintext bằng cách băm key, số nonce, và số block và thực hiện xor với plaintext.

Phần dưới đây sẽ nói chi tiết vầ Salsa20, bắt đầu từ ba thao tác đơn giản trên các từ 4 byte, sau đó thông qua hàm băm Salsa20 và kết thúc với hàm encryption.

Một byte là một phần tử của {0, 1, ..., 255}

## 2. word

Một **word** là một phần tử trong tập { $0, 1, ..., 2^{32}-1$ }. các **word** trong tài liệu này sẽ thường được viết bằng hexa, bao gồm cả ký tự `0x`. Ví dụ, `0xc0a8787e` = $12 · 2^{28} + 0  2^{24} + 10 · 2^{20} + 8 · 2^{16} + 7 · 2^{12} + 8 · 2^8 + 7 · 2^4 + 14 · 2^0 = 3232266366$

Tổng của 2 **word** $u, v$ là $u + v mod 2^{32}$. Ví dụ ` 0xc0a8787e + 0x9fd1161d = 0x60798e9b`

Xor của 2 **word** $u, v$ là $u ⊕ v$. Cụ thể, nếu $u = \sum_{i} 2^i u_i$ và $v = \sum_{i} 2^i v_i$ thì $u ⊕ v = \sum_{i} 2^i (u_i+v_i-2u_iv_i)$. Ví dụ: `0xc0a8787e ⊕ 0x9fd1161d = 0x5f796e63`

Với mỗi $c \in$ { $0, 1, 2, 3,...$ }, phép quay trái bit của một **word** $u$ được ký hiệu là $u <<< c$. Hay hiểu theo cách khác, nếu $u = \sum_{i} 2^i u_i$ thì $u <<< c = \sum_{i} 2^{i+c mod 32} u_i$. Ví dụ, `0xc0a8787e <<< 5 = 0x150f0fd8`.

## 3. Hàm `phần tư` (`quarterround` function)
### Các input và output
Nếu $y$ là một chuỗi 4 **word** thì `quarterround(y)` là một chuỗi 4 **word**
### Định nghĩa
Nếu $y = (y_0, y_1, y_2, y_3)$ thì `quarterround(y)` -> $(z_0, z_1, z_2, z_3)$. Trong đó:
- $z_1 = y_1 ⊕ ((y_0 + y_3) <<< 7)$
- $z_2 = y_2 ⊕ ((z_1 + y_0) <<< 9)$
- $z_3 = y_3 ⊕ ((z_2 + z_1) <<< 13)$
- $z_0 = y_0 ⊕ ((z_3 + z_2) <<< 18)$

Ví dụ:
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
### Các input và output
Nếu $y$ là một chuỗi 16 **word** thì `rowround(y)` là một chuỗi 16 **word**
### Định nghĩa
Nếu $y = (y_0, y_1, y_2,..., y_15)$ thì `rowround(y)` -> $(z_0, z_1, z_2,..., z_15)$. Trong đó:
- $(z_0, z_1, z_2, z_3) = quarterround(y_0, y_1, y_2, y_3)$
- $(z_5, z_6, z_7, z_4) = quarterround(y_5, y_6, y_7, y_4)$
- $(z_{10}, z_{11}, z_8, z_9) = quarterround(y_{10}, y_{11}, y_8, y_9)$
- $(z_{15}, z_{12}, z_{13}, z_{14}) = quarterround(y_{15}, y_{12}, y_{13}, y_{14})$
