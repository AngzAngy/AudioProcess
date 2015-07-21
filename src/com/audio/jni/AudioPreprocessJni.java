package com.audio.jni;

public class AudioPreprocessJni {
    static {
        System.loadLibrary("speexdspjni");
    }
	public AudioPreprocessJni() {
	}

	public native static boolean preprocess(String inpcmfile, String outpcmfile, int sampleRete, int channel, int noiseSuppress);
}
