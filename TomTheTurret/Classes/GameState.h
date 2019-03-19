//
//  GameState.h
//  TomTheTurret
//
//  Created by Jaewhan Lee on 13. 1. 21..
//
//

#ifndef __TomTheTurret__GameState__
#define __TomTheTurret__GameState__

#include "cocos2d.h"

using namespace cocos2d;

class GameLevel;

class GameState : public CCLayer
{
public:
    static GameState* getInstance();
    
    void reset();
    void nextLevel();
    void winGame();
    void loseGame();

    // Level pointer
    GameLevel *curLevel;
    int curLevelIndex;
    
private:
    GameState();
    ~GameState();
    
    void initLevel();
    
    // 레벨별 게임 시작 상태 저장용
    CCArray *levels;
};

#endif /* defined(__TomTheTurret__GameState__) */



