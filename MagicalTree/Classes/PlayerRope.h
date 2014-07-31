#pragma once
#include "PlayerState.h"

class PlayerRope : public PlayerState
{
public:
	PlayerRope(Player* pPlayer);
	~PlayerRope();

	virtual void OnTouch(CCPoint& touchPoint);
	virtual void OnMoved(CCPoint& touchPoint);
	virtual void OnTouchEnd(CCPoint& touchPoint);

	virtual void CheckCollison(GameLayer* pLayer);
	virtual void update(float dt);

	virtual void OnEnter(PlayerState* pPrevState);
	virtual void OnExit();

private:
	int m_dir;
	CCPoint m_targetPos;
};

