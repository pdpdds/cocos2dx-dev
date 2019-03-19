//
//  Coin.h
//  PaoKu
//
//  Created by YTB on 13-11-25.
//
//

#ifndef __PaoKu__Coin__
#define __PaoKu__Coin__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "chipmunk.h"

USING_NS_CC;

class Coin : public extension::CCPhysicsSprite
{
public:
    static Coin *create(CCSpriteBatchNode *spriteSheet, cpSpace *space, CCPoint position);
    Coin(CCSpriteBatchNode *spriteSheet, cpSpace *space, CCPoint position);
    virtual void removeFromParent();
    
    
    static const CCSize* getCoinContentSize()
	{
        CCPhysicsSprite *sprite = CCPhysicsSprite::createWithSpriteFrameName("coin0.png");
        return &sprite->getContentSize();
	}
private:
    cpSpace *pSpace;//weak ref
    cpShape *pShape;
    cpBody *pBody;

};

#endif /* defined(__PaoKu__Coin__) */
