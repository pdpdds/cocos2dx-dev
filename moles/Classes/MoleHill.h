//
//  File.h
//  moles
//
//  Created by breaklee on 6/9/13.
//
//

#ifndef __moles__File__
#define __moles__File__

#include <iostream>
#include "cocos2d.h"
#include "Mole.h"

USING_NS_CC;

class MoleHill : public CCNode {
    
public:
    virtual ~MoleHill() { CCLOG("HILL DESTRUCT"); }
    CREATE_FUNC(MoleHill);
    virtual bool init();
    
    CCSprite* moleHillTop;
    CCSprite* moleHillBottom;
    Mole *hillMole;
    
    int moleHillID;
    int moleHillBaseZ;
    
    bool isOccupied;
    
};

#endif /* defined(__moles__File__) */
