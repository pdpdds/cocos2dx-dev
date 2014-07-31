
#include "ProtocolSocket.h"
#include "ProtocolHandler.h"
#include "SFPacket.h"

ProtocolHandler::ProtocolHandler()
:SocketHandler()
{
}


ProtocolHandler::~ProtocolHandler()
{
}

void ProtocolHandler::OnSample1(ProtocolSocket *p, SFPacket* pPacket)
{
	float a;
	float b;
	*pPacket >> a;
	*pPacket >> b;

	printf("%f, %f\n", a, b);
}


