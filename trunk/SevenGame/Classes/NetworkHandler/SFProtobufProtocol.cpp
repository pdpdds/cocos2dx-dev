#include "SFProtobufPacketImpl.h"
#include "SFProtobufProtocol.h"

#define SignatureStart 16018
#define SignatureEnd 16108


#define nReceiveBufferSize 1024 * 8/*8192*/
#define nSendBufferSize 1024 * 8/*8192*/
SFProtobufProtocol::SFProtobufProtocol(void)
	:oBuffer(nSendBufferSize), m_Buffer(nReceiveBufferSize)
{
	
}


SFProtobufProtocol::~SFProtobufProtocol(void)
{

}

bool SFProtobufProtocol::Initialize(int ioBufferSize, unsigned short packetSize)
{
	//SFPacket::SetMaxPacketSize(packetSize);
	return true;
}

bool SFProtobufProtocol::AddTransferredData(char* pBuffer, unsigned int dwTransferred)
{
	m_Buffer.Append(pBuffer,dwTransferred);

	return true;
}

bool SFProtobufProtocol::Reset()
{
	return true;
}

bool SFProtobufProtocol::SendRequest(SFProtobufPacketImpl* pPacket)
{
	unsigned int uWrittenBytes = 0;
	int iResult = serializeOutgoingPacket(*pPacket, oBuffer, uWrittenBytes);
	if (iResult!= SFProtocol::Success)
	{
		return false;
	}

	unsigned int uSize = oBuffer.GetDataSize();
	//SFEngine::GetInstance()->SendInternal(pPacket->GetOwnerSerial(), oBuffer.GetBuffer(), uSize);

	oBuffer.Pop(uSize);

	return true;
}

bool SFProtobufProtocol::GetPacketData(SFProtobufPacketImpl* pPacket, char* buffer, const int BufferSize, unsigned int& writtenSize)
{
	unsigned int uWrittenBytes = 0;
	int iResult = serializeOutgoingPacket(*pPacket, oBuffer, uWrittenBytes);
	
	if (iResult!= SFProtocol::Success)
	{
		return false;
	}

	unsigned int uSize = oBuffer.GetDataSize();

	if(uSize == 0)
		return false;

	memcpy(buffer, oBuffer.GetBuffer(), uSize);
	writtenSize = uSize;

	oBuffer.Pop(uSize);

	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////
int SFProtobufProtocol::decodeIncomingPacket(SFProtobufPacketImpl* pPacket, int& serviceId)
{
	return SFProtocol::Success;
}

void SFProtobufProtocol::disposeOutgoingPacket(SFProtobufPacketImpl* pPacket)
{
	delete pPacket;
}

void SFProtobufProtocol::disposeIncomingPacket(SFProtobufPacketImpl* pPacket)
{
	delete pPacket;
}

bool SFProtobufProtocol::DisposePacket(SFProtobufPacketImpl* pPacket)
{
	delete pPacket;
	return true;
}

int SFProtobufProtocol::tryDeframeIncomingPacket(DataBuffer& Buffer, SFProtobufPacketImpl*& pPacket, int& PacketId, unsigned int& nExtractedBytes)
{
	if(Buffer.GetDataSize() < 8)
		return SFProtocol::eIncompletePacket;


    char* pBuffer = Buffer.GetBuffer();

    unsigned int sStart = 0;
    unsigned int packetLen = 0;
    PacketId = 0;
    unsigned int sEnd = 0;

    for (int i=0; i<2; i++)
    {
        *((unsigned char*)(&sStart)+i)=pBuffer[i];
		*((unsigned char*)(&packetLen) + i) = pBuffer[i + 2];
		*((unsigned char*)(&PacketId) + i) = pBuffer[i + 4];
    }

    if (sStart!=SignatureStart)
        return SFProtocol::eCorruptPacket;

    if (packetLen > Buffer.GetDataSize())
        return SFProtocol::eIncompletePacket;

    for (int i=0; i<2; i++)
		*((unsigned char*)(&sEnd) + i) = pBuffer[packetLen - 2 + i];

    if(sEnd!=SignatureEnd)
        return SFProtocol::eCorruptPacket;

    char* pData = pBuffer + 6;
    unsigned int dataSize = packetLen - 8;

    nExtractedBytes = packetLen;

	pPacket = CreateIncomingPacketFromPacketId(PacketId);

	if(pPacket==0)
		return SFProtocol::eUndefinedFailure;

	if(!pPacket->Decode(pData, dataSize))
	{
		disposeIncomingPacket(pPacket);
		return SFProtocol::eDecodingFailure;
	}

	return SFProtocol::Success;
}

SFProtobufPacketImpl* SFProtobufProtocol::GetPacket(int& ErrorCode)
{
	//The Processing Loop.
	int uCommandID;
	SFProtobufPacketImpl* pPacket = NULL;
	int iResult;
	unsigned int uExtractedBytes;

	ErrorCode = 0;

	//watch.GetElapsedTime(false);
	iResult = tryDeserializeIncomingPacket(m_Buffer , pPacket, uCommandID, uExtractedBytes);
	//iResult = pProtocol->tryDeframeIncomingPacket(pChannel->GetReceiveBuffer().GetBuffer(), pChannel->GetReceiveBuffer().GetDataSize(),uCommandID, pPacket, uExtractedBytes);
	//wcout << L"Packet deframed : " << watch.GetElapsedTime(false) << std::endl;
	if (iResult == SFProtocol::Success)
	{
		m_Buffer.Pop(uExtractedBytes);
//		pPacket->SetPacketID(uCommandID);
		pPacket->getServiceId();
	}
	else if (iResult == SFProtocol::eDecodingFailure)
	{
		m_Buffer.Pop(uExtractedBytes);
		ErrorCode = -1;

		return NULL;
	}
	
	return pPacket;

}

int SFProtobufProtocol::encodeOutgoingPacket(SFProtobufPacketImpl& gPacket)
{
	return gPacket.Encode() ? SFProtocol::Success : SFProtocol::eEncodingFailure;
}

int SFProtobufProtocol::frameOutgoingPacket(SFProtobufPacketImpl& gPacket, DataBuffer& buffer, unsigned int& nWrittenBytes)
{
	nWrittenBytes = 6 + gPacket.getEncodedStream()->size() + 2;

	if(nWrittenBytes > buffer.getRemainingSize())
		return SFProtocol::eInsufficientBuffer;
	//
	unsigned int sStart = SignatureStart;
	unsigned int packetLen = gPacket.getEncodedStreamSize() + 8;
	unsigned int commandID = gPacket.getServiceId();
	unsigned int sEnd = SignatureEnd;

	buffer.Append((char*) &sStart, 2);
	buffer.Append((char*)&packetLen, 2);
	buffer.Append((char*)&commandID, 2);

	buffer.Append((char*)gPacket.getEncodedStream()->c_str(), gPacket.getEncodedStreamSize());

	buffer.Append((char*)&sEnd, 2);


	return SFProtocol::Success;
	
}