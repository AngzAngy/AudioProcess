LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_MODULE := aaudiojni

LOCAL_SRC_FILES := AudioRecordJni.cpp AudioRecord.cpp

LOCAL_LDLIBS := -llog  -landroid -lz -lOpenSLES

include $(BUILD_SHARED_LIBRARY)