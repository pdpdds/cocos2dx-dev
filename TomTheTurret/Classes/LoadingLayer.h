//
//  LoadingLayer.h
//  TomTheTurret
//
//  Created by Jaewhan Lee on 13. 1. 21..
//
//

#ifndef __TomTheTurret__LoadingLayer__
#define __TomTheTurret__LoadingLayer__

#include "cocos2d.h"

using namespace cocos2d;

class LoadingLayer : public cocos2d::CCLayer
{
public:
    virtual bool init();
    
    static cocos2d::CCScene* scene();
    
    CREATE_FUNC(LoadingLayer);


    CCSize              winSize;
    
    bool                isLoading;
    bool                imagesLoaded;
    
    CCSprite            *defaultImage;
    CCSpriteBatchNode   *batchNode;
    CCSprite            *main_bkgrnd;
    CCSprite            *main_title;
    CCSprite            *tom;
    CCSprite            *tapToCont;
    CCSprite            *loading;

    void spritesLoaded(CCObject *tex1);
    void tick(float dt);
    virtual void ccTouchesEnded(cocos2d::CCSet *pTouches, cocos2d::CCEvent* event);

};

#endif /* defined(__TomTheTurret__LoadingLayer__) */



