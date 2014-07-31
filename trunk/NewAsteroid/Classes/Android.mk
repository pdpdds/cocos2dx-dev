LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := game_logic_static

LOCAL_MODULE_FILENAME := libgame_logic

LOCAL_SRC_FILES := AppDelegate.cpp \
                   CCParallaxNodeExtras.cpp \
                   HelloWorldScene.cpp
                   
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../android/jni/library 


LOCAL_STATIC_LIBRARIES := png_static_prebuilt
LOCAL_STATIC_LIBRARIES += xml2_static_prebuilt
LOCAL_STATIC_LIBRARIES += jpeg_static_prebuilt
LOCAL_WHOLE_STATIC_LIBRARIES += cocos2dx_static
LOCAL_WHOLE_STATIC_LIBRARIES += portable
                
LOCAL_SHARED_LIBRARIES := cocosdenshion_shared
            
include $(BUILD_STATIC_LIBRARY)

$(call import-module,cocos2dx/platform/third_party/android/modules/libpng)
$(call import-module,cocos2dx/platform/third_party/android/modules/libxml2)
$(call import-module,cocos2dx/platform/third_party/android/modules/libjpeg)

