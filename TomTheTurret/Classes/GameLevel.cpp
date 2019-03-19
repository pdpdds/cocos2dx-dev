//
//  GameLevel.cpp
//  TomTheTurret
//
//  Created by Jaewhan Lee on 13. 1. 21..
//
//

#include "GameLevel.h"

GameLevel::GameLevel()
{
    spawnIds = cocos2d::CCArray::create();
    spawnIds->retain();
}

GameLevel::~GameLevel()
{
    spawnIds->release();
}

