//
//  GameLayer.h
//  twxes10
//
//  Created by diwwu on 5/17/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef twxes10_GameLayer_h
#define twxes10_GameLayer_h

#include "Terrain.h"
#include "cocos2d.h"
#include "Hero.h"
#include "box2d/Box2D.h"

using namespace cocos2d;

class GameLayer : public CCLayer {
    
public:
    
    float screenW;
	float screenH;
    
    b2World *world;    
    bool tapDown;
    CC_SYNTHESIZE(Hero *, hero, Hero);
    
    CC_SYNTHESIZE(CCSprite *, background, Background);
    CC_SYNTHESIZE(Terrain *, terrain, Terrain);
    NODE_FUNC(GameLayer);
    static CCScene * scene();
    
    bool init();
    
    ~GameLayer();
    
    void registerWithTouchDispatcher();
    bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
	void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);

    void generateBackground();
    
    void update(float dt);
    
    void createBox2DWorld();
    
    ccColor3B generateDarkColor();
    
    void draw();

};

#endif
