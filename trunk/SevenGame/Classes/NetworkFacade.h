#pragma once

class ProtobufHandler;
class ProtobufSocket;
class SFProtobufPacketImpl;

class NetworkFacade
{
public:	
	virtual ~NetworkFacade();

	static NetworkFacade* GetInstance()
	{
		if (m_pNetworkFacade == 0)
		{
			m_pNetworkFacade = new NetworkFacade();
		}

		return m_pNetworkFacade;
	}

	bool InitNetwork();
	bool TCPSend(SFProtobufPacketImpl* pPacket);
	bool Poll();

protected:

private:
	NetworkFacade();

	static NetworkFacade* m_pNetworkFacade;

	ProtobufSocket* m_pSocket;
	ProtobufHandler* m_pHandler;
};

