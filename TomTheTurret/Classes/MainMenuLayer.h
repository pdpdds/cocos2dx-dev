//
//  MainMenuLayer.h
//  TomTheTurret
//
//  Created by Jaewhan Lee on 13. 1. 21..
//
//

#ifndef __TomTheTurret__MainMenuLayer__
#define __TomTheTurret__MainMenuLayer__

#include "cocos2d.h"

using namespace cocos2d;

class MainMenuLayer : public cocos2d::CCLayer
{
public:
    virtual bool init();
    
    static cocos2d::CCScene* scene();
    
    CREATE_FUNC(MainMenuLayer);
    
    
    CCSpriteBatchNode   *batchNode;
    CCSprite            *main_bkgrnd;

    void newGameSpriteTapped(CCObject* sender);

};

#endif /* defined(__TomTheTurret__MainMenuLayer__) */
