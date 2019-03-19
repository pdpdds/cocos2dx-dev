//
//  MenuLayer.h
//  moles
//
//  Created by breaklee on 6/9/13.
//
//

#ifndef __moles__MenuLayer__
#define __moles__MenuLayer__

#include <iostream>
#include "cocos2d.h"

USING_NS_CC;

class MenuLayer : public CCLayer {
    public :
    static CCScene* scene();
    CREATE_FUNC(MenuLayer);
    
    virtual bool init();
    virtual void onEnter();
    
	void startGame(CCObject* pObject);
    
    bool started;
    
};

#endif /* defined(__moles__MenuLayer__) */
