//
//  Mole.h
//  moles
//
//  Created by breaklee on 6/9/13.
//
//

#ifndef __moles__Mole__
#define __moles__Mole__

#include <iostream>
#include "cocos2d.h"

USING_NS_CC;

class MoleHill;

typedef enum {
    
    kMoleDead = 0,
    kMoleHidden,
    kMoleMoving,
    kMoleHit,
    kMoleAlive
    
} MoleState;

#define SND_MOLE_NORMAL "penguin_call.caf"
#define SND_MOLE_SPECIAL "penguin_call_echo.caf"
#define SND_BUTTON "button.caf"

class Mole : public CCTargetedTouchDelegate, public CCNode  {

public :
    
    CCSprite* moleSprite;
    MoleHill* parentHill;

    float moleGroundY;
    MoleState moleState;
    bool isSpecial;
    
    CREATE_FUNC(Mole);
    
    virtual bool init();
    
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    
    void destoryTouchDelegate();
};


#endif /* defined(__moles__Mole__) */
