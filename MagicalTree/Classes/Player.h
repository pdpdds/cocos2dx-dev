#pragma once
#include "GameDefines.h"
#include "cocos2d.h"
#include "Box2d/Box2D.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;
using namespace CocosDenshion;

class PlayerFSM;
class GameLayer;

class Player : public CCSprite
{
public:
	Player(char* filename, GameLayer* pLayer);
	virtual ~Player();

	void update(float dt);

	void ResetPhysicsParam();

	int forwardMarch;
	bool mightAsWellJump;
	int jumpingForforwardMarch;

	CCPoint& GetVelocity(){ return velocity; }
	void SetDesiredPosition(CCPoint pos){ desiredPosition = pos; }

	void CheckCollison(GameLayer* pLayer);
	void CheckCollisonWithItem(GameLayer* pLayer);
	void ItemCallback(CCNode * node);

	PlayerFSM* GetFSM(){ return m_pPlayerFSM; }

	CCPoint desiredPosition;
	CCPoint velocity;

	CCSprite* _curPlatform;

	GameLayer* GetGameLayer(){ return m_pLayer; }

protected:

private:
	PlayerFSM* m_pPlayerFSM;
	GameLayer* m_pLayer;
	
};

