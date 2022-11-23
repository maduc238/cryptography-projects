import java.io.FileWriter;
import java.io.IOException;

public class STS212 {
    private static String key;
    private static String nonce;   // 8 byte
    
    public static void main(String[] args) {
        key = "*Thirty-two byte (256 bits) key*";
        nonce = "hello :)";
        /* 10^6 = 1000000 bit
         * each num = 64x8=512 bit
         * => num = 1954
        */
        String filename = "salsa20.test";
        encrypt(filename);
    }

    public static void encrypt(String filename) {
        int num = 1954;
        try {
            FileWriter myWriter = new FileWriter(filename);
            if (key.length() == 16) {
                for(int i=0; i<num; i++) {
                    int[] a = Salsa20(make_n(convertToInt(nonce),i), convertToInt(key));
                    // 64 byte x 8 bit
                    for (int j=0; j<64; j++) {
                        // System.out.print(String.format("%8s", Integer.toBinaryString(a[j] & 0xFF)).replace(' ', '0'));
                        myWriter.write(String.format("%8s", Integer.toBinaryString(a[j] & 0xFF)).replace(' ', '0'));
                    }
                }
            }
            else if (key.length() == 32) {
                for(int i=0; i<num; i++) {
                    int[] a = Salsa20(make_n(convertToInt(nonce),i), convertToInt(key.substring(0, 16)), convertToInt(key.substring(16)));
                    // 64 byte x 8 bit
                    for (int j=0; j<64; j++) {
                        // System.out.print(String.format("%8s", Integer.toBinaryString(a[j] & 0xFF)).replace(' ', '0'));
                        myWriter.write(String.format("%8s", Integer.toBinaryString(a[j] & 0xFF)).replace(' ', '0'));
                    }
                }
            }
            myWriter.close();
            System.out.println("Successfully wrote to the file.");
        } catch (IOException e) {
            System.out.println("An error occurred.");
            e.printStackTrace();
        }
    }

    /**
     * Hàm quarterround
     * @param y mảng y[] có 4 phần tử (4 word). Ví dụ input int[] y = {0xe7e8c006,0xc4f9417d,0x6479b4b2,0x68c67137};
     * @return một mảng z[] có 4 phần tử (4 word)
     */
    public static int[] quarterround(int[] y) {
        int[] z = new int[4];
        z[1] = y[1] ^ Integer.rotateLeft((y[0]+y[3]), 7);
        z[2] = y[2] ^ Integer.rotateLeft((z[1]+y[0]), 9);
        z[3] = y[3] ^ Integer.rotateLeft((z[2]+z[1]), 13);
        z[0] = y[0] ^ Integer.rotateLeft((z[3]+z[2]), 18);
        return z;
    }

    /**
     * Hàm quarterround có 4 phần tử (4 word). Ví dụ quarterround(0xe7e8c006,0xc4f9417d,0x6479b4b2,0x68c67137)
     * @return một mảng z[] có 4 phần tử (4 word)
     */
    public static int[] quarterround(int y0, int y1, int y2, int y3) {
        int[] z = new int[4];
        z[1] = y1 ^ Integer.rotateLeft((y0+y3), 7);
        z[2] = y2 ^ Integer.rotateLeft((z[1]+y0), 9);
        z[3] = y3 ^ Integer.rotateLeft((z[2]+z[1]), 13);
        z[0] = y0 ^ Integer.rotateLeft((z[3]+z[2]), 18);
        return z;
    }

    /**
     * Hàm rowround
     * @param y mảng y[] có 16 phần tử (16 word)
     * @return mảng z[] có 16 phần tử (16 word)
     */
    public static int[] rowround(int[] y) {
        int[] z = new int[16];
        int[] row = quarterround(y[0],y[1],y[2],y[3]);
        z[0]=row[0];z[1]=row[1];z[2]=row[2];z[3]=row[3];
        row = quarterround(y[5],y[6],y[7],y[4]);
        z[5]=row[0];z[6]=row[1];z[7]=row[2];z[4]=row[3];
        row = quarterround(y[10],y[11],y[8],y[9]);
        z[10]=row[0];z[11]=row[1];z[8]=row[2];z[9]=row[3];
        row = quarterround(y[15],y[12],y[13],y[14]);
        z[15]=row[0];z[12]=row[1];z[13]=row[2];z[14]=row[3];
        return z;
    }

    /**
     * Hàm columnround
     * @param x mảng x[] có 16 phần tử (16 word)
     * @return mảng y[] có 16 phần tử (16 word)
     */
    public static int[] columnround(int[] x) {
        int[] y = new int[16];
        int[] row = quarterround(x[0],x[4],x[8],x[12]);
        y[0]=row[0];y[4]=row[1];y[8]=row[2];y[12]=row[3];
        row = quarterround(x[5],x[9],x[13],x[1]);
        y[5]=row[0];y[9]=row[1];y[13]=row[2];y[1]=row[3];
        row = quarterround(x[10],x[14],x[2],x[6]);
        y[10]=row[0];y[14]=row[1];y[2]=row[2];y[6]=row[3];
        row = quarterround(x[15],x[3],x[7],x[11]);
        y[15]=row[0];y[3]=row[1];y[7]=row[2];y[11]=row[3];
        return y;
    }

