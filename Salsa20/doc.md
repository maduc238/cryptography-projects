# Salsa20

## 1. Giới thiệu

Salsa20 là một hàm băm với input là 64 byte và output là 64 byte. Hàm băm được sử dụng trong chế độ bộ đếm như một mã dòng: Salsa20 mã hóa khối 64 byte plaintext bằng cách băm key, số nonce, và số block và thực hiện xor với plaintext.

Phần dưới đây sẽ nói chi tiết vầ Salsa20, bắt đầu từ ba thao tác đơn giản trên các từ 4 byte, sau đó thông qua hàm băm Salsa20 và kết thúc với hàm encryption.

Một byte là một phần tử của {0, 1, ..., 255}

## 2. word

Một **word** là một phần tử của { $0, 1, ..., 2^{32}-1$ }. các **word** trong tài liệu này sẽ thường được viết bằng hexa, bao gồm cả ký tự `0x`. Ví dụ, `0xc0a8787e` = $12 · 2^{28} + 0  2^{24} + 10 · 2^{20} + 8 · 2^{16} + 7 · 2^{12} + 8 · 2^8 + 7 · 2^4 + 14 · 2^0 = 3232266366$

Tổng của 2 **word** $u, v$ là $u + v mod 2^{32}$. Ví dụ ` 0xc0a8787e + 0x9fd1161d = 0x60798e9b`

Xor của 2 **word** $u, v$ là $u ⊕ v$. Cụ thể, nếu $u = sum_(i) 2^i u_i$ và $v = sum_(i) 2^i v_i$ thì $u ⊕ v = sum_i 2^i (u_i+v_i-2u_iv_i)$. Ví dụ: `0xc0a8787e ⊕ 0x9fd1161d = 0x5f796e63`

Với mỗi 
