//
//  Rock.cpp
//  PaoKu
//
//  Created by YTB on 13-11-26.
//
//

#include "Rock.h"
#include "Map.h"
#include "MapManager.h"
#include "Runner.h"
#include "Constant.h"


Rock *Rock::create(CCSpriteBatchNode *spriteSheet, cpSpace *space, CCPoint position)
{
    Rock *rock = new Rock(spriteSheet, space, position);
    rock->autorelease();
    return rock;
}

Rock::Rock(CCSpriteBatchNode *spriteSheet, cpSpace *space, CCPoint position)
{
    this->pSpace = space;
    
    if ( position.y >= (MapManager::getGroundHeight() + Runner::getCrouchContentSize()->height) ) {
        this->initWithSpriteFrameName("hathpace.png");
    } else {
        this->initWithSpriteFrameName("rock.png");
    }

    // init physics
    this->pBody = cpBodyNewStatic();
    cpBodySetPos(this->pBody, cpv(position.x, position.y));
    this->setCPBody(this->pBody);

    this->pShape = cpBoxShapeNew(this->pBody, this->getContentSize().width, this->getContentSize().height);

    this->pShape->collision_type = SpriteTagrock;
    this->pShape->sensor = true;
    cpSpaceAddStaticShape(this->pSpace, this->pShape);

    // for collision
    cpShapeSetUserData(this->pShape, this);
    
    spriteSheet->addChild(this);
}

void Rock::removeFromParent()
{
    cpSpaceRemoveStaticShape(this->pSpace, this->pShape);
    cpShapeFree(this->pShape);
    cpBodyFree(this->pBody);
    
    CCPhysicsSprite::removeFromParent();
}