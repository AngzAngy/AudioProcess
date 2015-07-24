LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_MODULE := speex_jni

MY_SRC := ./../../speex-1.2rc1/libspeex

LOCAL_CFLAGS = -DFIXED_POINT -DUSE_KISS_FFT -DEXPORT="" -UHAVE_CONFIG_H  

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../speex-1.2rc1/include

LOCAL_SRC_FILES := $(MY_SRC)/bits.c \
		$(MY_SRC)/buffer.c \
		$(MY_SRC)/cb_search.c \
		$(MY_SRC)/exc_10_16_table.c \
		$(MY_SRC)/exc_10_32_table.c \
		$(MY_SRC)/exc_20_32_table.c \
		$(MY_SRC)/exc_5_256_table.c \
		$(MY_SRC)/exc_5_64_table.c \
		$(MY_SRC)/exc_8_128_table.c \
		$(MY_SRC)/fftwrap.c \
		$(MY_SRC)/filterbank.c \
		$(MY_SRC)/filters.c \
		$(MY_SRC)/gain_table_lbr.c \
		$(MY_SRC)/gain_table.c \
		$(MY_SRC)/hexc_10_32_table.c \
		$(MY_SRC)/hexc_table.c \
		$(MY_SRC)/high_lsp_tables.c \
		$(MY_SRC)/jitter.c \
		$(MY_SRC)/kiss_fft.c \
		$(MY_SRC)/kiss_fftr.c \
		$(MY_SRC)/lpc.c \
		$(MY_SRC)/lsp_tables_nb.c \
		$(MY_SRC)/lsp.c \
		$(MY_SRC)/ltp.c \
		$(MY_SRC)/mdf.c \
		$(MY_SRC)/modes_wb.c \
		$(MY_SRC)/modes.c \
		$(MY_SRC)/nb_celp.c \
		$(MY_SRC)/preprocess.c \
		$(MY_SRC)/quant_lsp.c \
		$(MY_SRC)/resample.c \
		$(MY_SRC)/sb_celp.c \
		$(MY_SRC)/scal.c \
		$(MY_SRC)/smallft.c \
		$(MY_SRC)/speex_callbacks.c \
		$(MY_SRC)/speex_header.c \
		$(MY_SRC)/speex.c \
		$(MY_SRC)/stereo.c \
		$(MY_SRC)/vbr.c \
		$(MY_SRC)/vq.c \
		$(MY_SRC)/window.c \
		AudioPreprocessJni.cpp

LOCAL_LDLIBS := -llog

include $(BUILD_SHARED_LIBRARY)