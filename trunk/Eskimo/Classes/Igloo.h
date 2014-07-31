//
//  Igloo.h
//  Eskimo
//
//  Created by Roger Engelbert on 1/23/13.
//
//

#ifndef __Eskimo__Igloo__
#define __Eskimo__Igloo__

#include "Eskimo.h"

using namespace cocos2d;

class Igloo : public CCSprite {
public:
    
    ~Igloo();
    Igloo();
    
    static Igloo* create();
    void initIgloo(int gravity, CCPoint position);
    void onGravityChanged(CCObject * note);
    void onLevelCompleted(CCObject * note);
    
private:
    CCSprite * _block;
    CCSprite * _iglooOn;
    inline void addBlock () {
        _block = CCSprite::createWithSpriteFrameName("block_large_1.png");
        _block->setAnchorPoint(ccp(0,0));
        _block->setPosition(ccp(0, -TILE * 0.75f));
        this->addChild(_block, kBackground);
    }
};

#endif /* defined(__Eskimo__Igloo__) */
