LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

MY_PATH :=$(LOCAL_PATH)

LOCAL_MODULE := aaudiojni

LOCAL_SRC_FILES := AudioRecordJni.cpp AudioRecord.cpp

LOCAL_LDLIBS := -llog  -landroid -lz -lOpenSLES

include $(BUILD_SHARED_LIBRARY)

include $(MY_PATH)/soundProcessJni/jni/Android.mk
#include $(MY_PATH)/soundeffect/jni/Android.mk