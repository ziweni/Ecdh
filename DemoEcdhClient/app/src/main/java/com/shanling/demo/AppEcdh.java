package com.shanling.demo;

import android.util.Log;

public class AppEcdh {
    static {
        System.loadLibrary("wap-ecdh");
    }

    public static final String S_PUB_KEY = "04af8efd25576ed8ab550b5d65e0ad36916566708cbe5a3ad54e127ae42951d09212fc04a1ac463aff66d3dbf5ac6f46a1";
    public static byte[] _c_pri_key = new byte[0];
    public static byte[] _c_pub_key = new byte[0];
    private static byte[] _g_share_key = new byte[0];
    private static boolean userOpenSSLLib = true;
    /***
     * 调用 So 库里的函数
     * @param str1 S_PUB_KEY 服务器初始公钥
     * @param str2 _c_pub_key 客户生成的公钥
     * @param str3 _c_pri_key 客户生成的私钥
     * @return 返回：1 ：表示成功
     */
    public native int GenECDHKeyEx(String str1, String str2, String str3);

    public int GenereateKey(){
        try {
            int GenECDHKeyEx;
            synchronized (AppEcdh.class) {
                GenECDHKeyEx = GenECDHKeyEx(S_PUB_KEY, "", "");
            }
            return GenECDHKeyEx;
        } catch (UnsatisfiedLinkError e) {
            // util.LOGI("GenereateKey failed " + e.getMessage(), "");
            Log.i("TAG","GenereateKey failed " + e.getMessage());
            return -1;
        }
    }

    public byte[] calShareKeyMd5ByPeerPublicKey(byte[] bArr) {
        // Log.i("TAG", "userOpenSSLLib " + userOpenSSLLib + " peerRawPublicKey " + util.buf_to_string(bArr));
        if (true == userOpenSSLLib) {
            return calShareKeyByOpenSSL(util.buf_to_string(_c_pri_key), util.buf_to_string(_c_pub_key), util.buf_to_string(bArr));
        }
        return null;
    }

    private byte[] calShareKeyByOpenSSL(String str, String str2, String str3) {
        // Log.i("TAG", "calShareKeyByOpenSSL publickey " + str2);
        if (GenECDHKeyEx(str3, str2, str) == 1) {
            return _g_share_key;
        }
        // t.al.attr_api(2461268);
        return null;
    }

    public byte[] get_c_pub_key() {
        return (byte[]) _c_pub_key.clone();
    }

    public void set_c_pub_key(byte[] bArr) {
        if (bArr != null) {
            _c_pub_key = (byte[]) bArr.clone();
        } else {
            _c_pub_key = new byte[0];
        }
    }

    public void set_c_pri_key(byte[] bArr) {
        if (bArr != null) {
            _c_pri_key = (byte[]) bArr.clone();
        } else {
            _c_pri_key = new byte[0];
        }
    }

    public byte[] get_g_share_key() {
        return (byte[]) _g_share_key.clone();
    }

    public void set_g_share_key(byte[] bArr) {
        if (bArr != null) {
            _g_share_key = (byte[]) bArr.clone();
        } else {
            _g_share_key = new byte[0];
        }
    }
}
