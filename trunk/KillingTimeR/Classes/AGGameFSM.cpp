#include "AGGameFSM.h"
#include "AGF.h"
#include "AGGameIntro.h"
#include "AGGameTitle.h"
#include "AGGameMain.h"

AGGameFSM::AGGameFSM(void)
: m_pCurrentState(0)
{
}

AGGameFSM::~AGGameFSM(void)
{
}

bool AGGameFSM::AddState( int GameState )
{
	switch(GameState)
	{
	case GAMESTATE_INTRO:
		{
			AGGameState* pGameState = new AGGameIntro();
			m_GameStateMap.insert(make_pair(GAMESTATE_INTRO, pGameState));
		}
		break;
	case GAMESTATE_TITLE:
		{
			AGGameState* pGameState = new AGGameTitle();
			m_GameStateMap.insert(make_pair(GAMESTATE_TITLE, pGameState));
		}
		break;
	case GAMESTATE_MAIN:
		{
			AGGameState* pGameState = new AGGameMain();
			m_GameStateMap.insert(make_pair(GAMESTATE_MAIN, pGameState));
		}
		break;
	default:
		return false;
	}

	return true;
}

bool AGGameFSM::ChangeState( int State)
{
	GameStateMap::iterator iter = m_GameStateMap.find(State);

	if(iter == m_GameStateMap.end())
		return false;
	
	if(m_pCurrentState == 0)
	{
		m_pCurrentState = iter->second;

		m_pCurrentState->OnEnter();
			
		return true;
	}

	if(m_pCurrentState == iter->second)
		return false;

	m_pCurrentState->OnLeave();
	m_pCurrentState = iter->second;
	m_pCurrentState->OnEnter();

	return true;
}