//
//  GameLayer.h
//  MiniPool
//
//  Created by Roger Engelbert on 1/13/13.
//  Copyright __MyCompanyName__ 2013. All rights reserved.
//
#ifndef __GAME_LAYER_H__
#define __GAME_LAYER_H__

#include "cocos2d.h"
#include "SimpleAudioEngine.h"
#include "Ball.h"
#include "Cue.h"
#include "LineContainer.h"
#include "GLES-Render.h"

enum {
    kBackground,
    kMiddleground,
    kForeground
};


enum {
    kGameIntro,
    kGamePlay,
    kGameOver
};

class GameLayer : public cocos2d::CCLayer {

public:
    
    ~GameLayer();
    GameLayer();
    
    CC_SYNTHESIZE(b2World *, _world, World);
    CC_SYNTHESIZE(bool, _canShoot, CanShoot);
    
    // returns a Scene that contains the GameLayer as the only child
    static cocos2d::CCScene* scene();
    
    void initPhysics();

    virtual void draw();
    virtual void ccTouchesBegan(cocos2d::CCSet* touches, cocos2d::CCEvent* event);
    virtual void ccTouchesMoved(cocos2d::CCSet* touches, cocos2d::CCEvent* event);
    virtual void ccTouchesEnded(cocos2d::CCSet* touches, cocos2d::CCEvent* event);
    void update(float dt);
	void ticktock(float dt);
    
private:

    GLESDebugDraw * m_debugDraw;
    b2ContactListener * _collisionListener;
    
    CCTouch * _touch;
    CCArray * _balls;
    CCArray * _pockets;
    CCSpriteBatchNode * _gameBatchNode;
    CCSprite * _intro;
    CCSprite * _gameOver;
    CCLabelBMFont * _timer;
    
    LineContainer * _lineContainer;
    
    
    CCPoint _cueStartPoint;
    CCSize _screenSize;
    bool _running;
    bool _usingCue;
    
    float _pullBack;
    float _cueAngle;
    int _gameState;
    int _ballsInPlay;
    int _time;
    
    Ball * _player;
    Cue * _cue;
    
    void placeCue(CCPoint position);
    void resetGame(void);
};

#endif // __GAME_LAYER_H__
