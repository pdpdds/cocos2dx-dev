#include <NetworkHandler/SFProtobufPacketImpl.h>
#include "SevenGameProtocol.h"
#include "SFProtobufPacket.h"
#include <NetworkHandler/SFPacketID.h>
#include <NetworkHandler/SevenGamePacketID.h>
#include <NetworkHandler/PacketCore.pb.h>
#include <NetworkHandler/SFPacketStore.pb.h>
#include <NetworkHandler/SevenGamePacket.pb.h>
#include "ProtobufSocket.h"

SevenGameProtocol::SevenGameProtocol(ProtobufSocket* pSocket)
: m_pSocket(pSocket)
{

}

SevenGameProtocol::~SevenGameProtocol(void)
{
}

SFProtobufPacketImpl* SevenGameProtocol::CreateIncomingPacketFromPacketId(int PacketId)
{
	switch (PacketId)
	{
	case CGSF::Auth:
		return new SFProtobufPacket<PacketCore::Auth>(PacketId);

	case CGSF::LoginSuccess:
		return new SFProtobufPacket<SFPacketStore::LoginSuccess>(PacketId);

	case CGSF::LoginFail:
		return new SFProtobufPacket<SFPacketStore::LoginFail>(PacketId);

	case CGSF::EnterLobby:
		return new SFProtobufPacket<SFPacketStore::EnterLobby>(PacketId);

	case CGSF::CreateRoom:
		return new SFProtobufPacket<SFPacketStore::CreateRoom>(PacketId);
		break;

	case CGSF::EnterRoom:
		return new SFProtobufPacket<SFPacketStore::EnterRoom>(PacketId);

	case CGSF::LeaveRoom:
		return new SFProtobufPacket<SFPacketStore::LeaveRoom>(PacketId);

	case CGSF::EnterTeamMember:
		return new SFProtobufPacket<SFPacketStore::EnterTeamMember>(PacketId);

	case CGSF::LeaveTeamMember:
		return new SFProtobufPacket<SFPacketStore::LeaveTeamMember>(PacketId);

	case CGSF::RoomList:
		return new SFProtobufPacket<SFPacketStore::RoomList>(PacketId);

	case CGSF::RoomMember:
		return new SFProtobufPacket<SFPacketStore::RoomMember>(PacketId);

	case CGSF::ChatRes:
		return new SFProtobufPacket<SFPacketStore::ChatRes>(PacketId);

	case CGSF::LoadingStart:
		return new SFProtobufPacket<SFPacketStore::LoadingStart>(PacketId);

	case CGSF::PlayStart:
		return new SFProtobufPacket<SFPacketStore::PlayStart>(PacketId);

	case CGSF::PlayEnd:
		return new SFProtobufPacket<SFPacketStore::PlayStart>(PacketId);

		///////////////////////////////////////////////////////////////////////
		//P2P와 관계된 기능. P2P를 이용하지 않는다면 신경쓰지 않아도 된다.
		//////////////////////////////////////////////////////////////////////
	case CGSF::PeerList:
		return new SFProtobufPacket<SFPacketStore::PeerList>(PacketId);

	case CGSF::AddPeer:
		return new SFProtobufPacket<SFPacketStore::ADD_PEER>(PacketId);

	case CGSF::DeletePeer:
		return new SFProtobufPacket<SFPacketStore::DELETE_PEER>(PacketId);

	case CGSF::MSG_CREATE_PLAYER:
		return new SFProtobufPacket<SFPacketStore::MSG_CREATE_PLAYER>(PacketId);

	case CGSF::MSG_DESTROY_PLAYER:
		return new SFProtobufPacket<SFPacketStore::MSG_DESTROY_PLAYER>(PacketId);

///////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////

	case SevenGame::InitCardCount:
		return new SFProtobufPacket<SevenGamePacket::InitCardCount>(PacketId);
		break;
	case SevenGame::MyCardInfo:
		return new SFProtobufPacket<SevenGamePacket::MyCardInfo>(PacketId);
		break;
	case SevenGame::CurrentTurn:
		return new SFProtobufPacket<SevenGamePacket::CurrentTurn>(PacketId);
		break;
	case SevenGame::TurnPass:
		return new SFProtobufPacket<SevenGamePacket::TurnPass>(PacketId);
		break;
	case SevenGame::CardSubmit:
		return new SFProtobufPacket<SevenGamePacket::CardSubmit>(PacketId);
		break;
	case SevenGame::Winner:
		return new SFProtobufPacket<SevenGamePacket::Winner>(PacketId);
		break;
	case SevenGame::PlayerID:
		return new SFProtobufPacket<SevenGamePacket::PlayerID>(PacketId);
		break;
	case SevenGame::TableUpdate:
		return new SFProtobufPacket<SevenGamePacket::TableUpdate>(PacketId);
		break;
	case SevenGame::UserDie:
		return new SFProtobufPacket<SevenGamePacket::UserDie>(PacketId);
		break;

	default:
		return NULL;
		break;
	}
	return NULL;
}


bool SevenGameProtocol::TCPSend(SFProtobufPacketImpl* pPacket)
{
	unsigned int uWrittenBytes = 0;
	int iResult = serializeOutgoingPacket(*pPacket, oBuffer, uWrittenBytes);
	if (iResult != SFProtocol::Success)
	{
		return false;
	}

	unsigned int uSize = oBuffer.GetDataSize();

	m_pSocket->SendBuf((const char*)oBuffer.GetBuffer(), uSize);

	oBuffer.Pop(uSize);

	return true;

	return true;
}