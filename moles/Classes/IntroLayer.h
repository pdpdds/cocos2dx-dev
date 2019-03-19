//
//  IntroLayer.h
//  moles
//
//  Created by breaklee on 6/9/13.
//
//

#ifndef __moles__IntroLayer__
#define __moles__IntroLayer__

#include <iostream>
#include "COCOS2d.h"

USING_NS_CC;

class IntroLayer : public CCLayer {
  
public :
    static CCScene* scene();
    CREATE_FUNC(IntroLayer);
    virtual bool init();
    
    virtual void onEnter();
    
    void loadMenu(float dt);
    
};

#endif /* defined(__moles__IntroLayer__) */
