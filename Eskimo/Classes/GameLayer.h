//
//  GameLayerScene.h
//  Eskimo
//
//  Created by Roger Engelbert on 1/17/13.
//  Copyright __MyCompanyName__ 2013. All rights reserved.
//
#ifndef __GAMELAYER_H__
#define __GAMELAYER_H__

#include "Eskimo.h"
#include "Igloo.h"

#include "SimpleAudioEngine.h"
#include "GLES-Render.h"


USING_NS_CC;
using namespace CocosDenshion;

class GameLayer : public cocos2d::CCLayer {
public:
    
    CC_SYNTHESIZE(b2World *, _world, World);
    CC_SYNTHESIZE(int, _gravity, Gravity);
    CC_SYNTHESIZE(int, _levelsCompleted, LevelsCompleted);
    
    ~GameLayer();
    GameLayer();
    
    static GameLayer * create(int level, int levelsCompleted);
    static cocos2d::CCScene* scene(int level, int levelsCompleted);
    
    //virtual void draw();
    virtual void ccTouchesBegan(cocos2d::CCSet* touches, cocos2d::CCEvent* event);
    virtual void ccTouchesEnded(cocos2d::CCSet* touches, cocos2d::CCEvent* event);
    virtual void didAccelerate(CCAcceleration* pAccelerationValue);
    void update(float dt);
    void loadLevel(int level);
    void newLevel(void);

        
private:
    
    GLESDebugDraw * m_debugDraw;
    CCSpriteBatchNode * _gameBatchNode;
    
    CCSprite * _btnStart;
    CCSprite * _btnReset;
    CCSprite * _btnPause;
    CCSprite * _btnAgain;
    CCSprite * _btnMenu;
    CCLabelBMFont * _messages;
    CCLabelTTF * _tutorialLabel;
    
    CCParticleSystem * _smoke;
    
    CCArray * _platformPool;
    CCArray * _gSwitchPool;
    CCArray * _levels;
    CCArray * _buttons;
    
    Eskimo * _player;
    Igloo * _igloo;
    
    CCSize _screenSize;
    bool _running;
    int _gameState;
    int _platformPoolIndex;
    int _currentLevel;
    int _tutorialStep;
    float _tutorialCounter;
    
    CCPoint _acceleration;
    
    void initPhysics();
    void createScreen(void);
    void resetLevel(void);
    void levelCompleted(void);
    void createPools(void);
    void clearLayer(void);
    
};

#endif // __GAMELAYER_H__
