package com.audio.demo;

import java.io.File;

import android.app.Activity;
import android.media.AudioFormat;
import android.media.AudioRecord;
import android.os.Bundle;
import android.os.Environment;
import android.os.Process;
import android.view.View;
import android.widget.Toast;

import com.audio.jni.AudioPreprocessJni;
import com.audio.jni.AudioRecordJni;
import com.audio.jni.SoundEffectJni;

public class AudioPlayerActivity extends Activity implements
		View.OnClickListener {
	private final static String TAG = AudioPlayerActivity.class.getSimpleName();
	private String mSaveFileName;
	private String mDenoiseFileName;
	private int sampleRate;
	private int channel;
	private AudioRecordJni mRecordJni;

	@Override
	public void onCreate(Bundle icicle) {
		super.onCreate(icicle);
		setContentView(R.layout.main);
		findViewById(R.id.start_recoard).setOnClickListener(this);
		findViewById(R.id.pause_recoard).setOnClickListener(this);
		findViewById(R.id.stop_recoard).setOnClickListener(this);
		findViewById(R.id.denoise_recoard).setOnClickListener(this);
		findViewById(R.id.play_recoard).setOnClickListener(this);
		mSaveFileName = getBufferDir()+"/myorig.pcm";
		mDenoiseFileName = getBufferDir()+"/myproc.pcm";
		sampleRate = 44100;
		channel = 1;
	}

	public void onDestroy(){
		super.onDestroy();
		if (mRecordJni != null) {
			mRecordJni.stop();
			mRecordJni.release();
			mRecordJni = null;
		}
		Process.killProcess(Process.myPid());
	}

	public String getBufferDir() {
		File dir = new File(Environment.getExternalStorageDirectory()
				.getAbsolutePath() + "/DCIM");
		if (!dir.exists()) {
			dir.mkdirs();
		}
		return dir.getAbsolutePath();
	}

	@Override
	public void onClick(View v) {
		switch (v.getId()) {
		case R.id.start_recoard:
			android.util.Log.e("javaLog", "java start record");
			if (mRecordJni == null) {
				if (channel == 1) {
					int minBufferSize = AudioRecord.getMinBufferSize(sampleRate, AudioFormat.CHANNEL_IN_MONO, AudioFormat.ENCODING_PCM_16BIT);
					mRecordJni = new AudioRecordJni(mSaveFileName,
							AudioRecordJni.CHANNEL_1, sampleRate, minBufferSize);
				} else {
					int minBufferSize = AudioRecord.getMinBufferSize(sampleRate, AudioFormat.CHANNEL_IN_STEREO, AudioFormat.ENCODING_PCM_16BIT);
					mRecordJni = new AudioRecordJni(mSaveFileName,
							AudioRecordJni.CHANNEL_2, sampleRate, minBufferSize);
				}
			}
			mRecordJni.start();
			break;
		case R.id.pause_recoard:
			if (mRecordJni != null) {
				mRecordJni.pause();
			}
			break;
		case R.id.stop_recoard:
			if (mRecordJni != null) {
				mRecordJni.stop();
				mRecordJni.release();
				mRecordJni = null;
			}
			break;
		case R.id.denoise_recoard:
			Thread t = new Thread(){
				public void run(){
//					int minBufferSize = 0;
//					if (channel == 1) {
//						minBufferSize = AudioRecord.getMinBufferSize(sampleRate, AudioFormat.CHANNEL_IN_MONO, AudioFormat.ENCODING_PCM_16BIT);
//					} else {
//						minBufferSize = AudioRecord.getMinBufferSize(sampleRate, AudioFormat.CHANNEL_IN_STEREO, AudioFormat.ENCODING_PCM_16BIT);
//					}
//					for(int i=-50;i<-1;i++){
					int i = -1;
						mDenoiseFileName = getBufferDir()+"/myproc_"+i+".pcm";
						AudioPreprocessJni.preprocess(mSaveFileName, mDenoiseFileName, sampleRate, channel, i);
//					}
					runOnUiThread(new Runnable() {
						
						@Override
						public void run() {
							Toast.makeText(AudioPlayerActivity.this, "denoise finish", Toast.LENGTH_LONG).show();
						}
					});
				}
			};
			t.start();
			break;
		case R.id.play_recoard:
			MyAudioTrack track = null;
			if (channel == 1) {
				track = new MyAudioTrack(sampleRate,
						AudioFormat.CHANNEL_CONFIGURATION_MONO,
						AudioFormat.ENCODING_PCM_16BIT, mSaveFileName);
			} else {
				track = new MyAudioTrack(sampleRate,
						AudioFormat.CHANNEL_CONFIGURATION_STEREO,
						AudioFormat.ENCODING_PCM_16BIT, mSaveFileName);
			}

			track.play();
			break;
		}
	}
}