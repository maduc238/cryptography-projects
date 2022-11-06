# Code Java

![Spring Boot 2.0](https://img.shields.io/badge/Spring%20Boot-2.0-brightgreen.svg)
![JDK 11.1](https://img.shields.io/badge/JDK-11.1-brightgreen.svg)
![Maven](https://img.shields.io/badge/Maven-3.5.0-yellowgreen.svg)

## Requirements
(Bản càng mới càng ok :grin:)
+ JDK
+ Maven
+ Spring Boot
+ VSCode (cho dễ dùng :sunglasses:)

## Cách chạy
### Cách 1 (Đơn giản :innocent:): Chạy file `.jar`

Nếu máy bạn cài java rồi thì lấy file `salsa20-0.0.1-SNAPSHOT.jar` có ngay trên github này (ngoài ra sẽ còn có phiên bản mới hơn). Rồi nhập lệnh này trên terminal của máy tính

```
java -jar salsa20-0.0.1-SNAPSHOT.jar
```

Nếu ra kết quả build xong Spring như ảnh thì ok :satisfied:

![image](https://user-images.githubusercontent.com/95759699/200101398-935685e7-b093-456f-8aa7-8228143c3250.png)

### Cách 2 (Khó hơn chút :sweat_smile:): Build source code

Bước đầu tiên là git clone project này về
```
git clone https://github.com/maduc238/cryptography-projects.git
```

Sau đó mở VSCode, cài các Extensions cần thiết cho support việc chạy Java
![image](https://user-images.githubusercontent.com/95759699/200100167-addf631d-5c5f-402d-be08-b9fa6ba8a513.png)

Vào file `Application.java`. Tại đây nhấn `run` để chạy app

![image](https://user-images.githubusercontent.com/95759699/200100208-96336411-aff4-4407-8ce6-206d01cec371.png)

Sau đó chỉ cần đợi load code thôi :grimacing:

Khi chạy xong, truy cập vào địa chỉ http://127.0.0.1:8080/. Lúc này giao diện của bạn sẽ hiển thị

![image](https://user-images.githubusercontent.com/95759699/200100269-36c7067e-83d8-41f4-9185-d409e2b6b9b8.png)

## Sử dụng

Chỉ cần nhập văn bản vào mục Plaintext, cùng với đó là nonce (bắt buộc 8 byte) và key (bắt buộc 16 hoặc 32 byte). Ngoài ra tại ô check box bạn cũng có thể sử dụng input dạng hexa

Ví dụ với input như này: 

![image](https://user-images.githubusercontent.com/95759699/200100631-95c70b9d-e002-463a-b60a-1019cd85da7b.png)

Sau khi ấn `Submit` sẽ thu đươc kết quả:

![image](https://user-images.githubusercontent.com/95759699/200100639-5fc6f4af-0c88-4f97-b27e-ad7654a31586.png)

Ngoài ra cách giải mã, bạn chỉ cần nhập ngược lại thôi :sunglasses:

![image](https://user-images.githubusercontent.com/95759699/200100648-7009cb9f-6a68-4402-8ee9-68cbbb185977.png)

![image](https://user-images.githubusercontent.com/95759699/200100654-a4e6f295-15fe-4cea-9cc4-9217e42fa9cf.png)

## Cấu hình

Tại đường dẫn tới file `salsa20\src\main\resources\application.properties`, bạn có thể cấu hình thông số ip address và port khi bật ứng dụng:

```
server.address = 127.0.0.1
server.port = 8080
```
