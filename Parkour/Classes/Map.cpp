//
//  Map.cpp
//  PaoKu
//
//  Created by YTB on 13-11-26.
//
//

#include "Rock.h"
#include "Map.h"
#include "Runner.h"
#include "Constant.h"
#include "MapManager.h"

#define GROUND_TAG (10)

Map *Map::create(int index)
{
    Map *map = new Map(index);
    map->autorelease();
    return map;
}

Map::Map(int index)
{
    this->initWithFile(getMapName(index));
    this->setAnchorPoint(ccp(0, 0));
    this->setPosition(ccp(getContentSize().width * index, 0));
    
    CCSprite *ground = CCSprite::create(getGroundName(index));
    ground->setAnchorPoint(ccp(0, 1));
    ground->setTag(GROUND_TAG);
    ground->setPosition(ccp(0, MapManager::getGroundHeight()));
    addChild(ground);
}

Map::~Map()
{
}

const char *Map::getMapName(int index)
{
    CCString *fileName = CCString::createWithFormat( "Map%02d.png", index % MAP_COUNT);
    return fileName->getCString();
}

const char *Map::getGroundName(int index)
{
    CCString *fileName = CCString::createWithFormat( "Ground%02d.png", index % MAP_COUNT);
    return fileName->getCString();
}

void Map::reload(int index)
{
    CCTexture2D *texture = CCTextureCache::sharedTextureCache()->addImage(getMapName(index));
    this->setTexture(texture);
    this->setPositionX(getContentSize().width * index);
    
    CCTexture2D *textureGround = CCTextureCache::sharedTextureCache()->addImage(getGroundName(index));
    CCSprite *ground = (CCSprite *)getChildByTag(GROUND_TAG);
    ground->setTexture(textureGround);
}