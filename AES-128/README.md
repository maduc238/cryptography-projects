# AES-128
References: [Announcing the ADVANCED ENCRYPTION STANDARD (AES)](https://nvlpubs.nist.gov/nistpubs/fips/nist.fips.197.pdf)
## Giới thiệu chung
Chuẩn mã hóa AES - ADVANCED ENCRYPTION STANDARD là một loại mã khối để xử lý dữ liệu đầu vào **128 bit**, cùng với khóa **key** có các kiểu độ dài là **128**, **192**, và **256 bit** tương ứng các chuẩn của mã hóa này: ***“AES-128”***, ***“AES-192”***, và ***“AES-256”***

Mã khối này được thực hiện thông qua 5 hàm chính là **AddRoundKey()**, **SubBytes()**, **ShiftRows()**, **MixColumns()** và **KeyExpansion()** sẽ được nói chi tiết trong các phần sau.

Còn các định nghĩa về toán học, những cái đã đề cập trong mã hóa [Salsa20](https://github.com/maduc238/cryptography-projects/tree/main/Salsa20) mình sẽ bỏ qua, còn giờ chỉ nói một vài ký hiệu mới thôi :stuck_out_tongue_winking_eye:
### Phép nhân với x
Nhân đa thức nhị phân với đa thức $x$:

$b_7 x^8 + b_6 x^7 + b_5 x^6 + b_4 x^5 + b_3 x^4 + b_2 x^3 + b_1 x^2 + b_0 x$

Với $b_i$ là bit vị trí thứ $i$. Phép nhân đa thức này đã học trong môn Cơ sở truyền tin. Và việc nhân này áp dụng cho cả 2 byte

Ví dụ phép tính {57} • {13}. Để dễ tính toán hơn thì khai triển ra theo cách này:
- {57} • {02} = {ae}
- {57} • {04} = {47}
- {57} • {08} = {8e}
- {57} • {10} = {07}

Do đó: {57} • {13} = {57} • ({01} ⊕ {02} ⊕ {10}) = {57} ⊕ {ae} ⊕ {07} = {fe}

### Một số ký hiệu
...

## Các hàm sử dụng
### AddRoundKey()
Trong việc sử dụng hàm biến đổi này, một Round Key được thêm vào State bởi phép cộng XOR theo bit. Mỗi Round Key bao gồm Nb word trong key schedule. Mỗi Nb word đó được thêm vào cột của State sao cho:

$[s_{0,c}', s_{1,c}', s_{2,c}', s_{3,c}']=[s_{0,c}, s_{1,c}, s_{2,c}, s_{3,c}]⊕[w_{round*Nb+c}]$

Trong đó [$w_i$] là key schedule word, và $round$ là một giá trị nằm trong 0 ≤ $round$ ≤ Nr.

![image](https://user-images.githubusercontent.com/95759699/203766667-389dc5f8-db2b-44ce-a526-8d1a99b441b7.png)

Đơn giản hơn thì thực chất chỉ là phép **XOR 16 word với 16 word thôi**. Ma trận bên phải sẽ lần lượt XOR với ma trận giữa theo từng cột sẽ ra được ma trận bên trái. Vì vậy đầu ra cũng chỉ là 16 word.

### SubBytes()
Đây là hàm biến đổi, nó sẽ thay thế từng byte của State bằng cách sử dụng S-box. Hiểu đơn giản S-box là một bảng tra cứu, từ đầu vào ta tra từ hàng và cột sẽ ra kết quả tương ứng. Được cái S-box này không thể bị đảo được, và xây dựng bằng cách kết hợp hai phép biến đổi:
- Lấy nghịch đảo bit đó trong trường **GF(28)** (ai không nhớ về field thì học lại mở đầu chương này :v); giá trị {00} thì ánh xạ tới chính nó
- Áp dụng phép biến đổi affine (trên GF(2)):
  + Với 0 ≤ i < 8, $b_i$ là bit thứ i của một byte, $c_i$ là bit thứ i của byte c với giá trị {63} hay {01100011}
  + $b_i'=b_i⊕b_{(i+4)mod8}⊕b_{(i+5)mod8}⊕b_{(i+6)mod8}⊕b_{(i+7)mod8}⊕c_i$

Dưới dạng ma trận, biến đổi affine của S-box có thể được biểu diễn:
![image](https://user-images.githubusercontent.com/95759699/203770568-19f2703e-7b4f-4f48-ab22-ba549cbb6ff2.png)

Và cuối cùng sau phép biến đổi toán học, S-box cụ thể sẽ có những giá trị như sau:
![image](https://user-images.githubusercontent.com/95759699/203770811-505b5f22-853a-4a82-9055-e247a00853e1.png)

Giải thích cách dùng: Nếu một byte dạng hex là $s_{1,1}$={5,3} thì ta tra số ***5*** tương ứng với hàng (nghĩa là $x$) và số ***3*** tương ứng với cột (nghĩa là $y$), khi dò bảng thì kết quả thu được là {ed}. Nghe khá dễ dúng không :smiley:

Vậy tựu chung lại, hàm **SubBytes()** chỉ đơn giản là phép biến đối byte từ S-box thôi!
![image](https://user-images.githubusercontent.com/95759699/203771597-98ef8fc1-eece-4674-90f1-7ae6d9570484.png)

## ShiftRows()
Là hàm biến đổi, trong đó mỗi hàng tương ứng với 4 byte sẽ bị quay trái theo byte tương ứng với quy luật tăng dần theo index của hàng. Biểu diễn quá trình này sẽ được thể hiện như sau:

$s_{r,c}'=s_{r,c+shift(r,Nb)modNb}$ với 0 < r < 4 và 0 ≤ c ≤ Nb

Có nghĩa là hàng thứ bao nhiêu trong ma trận dữ liệu 128 bit (tính từ hàng số 0) thì nó sẽ bị quay trái bằng đấy lần

Cho dễ hiểu hơn về hàm **ShiftRows()** thì hình sau sẽ là input và output của biến đổi này
![image](https://user-images.githubusercontent.com/95759699/203773151-707ca86d-48b5-41c7-aa57-c48dbe879b06.png)

Tại ma trận bên trái, dãy 4 byte trên hàng đầu tiên vẫn giữ nguyên (vì index của hàng bằng 0), rồi hàng thứ hai thì bị xoay trái 1 byte, tương ứng phần bôi đen bị chuyển sang bên phải, rồi hàng thứ ba bị xoay 2 byte, hàng thứ tư bị xoay 3 byte.

## MixColumns()
Hàm biến đổi này hoạt động trên từng cột State, coi mỗi cột là một đa thức bậc bốn. Việc xây dựng này dựa trên việc coi các cột là đa thức trên GF(28) và nhân modulo $x^4+1$ với một đa thức $a(x)=${03} $x^3+$ {01} $x^2+$ {01} $x+$ {02}

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

Áp dụng tính chất giao hoán của field ( $a(x)•(b(x)+c(x))=a(x)•b(x)+a(x)•c(x)$ ) để tính thôi

Để dễ hình dung hơn thì hàm **MixColumns()** thực hiện biến đổi với từng cột từ việc nhân ma trận trước đó như sau:
![image](https://user-images.githubusercontent.com/95759699/203778337-b7179f17-9433-4a31-b1a6-2f53afcd61fb.png)

## Key Expansion
Nghe tên là biết chỉ áp dụng biến đổi cho khóa K, vì vậy đầu vào của hàm này luôn là khóa K (trường trường hợp AES-128). Mục địch chính là mở rộng cái khóa này và sẽ kết hợp với hàm **AddRoundKey()** (chi tiết cụ thể sẽ nói sau). **Key Expansion** tạo ra tổng cộng **Nb**(**Nr** + 1) word: Thiếu toán yêu cầu một bộ khởi tạo Nb word, mỗi round Nr yêu cầu Nb word dữ liẹu key. Kết quả của key schedule bao gồm một mảng gồm các word, ký hiệu là [$w_i$], với i trong khoảng 0 ≤ i ≤ **Nb**(**Nr** + 1)

**KeyExpansion()** yêu cầu một số hàm nhỏ như **RotWord()**, **SubWord()**, **Rcon()**
### SubWord()
...
### Chi tiết thuật toán
...



