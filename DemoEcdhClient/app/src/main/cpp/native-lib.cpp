#include <jni.h>
#include <string>


extern "C" JNIEXPORT jstring JNICALL
Java_com_shanling_demo_MainActivity_stringFromJNI(
        JNIEnv* env,
        jobject /* this */) {
    std::string hello = "Hello from C++";
    // EC_KEY * ec_key = EC_KEY_new();
    // return env->NewStringUTF(OpenSSL_version(OPENSSL_VERSION));
    return env->NewStringUTF(hello.c_str());
}
