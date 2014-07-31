#include "AGGameMgr.h"
#include "AGF.h"
#include "AGGameFSM.h"

AGGameMgr::AGGameMgr(void)
{
}

AGGameMgr::~AGGameMgr(void)
{
}

bool AGGameMgr::Initialize()
{
	m_GameFSM.AddState(GAMESTATE_INTRO);
	m_GameFSM.AddState(GAMESTATE_TITLE);
	m_GameFSM.AddState(GAMESTATE_MAIN);

	m_GameFSM.ChangeState(GAMESTATE_INTRO);

	return true;
}