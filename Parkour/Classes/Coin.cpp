//
//  Coin.cpp
//  PaoKu
//
//  Created by YTB on 13-11-25.
//
//

#include "Coin.h"
#include "Constant.h"

Coin *Coin::create(CCSpriteBatchNode *spriteSheet, cpSpace *space, CCPoint position)
{
    Coin *coin = new Coin(spriteSheet, space, position);
    coin->autorelease();
    return coin;
}

Coin::Coin(CCSpriteBatchNode *spriteSheet, cpSpace *space, CCPoint position)
{
    this->pSpace = space;

    CCArray *animFrames = CCArray::create();
    for (int i = 0; i < 8; i++)
    {
        CCString *name = CCString::createWithFormat("coin%d.png",i);
        CCSpriteFrame *frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(name->getCString());
        animFrames->addObject(frame);
    }
    CCAnimation *animation = CCAnimation::createWithSpriteFrames(animFrames, 0.1);
    CCAction *action =CCRepeatForever::create(CCAnimate::create(animation));

    this->initWithSpriteFrameName("coin0.png");

    // init physics
    float radius = 0.95 * this->getContentSize().width / 2;
    this->pBody = cpBodyNewStatic();
    cpBodySetPos(this->pBody, cpv(position.x, position.y));
    this->setCPBody(this->pBody);

    this->pShape = cpCircleShapeNew(this->pBody, radius, cpvzero);
    this->pShape->collision_type = SpriteTagcoin;
    this->pShape->sensor = true;
    cpSpaceAddStaticShape(this->pSpace, this->pShape);

    // for collision
    cpShapeSetUserData(this->pShape, this);
    
    this->runAction(action);
    spriteSheet->addChild(this);
}

void Coin::removeFromParent()
{
    cpSpaceRemoveStaticShape(this->pSpace, this->pShape);
    cpShapeFree(this->pShape);
    cpBodyFree(this->pBody);
    
    CCPhysicsSprite::removeFromParent();
}