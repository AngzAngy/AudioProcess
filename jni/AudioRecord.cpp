#include "AudioRecord.h"
#include "jnilogger.h"
#include <stdlib.h>
#include<fcntl.h>
/* Size of the recording buffer queue */
#define NB_BUFFERS_IN_QUEUE 1

/* Explicitly requesting SL_IID_ANDROIDSIMPLEBUFFERQUEUE and SL_IID_ANDROIDCONFIGURATION
 * on the AudioRecorder object */
#define NUM_EXPLICIT_INTERFACES_FOR_RECORDER 2

/* Size of the recording buffer queue */
//#define NB_BUFFERS_IN_QUEUE 1
/* Size of each buffer in the queue */
//#define BUFFER_SIZE_IN_SAMPLES 8192
//#define BUFFER_SIZE_IN_BYTES   (2 * BUFFER_SIZE_IN_SAMPLES)

/* Local storage for Audio data */
//int8_t pcmData[NB_BUFFERS_IN_QUEUE * BUFFER_SIZE_IN_BYTES];

/* Callback for recording buffer queue events */

//#define ORG_SOUND

#ifdef ORG_SOUND
#include <string>
static FILE *gorgfile = NULL;
using namespace std;
#endif

void recCallback(SLRecordItf caller, void *pContext, SLuint32 event) {
	if (SL_RECORDEVENT_HEADATNEWPOS & event) {
		SLmillisecond pMsec = 0;
		(*caller)->GetPosition(caller, &pMsec);
		LOGI("NEWPOS current position=%ums\n", pMsec);
	}

	if (SL_RECORDEVENT_HEADATMARKER & event) {
		SLmillisecond pMsec = 0;
		(*caller)->GetPosition(caller, &pMsec);
 		LOGI("MARKER current position=%ums\n", pMsec);
	}
}

/* Callback for recording buffer queue events */
void AudioRecord::recBufferQueueCallback(SLAndroidSimpleBufferQueueItf queueItf, void *pContext) {

	AudioRecord *pCntxt = (AudioRecord*) pContext;

//	if(pCntxt->isFirst){
//		LOGI("first recBufferQueueCallback");
//		pCntxt->isFirst = false;
//	}else
//	{
		/* Save the recorded data  */
//		fwrite(pCntxt->pDataBase, BUFFER_SIZE_IN_BYTES, 1, pCntxt->pfile);
#ifdef ORG_SOUND
		if(gorgfile){
			fwrite(pCntxt->pDataBase, pCntxt->bufSize, sizeof(int8_t), gorgfile);
		}
#endif
		if(pCntxt->pfile){
			fwrite(pCntxt->pDataBase, pCntxt->bufSize, sizeof(int8_t), pCntxt->pfile);
		}

		if(pCntxt->mFrameCallback){
			pCntxt->mFrameCallback(pCntxt->pDataBase, (pCntxt->bufSize)*sizeof(int8_t), pCntxt->mUserData);
		}

	LOGI("recordbufEnqueue,,bufsize: %d", pCntxt->bufSize);

//	}

//	pCntxt->isCanRead = true;
	/* Increase data pointer by buffer size */
//	pCntxt->pData += BUFFER_SIZE_IN_BYTES;
//
//	if (pCntxt->pData >= pCntxt->pDataBase + (NB_BUFFERS_IN_QUEUE * BUFFER_SIZE_IN_BYTES)) {
//		pCntxt->pData = pCntxt->pDataBase;
//	}

//	(*queueItf)->Enqueue(queueItf, pCntxt->pDataBase, BUFFER_SIZE_IN_BYTES);
	(*queueItf)->Enqueue(queueItf, pCntxt->pDataBase, pCntxt->bufSize);

	SLAndroidSimpleBufferQueueState recQueueState;
	(*queueItf)->GetState(queueItf, &recQueueState);

}

