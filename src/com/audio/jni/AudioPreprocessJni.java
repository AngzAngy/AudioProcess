package com.audio.jni;

public class AudioPreprocessJni {
    static {
        System.loadLibrary("kuwosoundprocess");
    }
	public AudioPreprocessJni() {
	}

	public native static boolean preprocess(String inpcmfile, String outpcmfile, int sampleRete, int bytesPerSample, int channel, int noiseSuppress);
}
