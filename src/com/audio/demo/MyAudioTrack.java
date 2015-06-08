package com.audio.demo;

import java.io.File;
import java.io.FileInputStream;

import android.media.AudioManager;
import android.media.AudioTrack;
import android.util.Log;

public class MyAudioTrack extends Thread {

	int mSampRate;

	int mChannel;

	int mSampBit;

	AudioTrack mAudioTrack;

	String mFileName;

	public MyAudioTrack(int sampRate, int channel, int sampbit, String fileName) {

		mSampRate = sampRate;

		mChannel = channel;

		mSampBit = sampbit;
		mFileName = fileName;
	}

	public void init() {

		if (mAudioTrack != null) {

			release();

		}

		int minBufSize = AudioTrack.getMinBufferSize(mSampRate,

		mChannel,

		mSampBit);

		mAudioTrack = new AudioTrack(AudioManager.STREAM_MUSIC,

		mSampRate,

		mChannel,

		mSampBit,

		minBufSize,

		AudioTrack.MODE_STREAM);

		mAudioTrack.play();

	}

	public void play() {
		start();
	}

	public void release() {

		if (mAudioTrack != null) {

			mAudioTrack.stop();

			mAudioTrack.release();

		}

	}

	public void playAudioTrack(byte[] data, int offset, int length) {

		if (data == null || data.length == 0) {

			return;

		}

		try {

			mAudioTrack.write(data, offset, length);

		} catch (Exception e) {

			// TODO: handle exception

			Log.i("MyAudioTrack", "catch exception...");

		}

	}

	public int getPrimePlaySize() {

		int minBufSize = AudioTrack.getMinBufferSize(mSampRate,

		mChannel,

		mSampBit);

		return minBufSize * 2;

	}

	public void run() {

		File file = new File(mFileName);

		FileInputStream fis = null;

		if (file.exists()) {

			byte buf[] = new byte[4096];

			init();

			try {

				fis = new FileInputStream(file);

				int r = 0;

				while ((r = fis.read(buf)) != -1) {

					mAudioTrack.write(buf, 0, r);

				}

				fis.close();

			} catch (Exception e) {

				e.printStackTrace();

			} finally {

				release();

			}

		}

	}
}
