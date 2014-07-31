#include "NetworkFacade.h"
#include "SFProtobufPacketImpl.h"
#include "SevenGameProtocol.h"

NetworkFacade* NetworkFacade::m_pNetworkFacade = 0;

#include "NetworkHandler/ProtobufHandler.h"
#include "NetworkHandler/ProtobufSocket.h"

#define PROTOCOL_SERVER_IP  "127.0.0.1"
#define PROTOCOL_SERVER_PORT 25251

NetworkFacade::NetworkFacade()
{
}


NetworkFacade::~NetworkFacade()
{
}

bool NetworkFacade::InitNetwork()
{
	m_pHandler = new ProtobufHandler();
	m_pSocket = new ProtobufSocket(*m_pHandler);
	m_pSocket->Open(PROTOCOL_SERVER_IP, PROTOCOL_SERVER_PORT);
	// Add after Open
	m_pHandler->Add(m_pSocket);
	m_pHandler->Select(1, 0);
	

	return true;
}

bool NetworkFacade::Poll()
{
	if (m_pHandler->GetCount())
	{
		m_pHandler->Select(1, 0);
		return true;
	}

	return false;
}

bool NetworkFacade::TCPSend(SFProtobufPacketImpl* pPacket)
{
	SevenGameProtocol* pProtocol = m_pSocket->GetSevenGameProtocol();
	pProtocol->TCPSend(pPacket);

	return true;
}