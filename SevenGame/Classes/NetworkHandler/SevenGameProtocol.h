#pragma once
#include "SFProtobufProtocol.h"

class SFProtobufPacketImpl;
class ProtobufSocket;

class SevenGameProtocol : public SFProtobufProtocol
{
public:
	SevenGameProtocol(ProtobufSocket* pSocket);
	virtual ~SevenGameProtocol(void);

	SFProtobufPacketImpl* CreateIncomingPacketFromPacketId(int PacketId) override;
	bool TCPSend(SFProtobufPacketImpl* pPacket);

	ProtobufSocket* m_pSocket;
};


