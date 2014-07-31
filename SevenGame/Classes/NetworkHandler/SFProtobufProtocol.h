#pragma once
#include "SFProtocol.h"
#include "DataBuffer.h"

class SFProtobufPacketImpl;

class SFProtobufProtocol : public SFProtocol
{
public:
	SFProtobufProtocol(void);
	virtual ~SFProtobufProtocol(void);

	bool Initialize(int ioBufferSize, unsigned short packetDataSize);

	bool Reset();
	SFProtobufPacketImpl* GetPacket(int& ErrorCode);
	bool AddTransferredData(char* pBuffer, unsigned int dwTransferred);
	bool SendRequest(SFProtobufPacketImpl* pPacket);

	bool GetPacketData(SFProtobufPacketImpl* pPacket, char* buffer, const int BufferSize, unsigned int& writtenSize);

	virtual void disposeOutgoingPacket(SFProtobufPacketImpl* pPacket);
	virtual void disposeIncomingPacket(SFProtobufPacketImpl* pPacket);
	SFProtobufPacketImpl* CreatePacket(){ return 0; }
	bool DisposePacket(SFProtobufPacketImpl* pPacket);

protected:
	virtual SFProtobufPacketImpl* CreateIncomingPacketFromPacketId(int PacketId) = 0;

protected:
	virtual int encodeOutgoingPacket(SFProtobufPacketImpl& packet);
	virtual int frameOutgoingPacket(SFProtobufPacketImpl& packet, DataBuffer& buffer, unsigned int& nWrittenBytes);

	//
	virtual int tryDeframeIncomingPacket(DataBuffer& Buffer, SFProtobufPacketImpl*& pPacket, int& serviceId, unsigned int& nExtractedBytes);
	virtual int decodeIncomingPacket(SFProtobufPacketImpl* pPacket, int& PacketId);

protected:
	DataBuffer m_Buffer;
	DataBuffer oBuffer;

private:
	
};

