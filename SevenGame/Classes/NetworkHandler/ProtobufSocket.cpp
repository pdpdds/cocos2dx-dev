#include "SFProtobufPacketImpl.h"
#include "ProtobufSocket.h"
#include "SevenGameProtocol.h"
#include "ProtobufHandler.h"
#include "SevenGamePacketID.h"
#include "SFPacketID.h"

ProtobufSocket::ProtobufSocket(ISocketHandler& h)
:TcpSocket(h)
{
	SetLineProtocol(false);

	m_pProtocol = new SevenGameProtocol(this);
	m_pProtocol->Initialize(16384, 8096);
}

ProtobufSocket::~ProtobufSocket()
{
}

void ProtobufSocket::OnConnect()
{
	printf("Welcome.\n");

}

void ProtobufSocket::OnRawData(const char *buf, size_t len)
{
	bool bRet = m_pProtocol->AddTransferredData((char*)buf, len);

	if (false == bRet)
	{
		return;
	}

	int iErrorCode = 0;

	do
	{
		SFProtobufPacketImpl* pPacket = m_pProtocol->GetPacket(iErrorCode);

		if (NULL == pPacket)
			break;

		switch (pPacket->getServiceId())
		{
			case CGSF::Auth:
				static_cast<ProtobufHandler&>(Handler()).OnAuth(this, pPacket);			
				break;

			case CGSF::RoomList:
				static_cast<ProtobufHandler&>(Handler()).OnRoomList(this, pPacket);
				break;

			case CGSF::LoginSuccess:
				static_cast<ProtobufHandler&>(Handler()).OnLoginSuccess(this, pPacket);
				break;

			case CGSF::LoginFail:
				static_cast<ProtobufHandler&>(Handler()).OnLoginFail(this, pPacket);
				break;

			case CGSF::EnterLobby:
				static_cast<ProtobufHandler&>(Handler()).OnEnterLobby(this, pPacket);
				break;
		}

		delete pPacket;
		
	} while (true);
}

/*bool ProtobufSocket::GetPacket(SFPacket* pPacket, int& ErrorCode)
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
}*/

