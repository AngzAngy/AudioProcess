package com.audio.demo;

import java.io.File;

import android.app.Activity;
import android.media.AudioFormat;
import android.os.Bundle;
import android.os.Environment;
import android.view.View;

import com.audio.jni.AudioRecordJni;

public class AudioPlayerActivity extends Activity implements
		View.OnClickListener {
	private final static String TAG = AudioPlayerActivity.class.getSimpleName();
	private String mSaveFileName;
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
		findViewById(R.id.play_recoard).setOnClickListener(this);
		mSaveFileName = getBufferDir();
		sampleRate = 44100;
		channel = 2;
	}

	public String getBufferDir() {
		File dir = new File(Environment.getExternalStorageDirectory()
				.getAbsolutePath() + "/DCIM");
		if (!dir.exists()) {
			dir.mkdirs();
		}

		File fn = new File(dir, "mytest.pcm");
		return fn.getAbsolutePath();
	}

	@Override
	public void onClick(View v) {
		switch (v.getId()) {
		case R.id.start_recoard:
			if (mRecordJni == null) {
				if (channel == 1) {
					mRecordJni = new AudioRecordJni(mSaveFileName,
							AudioRecordJni.CHANNEL_1, sampleRate);
				} else {
					mRecordJni = new AudioRecordJni(mSaveFileName,
							AudioRecordJni.CHANNEL_2, sampleRate);
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
		}
	}
}