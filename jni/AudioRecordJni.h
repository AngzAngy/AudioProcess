/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
/* Header for class com_audio_jni_AudioRecordJni */

#ifndef _Included_com_audio_jni_AudioRecordJni
#define _Included_com_audio_jni_AudioRecordJni
#ifdef __cplusplus
extern "C" {
#endif
JNIEXPORT jlong JNICALL Java_com_audio_jni_AudioRecordJni_init
  (JNIEnv *, jobject, jstring, jint, jint);

JNIEXPORT void JNICALL Java_com_audio_jni_AudioRecordJni_start
  (JNIEnv *, jobject, jlong);

JNIEXPORT void JNICALL Java_com_audio_jni_AudioRecordJni_pause
  (JNIEnv *, jobject, jlong);

JNIEXPORT void JNICALL Java_com_audio_jni_AudioRecordJni_stop
  (JNIEnv *, jobject, jlong);


JNIEXPORT void JNICALL Java_com_audio_jni_AudioRecordJni_release
  (JNIEnv *, jobject, jlong);

#ifdef __cplusplus
}
#endif
#endif