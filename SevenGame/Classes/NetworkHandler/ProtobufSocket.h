#pragma once

#include "sockets/TcpSocket.h"

class SevenGameProtocol;

class ProtobufSocket : public TcpSocket
{
public:
	ProtobufSocket(ISocketHandler&);
	~ProtobufSocket();

	void OnConnect();
	virtual void OnRawData(const char *buf, size_t len);
	//bool GetPacket(SFPacket* pPacket, int& ErrorCode);

	SevenGameProtocol* GetSevenGameProtocol(){ return m_pProtocol; }

private:
	SevenGameProtocol* m_pProtocol;
};