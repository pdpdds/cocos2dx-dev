#pragma once
#include "sockets/SocketHandler.h"

class ProtobufSocket;
class SFProtobufPacketImpl;

class ProtobufHandler : public SocketHandler
{
public:
	ProtobufHandler();
	virtual ~ProtobufHandler();

	void OnAuth(ProtobufSocket *p, SFProtobufPacketImpl* pPacket);
	void OnLoginSuccess(ProtobufSocket *p, SFProtobufPacketImpl* pPacket);
	void OnLoginFail(ProtobufSocket *p, SFProtobufPacketImpl* pPacket);
	void OnEnterLobby(ProtobufSocket *p, SFProtobufPacketImpl* pPacket);
	void OnRoomList(ProtobufSocket *p, SFProtobufPacketImpl* pPacket);
};

