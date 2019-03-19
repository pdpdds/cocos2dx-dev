//
//  MainScene.h
//  Parkour
//
//  Created by YTB on 13-11-19.
//
//

#ifndef __Parkour__MainScene__
#define __Parkour__MainScene__

#include "cocos2d.h"

class MainLayer : public cocos2d::CCLayer
{
public:
    CREATE_FUNC(MainLayer);
    virtual bool init();
    static cocos2d::CCScene* scene();
private:
    void onPlay(CCObject* pSender);
};

#endif /* defined(__Parkour__MainScene__) */
