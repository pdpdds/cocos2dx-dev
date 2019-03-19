//
//  StoryLayer.h
//  TomTheTurret
//
//  Created by Jaewhan Lee on 13. 1. 21..
//
//

#ifndef __TomTheTurret__StoryLayer__
#define __TomTheTurret__StoryLayer__

#include "cocos2d.h"

using namespace cocos2d;

class StoryLayer : public cocos2d::CCLayer
{
public:
    virtual bool init();

    static cocos2d::CCScene* scene();
    
    CREATE_FUNC(StoryLayer);
    

    int                 curLevelIndex;
    
    CCSpriteBatchNode*  batchNode;
    CCSprite*           main_bkgrnd;
    CCLabelTTF*         label;
    CCSprite*           tapToCont;
    CCSprite*           spriteNewGame;

    int curStoryIndex;
    CCArray* story1;
    CCArray* story2;
    CCArray* story3;
    CCArray* story4;
    CCArray* story5;

    virtual void onEnter();
    void displayCurStoryString();
    virtual void ccTouchesEnded(cocos2d::CCSet *pTouches, cocos2d::CCEvent* event);

};
#endif /* defined(__TomTheTurret__StoryLayer__) */
