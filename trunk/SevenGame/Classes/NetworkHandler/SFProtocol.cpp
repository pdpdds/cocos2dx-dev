#include "SFProtobufPacketImpl.h"
#include "SFProtocol.h"

SFProtocol::SFProtocol(void)
{
}

SFProtocol::~SFProtocol(void)
{
}

int SFProtocol::serializeOutgoingPacket(SFProtobufPacketImpl& packet, DataBuffer& Buffer, unsigned int& nWrittenBytes)
{
	int nRet = encodeOutgoingPacket(packet);
	if(nRet != Success)
		return nRet;
	return frameOutgoingPacket(packet, Buffer, nWrittenBytes);
}

int SFProtocol::tryDeserializeIncomingPacket(DataBuffer& Buffer, SFProtobufPacketImpl*& pPacket, int& serviceId, unsigned int& nExtractedBytes)
{
	int nRet = tryDeframeIncomingPacket(Buffer, pPacket, serviceId, nExtractedBytes);
	if(nRet != Success)
		return nRet;

	return decodeIncomingPacket(pPacket, serviceId);
}