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
    private OnFrameCallback mOnFrameCallback;
    
    /**
     * @param fileName You save audio file.If is null, not save record file in native.
     * @param sampleRate audio sample rate(eg 44100)
     * @param bytesPerSample number bytes of one sample,MUST 1 or 2
     * @param channel @see CHANNEL_1 @see CHANNEL_2
     */
    public AudioRecordJni(String fileName, int sampleRate, int bytePerSample, int channel, int minBufferSize){
        mNativeObj = init(this, fileName, sampleRate, bytePerSample, channel, minBufferSize);
    }
    
    public void setOnFrameCallback(OnFrameCallback cb){
    	mOnFrameCallback = cb;
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

    
    public interface OnFrameCallback{
    	public void onFrameCallbackBuffer(byte[]data, int size);
    }
    
    private void nativeOnFrameCallback(byte[]data, int size){
    	if(mOnFrameCallback!=null){
    		mOnFrameCallback.onFrameCallbackBuffer(data, size);
    	}
    }
    private static native long init(Object obj, String fileName, int sampleRate, int bytesPerSample, int channel, int minBufferSize);
    private static native void start(long nativeObj);
    private static native void pause(long nativeObj);
    private static native void stop(long nativeObj);
    private static native int read(long nativeObj, byte[] audioData, int offsetInBytes, int sizeInBytes);
    private static native void release(long nativeObj);
}
