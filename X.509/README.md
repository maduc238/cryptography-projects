# Tạo và kiếm tra chứng thực chữ ký số X.509

## Giới thiệu chung về X.509

X.509 là một chứng chỉ, chữ ký số cho người dùng để xác định danh tính của người sở hữu. Ứng dụng phổ biến hiện nay được sử dụng như trong giao thức SSL/TLS trên internet, hay về mail, chữ ký điện tử,...

Code dưới đây sẽ hỗ trợ tạo chứng chỉ X.509

## Requirement

Chạy trên Python3, hỗ trợ các thư viện:

```
pip install asn1tools, rsa
```

