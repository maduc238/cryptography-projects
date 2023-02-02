""" SHA1 """

def _left_rotate(n, b):
    return ((n << b) | (n >> (32 - b))) & 0xffffffff

H = [0x67452301,0xEFCDAB89,0x98BADCFE,0x10325476,0xC3D2E1F0]
A = H[0]
B = H[1]
C = H[2]
D = H[3]
E = H[4]
F = D ^ (B & (C ^ D))
print('F = %08x' % F)
K = 0x5A827999
W = 0x00000000

temp = _left_rotate(A, 5) + F + E + K + W
print('A <<< 5 = %08x' % _left_rotate(A, 5))
E = D
D = C
C = _left_rotate(B, 30)
B = A
A = temp & 0xffffffff

H[0] = (H[0] + A) & 0xffffffff
H[1] = (H[1] + B) & 0xffffffff
H[2] = (H[2] + C) & 0xffffffff
H[3] = (H[3] + D) & 0xffffffff
H[4] = (H[4] + E) & 0xffffffff

for i in range(5):
    print(f'H{i} = %08x' % H[i])
