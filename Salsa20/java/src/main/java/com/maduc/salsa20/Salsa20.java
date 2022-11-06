package com.maduc.salsa20;

public class Salsa20 {
    private String key;
    private String nonce;   // 8 byte

    /**
     * Hàm khởi tạo class {@code Salsa20}
     * @param key có chiều dài 16 hay 32 ký tự ASCII (tương ứng 16 hoặc 32 byte)
     * @param nonce có chiều dài 8 ký tự (tương ứng 8 byte)
     */
    public Salsa20(String key, String nonce) {
        this.key = key;
        this.nonce = nonce;
    }

    /**
     * Sử dụng mã hóa và giải mã cho đoạn text ASCII.
     * Lưu ý: Muốn dùng dạng hex hay text Unicode phải cần có chuyển đổi trước
     * @param plaintext Dạng {@code String}
     * @return
     */
    public String encrypt(String plaintext) {
        int len = plaintext.length();
        int c = len / 64;
        int count = 0;
        StringBuffer sb = new StringBuffer("");
        if (key.length() == 16) {
            for(int i=0; i<=c; i++) {
                int[] a = Functions.Salsa20(Functions.make_n(Functions.convertToInt(nonce),i), Functions.convertToInt(key));
                int j = 0;
                while(j < 64) {
                    sb.append((char) (a[j] ^ (int)plaintext.charAt(64*i + j)));
                    count++; j++;
                    if (count == len) {
                        return sb.toString();}

                }
            }
        } else if (key.length() == 32) {
            for(int i=0; i<=c; i++) {
                int[] a = Functions.Salsa20(Functions.make_n(Functions.convertToInt(nonce),i), Functions.convertToInt(key.substring(0, 16)), Functions.convertToInt(key.substring(16)));
                int j = 0;
                while(j < 64) {
                    sb.append((char) (a[j] ^ (int)plaintext.charAt(64*i + j)));
                    count++; j++;
                    if (count == len) {
                        return sb.toString();}

                }
            }
        }
        return "";
    }


}
