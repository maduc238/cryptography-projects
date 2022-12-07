# AES-128
References: [Announcing the ADVANCED ENCRYPTION STANDARD (AES)](https://nvlpubs.nist.gov/nistpubs/fips/nist.fips.197.pdf)
## 1. Giới thiệu chung
Chuẩn mã hóa AES - ADVANCED ENCRYPTION STANDARD là một loại mã khối để xử lý dữ liệu đầu vào **128 bit**, cùng với khóa **key** có các kiểu độ dài là **128**, **192**, và **256 bit** tương ứng các chuẩn của mã hóa này: ***“AES-128”***, ***“AES-192”***, và ***“AES-256”***

Còn các định nghĩa về toán học, những cái đã đề cập trong mã hóa [Salsa20](https://github.com/maduc238/cryptography-projects/tree/main/Salsa20) mình sẽ bỏ qua, còn giờ chỉ nói một vài ký hiệu mới thôi :stuck_out_tongue_winking_eye:
### 1.1. Phép nhân với x
Nhân đa thức nhị phân với đa thức $x$:

$b_7 x^8 + b_6 x^7 + b_5 x^6 + b_4 x^5 + b_3 x^4 + b_2 x^3 + b_1 x^2 + b_0 x$

Với $b_i$ là bit vị trí thứ $i$. Phép nhân đa thức này đã học trong môn Cơ sở truyền tin. Và việc nhân này áp dụng cho cả 2 byte. Sau đó lấy dư với 0x11b nếu như tràn giá trị.

Ví dụ phép tính {57} • {13}. Để dễ tính toán hơn thì khai triển ra theo cách này:
- {57} • {02} = {ae}
- {57} • {04} = {47}
- {57} • {08} = {8e}
- {57} • {10} = {07}

Do đó: {57} • {13} = {57} • ({01} ⊕ {02} ⊕ {10}) = {57} ⊕ {ae} ⊕ {07} = {fe}

### 1.2. Một số ký hiệu
- **Nk**: Số word trong key
- **Nr**: Số vòng (round) thực hiện
- **Nb**: Số cột byte khi xét trong **State**, với chuẩn này thì Nb=4

| # | Nk | Nb | Nr |
| --- | --- | --- | --- |
| **AES-128** | 4 | 4 | 10 |
| **AES-192** | 6 | 4 | 12 |
| **AES-256** | 8 | 4 | 14 |

- **State**: Dữ liệu trung gian được lưu dưới dạng ma trận các byte, có bốn hàng và **Nb** cột

![image](https://user-images.githubusercontent.com/95759699/203812172-3c47710b-678a-441f-9470-f248a46db03c.png)

## 2. Các hàm sử dụng trong mã hóa
Mã khối này được thực hiện thông qua 5 hàm chính là **AddRoundKey()**, **SubBytes()**, **ShiftRows()**, **MixColumns()** và **KeyExpansion()** sẽ được nói chi tiết trong các phần sau.
### 2.1. AddRoundKey()
Trong việc sử dụng hàm biến đổi này, một Round Key được thêm vào State bởi phép cộng XOR theo bit. Mỗi Round Key bao gồm Nb word trong key schedule. Mỗi Nb word đó được thêm vào cột của State sao cho:

$[s_{0,c}', s_{1,c}', s_{2,c}', s_{3,c}']=[s_{0,c}, s_{1,c}, s_{2,c}, s_{3,c}]⊕[w_{round*Nb+c}]$

Trong đó [ $w_i$ ] là key schedule word, và $round$ là một giá trị nằm trong 0 ≤ $round$ ≤ Nr.

![image](https://user-images.githubusercontent.com/95759699/203766667-389dc5f8-db2b-44ce-a526-8d1a99b441b7.png)

Đơn giản hơn thì thực chất chỉ là phép **XOR 16 byte với 16 byte**. Ma trận bên phải sẽ lần lượt XOR với ma trận giữa theo từng cột sẽ ra được ma trận bên trái. Vì vậy đầu ra cũng chỉ là 16 byte. Hàm này thường được sử dụng với **Key** và các **Key Expansion**

### 2.2. SubBytes()
Đây là hàm biến đổi, nó sẽ thay thế từng byte của State bằng cách sử dụng S-box. Hiểu đơn giản S-box là một bảng tra cứu, từ đầu vào ta tra từ hàng và cột sẽ ra kết quả tương ứng. Được cái S-box này không thể bị đảo được, và xây dựng bằng cách kết hợp hai phép biến đổi:
- Lấy nghịch đảo bit đó trong trường **GF( $2^8$ )**; giá trị {00} thì ánh xạ tới chính nó
- Áp dụng phép biến đổi affine (trên GF(2)):
  + Với 0 ≤ i < 8, $b_i$ là bit thứ i của một byte, $c_i$ là bit thứ i của byte c với giá trị {63} hay {01100011}
  + $b_i'=b_i⊕b_{(i+4)mod8}⊕b_{(i+5)mod8}⊕b_{(i+6)mod8}⊕b_{(i+7)mod8}⊕c_i$

Dưới dạng ma trận, biến đổi affine của S-box có thể được biểu diễn:

![image](https://user-images.githubusercontent.com/95759699/203814451-1e9466e5-f552-494c-ab0b-f0df7d29a1b4.png)

Và cuối cùng sau phép biến đổi toán học, S-box cụ thể sẽ có những giá trị như sau:
![image](https://user-images.githubusercontent.com/95759699/203770811-505b5f22-853a-4a82-9055-e247a00853e1.png)

Giải thích cách dùng: Nếu một byte dạng hex là $s_{1,1}$={5,3} thì ta tra số ***5*** tương ứng với hàng (nghĩa là $x$) và số ***3*** tương ứng với cột (nghĩa là $y$), khi dò bảng thì kết quả thu được là {ed}. Nghe khá dễ dúng không :smiley:

Vậy tựu chung lại, hàm **SubBytes()** chỉ đơn giản là phép biến đối byte từ S-box thôi!
![image](https://user-images.githubusercontent.com/95759699/203771597-98ef8fc1-eece-4674-90f1-7ae6d9570484.png)

### 2.3. ShiftRows()
Là hàm biến đổi, trong đó mỗi hàng tương ứng với 4 byte sẽ bị quay trái theo byte tương ứng với quy luật tăng dần theo index của hàng. Biểu diễn quá trình này sẽ được thể hiện như sau:

$s_{r,c}'=s_{r,c+shift(r,Nb)modNb}$ với 0 < r < 4 và 0 ≤ c ≤ Nb

Có nghĩa là hàng thứ bao nhiêu trong ma trận dữ liệu 128 bit (tính từ hàng số 0) thì nó sẽ bị quay trái bằng đấy lần

Cho dễ hiểu hơn về hàm **ShiftRows()** thì hình sau sẽ là input và output của biến đổi này
![image](https://user-images.githubusercontent.com/95759699/203773151-707ca86d-48b5-41c7-aa57-c48dbe879b06.png)

Tại ma trận bên trái, dãy 4 byte trên hàng đầu tiên vẫn giữ nguyên (vì index của hàng bằng 0), rồi hàng thứ hai thì bị xoay trái 1 byte, tương ứng phần bôi đen bị chuyển sang bên phải, rồi hàng thứ ba bị xoay 2 byte, hàng thứ tư bị xoay 3 byte.

### 2.4. MixColumns()
Hàm biến đổi này hoạt động trên từng cột State, coi mỗi cột là một đa thức bậc bốn. Việc xây dựng này dựa trên việc coi các cột là đa thức trên GF( $2^8$ ) và nhân modulo $x^4+1$ với một đa thức $a(x)=${03} $x^3+$ {01} $x^2+$ {01} $x+$ {02}.

Với từng cột trên ma trận 16 byte ta có phép tính ra kết quả của hàm **MixColumns()** bằng việc nhân ma trận:

$$
\begin{pmatrix}
s_{0,c}' \\
s_{1,c}' \\
s_{2,c}' \\
s_{3,c}'
\end{pmatrix}
\=
\begin{pmatrix}
02 & 03 & 01 & 01 \\
01 & 02 & 03 & 01 \\
01 & 01 & 02 & 03\\
03 & 01 & 01 & 02 \\
\end{pmatrix}
\begin{pmatrix}
s_{0,c} \\
s_{1,c} \\
s_{2,c} \\
s_{3,c}
\end{pmatrix}
$$

Để dễ hình dung hơn thì hàm **MixColumns()** thực hiện biến đổi với từng cột từ việc nhân ma trận trước đó như sau:
![image](https://user-images.githubusercontent.com/95759699/203778337-b7179f17-9433-4a31-b1a6-2f53afcd61fb.png)

### 2.5. Key Expansion
Nghe tên là biết chỉ áp dụng biến đổi cho khóa **K** và mở rộng nó, vì vậy đầu vào của hàm này luôn là khóa K (trường trường hợp AES-128). Mục địch chính là mở rộng cái khóa này và sẽ kết hợp với hàm **AddRoundKey()** (chi tiết cụ thể sẽ nói sau). **Key Expansion** tạo ra tổng cộng **Nb**(**Nr** + 1) word: Thuật toán yêu cầu một bộ khởi tạo Nb word, mỗi round Nr yêu cầu Nb word dữ liẹu key. Kết quả của key schedule bao gồm một mảng gồm các word, ký hiệu là [ $w_i$ ], với i trong khoảng 0 ≤ i ≤ **Nb**(**Nr** + 1). Trong thuật toán bên dưới sẽ được lưu trong mảng `w[i]`

**KeyExpansion()** yêu cầu một số hàm nhỏ như **RotWord()**, **SubWord()**, và phần tử mảng con **Rcon[i]**
#### SubWord()
Là hàm lấy một word và áp dụng S-box để tạo ra một output word.
#### RotWord()
Lấy input là một word dạng [ $a_0,a_1,a_2,a_3$ ], trả về kết quả một word dạng [ $a_1,a_2,a_3,a_0$ ]
#### Rcon[i]
Với mỗi round chứa một mảng word, **Rcon[i]** chứa các giá trị [ $x^{i-1}$, {00}, {00}, {00}], với $x^{i-1}$ là lũy thừa bậc i-1 của x (x được đặt là {02}) trong field GF( $2^8$ ), i bắt đầu từ 1. Giá trị đầu này tạm được lưu trong mảng GC[j], vì vậy sẽ có các giá trị:

| j | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 | 10 |
| --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- |
| RC[j] | 01 | 02 | 04 | 08 | 10 | 20 | 40 | 80 | 1b | 36 |

#### Chi tiết thuật toán
Với ***AES-128***, **Nk**=4
```Python
def KeyExpansion(byte key[4*Nk], word w[Nb*(Nr+1)], Nk):
  word temp
  i = 0
  while (i < Nk):
    w[i] = word(key[4*i], key[4*i+1], key[4*i+2], key[4*i+3])
    # Nghĩa là w tại round đầu tiên chính là key
    i = i+1
  
  i = Nk
  while (i < Nb * (Nr+1)):
    temp = w[i-1]
    if (i mod Nk = 0):
      temp = SubWord(RotWord(temp)) xor Rcon[i/Nk]
    else if (Nk > 6 and i mod Nk = 4):
      temp = SubWord(temp)
    w[i] = w[i-Nk] xor temp
    i = i + 1
```
![image](https://user-images.githubusercontent.com/95759699/203910066-0d900440-3e91-42b6-addf-dc3d3c1f339f.png)

***Tóm tắt:*** Từ 16 byte của một key ban đầu, gộp các hàng thành một mảng gồm 4 word. Đầu tiên trong một round, lấy word cuối cùng nghĩa là cột ngoài cùng bên phải (4 byte cuối cùng trong key), đưa nó vào trong hàm gọi là $g$ như hình - lệnh `if` đầu tiên trong thuật toán. Trong hàm $g$ này sẽ thực hiện hàm **RotWord()** -> **SubWord()** -> cộng XOR kết quả đó với **Rcon[]** để ra một word mới gọi là w'. Từ w' này kết hợp với w0 (cột đầu tiên) để ra w4 tương ứng với hàng đầu tiên trong Key mở rộng cho round tiếp theo. Còn các cột khác trong key, việc tạo ra word mới (cột mới) chỉ là lấy word vừa mới tạo trước XOR với word cùng cột với mình. Lặp lại kết quả này trong **Nr** lần.

Kết quả thu được `w[Nb*(Nr+1)]` để phục vụ cho việc mã hóa
## 3. Mã hóa
```Python
def Cipher(byte in[4*Nb], byte out[4*Nb], word w[Nb*(Nr+1)]):
  byte state[4,Nb]
  state = in
  AddRoundKey(state, w[0, Nb-1])
  
  for round = 1 step 1 to Nr–1:
    SubBytes(state)
    ShiftRows(state)
    MixColumns(state)
    AddRoundKey(state, w[round*Nb, (round+1)*Nb-1])
    
  SubBytes(state)
  ShiftRows(state)
  AddRoundKey(state, w[Nr*Nb, (Nr+1)*Nb-1])
  
  out = state
```
Để dễ hình dung hơn có thể tham khảo hình bên dưới. Dễ hiểu thế này chắc không cần giải thích tóm tắt nữa :sweat_smile:

![image](https://user-images.githubusercontent.com/95759699/203821765-339ec186-0e95-4e84-b540-a90c98022822.png)
## 4. Các hàm sử dụng trong giải mã
### 4.1. InvShiftRows()
Hàm ngược của **ShiftRows()**, cách làm là chỉ cần xoay ngược lại - xoay phải các byte theo index của hàng.
![image](https://user-images.githubusercontent.com/95759699/203921241-63c8a79c-d19a-41db-93dd-be8fd6e99567.png)
### 4.2. InvSubBytes()
Hàm ngược của **SubBytes()**, nghĩa là tạo bẳng Inverse S-box trong GF( $2^8$ ) và tra kết quả. 
![image](https://user-images.githubusercontent.com/95759699/203921532-ddde4d54-5b48-4b39-89b9-3d4d6e3e065c.png)
### 4.3. InvMixColumns()
Hàm ngược của **MixColumns()**, nên là nhân modulo $x^4 + 1$ nên thu được đa thức:
$a^{-1}(x)=${0b} $x^3+$ {0d} $x^2+$ {09} $x+$ {0e}

Viết dưới dạng ma trận:

$$
\begin{pmatrix}
s_{0,c}' \\
s_{1,c}' \\
s_{2,c}' \\
s_{3,c}'
\end{pmatrix}
\=
\begin{pmatrix}
0e & 0b & 0d & 09 \\
09 & 0e & 0b & 0d\\
0d & 09 & 0e & 0b \\
0b & 0d & 09 & 0e \\
\end{pmatrix}
\begin{pmatrix}
s_{0,c} \\
s_{1,c} \\
s_{2,c} \\
s_{3,c}
\end{pmatrix}
$$

## 5. Giải mã
```Python
EqInvCipher(byte in[4*Nb], byte out[4*Nb], word dw[Nb*(Nr+1)]):
  byte state[4,Nb]
  state = in
  AddRoundKey(state, dw[Nr*Nb, (Nr+1)*Nb-1])
  
  for round = Nr-1 step -1 downto 1:
    InvShiftRows(state)
    InvSubBytes(state)
    AddRoundKey(state, dw[round*Nb, (round+1)*Nb-1])
    InvMixColumns(state)
    
  InvShiftRows(state)
  InvSubBytes(state)
  AddRoundKey(state, dw[0, Nb-1])
  out = state
```
Lưu ý:
- `dw` vẫn được tạo từ S-box xuôi.
- Có nhiều cách khác nhau để giải được mã, không nhất thiết phải theo các bước này.

![image](https://user-images.githubusercontent.com/95759699/203981521-a9c63237-6007-4142-823e-307cde4b8cc0.png)
