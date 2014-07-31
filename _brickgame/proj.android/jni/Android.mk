LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := cocos2dcpp_shared

LOCAL_MODULE_FILENAME := libcocos2dcpp

# 새로 추가 --------------------------------------------------
CLASSES_DIRECTORY := $(LOCAL_PATH)/../../Classes
# Get a list of source files
SOURCE_FILES := $(shell find $(CLASSES_DIRECTORY) -name *.cpp)
SOURCE_FILES += $(shell find $(CLASSES_DIRECTORY) -name *.hpp)
SOURCE_FILES := $(sort $(SOURCE_FILES))
SOURCE_FILES := $(subst $(LOCAL_PATH)/,,$(SOURCE_FILES))


LOCAL_SRC_FILES := hellocpp/main.cpp \
                   $(SOURCE_FILES)
# 기존 주석 처리 ---------------------------------------------
#LOCAL_SRC_FILES := hellocpp/main.cpp \
#                   ../../Classes/AppDelegate.cpp \
#                   ../../Classes/HelloWorldScene.cpp

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../Classes

LOCAL_WHOLE_STATIC_LIBRARIES := cocos2dx_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocosdenshion_static
LOCAL_WHOLE_STATIC_LIBRARIES += box2d_static


include $(BUILD_SHARED_LIBRARY)

$(call import-module,2d)
$(call import-module,audio/android)
$(call import-module,Box2D)
