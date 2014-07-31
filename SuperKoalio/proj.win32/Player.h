#pragma once
#include "cocos2d.h"

USING_NS_CC;

class Player : public CCSprite
{
public:
	Player(char* filename);
	virtual ~Player();

	void update(float dt);

	CCRect collisionBoundingBox();

	bool onGround;
	CCPoint desiredPosition;
	CCPoint velocity;

	bool forwardMarch;
	bool mightAsWellJump;
};

