package com.shanling.demo;

public class util {
    /***
     * 字节数组转到 Hex 十六进制文本
     * @param bArr
     * @return
     */
    public static String buf_to_string(byte[] bArr) {
        if (bArr == null) {
            return "";
        }
        String str = "";
        for (int i = 0; i < bArr.length; i++) {
            str = (str + Integer.toHexString((bArr[i] >> 4) & 15)) + Integer.toHexString(bArr[i] & 15);
        }
        return str;
    }

    public static byte get_char(byte b) {
        if (b >= (byte) 48 && b <= (byte) 57) {
            return (byte) (b - 48);
        }
        if (b >= (byte) 97 && b <= (byte) 102) {
            return (byte) ((b - 97) + 10);
        }
        if (b < (byte) 65 || b > (byte) 70) {
            return (byte) 0;
        }
        return (byte) ((b - 65) + 10);
    }

    /***
     * Hex 十六进制文本转到字节数组
     * @param str
     * @return
     */
    public static byte[] string_to_buf(String str) {
        int i = 0;
        if (str == null) {
            return new byte[0];
        }
        byte[] bArr = new byte[(str.length() / 2)];
        while (i < str.length() / 2) {
            bArr[i] = (byte) ((get_char((byte) str.charAt(i * 2)) << 4) + get_char((byte) str.charAt((i * 2) + 1)));
            i++;
        }
        return bArr;
    }
}
