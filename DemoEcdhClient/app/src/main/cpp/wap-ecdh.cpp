#include <jni.h>
#include <string.h>
#include <stdio.h>
#include <android/log.h>
#include "openssl/crypto.h"
#include "openssl/ssl.h"
#include "openssl/md5.h"
#include "openssl/bn.h"
#include "util.cpp"

#define ECDH_SIZE 67
#define MALLOC_SIZE 0x400u
#define LOG_TAG  "C_TAG"
#define LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG, __VA_ARGS__)
#define LOGI(...)  __android_log_print(ANDROID_LOG_INFO, __VA_ARGS__)
#define LOGE(...)  __android_log_print(ANDROID_LOG_ERROR, __VA_ARGS__)
#define LOGW(...)  __android_log_print(ANDROID_LOG_WARN, __VA_ARGS__)

/* s1 ： 生成（1）/协商（2） s2 ：_s_pub_key  s3 ：_c_pub_key s4 ：_c_pri_key */
int GenECDHKey(JNIEnv* env, jobject obj, char * s2, char * s3, char * s4){
    char* p1 = s2;
    char* p2 = s3;
    char* p3 = s4;
    if(strlen(p1) == 0) return -0x10;
	EC_KEY * ec_key = EC_KEY_new();
	ec_key = EC_KEY_new_by_curve_name(NID_secp192k1);//711
	if(!ec_key) return -0x07;
	if(strlen(s4) > 0){
	    BIGNUM *big = BN_new();
	    if(!big) return -0x05;
	    unsigned char *sout3 = (unsigned char*)malloc(MALLOC_SIZE);memset(sout3, 0, MALLOC_SIZE);
        int len3 = String2Buffer((unsigned char*)p3, strlen(p3), sout3);
        BN_mpi2bn(sout3, len3, big);
        if(!big) return -0x06;
        char *p = BN_bn2hex(big);
        if (p){
        	OPENSSL_free(p);//释放p
        	free(sout3);//释放 sout3
        }
        int r = EC_KEY_set_private_key(ec_key, big);
        BN_free(big);//释放
        if(r != 1) return -0x01;
        const EC_GROUP *group = EC_KEY_get0_group(ec_key);
        EC_POINT *point = EC_POINT_new(group);
        if(strlen(p2) > 0){
            unsigned char *str2bin = (unsigned char*)malloc(MALLOC_SIZE); memset(str2bin, 0, MALLOC_SIZE);
            int len22 = String2Buffer((unsigned char*)p2, strlen(p2), str2bin);
            int r1 = EC_POINT_oct2point(group, point, str2bin, len22, NULL);
            free(str2bin);
            if (r1 != 1) return -0x11;
        }else{
            int r2 = EC_POINT_mul(group, point, NULL, NULL, NULL, NULL);
            if (r2 != 1) return -0x02;
        }
        int r3 = EC_KEY_set_public_key(ec_key, point);
        if (r3 != 1) return -0x03;
	}else{
	    int r5 = EC_KEY_generate_key(ec_key);
        if (!r5) return -0x55;
	}
	const EC_GROUP *group1 = EC_KEY_get0_group(ec_key);
    const EC_POINT *point1 = EC_KEY_get0_public_key(ec_key);
    //get pubkey
    unsigned char *pubkey = (unsigned char*)malloc(MALLOC_SIZE);
    memset(pubkey, 0, MALLOC_SIZE);
    int publen = EC_POINT_point2oct(group1, point1, POINT_CONVERSION_COMPRESSED, pubkey, ECDH_SIZE, NULL);
    // 此处赋值pubkey
    jclass dpclazz = env->GetObjectClass(obj); if(!dpclazz) return -0x80;
    jmethodID methodID1 = env->GetMethodID(dpclazz,"set_c_pub_key", "([B)V"); if(!methodID1) return -0x81;
    jbyteArray para1 = env->NewByteArray(publen);
    env->SetByteArrayRegion(para1, 0, publen, (jbyte *)pubkey);
    env->CallVoidMethod(obj, methodID1, para1);
    free(pubkey);//释放pubkey

    //get privatekey
    const BIGNUM *pribig = EC_KEY_get0_private_key(ec_key);
   	unsigned char *pout = (unsigned char*)malloc(MALLOC_SIZE); memset(pout, 0, MALLOC_SIZE);
    int lenpri = BN_bn2mpi(pribig, pout);
    // 此处赋值prikey
    jmethodID methodID2 = env->GetMethodID(dpclazz,"set_c_pri_key", "([B)V"); if(!methodID2) return -0x82;
    jbyteArray para2 = env->NewByteArray(lenpri);
    env->SetByteArrayRegion(para2, 0, lenpri, (jbyte *)pout);
    env->CallVoidMethod(obj, methodID2, para2);
    free(pout);//释放prikey

    //set p1
    unsigned char *str1bin = (unsigned char*)malloc(MALLOC_SIZE);
    memset(str1bin, 0, MALLOC_SIZE);
    int len11 = String2Buffer((unsigned char*)p1, strlen(p1), str1bin);
    EC_POINT *point2 = EC_POINT_new(group1);
    int r4 = EC_POINT_oct2point(group1, point2, str1bin, len11, NULL);
    free(str1bin);//释放str1bin
    if (r4 != 1) return -0x04;

    //get sharedkey
    unsigned char *shared = (unsigned char *)malloc(MALLOC_SIZE);
    memset(shared, 0, MALLOC_SIZE);
    int len = ECDH_compute_key(shared, 512, point2, ec_key, NULL);
    if (len <= 0) return -0x09;
	unsigned char md5share[MD5_DIGEST_LENGTH];
	MD5(shared, len, md5share);
	// 此处赋值sharekey
    jmethodID methodID3 = env->GetMethodID(dpclazz,"set_g_share_key", "([B)V"); if(!methodID3) return -0x83;
    jbyteArray para3 = env->NewByteArray(MD5_DIGEST_LENGTH);
    env->SetByteArrayRegion(para3, 0, MD5_DIGEST_LENGTH, (jbyte *)md5share);
    env->CallVoidMethod(obj, methodID3, para3);
	free(shared);//释放shaedkey
	//free(md5share);
	EC_KEY_free(ec_key);//释放eckey
	return 1;
}

extern "C" JNIEXPORT jint JNICALL
Java_com_shanling_demo_AppEcdh_GenECDHKeyEx(JNIEnv* env, jobject obj, jstring s1, jstring s2, jstring s3, jstring s4, jstring s5) {
    return GenECDHKey(env, obj, JString2Char(env, s1), JString2Char(env, s2), JString2Char(env, s3));
}
