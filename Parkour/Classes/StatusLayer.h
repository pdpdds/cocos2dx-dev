//
//  StatusLayer.h
//  Parkour
//
//  Created by YTB on 13-11-20.
//
//

#ifndef __Parkour__StatusLayer__
#define __Parkour__StatusLayer__

#include "cocos2d.h"

USING_NS_CC;

class StatusLayer :  public CCLayer
{
public:
    virtual bool init();
    void addCoin(int num);
    void updateMeter(int px);
    CREATE_FUNC(StatusLayer);
private:
    CCLabelTTF *labelCoin;
    CCLabelTTF *labelMeter;
    unsigned long int coins;
};

#endif /* defined(__Parkour__StatusLayer__) */
