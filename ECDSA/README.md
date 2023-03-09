# Thuật toán chữ ký số Elliptic Curve Digital Signature Algorithm

## Mã hóa và giải mã bằng Elliptic Curve

Với Private ket là $d$, Public key là $Q$: $Q=[d]G$

Mã hóa plaintext $P_m$ được ciphertext:

$$C_{m} =\{[ k] G,\ P_{m} +[ k] Q\}$$

Giải mã được plaintext:

$$B-[ d] A=P_{m} +[ k] Q-[ d\cdotp k] G=P_{m} +[ k\cdotp d] G-[ d\cdotp k] G=P_{m}$$

## Thuật toán chữ ký số Elliptic Curve

Tạo chữ ký số:

![ECDSA1](https://user-images.githubusercontent.com/95759699/223955733-2e0bbf53-1668-4c51-83f6-707dbef354fe.png)

Xác thực chữ ký số:

![ECDSA2](https://user-images.githubusercontent.com/95759699/223955776-76b64f54-74d6-437d-a6b0-2b4ab77c22a2.png)
