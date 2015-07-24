package com.audio.jni;

public class AudioPreprocessJni {
    static {
        System.loadLibrary("speex_jni");
    }
	public AudioPreprocessJni() {
	}

	public native static boolean preprocess(String inpcmfile, String outpcmfile, int sampleRete, int bytesPerSample, int channel, int noiseSuppress);
}
