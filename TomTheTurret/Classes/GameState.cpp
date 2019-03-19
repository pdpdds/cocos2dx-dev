//
//  GameState.cpp
//  TomTheTurret
//
//  Created by Jaewhan Lee on 13. 1. 21..
//
//

#include "GameState.h"
#include "GameLevel.h"

GameState::GameState()
{
}

GameState::~GameState()
{
    levels->release();
}

static GameState *pSingleton;

GameState* GameState::getInstance()
{
    if (!pSingleton)
    {
        pSingleton = new GameState();
        pSingleton->initLevel();
    }
    
    return pSingleton;
}

void GameState::initLevel()
{
    levels = cocos2d::CCArray::create();
    levels->retain();

    // Level 1
    GameLevel *level1 = new GameLevel;
    level1->spawnSeconds = 15;
    level1->spawnRate = 3;
    level1->spawnIds->addObject( CCString::create("0") ); // MonsterTypeFastAndWeak
    level1->spawnIds->addObject( CCString::create("0") );
    levels->addObject(level1);
    
    // Level 2
    GameLevel *level2 = new GameLevel;
    level2->spawnSeconds = 15;
    level2->spawnRate = 4;
    level2->spawnIds->addObject( CCString::create("1") ); // MonsterTypeSlowAndStrong
    level2->spawnIds->addObject( CCString::create("0") );
    level2->spawnIds->addObject( CCString::create("0") );
    levels->addObject(level2);

    // Level 3
    GameLevel *level3 = new GameLevel;
    level3->spawnSeconds = 15;
    level3->spawnRate = 16;
    level3->spawnIds->addObject( CCString::create("2") ); // MonsterTypeBoss
    levels->addObject(level3);

    curLevelIndex = 0;
    curLevel = (GameLevel *)levels->objectAtIndex( curLevelIndex );
}

void GameState::reset()
{
    curLevelIndex = 0;
    curLevel = (GameLevel *)levels->objectAtIndex( curLevelIndex );
}

void GameState::nextLevel()
{    
    curLevelIndex++;
    if (curLevelIndex < levels->count()) {
        curLevel = (GameLevel *)levels->objectAtIndex( curLevelIndex );
    }
}

void GameState::winGame()
{
    curLevelIndex = 3;
}

void GameState::loseGame()
{
    curLevelIndex = 4;
}


