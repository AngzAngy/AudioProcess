package com.audio.jni;

public class AudioRecordJni {
    static {
        System.loadLibrary("aaudiojni");
    }
    /*
     * supported channel number
     */
    public static final int CHANNEL_1 = 1;
    public static final int CHANNEL_2 = 2;
    private long mNativeObj;
    
    /**
     * @param fileName you save audio file.The file MUST be exist and can be accessed.
     * @param channel @see CHANNEL_1 @see CHANNEL_2
     * @param sampleRate audio sample rate(eg 44100)
     */
    public AudioRecordJni(String fileName, int channel, int sampleRate){
        mNativeObj = init(fileName, channel, sampleRate);
    }
    
    public  void start(){
        if(mNativeObj!=0){
            start(mNativeObj);
        }
    }
    public  void pause(){
        if(mNativeObj!=0){
            pause(mNativeObj);
        }
    }
    public  void stop(){
        if(mNativeObj!=0){
            stop(mNativeObj);
        }
    }
    public  void release(){
        if(mNativeObj!=0){
            release(mNativeObj);
            mNativeObj=0;
        }
    }

    private static native long init(String fileName, int ch, int sampleRate);
    private static native void start(long nativeObj);
    private static native void pause(long nativeObj);
    private static native void stop(long nativeObj);
    private static native void release(long nativeObj);
}
