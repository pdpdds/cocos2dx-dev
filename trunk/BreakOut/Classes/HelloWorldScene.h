//
//  HelloWorldScene.h
//  BreakoutCocos2D-x
//
//  Created by Clawoo on 9/15/11.
//  Copyright __MyCompanyName__ 2011. All rights reserved.
//
#ifndef __HELLO_WORLD_H__
#define __HELLO_WORLD_H__

#define COCOS2D_DEBUG 1

// When you import this file, you import all the cocos2d classes
#include "cocos2d.h"
#include "box2d/Box2D.h"
#include "MyContactListener.h"
#include "GameOverScene.h"
#include "SimpleAudioEngine.h"

class HelloWorld : public cocos2d::CCLayer {
public:
    ~HelloWorld();
    HelloWorld();
    // returns a Scene that contains the HelloWorld as the only child
    static cocos2d::CCScene* scene();

    virtual void draw();
    virtual void ccTouchesBegan(cocos2d::CCSet* touches, cocos2d::CCEvent* event);
    virtual void ccTouchesMoved(cocos2d::CCSet* touches, cocos2d::CCEvent* event);
    virtual void ccTouchesCancelled(cocos2d::CCSet* touches, cocos2d::CCEvent* event);
    virtual void ccTouchesEnded(cocos2d::CCSet* touches, cocos2d::CCEvent* event);
    void tick(float dt);
    
private:
    b2World* _world;
    b2Body *_groundBody;
    b2Fixture *_bottomFixture;
    b2Fixture *_ballFixture;

	b2Fixture *_GroundBoxFixture;

    b2Body *_paddleBody;
    b2Fixture *_paddleFixture;
    b2MouseJoint *_mouseJoint;
    
    MyContactListener *_contactListener;
};

#endif // __HELLO_WORLD_H__
