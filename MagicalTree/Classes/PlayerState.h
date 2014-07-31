#pragma once
#include "cocos2d.h"

class Player;
class GameLayer;

USING_NS_CC;

enum
{
	PLAYER_GROUND = 0,
	PLAYER_VOID,
	PLAYER_ROPE,
	PLAYER_PLATFORM,
};

class PlayerState
{
public:
	PlayerState(Player* pOwner, int state);
	~PlayerState();

	Player* GetOwner(){ return m_pOwner; }
	int GetState(){ return m_state; }

	virtual void OnEnter(PlayerState* pPrevState){};
	virtual void OnExit(){};

	virtual void OnTouch(CCPoint& touchPoint) = 0;
	virtual void OnMoved(CCPoint& touchPoint) = 0;
	virtual void OnTouchEnd(CCPoint& touchPoint) = 0;

	virtual void update(float dt) = 0;
	virtual void CheckCollison(GameLayer* pLayer) {};

protected:

private:
	Player* m_pOwner;
	int m_state;
};