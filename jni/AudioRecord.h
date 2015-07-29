#include <stdio.h>
#include <assert.h>

#include <SLES/OpenSLES.h>
#include <SLES/OpenSLES_Android.h>

#include "SoundPreprocessor.h"

#ifndef __AudioRecord_H_
#define __AudioRecord_H_
typedef  void (*onFrameCallback)(void *buf, int32_t size, void* userData);
class AudioRecord{
public:
    AudioRecord(const char *fileName, int sampleRate, int bytesPerSample, int channelNumbre, int minBufferSize);
    ~AudioRecord();
    int getBufferSize(){
    	return bufSize;
    }
    void *getUserData(){
    	return mUserData;
    }
    void setOnFrameCallback(onFrameCallback cb, void* userData);
    void start();
    void pause();
    void stop();
    int read(void *buf, int size);
    void release();
private:
    static SLuint32 convertSLSamplerate(int sampleRate);
    static void recBufferQueueCallback(SLAndroidSimpleBufferQueueItf queueItf, void *pContext);

    // engine interfaces
    SLObjectItf engineObject;
    SLEngineItf engineEngine;

    //audio record interfaces
    SLObjectItf recorderObject;
    SLRecordItf recordItf;
    SLAndroidSimpleBufferQueueItf recBuffQueueItf;
    SLAndroidConfigurationItf configItf;

	uint32_t  bufSize;
    int8_t*   pDataBase;    // Base address of local audio data storage
//    uint32_t baseOffset;
//    uint32_t miniSize;
    FILE *pfile;
//    bool isCanRead;
    onFrameCallback mFrameCallback;
    void *mUserData;
};

#endif
