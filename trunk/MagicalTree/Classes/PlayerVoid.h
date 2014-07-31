#pragma once
#include "PlayerState.h"

class GameLayer;

class PlayerVoid : public PlayerState
{
public:
	PlayerVoid(Player* pPlayer);
	virtual ~PlayerVoid();

	virtual void OnTouch(CCPoint& touchPoint);
	virtual void OnMoved(CCPoint& touchPoint);
	virtual void OnTouchEnd(CCPoint& touchPoint);

	virtual void update(float dt);

	virtual void CheckCollison(GameLayer* pLayer);
	virtual void OnEnter(PlayerState* pPrevState);
	virtual void OnExit();
};

