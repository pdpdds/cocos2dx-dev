#pragma once
#include <map>
#include "PlayerState.h"

class PlayerFSM : public PlayerState
{
	typedef std::map<int, PlayerState*> mapPlayerState;
public:
	PlayerFSM(Player* pOwner);
	~PlayerFSM();

	bool AddState(PlayerState* pState);
	bool ChangeState(int plyerState);

	virtual void OnTouch(CCPoint& touchPoint);
	virtual void OnMoved(CCPoint& touchPoint);
	virtual void OnTouchEnd(CCPoint& touchPoint);

	virtual void update(float dt);
	virtual void CheckCollison(GameLayer* pLayer);

protected:

private:
	mapPlayerState m_mapPlayerState;
	PlayerState* m_pCurState;
};

