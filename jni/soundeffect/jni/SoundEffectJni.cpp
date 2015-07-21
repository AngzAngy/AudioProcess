#include "Interface.h"
#include "SoundEffectJni.h"
#include "jnilogger.h"

JNIEXPORT void JNICALL Java_com_audio_jni_SoundEffectJni_test
  (JNIEnv *env, jobject jthiz){
	void* p= (void*)CreateVAPInstance();
	if(p){
		LOGE("11111111111");
	}else{
		LOGE("0000");
	}
}
