#include "cocos2d.h"
#include "ProtobufHandler.h"
#include "ProtobufSocket.h"
#include "SFProtobufPacketImpl.h"
#include "SFPacketStore.pb.h"
#include "SFProtobufPacket.h"
#include "LoginLayer.h"
#include "CommonStructure.h"
#include "SFPacketID.h"
#include "SevenGamePacketID.h"
#include "NetworkFacade.h"

USING_NS_CC;

#define SF_GETPACKET_ARG(a,b,c) memcpy(a,b.c_str(), sizeof(c));

ProtobufHandler::ProtobufHandler()
:SocketHandler()
{
}


ProtobufHandler::~ProtobufHandler()
{
}

void ProtobufHandler::OnAuth(ProtobufSocket *p, SFProtobufPacketImpl* pPacket)
{
	//Director::getInstance()->replaceScene(LoginLayer);
	CCScene* pScene = LoginLayer::scene();
	CCDirector::sharedDirector()->replaceScene(pScene);
}

void ProtobufHandler::OnLoginSuccess(ProtobufSocket *p, SFProtobufPacketImpl* pPacket)
{
	SFProtobufPacket<SFPacketStore::LoginSuccess>* pLoginSuccess = (SFProtobufPacket<SFPacketStore::LoginSuccess>*)pPacket;
	_UserInfo info;
	SF_GETPACKET_ARG(&info, pLoginSuccess->GetData().userinfo(), _UserInfo);
	

	SFProtobufPacket<SFPacketStore::EnterLobby> request(CGSF::EnterLobby);
	NetworkFacade::GetInstance()->TCPSend(&request);
}

void ProtobufHandler::OnLoginFail(ProtobufSocket *p, SFProtobufPacketImpl* pPacket)
{
	
}

void ProtobufHandler::OnEnterLobby(ProtobufSocket *p, SFProtobufPacketImpl* pPacket)
{

}

void ProtobufHandler::OnRoomList(ProtobufSocket *p, SFProtobufPacketImpl* pPacket)
{
	SFProtobufPacket<SFPacketStore::RoomList>* pRoomList = (SFProtobufPacket<SFPacketStore::RoomList>*)pPacket;

	for (int i = 0; i < pRoomList->GetData().info_size(); i++)
	{
		const SFPacketStore::RoomList::RoomInfo& info = pRoomList->GetData().info(i);
	
		info.roomname().c_str();
		printf("%s", info.roomname().c_str());
		
	}

	/*
	Listbox* lbox = static_cast<Listbox*>(d_root->getChild("RoomList"));
	lbox->resetList();

	*/
}
