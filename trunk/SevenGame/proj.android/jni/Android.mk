LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := cocos2dcpp_shared

LOCAL_MODULE_FILENAME := libcocos2dcpp

LOCAL_SRC_FILES := hellocpp/main.cpp \
                   ../../Classes/AppDelegate.cpp \
                   ../../Classes/HelloWorldScene.cpp \
				   ../../Classes/sockets/Base64.cpp \
					../../Classes/sockets/Mutex.cpp \
					../../Classes/sockets/Parse.cpp \
					../../Classes/sockets/Semaphore.cpp \
					../../Classes/sockets/Exception.cpp \
					../../Classes/sockets/File.cpp \
					../../Classes/sockets/FileStream.cpp \
					../../Classes/sockets/Ipv4Address.cpp \
					../../Classes/sockets/Ipv6Address.cpp \
					../../Classes/sockets/Lock.cpp \
					../../Classes/sockets/MemFile.cpp \
					../../Classes/sockets/Utility.cpp	\
					../../Classes/sockets/SSLInitializer.cpp \
					../../Classes/sockets/IEventOwner.cpp \
					../../Classes/sockets/Socket.cpp \
					../../Classes/sockets/Debug.cpp \
					../../Classes/sockets/Event.cpp \
					../../Classes/sockets/EventTime.cpp \
					../../Classes/sockets/EventHandler.cpp \
					../../Classes/sockets/ResolvServer.cpp \
					../../Classes/sockets/ResolvSocket.cpp \
					../../Classes/sockets/SocketHandler.cpp \
					../../Classes/sockets/SocketHandlerEp.cpp \
					../../Classes/sockets/SocketHandlerThread.cpp \
					../../Classes/sockets/SocketStream.cpp \
					../../Classes/sockets/SocketThread.cpp \
					../../Classes/sockets/StdoutLog.cpp \
					../../Classes/sockets/StreamSocket.cpp \
					../../Classes/sockets/StreamWriter.cpp \
					../../Classes/sockets/TcpSocket.cpp \
					../../Classes/sockets/Thread.cpp	\
					../../Classes/sockets/UdpSocket.cpp	\
					../../Classes/NetworkHandler/IOBuffer.cpp	\
					../../Classes/NetworkHandler/ProtocolHandler.cpp	\
					../../Classes/NetworkHandler/ProtocolSocket.cpp	\
					../../Classes/NetworkHandler/SFPacket.cpp	\
					../../Classes/NetworkHandler/SevenGamePacket.pb.cc	\
					../../Classes/NetworkHandler/PacketCore.pb.cc	\
					../../Classes/NetworkHandler/SFPacketStore.pb.cc

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../Classes
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../../../external/protobuf/src

LOCAL_WHOLE_STATIC_LIBRARIES += cocos2dx_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocosdenshion_static
LOCAL_WHOLE_STATIC_LIBRARIES += box2d_static
LOCAL_WHOLE_STATIC_LIBRARIES += chipmunk_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocos_extension_static
LOCAL_WHOLE_STATIC_LIBRARIES += protobuf

include $(BUILD_SHARED_LIBRARY)

$(call import-module,cocos2dx)
$(call import-module,cocos2dx/platform/third_party/android/prebuilt/libcurl)
$(call import-module,CocosDenshion/android)
$(call import-module,extensions)
$(call import-module,external/Box2D)
$(call import-module,external/chipmunk)
$(call import-module,external/protobuf)
