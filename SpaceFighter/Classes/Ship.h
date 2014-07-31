//
//  Ship.h
//  Asteroids
//
//  Created by Clawoo on 9/16/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#ifndef __SHIP_H__
#define __SHIP_H__

#include "cocos2d.h"
USING_NS_CC;

class Ship : public cocos2d::CCSprite
{
public:
    virtual void update(float dt);
	static Ship* spriteWithFile(const char *pszFileName);
    CC_SYNTHESIZE(cocos2d::CCPoint, velocity_, Velocity);
	// implement the "static node()" method manually
	NODE_FUNC(Ship);

	void Destroy(cocos2d::CCLayer* pOwner);
	void ExplosionFinished(CCNode* pSender);

	cocos2d::CCLayer* m_pOwner;
};

#endif // __SHIP_H__