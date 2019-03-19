//
//  Monster.h
//  TomTheTurret
//
//  Created by Jaewhan Lee on 13. 1. 22..
//
//

#ifndef __TomTheTurret__Monster__
#define __TomTheTurret__Monster__

#include "cocos2d.h"

class Monster : public cocos2d::CCSprite
{
public:
    int hp;
    int maxHp;
    int minMoveDuration;
    int maxMoveDuration;
    int hitEffectSoundId;
    float hitEffectGain;

    static Monster* monsterWithType(int monsterType);
};

#endif /* defined(__TomTheTurret__Monster__) */
