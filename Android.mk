LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_CPP_EXTENSION := .cpp .cc
LOCAL_MODULE    := GiroflexVSL
LOCAL_SRC_FILES := main.cpp mod/logger.cpp mod/config.cpp Log.cpp GiroflexVSL.cpp menu/Draw.cpp menu/Menu.cpp menu/Item.cpp menu/Window.cpp Input.cpp windows/WindowTest.cpp
LOCAL_CFLAGS += -O2 -mfloat-abi=softfp -DNDEBUG -std=c++17
LOCAL_C_INCLUDES += ./include
LOCAL_LDLIBS += -llog
include $(BUILD_SHARED_LIBRARY)