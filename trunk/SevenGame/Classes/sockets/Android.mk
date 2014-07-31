LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)

LOCAL_MODULE    := libSOCKETS
LOCAL_SRC_FILES := \
					$(LOCAL_PATH)/Base64.cpp \
					$(LOCAL_PATH)/Mutex.cpp \
					$(LOCAL_PATH)/Parse.cpp \
					$(LOCAL_PATH)/Semaphore.cpp \
					$(LOCAL_PATH)/Exception.cpp \
					$(LOCAL_PATH)/File.cpp \
					$(LOCAL_PATH)/FileStream.cpp \
					$(LOCAL_PATH)/Ipv4Address.cpp \
					$(LOCAL_PATH)/Ipv6Address.cpp \
					$(LOCAL_PATH)/Lock.cpp \
					$(LOCAL_PATH)/MemFile.cpp \
					$(LOCAL_PATH)/Utility.cpp	\
					$(LOCAL_PATH)/SSLInitializer.cpp \
					$(LOCAL_PATH)/IEventOwner.cpp \
					$(LOCAL_PATH)/Socket.cpp \
					$(LOCAL_PATH)/Debug.cpp \
					$(LOCAL_PATH)/Event.cpp \
					$(LOCAL_PATH)/EventTime.cpp \
					$(LOCAL_PATH)/EventHandler.cpp \
					$(LOCAL_PATH)/ResolvServer.cpp \
					$(LOCAL_PATH)/ResolvSocket.cpp \
					$(LOCAL_PATH)/SocketHandler.cpp \
					$(LOCAL_PATH)/SocketHandlerEp.cpp \
					$(LOCAL_PATH)/SocketHandlerThread.cpp \
					$(LOCAL_PATH)/SocketStream.cpp \
					$(LOCAL_PATH)/SocketThread.cpp \
					$(LOCAL_PATH)/StdoutLog.cpp \
					$(LOCAL_PATH)/StreamSocket.cpp \
					$(LOCAL_PATH)/StreamWriter.cpp \
					$(LOCAL_PATH)/TcpSocket.cpp \
					$(LOCAL_PATH)/Thread.cpp	\
					$(LOCAL_PATH)/UdpSocket.cpp
					
#		Ajp13Socket.cpp \
#		AjpBaseSocket.cpp \
#		HTTPSocket.cpp \
#		HttpBaseSocket.cpp \
#		HttpClientSocket.cpp \
#		HttpDebugSocket.cpp \
#		HttpGetSocket.cpp \
#		HttpPostSocket.cpp \
#		HttpPutSocket.cpp \
#		HttpRequest.cpp \
#		HttpResponse.cpp \
#		HttpTransaction.cpp \
#		HttpdCookies.cpp \
#		HttpdForm.cpp \
#		HttpdSocket.cpp \
#		Json.cpp \
#		SctpSocket.cpp \
#		SmtpdSocket.cpp \
#		XmlDocument.cpp \
#		XmlNode.cpp \
#		XmlException.cpp					
#		socket_include.cpp \

LOCAL_C_INCLUDES := $(LOCAL_PATH)/

include $(BUILD_SHARED_LIBRARY)
#include $(BUILD_STATIC_LIBRARY)


                                                       