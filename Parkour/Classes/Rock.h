//
//  Rock.h
//  PaoKu
//
//  Created by YTB on 13-11-26.
//
//

#ifndef __PaoKu__Rock__
#define __PaoKu__Rock__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "chipmunk.h"

USING_NS_CC;

class Rock : public extension::CCPhysicsSprite
{
public:
    static Rock *create(CCSpriteBatchNode *spriteSheet, cpSpace *space, CCPoint position);
    Rock(CCSpriteBatchNode *spriteSheet, cpSpace *space, CCPoint position);
    virtual void removeFromParent();
    
    static  const CCSize* getRockContentSize()
    {
        CCPhysicsSprite *sprite = CCPhysicsSprite::createWithSpriteFrameName("rock.png");
        return &sprite->getContentSize();

    };
private:
    cpSpace *pSpace;
    cpShape *pShape;
    cpBody *pBody;
};

#endif /* defined(__PaoKu__Rock__) */
