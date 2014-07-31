#pragma once
#include "PlayerState.h"

class PlayerPlatform : public PlayerState
{
public:
	PlayerPlatform(Player* pPlayer);
	virtual ~PlayerPlatform();

	virtual void OnTouch(CCPoint& touchPoint);
	virtual void OnMoved(CCPoint& touchPoint);
	virtual void OnTouchEnd(CCPoint& touchPoint);

	virtual void CheckCollison(GameLayer* pLayer);
	virtual void update(float dt);

	virtual void OnEnter(PlayerState* pPrevState);
	virtual void OnExit();

	CCPoint m_targetPos;
	int m_dir;

protected:
	bool IsValidGroundPos(CCPoint pos);
	bool CheckRopeInteraction(CCPoint& touchPoint);
};

