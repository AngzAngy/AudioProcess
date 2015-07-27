#include "AudioPreprocessJni.h"
#include "jnilogger.h"
#include "SoundPreprocessor.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#define NN 882
#define FRAMESIZE_MS  10
#define MAX_CH 32768

JNIEXPORT jboolean JNICALL Java_com_audio_jni_AudioPreprocessJni_preprocess(
		JNIEnv *env, jobject jobj, jstring jinpcm, jstring joutpcm,
		jint jsamRate, jint jBytesPerSample, jint jchannelNumber, jint jnoiseSuppress) {
	LOGD("Enter func : %s", __FUNCTION__);
	if (!jinpcm || !joutpcm) {
		LOGE("error filename is null");
		return JNI_FALSE;
	}
	if(jchannelNumber!=1 && jchannelNumber!=2){
		LOGE("error jchannelNumber is %d", jchannelNumber);
		return JNI_FALSE;
	}
	if(jBytesPerSample!=1 && jBytesPerSample!=2){
		LOGE("error jBytesPerSample is %d", jBytesPerSample);
		return JNI_FALSE;
	}
	FILE *ifp, *ofp;

//	long iflen = 0, nSetup = 0;
//	spx_int32_t i;
//	spx_int32_t noisesuppress;
//	SpeexPreprocessState *st;
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

	SoundPreprocessor pro(jsamRate, jBytesPerSample, jchannelNumber, jnoiseSuppress);
//	int nBytesPerSecond = jsamRate * jchannelNumber * jBytesPerSample;
	int bufSize = pro.getBytesPerFrame();

	if(bufSize<=0){
		LOGE("error getBytesPerFrame: %d",bufSize);
		env->ReleaseStringUTFChars(joutpcm, utf8out);
		return JNI_FALSE;
	}
//	fseek(ifp,0L,SEEK_END);
//	iflen = ftell(ifp);
//	fseek(ifp,0L,SEEK_SET);
//
//	nSetup = iflen / m_nBytesPerFrame;

	//鍒嗛厤鍐呭瓨绌洪棿
	int8_t *procbuf = new int8_t[bufSize];
	if(!procbuf){
		LOGE("no src memory");
		return JNI_FALSE;
	}
	LOGD("func-: %s,,sampleRate: %d,,bytesPerSample: %d,,channel: %d,,bufSize: %d,,noiseSuppress: %d",
			__FUNCTION__, jsamRate, jBytesPerSample, jchannelNumber, bufSize, jnoiseSuppress);

//	st = speex_preprocess_state_init(m_nBytesPerFrame / sizeof(short), jsamRate);

//	i=1;
//	speex_preprocess_ctl(st, SPEEX_PREPROCESS_SET_DENOISE, &i);

//	noisesuppress = (spx_int32_t)jnoiseSuppress;
//	speex_preprocess_ctl(st, SPEEX_PREPROCESS_SET_NOISE_SUPPRESS, &noisesuppress);

//	spx_int32_t maxAttenuationNoise = 0, noisesuppress = 0;
//	speex_preprocess_ctl(st, SPEEX_PREPROCESS_GET_NOISE_SUPPRESS, &maxAttenuationNoise);
//	noisesuppress = -200;//maxAttenuationNoise / 2;
//	LOGD("func : %s,,max attenuation : %d,,noisesuppress: %d", __FUNCTION__, maxAttenuationNoise, noisesuppress);
//	if(noisesuppress<0){
//	noisesuppress = -10;
//	speex_preprocess_ctl(st, SPEEX_PREPROCESS_SET_NOISE_SUPPRESS, &noisesuppress);
//	LOGD("func : %s,,noisesuppress: %d", __FUNCTION__,  noisesuppress);
//	}

//	FILE *dmpfp = fopen("/sdcard/DCIM/pcm_data.txt", "wb");
	for (;;) {
		//浠庢枃浠惰鍙栨暟鎹�
		actual_len = fread(procbuf, sizeof(int8_t), bufSize, ifp);
		if (feof(ifp)) {
			//鏁版嵁宸茬粡璇诲彇瀹屾瘯
			break;
		}
//		if (Channels == 1) {
		pro.preprocess(procbuf, actual_len);
//		} else {
//		}
//#define LN 16
//			char str[LN]={0};
//			for(int i=0; i<actual_len / 2; i++){
//				sprintf(str,"%d,", procbuf[i]);
//				fwrite(str, 1, LN, dmpfp);
//				if(procBuf[i]>MAX_CH){
//				if(i<50)
//					LOGD("func : %s,,maxV: %d", __FUNCTION__,  procBuf[i]);
//					procbuf[i] = MAX_CH;
//				}else if(procBuf[i]<-(MAX_CH+1)){
//					LOGD("func : %s,,minV: %d", __FUNCTION__,  procBuf[i]);
//					procbuf[i] = -(MAX_CH+1);
//				}
//			}
		fwrite(procbuf, sizeof(int8_t), actual_len, ofp);
	}
//	fclose(dmpfp);

	//閲婃斁绌洪棿
	delete []procbuf;

	//鍏抽棴鏂囦欢
	fclose(ifp);
	fclose(ofp);

//	speex_preprocess_state_destroy(st);

	env->ReleaseStringUTFChars(jinpcm, utf8in);
	env->ReleaseStringUTFChars(joutpcm, utf8out);

	LOGD("Leave func : %s", __FUNCTION__);
	return JNI_TRUE;
}
