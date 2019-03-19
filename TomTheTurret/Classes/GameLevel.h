//
//  GameLevel.h
//  TomTheTurret
//
//  Created by Jaewhan Lee on 13. 1. 21..
//
//

#ifndef __TomTheTurret__GameLevel__
#define __TomTheTurret__GameLevel__

#include "cocos2d.h"

class GameLevel : public cocos2d::CCObject
{
public:
    GameLevel();
    ~GameLevel();
    
    float spawnSeconds;
    float spawnRate;
    cocos2d::CCArray *spawnIds;
};

#endif /* defined(__TomTheTurret__GameLevel__) */
