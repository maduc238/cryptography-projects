# Chạy bộ kiểm thử cho chuỗi ngẫu nhiên và giả ngẫu nhiên

Đường link tới công cụ kiểm thử: [NIST Statistical Test Suite](https://csrc.nist.gov/projects/random-bit-generation/documentation-and-software)

## Hướng dẫn cài

- Giải nén file zip: `unzip sts-2_1_2.zip`
- Chỉnh sửa file `makefile` và chỉnh sửa các dòng sau:
    + **CC** tới bin của GNU Compiler
    + **ROOTDIR** tới đường dẫn thực thi sts này

Ví dụ:
```
CC = /usr/bin/gcc
ROOTDIR = $HOME/sts-2.1.2/
```
- Chạy file makefile bằng lệnh `make`
- Sau khi cài xong, máy có thể chạy với lệnh
```
./assess <sequenceLength>
```
Với `<sequenceLength>` là độ dài bit mong muốn

## Chạy Test Code

Ban đầu nhập chiều dài bit
```
./assess 100000
```
Màn hình lúc này sẽ hiển thị:
```
           G E N E R A T O R    S E L E C T I O N 
           ______________________________________

    [0] Input File                 [1] Linear Congruential
    [2] Quadratic Congruential I   [3] Quadratic Congruential II
    [4] Cubic Congruential         [5] XOR
    [6] Modular Exponentiation     [7] Blum-Blum-Shub
    [8] Micali-Schnorr             [9] G Using SHA-1

   Enter Choice: ...
```
Nhập `0`
```
                User Prescribed Input File: ...
```
Chạy file input để test, tại đây có sẵn input file mẫu nên ta nhập: `data/data.pi`

Sau đó màn hình hiển thị lựa chọn Statistical Tests
```

                S T A T I S T I C A L   T E S T S
                _________________________________

    [01] Frequency                       [02] Block Frequency
    [03] Cumulative Sums                 [04] Runs
    [05] Longest Run of Ones             [06] Rank
    [07] Discrete Fourier Transform      [08] Nonperiodic Template Matchings
    [09] Overlapping Template Matchings  [10] Universal Statistical
    [11] Approximate Entropy             [12] Random Excursions
    [13] Random Excursions Variant       [14] Serial
    [15] Linear Complexity

         INSTRUCTIONS
            Enter 0 if you DO NOT want to apply all of the
            statistical tests to each sequence and 1 if you DO.

   Enter Choice: ...
```
Trong trường hợp này, chọn `0` để áp dụng cho một tập hợp con các statistical tests có sẵn. Sau đó màn hình sẽ hiển thị
```
         INSTRUCTIONS
            Enter a 0 or 1 to indicate whether or not the numbered statistical
            test should be applied to each sequence.

      123456789111111
               012345
                  ...
```
Giờ ta nhập 15 bit tương ứng để chọn templates test. Bit `1` nghĩa là chọn còn bit `0` nghĩa là không chọn

Nếu như muốn chọn test số 9 thôi thì nhập: `000000001000000`. Sau đó màn hình hiển thị xác nhận cách chọn
```
        P a r a m e t e r   A d j u s t m e n t s
        -----------------------------------------
    [1] Overlapping Template Test - block length(m):    9

   Select Test (0 to continue): ...
```
Vậy là ta chọn cái số 9 thành công. Nhập `0` để tiếp tục. Sau đó màn hình sẽ hỏi:
```
   How many bitstreams? ...
```
Nhập số bitstreams để kiểm thử từ file `data/data.pi`. Ví dụ đây là nhập `10` tương ứng với 10 chuỗi
```
   Input File Format:
    [0] ASCII - A sequence of ASCII 0's and 1's
    [1] Binary - Each byte in data file contains 8 bits of data

   Select input mode: ...
```
Tại file này toàn chuỗi bit 0-1 nên nhập `0`. Sau đó đợi chạy
```
     Statistical Testing In Progress.........
```
Cho tới khi kết quả chạy thành công
```
     Statistical Testing Complete!!!!!!!!!!!!
```
Kết quả kiểm định này sẽ xuất hiện trong folder `experiments/`

## Kết quả

Tổng hợp kết quả thu được tại đường dẫn: `experiments/AlgorithmTesting/finalAnalysisReport.txt`

```
------------------------------------------------------------------------------
RESULTS FOR THE UNIFORMITY OF P-VALUES AND THE PROPORTION OF PASSING SEQUENCES
------------------------------------------------------------------------------
   generator is <data/data.pi>
------------------------------------------------------------------------------
 C1  C2  C3  C4  C5  C6  C7  C8  C9 C10  P-VALUE  PROPORTION  STATISTICAL TEST
------------------------------------------------------------------------------
  4   0   1   2   0   1   0   0   0   2  0.066882      9/10      OverlappingTemplate


- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
The minimum pass rate for each statistical test with the exception of the
random excursion (variant) test is approximately = 8 for a
sample size = 10 binary sequences.

For further guidelines construct a probability table using the MAPLE program
provided in the addendum section of the documentation.
- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

```
Ngoài ra còn nhiều file kết quả nữa nhưng tự mò đi :neutral_face:
