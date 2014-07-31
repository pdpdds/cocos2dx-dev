#ifndef GAMESCENE_H
#define GAMESCENE_H

#include "cocos2d.h"
#include "SimpleAudioEngine.h"
#include "Terrain.h"
#include "Player.h"

USING_NS_CC;
using namespace CocosDenshion;

typedef enum {
    kGameIntro,
    kGamePlay,
    kGameOver,
    kGameTutorial,
    kGameTutorialJump,
    kGameTutorialFloat,
    kGameTutorialDrop
    
} GameState;


class GameLayer : public cocos2d::CCLayer
{
    
    Terrain * _terrain;
    Player * _player;
    CCLabelBMFont * _scoreDisplay;
    
    CCSprite * _intro;
    CCSprite * _tryAgain;
    CCSprite * _background;
    CCSprite * _foreground;
    CCSprite * _hat;
    CCSprite * _jam;
    CCAction * _jamAnimate;
    CCAction * _jamMove;
    
    CCArray * _clouds;
    
    CCSpriteBatchNode * _gameBatchNode;
    CCLabelTTF * _tutorialLabel;
    CCMenu* _mainMenu;
    
    CCSize _screenSize;
    
    GameState _state;
    
    bool _running;
    float _score;
    int _speedIncreaseInterval;
	float _speedIncreaseTimer;
    
    void createGameScreen();
    void resetGame();
    
    
    
public:
    
    ~GameLayer();
    
    // Method 'init' in cocos2d-x returns bool, instead of 'id' in cocos2d-iphone (an object pointer)
    virtual bool init();

    // there's no 'id' in cpp, so we recommend to return the class instance pointer
    static cocos2d::CCScene* scene();
    
    // preprocessor macro for "static create()" constructor ( node() deprecated )
    CREATE_FUNC(GameLayer);
    
    void update (float dt);
    
    virtual void ccTouchesBegan(CCSet* pTouches, CCEvent* event);
    virtual void ccTouchesEnded(CCSet* pTouches, CCEvent* event);
    
    void showTutorial (CCObject* pSender);
    void startGame (CCObject* pSender);
};


#endif // __GAMESCENE_H__


