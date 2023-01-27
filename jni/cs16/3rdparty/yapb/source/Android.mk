LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

include $(XASH3D_CONFIG)

LOCAL_MODULE := yapb

ifeq ($(TARGET_ARCH_ABI),armeabi-v7a-hard)
LOCAL_MODULE_FILENAME = libyapb_hardfp
endif

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../include \

LOCAL_EXPORT_C_INCLUDES := $(LOCAL_C_INCLUDES)

LOCAL_SRC_FILES := \
	config.cpp \
	botlib.cpp \
	chatlib.cpp \
	combat.cpp \
	entities.cpp \
	engine.cpp \
	graph.cpp \
	manager.cpp \
	linkage.cpp \
	navigate.cpp \
	message.cpp \
	module.cpp \
	support.cpp \
	control.cpp \

LOCAL_CFLAGS += -O2 -std=c++11 -DLINUX -D_LINUX -DPOSIX -pipe -fno-strict-aliasing -Wall -Werror
LOCAL_CPPFLAGS += -fno-exceptions -fno-rtti

include $(BUILD_SHARED_LIBRARY)
