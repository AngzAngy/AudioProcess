#include "AudioRecordJni.h"
#include "jnilogger.h"
#include "AudioRecord.h"
#define L_2_OBJ(jl) AudioRecord *pObj = (AudioRecord *)(jl);

#define NativeFrameCallback "nativeOnFrameCallback"
#define NativeFrameCallbackSig "([BI)V"

static JavaVM* gVM = NULL;
//static SLuint32 convertSamplerate(int samplerate){
//     switch(samplerate) {
//     case 8000:
//         return SL_SAMPLINGRATE_8;
//         break;
//     case 11025:
//         return SL_SAMPLINGRATE_11_025;
//         break;
//     case 16000:
//         return SL_SAMPLINGRATE_16;
//         break;
//     case 22050:
//         return SL_SAMPLINGRATE_22_05;
//         break;
//     case 24000:
//         return SL_SAMPLINGRATE_24;
//         break;
//     case 32000:
//         return SL_SAMPLINGRATE_32;
//         break;
//     case 44100:
//         return SL_SAMPLINGRATE_44_1;
//         break;
//     case 48000:
//         return SL_SAMPLINGRATE_48;
//         break;
//     case 64000:
//         return SL_SAMPLINGRATE_64;
//         break;
//     case 88200:
//         return SL_SAMPLINGRATE_88_2;
//         break;
//     case 96000:
//         return SL_SAMPLINGRATE_96;
//         break;
//     case 192000:
//         return SL_SAMPLINGRATE_192;
//         break;
//     default:
//         return -1;
//     }
// }

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM* vm, void* reserved){
	JNIEnv* env = NULL;

	if(vm->GetEnv((void**)&env, JNI_VERSION_1_4) != JNI_OK){
	  return -1;
	}

	gVM = vm;
	return JNI_VERSION_1_4; //这里很重要，必须返回版本，否则加载会失败。
}

static void jniOnFrameCallback(void *buf, int32_t size, void* userData){
	OnFrameCallbackStruct *pCb = (OnFrameCallbackStruct*)userData;
	JNIEnv *env=NULL;
	if(pCb && gVM){
		jobject cbObj = pCb->callbackObj;
		if(cbObj){
			gVM->AttachCurrentThread(&env, NULL);
			if(env){
				jbyteArray jbarray = env->NewByteArray(size);
				env->SetByteArrayRegion(jbarray, 0, size, (const jbyte*)buf);

				env->CallVoidMethod(cbObj, pCb->callbackMethodId, jbarray, size);

				env->DeleteLocalRef(jbarray);
			}
			gVM->DetachCurrentThread();
		}
	}
}

JNIEXPORT jlong JNICALL Java_com_audio_jni_AudioRecordJni_init
  (JNIEnv *env, jobject jobj, jobject referObj, jstring jstr, jint jsampleRate, jint jBytesPerSample, jint jchannel, jint jminBufSize){
//    if(!jstr){
//        LOGI("filename is null");
//        return 0;
//    }
    if(jchannel!=1 && jchannel!=2){
        LOGI("channel is error num:%d",jchannel);
        return 0;
    }
    if(jminBufSize<=0){
    	LOGI("minBufferSize is error: %d",jminBufSize);
    	return 0;
    }
    LOGI("channelnum:%d,,sampleRate: %d,,bytesPerSample: %d,,minBufferSize: %d",jchannel, jsampleRate, jBytesPerSample, jminBufSize);

//    SLDataFormat_PCM format_pcm;
//    format_pcm.formatType = SL_DATAFORMAT_PCM;
//    format_pcm.numChannels = channel;
//    format_pcm.samplesPerSec = convertSamplerate(jsampleRate);
//    format_pcm.bitsPerSample = SL_PCMSAMPLEFORMAT_FIXED_16;
//    format_pcm.containerSize = 16;
//    if(channel==2){
//        format_pcm.channelMask = SL_SPEAKER_FRONT_LEFT | SL_SPEAKER_FRONT_RIGHT;
//    }else{
//        format_pcm.channelMask = SL_SPEAKER_FRONT_CENTER;
//    }
//    format_pcm.endianness = SL_BYTEORDER_LITTLEENDIAN;
    AudioRecord *pObj = NULL;
    if(jstr){
        const char* utf8 = env->GetStringUTFChars(jstr, NULL);

    	pObj = new AudioRecord(utf8, jsampleRate, jBytesPerSample, jchannel, jminBufSize);

    	env->ReleaseStringUTFChars(jstr, utf8);
    }else{
    	pObj = new AudioRecord(NULL, jsampleRate, jBytesPerSample, jchannel, jminBufSize);
    }
    if(pObj && referObj){
    	OnFrameCallbackStruct *cbStruct = new OnFrameCallbackStruct;
    	cbStruct->callbackObj = env->NewGlobalRef(referObj);
    	cbStruct->callbackMethodId = env->GetMethodID(env->GetObjectClass(referObj), NativeFrameCallback, NativeFrameCallbackSig);
    	pObj->setOnFrameCallback(jniOnFrameCallback, (void*)cbStruct);
    }
    return (jlong)(pObj);
}

JNIEXPORT void JNICALL Java_com_audio_jni_AudioRecordJni_start
  (JNIEnv *env, jobject jobj, jlong jl){
    L_2_OBJ(jl);
    if(pObj){
        pObj->start();
    }
}

JNIEXPORT void JNICALL Java_com_audio_jni_AudioRecordJni_pause
  (JNIEnv *env, jobject jobj, jlong jl){
    L_2_OBJ(jl);
    if(pObj){
        pObj->pause();
    }
}

JNIEXPORT void JNICALL Java_com_audio_jni_AudioRecordJni_stop
  (JNIEnv *env, jobject jobj, jlong jl){
    L_2_OBJ(jl);
    if(pObj){
        pObj->stop();
    }
}


//JNIEXPORT jint JNICALL Java_com_audio_jni_AudioRecordJni_read
//  (JNIEnv *env, jobject jobj, jlong jl, jbyteArray javaAudioData, jint offsetInBytes, jint sizeInBytes){
//	jbyte* recordBuff = NULL;
//
//	L_2_OBJ(jl);
//
//	if (!javaAudioData) {
//		LOGE("Invalid Java array to store recorded audio, can't record");
//		return 0;
//	}
//	if (pObj == NULL) {
//		LOGE("Unable to retrieve AudioRecord object, can't record");
//		return 0;
//	}
//
//	recordBuff = (jbyte *)env->GetByteArrayElements(javaAudioData, NULL);
//	 if (recordBuff == NULL) {
//		LOGE("Error retrieving destination for recorded audio data, can't record");
//		return 0;
//	}
//
//	// read the new audio data from the native AudioRecord object
//	int readSize = pObj->read(recordBuff + offsetInBytes, sizeInBytes);
//
//	env->ReleaseByteArrayElements(javaAudioData, recordBuff, 0);
//
//	return readSize;
//}

JNIEXPORT void JNICALL Java_com_audio_jni_AudioRecordJni_release
  (JNIEnv *env, jobject jobj, jlong jl){
    L_2_OBJ(jl);
    if(pObj){
    	OnFrameCallbackStruct *pCb = (OnFrameCallbackStruct*)pObj->getUserData();
    	if(pCb){
    		if(pCb->callbackObj){
    			env->DeleteGlobalRef(pCb->callbackObj);
    		}
    		delete pCb;
    	}
       delete pObj;
    }
}
