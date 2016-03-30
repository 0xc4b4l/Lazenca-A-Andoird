LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_LDLIBS := -lm -llog
LOCAL_SHARED_LIBRARIES := liblog libcutils

LOCAL_DEFAULT_CPP_EXTENSION := cpp
LOCAL_MODULE := MemoryCheat
LOCAL_SRC_FILES := \
    Main.cpp \

LOCAL_CFLAGS    := -fvisibility=hidden

include $(BUILD_EXECUTABLE)