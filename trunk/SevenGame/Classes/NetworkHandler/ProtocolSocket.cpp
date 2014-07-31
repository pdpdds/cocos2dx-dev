
#include "ProtocolHandler.h"
#include "ProtocolSocket.h"
#include "IOBuffer.h"
#include "SFPacket.h"

#define PACKETIO_SIZE					8096
#define PACKET_SAMPLE1 10000

ProtocolSocket::ProtocolSocket(ISocketHandler& h)
:TcpSocket(h)
{
	SetLineProtocol(false);

	m_pIOBuffer = new IOBuffer();
	m_pIOBuffer->AllocIOBuf(PACKETIO_SIZE);	
}

ProtocolSocket::~ProtocolSocket()
{
	if (m_pIOBuffer)
		delete m_pIOBuffer;

	m_pIOBuffer = 0;
}

void ProtocolSocket::OnConnect()
{	
	printf("Welcome.\n");	

	SFPacket packet(PACKET_SAMPLE1);
	float speed = 10.0f;
	float move = 235.0f;
	packet << speed << move;
	
	this->SendBuf((const char*)packet.GetHeader(), packet.GetPacketSize());
}

void ProtocolSocket::OnRawData(const char *buf, size_t len)
{
	m_pIOBuffer->AppendData(buf, len);

	SFPacket packet;
	int errorCode = 0;

	if (false == GetPacket(&packet, errorCode))
		return;
	
	if (packet.GetHeader()->packetID == PACKET_SAMPLE1)
	{
		static_cast<ProtocolHandler&>(Handler()).OnSample1(this, &packet);
	}
}

bool ProtocolSocket::GetPacket(SFPacket* pPacket, int& ErrorCode)
{
	ErrorCode = PACKETIO_ERROR_NONE;

	unsigned short HeaderSize = pPacket->GetHeaderSize();

	if (m_pIOBuffer->GetUsedBufferSize() < HeaderSize)
	{
		return false;
	}

	if (HeaderSize != m_pIOBuffer->GetData((char*)pPacket->GetHeader(), HeaderSize))
	{
		ErrorCode = PACKETIO_ERROR_HEADER;
	}

	int PacketSize = pPacket->GetPacketSize();

	int dataSize = PacketSize - HeaderSize;

	if (dataSize > MAX_PACKET_DATA)
	{
		ErrorCode = PACKETIO_ERROR_DATA;
		return false;
	}

	if (m_pIOBuffer->GetUsedBufferSize() < dataSize)
	{
		return false;
	}

	int getDataSize = m_pIOBuffer->GetData((char*)pPacket->GetDataBuffer(), dataSize);

	if (dataSize != getDataSize)
	{
		ErrorCode = PACKETIO_ERROR_DATA;
		return false;
	}

	return true;
}

