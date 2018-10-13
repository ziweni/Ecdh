#include <jni.h>
#include <string>


#define MALLOC_SIZE 0x400u

char* JString2Char(JNIEnv* env, jstring jstr) {
    char* rtn = NULL;
    jclass clsstring = env->FindClass("java/lang/String");
    jstring strencode = env->NewStringUTF("GB2312");
    jmethodID mid = env->GetMethodID(clsstring, "getBytes", "(Ljava/lang/String;)[B");
    jbyteArray barr = (jbyteArray) env->CallObjectMethod(jstr, mid, strencode);
    jsize alen = env->GetArrayLength(barr);
    jbyte* ba = env->GetByteArrayElements(barr, JNI_FALSE);
    if (alen > 0) {
        rtn = (char*) malloc(alen + 1);
        memcpy(rtn, ba, alen);
        rtn[alen] = 0;
    }
    env->ReleaseByteArrayElements(barr, ba, 0);
    if(!rtn) return (char*)"";
    return rtn;
}

/*hex转bin*/
int String2Buffer(unsigned char* src, int srclen, unsigned char* dest) {
	int i = 0;
	if (srclen % 2 != 0) return 0;
	for (i = 0; i < srclen / 2; i++)
	{
		char tmp[3];
		tmp[0] = *(src + 2 * i);
		tmp[1] = *(src + 2 * i + 1);
		tmp[2] = 0;
		int out = 0;
		sscanf(tmp, "%x", &out);
		unsigned char ch = (unsigned char)out;
		*(dest + i) = ch;
	}
	return i;
}

/*bin转hex*/
int Buffer2String(unsigned char* src, int srclen, unsigned char* dest) {
	int i;
	for (i = 0; i < srclen; i++)
	{
		char tmp[3] = { 0 };
		sprintf(tmp, "%x", *(src + i));
		if (strlen(tmp) == 1) {
			strcat((char*)dest, "0");
			strncat((char*)dest, tmp, 1);
		}
		else if (strlen(tmp) == 2) {
			strncat((char*)dest, tmp, 2);
		}
		else {
			strcat((char*)dest, "00");
		}
	}
	return i * 2;
}