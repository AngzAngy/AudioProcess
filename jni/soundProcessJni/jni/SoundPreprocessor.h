#include "speex/speex_preprocess.h"

#ifndef __SoundPreprocessor_H_
#define __SoundPreprocessor_H_

class SoundPreprocessor{
public:
    static int getBytesPerFrame(int sampleRate, int bytesPerSample, int channelNumber);

	SoundPreprocessor(int sampleRate, int bytesPerSample, int channelNumbre, int minBufferSize);
    ~SoundPreprocessor();

    int getBytesPerFrame(){
    	return mBytesPerFrame;
    }

    int preprocess(void *buf, int size);
private:
    SpeexPreprocessState *mpSpStat;
    int mBytesPerFrame;
};

#endif
