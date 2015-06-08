#include "AudioRecordJni.h"
#include "jnilogger.h"
#include "AudioRecord.h"
#define L_2_OBJ(jl) AudioRecord *pObj = (AudioRecord *)(jl);

static SLuint32 convertSamplerate(int samplerate){
     switch(samplerate) {
     case 8000:
         return SL_SAMPLINGRATE_8;
         break;
     case 11025:
         return SL_SAMPLINGRATE_11_025;
         break;
     case 16000:
         return SL_SAMPLINGRATE_16;
         break;
     case 22050:
         return SL_SAMPLINGRATE_22_05;
         break;
     case 24000:
         return SL_SAMPLINGRATE_24;
         break;
     case 32000:
         return SL_SAMPLINGRATE_32;
         break;
     case 44100:
         return SL_SAMPLINGRATE_44_1;
         break;
     case 48000:
         return SL_SAMPLINGRATE_48;
         break;
     case 64000:
         return SL_SAMPLINGRATE_64;
         break;
     case 88200:
         return SL_SAMPLINGRATE_88_2;
         break;
     case 96000:
         return SL_SAMPLINGRATE_96;
         break;
     case 192000:
         return SL_SAMPLINGRATE_192;
         break;
     default:
         return -1;
     }
 }
JNIEXPORT jlong JNICALL Java_com_audio_jni_AudioRecordJni_init
  (JNIEnv *env, jobject jobj, jstring jstr, jint jchannel, jint jsampleRate){
    if(!jstr){
        LOGI("filename is null");
        return 0;
    }
    if(jchannel!=1 && jchannel!=2){
        LOGI("channel is error num:%d",jchannel);
        return 0;
    }
    const char* utf8 = env->GetStringUTFChars(jstr, NULL);

    SLDataFormat_PCM format_pcm;
    format_pcm.formatType = SL_DATAFORMAT_PCM;
    format_pcm.numChannels = jchannel;
    format_pcm.samplesPerSec = convertSamplerate(jsampleRate);
    format_pcm.bitsPerSample = SL_PCMSAMPLEFORMAT_FIXED_16;
    format_pcm.containerSize = 16;
    if(jchannel==2){
        format_pcm.channelMask = SL_SPEAKER_FRONT_LEFT | SL_SPEAKER_FRONT_RIGHT;
    }else{
        format_pcm.channelMask = SL_SPEAKER_FRONT_CENTER;
    }
    format_pcm.endianness = SL_BYTEORDER_LITTLEENDIAN;

    AudioRecord *pObj = new AudioRecord(utf8, format_pcm);

    env->ReleaseStringUTFChars(jstr, utf8);
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


JNIEXPORT void JNICALL Java_com_audio_jni_AudioRecordJni_release
  (JNIEnv *env, jobject jobj, jlong jl){
    L_2_OBJ(jl);
    if(pObj){
       delete pObj;
    }
}
