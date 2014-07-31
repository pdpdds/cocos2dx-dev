//
//  GameOver.h
//  BrickEx
//
//  Created by Jaewhan Lee on 12. 12. 20..
//
//

#ifndef __BrickEx__GameOver__
#define __BrickEx__GameOver__

#include "cocos2d.h"

using namespace cocos2d;

class GameOver : public cocos2d::CCLayer
{
public:
    static cocos2d::CCScene* createScene();

    virtual bool init();

    CREATE_FUNC(GameOver);
    
    void doClose(CCObject* pSender);
};

#endif /* defined(__BrickEx__GameOver__) */
