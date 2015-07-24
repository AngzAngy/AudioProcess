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
     * @param sampleRate audio sample rate(eg 44100)
     * @param bytesPerSample number bytes of one sample,MUST 1 or 2
     * @param channel @see CHANNEL_1 @see CHANNEL_2
     */
    public AudioRecordJni(String fileName, int sampleRate, int bytePerSample, int channel, int minBufferSize){
        mNativeObj = init(fileName, sampleRate, bytePerSample, channel, minBufferSize);
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

    private static native long init(String fileName, int sampleRate, int bytesPerSample, int channel, int minBufferSize);
    private static native void start(long nativeObj);
    private static native void pause(long nativeObj);
    private static native void stop(long nativeObj);
    private static native void release(long nativeObj);
}
