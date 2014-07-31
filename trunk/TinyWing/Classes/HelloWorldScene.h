//
//  HelloWorldScene.h
//  twxes10
//
//  Created by diwwu on 5/16/12.
//  Copyright __MyCompanyName__ 2012. All rights reserved.
//
#ifndef __HELLO_WORLD_H__
#define __HELLO_WORLD_H__

// When you import this file, you import all the cocos2d classes
#include "cocos2d.h"
#include "box2d/Box2D.h"

class HelloWorld : public cocos2d::CCLayer {
public:
    ~HelloWorld();
    HelloWorld();
    
    // returns a Scene that contains the HelloWorld as the only child
    static cocos2d::CCScene* scene();
    
    // adds a new sprite at a given coordinate
    void addNewSpriteWithCoords(cocos2d::CCPoint p);
    virtual void draw();
    virtual void ccTouchesEnded(cocos2d::CCSet* touches, cocos2d::CCEvent* event);
    void tick(float dt);
    
private:
    b2World* world;
};

#endif // __HELLO_WORLD_H__
