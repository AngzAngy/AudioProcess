LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE := libAudPostprocess
LOCAL_SRC_FILES := libs/$(TARGET_ARCH_ABI)/libAudPostprocess.a
include $(PREBUILT_STATIC_LIBRARY)


include $(CLEAR_VARS)

LOCAL_MODULE := SoundEffectJni

LOCAL_SRC_FILES := SoundEffectJni.cpp

LOCAL_STATIC_LIBRARIES :=libAudPostprocess

LOCAL_LDLIBS := -llog

include $(BUILD_SHARED_LIBRARY)