SLuint32 AudioRecord::convertSLSamplerate(int sampleRate){
     switch(sampleRate) {
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
AudioRecord::AudioRecord(const char *fileName, int sampleRate, int bytesPerSample, int channelNumbre, int minBufferSize):
        engineObject(NULL),engineEngine(NULL),recorderObject(NULL),
        recordItf(NULL),recBuffQueueItf(NULL),configItf(NULL),
        bufSize(0), pDataBase(NULL), /*baseOffset(0), miniSize(0), */pfile(NULL)/*, isCanRead(false)*/,
        mFrameCallback(NULL),mUserData(NULL){
    SLresult result;
    SLEngineOption EngineOption[] = {
                {(SLuint32) SL_ENGINEOPTION_THREADSAFE, (SLuint32) SL_BOOLEAN_TRUE}};

    SLDataFormat_PCM format_pcm;
    format_pcm.formatType = SL_DATAFORMAT_PCM;
    format_pcm.numChannels = channelNumbre;
    format_pcm.samplesPerSec = convertSLSamplerate(sampleRate);
    format_pcm.bitsPerSample = SL_PCMSAMPLEFORMAT_FIXED_16;
    format_pcm.containerSize = 16;
    if(channelNumbre==2){
        format_pcm.channelMask = SL_SPEAKER_FRONT_LEFT | SL_SPEAKER_FRONT_RIGHT;
    }else{
        format_pcm.channelMask = SL_SPEAKER_FRONT_CENTER;
    }
    format_pcm.endianness = SL_BYTEORDER_LITTLEENDIAN;

    if(fileName){
		pfile = fopen(fileName, "w");
		if(!pfile){
			LOGE("Open file error,,fileName: %s", fileName);
			return;
		}
    }else{
    	LOGI("not write file in native");
    }
#ifdef ORG_SOUND
    string orgfn(fileName);
    orgfn+=".org";
    gorgfile = fopen(orgfn.c_str(), "w");
#endif
    result = slCreateEngine(&engineObject, 1, EngineOption, 0, NULL, NULL);
    assert(SL_RESULT_SUCCESS == result);

     /* Realizing the SL Engine in synchronous mode. */
    result = (*engineObject)->Realize(engineObject, SL_BOOLEAN_FALSE);
    assert(SL_RESULT_SUCCESS == result);

    // get the engine interface, which is needed in order to create other objects
    result = (*engineObject)->GetInterface(engineObject, SL_IID_ENGINE, &engineEngine);
    assert(SL_RESULT_SUCCESS == result);

    /* setup the data source*/
        SLDataLocator_IODevice ioDevice = {
                SL_DATALOCATOR_IODEVICE,
                SL_IODEVICE_AUDIOINPUT,
                SL_DEFAULTDEVICEID_AUDIOINPUT,
                NULL
        };

        SLDataSource recSource = {&ioDevice, NULL};

        SLDataLocator_AndroidSimpleBufferQueue recBufferQueue = {
                SL_DATALOCATOR_ANDROIDSIMPLEBUFFERQUEUE,
                NB_BUFFERS_IN_QUEUE
        };

        SLDataSink dataSink = { &recBufferQueue, &format_pcm };
        SLInterfaceID iids[NUM_EXPLICIT_INTERFACES_FOR_RECORDER] = {SL_IID_ANDROIDSIMPLEBUFFERQUEUE, SL_IID_ANDROIDCONFIGURATION};
        SLboolean required[NUM_EXPLICIT_INTERFACES_FOR_RECORDER] = {SL_BOOLEAN_TRUE, SL_BOOLEAN_TRUE};

        /* Create the audio recorder */
        result = (*engineEngine)->CreateAudioRecorder(engineEngine, &recorderObject , &recSource, &dataSink,
                NUM_EXPLICIT_INTERFACES_FOR_RECORDER, iids, required);
        assert(SL_RESULT_SUCCESS == result);


        /* get the android configuration interface*/
        result = (*recorderObject)->GetInterface(recorderObject, SL_IID_ANDROIDCONFIGURATION, &configItf);
        assert(SL_RESULT_SUCCESS == result);

        /* Realize the recorder in synchronous mode. */
        result = (*recorderObject)->Realize(recorderObject, SL_BOOLEAN_FALSE);
        assert(SL_RESULT_SUCCESS == result);

        /* Get the buffer queue interface which was explicitly requested */
        result = (*recorderObject)->GetInterface(recorderObject, SL_IID_ANDROIDSIMPLEBUFFERQUEUE, (void*) &recBuffQueueItf);
        assert(SL_RESULT_SUCCESS == result);


        /* get the record interface */
        result = (*recorderObject)->GetInterface(recorderObject, SL_IID_RECORD, &recordItf);
        assert(SL_RESULT_SUCCESS == result);


        /* Set up the recorder callback to get events during the recording */
        result = (*recordItf)->SetMarkerPosition(recordItf, 2000);
        assert(SL_RESULT_SUCCESS == result);

        result = (*recordItf)->SetPositionUpdatePeriod(recordItf, 500);
        assert(SL_RESULT_SUCCESS == result);

        result = (*recordItf)->SetCallbackEventsMask(recordItf,
                    SL_RECORDEVENT_HEADATMARKER | SL_RECORDEVENT_HEADATNEWPOS);
        assert(SL_RESULT_SUCCESS == result);

        result = (*recordItf)->RegisterCallback(recordItf, recCallback, NULL);
        assert(SL_RESULT_SUCCESS == result);

        /* Initialize the callback and its context for the recording buffer queue */
//        miniSize = minBufferSize;
        bufSize = minBufferSize * sizeof(int8_t);
        pDataBase = (int8_t*)malloc(bufSize);
//        baseOffset = 0;

        result = (*recBuffQueueItf)->RegisterCallback(recBuffQueueItf, recBufferQueueCallback, this);
        assert(SL_RESULT_SUCCESS == result);

//
//        /* Enqueue buffers to map the region of memory allocated to store the recorded data */
//        LOGI("Enqueueing buffer ");
//        for (int i = 0; i < NB_BUFFERS_IN_QUEUE; i++) {
//            LOGI("%d ", i);
//            result = (*recBuffQueueItf)->Enqueue(recBuffQueueItf, ctx.pData, BUFFER_SIZE_IN_BYTES);
//            assert(SL_RESULT_SUCCESS == result);
//            ctx.pData += BUFFER_SIZE_IN_BYTES;
//        }
//        ctx.pData = ctx.pDataBase;

}

AudioRecord::~AudioRecord(){
    release();
}

void AudioRecord::start(){
    SLresult result;
    if (recordItf != NULL) {
        // in case already recording, stop recording and clear buffer queue
        result = (*recordItf)->SetRecordState(recordItf, SL_RECORDSTATE_STOPPED);
        result = (*recBuffQueueItf)->Clear(recBuffQueueItf);


        /* Enqueue buffers to map the region of memory allocated to store the recorded data */
        LOGI("Enqueueing buffer ");
//        for (int i = 0; i < NB_BUFFERS_IN_QUEUE; i++) {
//            LOGI("%d ", i);
//            result = (*recBuffQueueItf)->Enqueue(recBuffQueueItf, ctx.pData, BUFFER_SIZE_IN_BYTES);
//    	if(baseOffset > bufSize){
//    		baseOffset = 0;
//    	}
            result = (*recBuffQueueItf)->Enqueue(recBuffQueueItf, pDataBase, bufSize);
            assert(SL_RESULT_SUCCESS == result);
//            ctx.pData += BUFFER_SIZE_IN_BYTES;
//        }
//        ctx.pData = ctx.pDataBase;

        result = (*recordItf)->SetRecordState(recordItf, SL_RECORDSTATE_RECORDING);
        assert(SL_RESULT_SUCCESS == result);
        LOGI("Start to record");
    }

}
void AudioRecord::pause(){
    SLresult result;
//    isCanRead = false;
    if (recordItf != NULL) {
        result = (*recordItf)->SetRecordState(recordItf, SL_RECORDSTATE_PAUSED);
        assert(SL_RESULT_SUCCESS == result);
        LOGI("pause to record");
    }
}
void AudioRecord::stop(){
//	isCanRead = false;
    if (recordItf != NULL) {
        SLresult result = (*recordItf)->SetRecordState(recordItf, SL_RECORDSTATE_STOPPED);
        assert(SL_RESULT_SUCCESS == result);
        LOGI("Stop to record");
    }
}

void AudioRecord::setOnFrameCallback(onFrameCallback cb, void* userData){
	mFrameCallback = cb;
	mUserData = userData;
}

int AudioRecord::read(void *buf, int size){

	return size;
}

void AudioRecord::release(){
//	isCanRead = false;
    //destroy recorder object , and invlidate all associated interfaces
    if (recorderObject != NULL) {
        (*recorderObject)->Destroy(recorderObject);
        recorderObject = NULL;
        recordItf = NULL;
        recBuffQueueItf = NULL;
        configItf = NULL;
    }

    // destroy engine object, and invalidate all associated interfaces
    if (engineObject != NULL) {
        (*engineObject)->Destroy(engineObject);
        engineObject = NULL;
        engineEngine = NULL;
    }

    //colse the file
    if ((pfile != NULL)) {
        fclose(pfile);
        pfile == NULL;
    }
    if(pDataBase != NULL){
    	free(pDataBase);
    	pDataBase=NULL;
    }
    LOGI("release record");
}
