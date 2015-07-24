LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_MODULE := speexdspjni

MY_SRC := ./../../speexdsp/libspeexdsp

LOCAL_CFLAGS = -DFIXED_POINT -DUSE_KISS_FFT -DEXPORT="" -UHAVE_CONFIG_H -DHAVE_STDINT_H

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../speexdsp/include

LOCAL_SRC_FILES := $(MY_SRC)/buffer.c \
		$(MY_SRC)/fftwrap.c \
		$(MY_SRC)/filterbank.c \
		$(MY_SRC)/jitter.c \
		$(MY_SRC)/kiss_fft.c \
		$(MY_SRC)/kiss_fftr.c \
		$(MY_SRC)/mdf.c \
		$(MY_SRC)/preprocess.c \
		$(MY_SRC)/resample.c \
		$(MY_SRC)/scal.c \
		$(MY_SRC)/smallft.c \
		AudioPreprocessJni.cpp

LOCAL_LDLIBS := -llog

include $(BUILD_SHARED_LIBRARY)