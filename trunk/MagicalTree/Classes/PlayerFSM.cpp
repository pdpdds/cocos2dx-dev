#include "PlayerFSM.h"


PlayerFSM::PlayerFSM(Player* pOwner)
: PlayerState(pOwner, -1)
, m_pCurState(0)
{
}


PlayerFSM::~PlayerFSM()
{
	mapPlayerState::iterator i;
	for (i; i != m_mapPlayerState.end(); i++)
	{
		PlayerState* pState = i->second;
		delete pState;
	}

	m_mapPlayerState.clear();
}

bool PlayerFSM::AddState(PlayerState* pState)
{
	m_mapPlayerState.insert(std::make_pair(pState->GetState(), pState));
	return true;
}

bool PlayerFSM::ChangeState(int plyerState)
{
	mapPlayerState::iterator state = m_mapPlayerState.find(plyerState);
	if (state != m_mapPlayerState.end())
	{
		if (m_pCurState != 0)
			m_pCurState->OnExit();

		PlayerState* pCurState = state->second;
		pCurState->OnEnter(m_pCurState);
		m_pCurState = pCurState;

		return true;
	}
	return false;
}

void PlayerFSM::OnTouch(CCPoint& touchPoint)
{
	m_pCurState->OnTouch(touchPoint);
}

void PlayerFSM::OnMoved(CCPoint& touchPoint)
{
	m_pCurState->OnMoved(touchPoint);
}

void PlayerFSM::OnTouchEnd(CCPoint& touchPoint)
{
	m_pCurState->OnTouchEnd(touchPoint);
}

void PlayerFSM::update(float dt)
{
	m_pCurState->update(dt);
}

void PlayerFSM::CheckCollison(GameLayer* pLayer)
{
	m_pCurState->CheckCollison(pLayer);
}