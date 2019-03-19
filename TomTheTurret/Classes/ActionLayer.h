//
//  ActionLayer.h
//  TomTheTurret
//
//  Created by Jaewhan Lee on 13. 1. 21..
//
//

#ifndef __TomTheTurret__ActionLayer__
#define __TomTheTurret__ActionLayer__

#include "cocos2d.h"

using namespace cocos2d;

class Monster;

//typedef struct
//{
//    CCSprite* pSprite;
//    int hp;
//    int maxHp;
//    int minMoveDuration;
//    int maxMoveDuration;
//    int hitEffectSoundId;
//    float hitEffectGain;
//} Monster;

class ActionLayer : public cocos2d::CCLayer
{
public:
    virtual bool init();

    static cocos2d::CCScene* scene();
    
    CREATE_FUNC(ActionLayer);

    
    virtual void onExit();
    virtual void onEnter();
    void update(float dt);
    void gameLogic(float dt);
    void addMonsters();
    void addMonster(Monster* monster);
    void spriteMoveFinished(CCObject* sender);
    void clearData(int victory);
    void finishShoot(CCObject* pSender);

    virtual void ccTouchesEnded(cocos2d::CCSet *pTouches, cocos2d::CCEvent* event);

    int curLevelIndex;
    

    CCSpriteBatchNode*  batchNode;
    CCSprite*           level_bkgrnd;
    CCSprite*           player;
    CCArray*            monsters;
    CCArray*            projectiles;
    CCSprite*           nextProjectile;
    bool                monsterHit;
    double              levelBegin;
    double              lastTimeMonsterAdded;
    bool                inLevel;

};

#endif /* defined(__TomTheTurret__ActionLayer__) */