    /**
     * Hàm doubleround
     * @param x mảng x[] có 16 phần tử (16 word)
     * @return mảng z[] có 16 phần tử (16 word)
     */
    public static int[] doubleround(int[] x) {
        return rowround(columnround(x));
    }

    /**
     * Hàm doubleround mũ n
     * @param x mảng x[] có 16 phần tử (16 word)
     * @return mảng result[] có 16 phần tử (16 word)
     */
    public static int[] doubleroundPowN(int[] x, int n) {
        int[] result = doubleround(x);
        for(int i=0; i<n-1; i++) {
            result = doubleround(result);
        }
        return result;
    }

    /**
     * Hàm littleendian với đầu vào là các bn, mỗi b là 1 byte
     * @param b0
     * @param b1
     * @param b2
     * @param b3
     * @return một word có 4 byte: littleendian(bn) = b0 + 2^8 b1 + 2^16 b2 + 2^24 b3
     */
    public static int littleendian(int b0, int b1, int b2, int b3) {
        return b0 + 256*b1 + 65536*b2 + 16777216*b3;
    }

    /**
     * Hàm littleendian^-1
     * @return mảng 4 phần tử, mỗi cái 1 byte
     */
    public static int[] ARClittleendian(int a) {
        int[] result = new int[4];
        for(int i=0; i<4; i++) {
            result[3-i] = Integer.decode("0x" + String.format("%8x", a).replace(' ', '0').substring(2*i,2*i+2));
        }
        return result;
    }

    /**
     * Hàm Salsa20 hash
     * @param x 64 phần tử 1 byte
     * @return 64 phần tử 1 byte
     */
    public static int[] Salsa20(int[] x) {
        int[] xi = new int[16];
        int[] z = new int[64];
        int[] y = new int[16];
        for(int i=0; i<16; i++) {
            xi[i] = littleendian(x[4*i], x[4*i+1], x[4*i+2], x[4*i+3]);
        }
        y = doubleroundPowN(xi, 10);
        for(int i=0; i<16; i++) {
            int[] temp = ARClittleendian(y[i] + xi[i]);
            for(int j=0; j<4; j++) {
                z[4*i + j] = temp[j];
            }
        }
        return z;
    }

    /**
     * Hàm Salsa20 expansion
     * @param n 16 phần tử 1 byte đầu vào
     * @param k0 16 phần tử 1 byte k0
     * @return Salsa20 hash của: <i>tau0</i>, <i>k0</i>, <i>tau1</i>, <i>n</i>, <i>tau2</i>, <i>k0</i>, <i>tau3</i>
     */
    public static int[] Salsa20(int[] n, int[] k0) {
        int[] input = new int[64];
        // tau0
        input[0]=101;input[1]=120;input[2]=112;input[3]=97;
        // tau1
        input[20]=110;input[21]=100;input[22]=32;input[23]=49;
        // tau2
        input[40]=54;input[41]=45;input[42]=98;input[43]=121;
        // tau3
        input[60]=116;input[61]=101;input[62]=32;input[63]=107;
        for(int i=0; i<16; i++) {
            input[24+i] = n[i];
            input[4+i] = k0[i];
            input[44+i] = k0[i];
        }
        return Salsa20(input);
    }

    /**
     * Hàm Salsa20 expansion
     * @param n 16 phần tử 1 byte đầu vào
     * @param k0 16 phần tử 1 byte k0
     * @param k1 16 phần tử 1 byte k1
     * @return Salsa20 hash của: <i>sigma0, <i>k0</i>, <i>sigma1</i>, <i>n</i>, <i>sigma2</i>, <i>k1</i>, <i>sigma3</i>
     */
    public static int[] Salsa20(int[] n, int[] k0, int[] k1) {
        int[] input = new int[64];
        // sigma0
        input[0]=101;input[1]=120;input[2]=112;input[3]=97;
        // sigma1
        input[20]=110;input[21]=100;input[22]=32;input[23]=51;
        // sigma2
        input[40]=50;input[41]=45;input[42]=98;input[43]=121;
        // sigma3
        input[60]=116;input[61]=101;input[62]=32;input[63]=107;
        for(int i=0; i<16; i++) {
            input[24+i] = n[i];
            input[4+i] = k0[i];
            input[44+i] = k1[i];
        }
        return Salsa20(input);
    }

    /**
     * Chuyển đổi dạng chuỗi chữ {@code String} thành mảng <i>len</i> phần tử 
     * số nguyên kích thước 1 byte
     * @param a {@code String} đầu vào
     * @return mảng {@code int} có chiều dài bằng {@code String}
     */
    public static int[] convertToInt(String a) {
        int[] result = new int[a.length()];
        for(int i=0; i<a.length(); i++) {
            result[i] = (int) a.charAt(i);
        }
        return result;
    }

    /**
     * Tạo n
     * @param nonce 8 byte
     * @param c bộ đếm
     * @return 16 byte n
     */
    public static int[] make_n(int[] nonce, int c) {
        int[] result = new int[16];
        for(int i=0; i<8; i++) {

            result[i] = nonce[i];
            result[i+8] = c%256;
            c /= 256;
        }
        return result;
    }
}
