#include <stdio.h>
#include <assert.h>

#include <SLES/OpenSLES.h>
#include <SLES/OpenSLES_Android.h>

#include "SoundPreprocessor.h"

#ifndef __AudioRecord_H_
#define __AudioRecord_H_

typedef struct CallbackCntxt_ {
    SLuint32   size;
    SLint8*   pDataBase;    // Base address of local audio data storage
//    SLint8*   pData;        // Current address of local audio data storage
    FILE *pfile;
//    bool isFirst;
    SoundPreprocessor *mPreproc;
} CallbackCntxt;

class AudioRecord{
public:
    AudioRecord(const char *fileName, int sampleRate, int bytesPerSample, int channelNumbre, int minBufferSize);
    ~AudioRecord();
    void start();
    void pause();
    void stop();
    void release();
private:
    static SLuint32 convertSLSamplerate(int sampleRate);
    CallbackCntxt ctx;

    // engine interfaces
    SLObjectItf engineObject;
    SLEngineItf engineEngine;

    //audio record interfaces
    SLObjectItf recorderObject;
    SLRecordItf recordItf;
    SLAndroidSimpleBufferQueueItf recBuffQueueItf;
    SLAndroidConfigurationItf configItf;
};

#endif
