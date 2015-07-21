#include "AudioPreprocessJni.h"
#include "jnilogger.h"
#include "speex/speex_preprocess.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#define NN 882
#define FRAMESIZE_MS  20

JNIEXPORT jboolean JNICALL Java_com_audio_jni_AudioPreprocessJni_preprocess(
		JNIEnv *env, jobject jobj, jstring jinpcm, jstring joutpcm,
		jint jsamRate, jint jchannelNumber, jint jnoiseSuppress) {
	LOGD("Enter func : %s", __FUNCTION__);
	if (!jinpcm || !joutpcm) {
		LOGE("filename is null");
		return JNI_FALSE;
	}
	if(jchannelNumber!=1 && jchannelNumber!=2){
		LOGE("jchannelNumber is %d", jchannelNumber);
		return JNI_FALSE;
	}

	FILE *ifp, *ofp;

	spx_int32_t i;
	float f;
	spx_int32_t noisesuppress;
	SpeexPreprocessState *st;
	size_t actual_len = 0;

	const char* utf8in = env->GetStringUTFChars(jinpcm, NULL);
	ifp = fopen(utf8in, "rb");
	if (!ifp) {
		LOGE("%s no exit",utf8in);
		env->ReleaseStringUTFChars(jinpcm, utf8in);
		return JNI_FALSE;
	}

	const char* utf8out = env->GetStringUTFChars(joutpcm, NULL);
	ofp = fopen(utf8out, "wb");
	if (!ofp) {
		LOGE("%s no exit",utf8out);
		env->ReleaseStringUTFChars(joutpcm, utf8out);
		return JNI_FALSE;
	}

	int frameSize = jsamRate * FRAMESIZE_MS / 1000;

	int bufSize = frameSize * 2;//sample deep is 16 bits
	//分配内存空间
	int8_t *src_all = new int8_t[bufSize];
	if(!src_all){
		LOGE("no src memory");
		return JNI_FALSE;
	}
	LOGD("func-: %s,,sampleRate: %d,,channel: %d,,frameSize: %d,,bufSize: %d", __FUNCTION__, jsamRate, jchannelNumber, frameSize, bufSize);

	st = speex_preprocess_state_init(frameSize, jsamRate);

	i=1;
	speex_preprocess_ctl(st, SPEEX_PREPROCESS_SET_DENOISE, &i);
	i=0;
	speex_preprocess_ctl(st, SPEEX_PREPROCESS_SET_AGC, &i);
	i=jsamRate;
	speex_preprocess_ctl(st, SPEEX_PREPROCESS_SET_AGC_LEVEL, &i);
	i=0;
	speex_preprocess_ctl(st, SPEEX_PREPROCESS_SET_DEREVERB, &i);
	f=.0;
	speex_preprocess_ctl(st, SPEEX_PREPROCESS_SET_DEREVERB_DECAY, &f);
	f=.0;
	speex_preprocess_ctl(st, SPEEX_PREPROCESS_SET_DEREVERB_LEVEL, &f);

	noisesuppress = (spx_int32_t)jnoiseSuppress;
	speex_preprocess_ctl(st, SPEEX_PREPROCESS_SET_NOISE_SUPPRESS, &noisesuppress);

//	spx_int32_t maxAttenuationNoise = 0, noisesuppress = 0;
//	speex_preprocess_ctl(st, SPEEX_PREPROCESS_GET_NOISE_SUPPRESS, &maxAttenuationNoise);
//	noisesuppress = -200;//maxAttenuationNoise / 2;
//	LOGD("func : %s,,max attenuation : %d,,noisesuppress: %d", __FUNCTION__, maxAttenuationNoise, noisesuppress);
//	if(noisesuppress<0){
//	noisesuppress = -10;
//	speex_preprocess_ctl(st, SPEEX_PREPROCESS_SET_NOISE_SUPPRESS, &noisesuppress);
	LOGD("func : %s,,noisesuppress: %d", __FUNCTION__,  noisesuppress);
//	}

	for (;;) {
		//从文件读取数据
		actual_len = fread(src_all, sizeof(int8_t), bufSize, ifp);
		if (!actual_len) {
			//数据已经读取完毕
			break;
		}
//		if (Channels == 1) {
			speex_preprocess_run(st, (spx_int16_t*) src_all);
//		} else {
//		}
		fwrite(src_all, sizeof(int8_t), actual_len, ofp);
	}

	//释放空间
	delete []src_all;

	//关闭文件
	fclose(ifp);
	fclose(ofp);

	speex_preprocess_state_destroy(st);

	env->ReleaseStringUTFChars(jinpcm, utf8in);
	env->ReleaseStringUTFChars(joutpcm, utf8out);

	LOGD("Leave func : %s", __FUNCTION__);
	return JNI_TRUE;
}
