LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
#CXXFLAGS += -fpermissive
#APP_CPPFLAGS += -fexceptions 

LOCAL_MODULE := libIMS

LCOAL_C_INCLUDES := $(LOCAL_PATH)/
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_C_INCLUDES)				

LOCAL_SRC_FILES :=  adlib.cpp \
					bnk.cpp \
					fmopl.cpp \
					Hangul.cpp \
					IMS.cpp \
					IMSManager.cpp \
					iss.cpp \
					outchip.cpp \
					pcm.cpp \
					Rol.cpp \
					setFreq.cpp \
					StringUtil.cpp

LOCAL_SHARED_LIBRARIES := SDL2 \
						  

LOCAL_CFLAGS += -DGL_GLEXT_PROTOTYPES
LOCAL_LDLIBS := -ldl -lGLESv1_CM -lGLESv2 -llog -landroid

include $(BUILD_SHARED_LIBRARY)
