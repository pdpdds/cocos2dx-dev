//
//  GameOverLayer.h
//  Parkour
//
//  Created by YTB on 13-11-20.
//
//

#ifndef __Parkour__GameOverLayer__
#define __Parkour__GameOverLayer__

#include "cocos2d.h"

USING_NS_CC;

class GameOverLayer : public CCLayerColor
{
public:
    static GameOverLayer* create(const ccColor4B& color);
    virtual bool initWithColor(const ccColor4B& color);
private:
    void onRestart(CCObject* pSender);
};

#endif /* defined(__Parkour__GameOverLayer__) */
