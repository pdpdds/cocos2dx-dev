//
//  Monster.cpp
//  TomTheTurret
//
//  Created by Jaewhan Lee on 13. 1. 22..
//
//

#include "Monster.h"

Monster* Monster::monsterWithType(int monsterType)
{
    Monster *monster = new Monster();   // 2013.03.17 modify

    const char *pszFileName;

    if (monsterType == 0) {
        // MonsterTypeFastAndWeak
        pszFileName = "Level_person1.png";
        monster->hp = 2;
        monster->maxHp = monster->hp;
        monster->minMoveDuration = 3;
        monster->maxMoveDuration = 6;
        monster->hitEffectSoundId = 2001;
        monster->hitEffectGain = 0.25f;
    } else if (monsterType == 1) {
        // MonsterTypeSlowAndStrong
        pszFileName = "Level_person2.png";
        monster->hp = 5;
        monster->maxHp = monster->hp;
        monster->minMoveDuration = 4;
        monster->maxMoveDuration = 7;
        monster->hitEffectSoundId = 1001;
        monster->hitEffectGain = 1.0f;
    } else if (monsterType == 2) {
        // MonsterTypeBoss
        pszFileName = "Level_scientist.png";
        monster->hp = 50;
        monster->maxHp = monster->hp;
        monster->minMoveDuration = 6;
        monster->maxMoveDuration = 9;
        monster->hitEffectSoundId = 1001;
        monster->hitEffectGain = 1.0f;
    }

    if (monster && monster->initWithSpriteFrameName(pszFileName)) {
        monster->autorelease();
        return monster;
    }
    
    CC_SAFE_DELETE(monster);
}



