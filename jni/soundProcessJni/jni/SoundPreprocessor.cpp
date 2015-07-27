#include "SoundPreprocessor.h"
#include "jnilogger.h"
#include <stdlib.h>

int SoundPreprocessor::getBytesPerFrame(int sampleRate, int bytesPerSample, int channelNumbre){
	int nBytesPerSecond = sampleRate * bytesPerSample * channelNumbre;
	int nBytesPerFrame = nBytesPerSecond / 100;
	return nBytesPerFrame;
}

SoundPreprocessor::SoundPreprocessor(int sampleRate, int bytesPerSample, int channelNumber, int denoiseLevel){
	spx_int32_t i=1;
	spx_int32_t noisesuppress=(spx_int32_t)denoiseLevel;

	mBytesPerFrame = getBytesPerFrame(sampleRate, bytesPerSample, channelNumber);

	mpSpStat = speex_preprocess_state_init(mBytesPerFrame / sizeof(short), sampleRate);

	if(mpSpStat){
		speex_preprocess_ctl(mpSpStat, SPEEX_PREPROCESS_SET_DENOISE, &i);
		speex_preprocess_ctl(mpSpStat, SPEEX_PREPROCESS_SET_NOISE_SUPPRESS, &noisesuppress);
	}else{
		mBytesPerFrame = 0;
	}
}

SoundPreprocessor::~SoundPreprocessor(){
	if(mpSpStat){
		speex_preprocess_state_destroy(mpSpStat);
	}
}

int SoundPreprocessor::preprocess(void *buf, int size){
	if(mpSpStat){
		int8_t *pTmp = (int8_t*)buf;
		if(size > mBytesPerFrame){
			int nSetup = size / mBytesPerFrame;

			for (int i = 0; i < nSetup; i++){
				speex_preprocess_run(mpSpStat, (short *)pTmp);
				pTmp += mBytesPerFrame;
			}
		}else if(size == mBytesPerFrame){
			speex_preprocess_run(mpSpStat, (short *)pTmp);
		}else{
			LOGD("func : %s,,not process,,bytesPerFrame:%d < bufSize:%d", __FUNCTION__,mBytesPerFrame, size);
		}
		return size;
	}else{
		return 0;
	}

}
