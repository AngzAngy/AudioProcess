package com.audio.jni;

public class SoundEffectJni {
    static {
        System.loadLibrary("SoundEffectJni");
    }
	public native static void test();

}
