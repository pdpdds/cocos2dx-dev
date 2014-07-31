//
//  MenuLayer.h
//  Eskimo
//
//  Created by Roger Engelbert on 1/22/13.
//
//

#ifndef __Eskimo__MenuLayer__
#define __Eskimo__MenuLayer__

#include "Eskimo.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;
using namespace CocosDenshion;

class MenuLayer : public CCLayer {
public:
    
    ~MenuLayer();
    MenuLayer();
    
    static cocos2d::CCScene* scene();
    
    void update(float dt);
	void showLevels(CCObject* pSender);
	void showHelp(CCObject* pSender);
    
private:
    CCSpriteBatchNode * _layerBatchNode;
    CCMenu* _mainMenu;
    
    CCSprite * _balloon;
    CCSprite * _caption1;
    CCSprite * _caption2;
    CCParticleSystem * _snow;
    
    CCSize _screenSize;
    
    
    void createScreen(void);
    float _msgCntDown;
        
};


#endif /* defined(__Eskimo__MenuLayer__) */
