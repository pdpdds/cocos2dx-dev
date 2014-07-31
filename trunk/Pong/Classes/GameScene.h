//
//  GameScene.h
//  Pong
//
//  Created by Clawoo on 9/18/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include "cocos2d.h"

class GameScene : public cocos2d::CCLayer
{
public:
	// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	virtual bool init();  
    
	// there's no 'id' in cpp, so we recommand to return the exactly class pointer
	static cocos2d::CCScene* scene();
	
    virtual void draw();
    virtual void update(float dt);
    
    virtual void ccTouchesBegan(cocos2d::CCSet* touches, cocos2d::CCEvent* event);
    virtual void ccTouchesMoved(cocos2d::CCSet* touches, cocos2d::CCEvent* event);
    virtual void ccTouchesEnded(cocos2d::CCSet* touches, cocos2d::CCEvent* event);
    
    void handleUserTouch(cocos2d::CCTouch *touch);
    bool handleUserPaddleCollision(cocos2d::CCRect ballRect);
    bool handleCpuPaddleCollision(cocos2d::CCRect ballRect);
    
	// implement the "static node()" method manually
	NODE_FUNC(GameScene);

private:
    cocos2d::CCSprite *userPaddle_;
    cocos2d::CCSprite *cpuPaddle_;
    cocos2d::CCSprite *ball_;
    cocos2d::CCRect gameArea_;
    cocos2d::CCRect touchArea_;
    
    bool userHasGrabbedThePaddle_;
    bool ballIsEscapingPaddle_;
    cocos2d::CCPoint velocity_;
    cocos2d::CCPoint touchLocation_;
};

#endif // __GAME_SCENE_H__
