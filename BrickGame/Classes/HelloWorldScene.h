#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"

USING_NS_CC;

class HelloWorld : public cocos2d::CCLayer
{
public:
    static cocos2d::CCScene* createScene();

    virtual bool init();  
    
    CREATE_FUNC(HelloWorld);


    CCTexture2D*  texture;
    CCSprite*     ball;
    CCSprite*     paddle;
    std::vector<CCSprite*> targets;

    int         BRICKS_HEIGHT;
    int         BRICKS_WIDTH;

    bool        isPlaying;
    bool        isPaddleTouched;

    CCPoint       ballMovement;

    ~HelloWorld();
    virtual void onEnter();
    virtual void onExit();
    virtual bool ccTouchBegan(cocos2d::CCTouch* touch, cocos2d::CCEvent* event) override;
	virtual void ccTouchMoved(cocos2d::CCTouch* touch, cocos2d::CCEvent* event) override;

	virtual void ccTouchesBegan(cocos2d::CCSet* touches, cocos2d::CCEvent* event);
	virtual void ccTouchesMoved(CCSet *touches, CCEvent *event);
	virtual void ccTouchesEnded(CCSet *touches, CCEvent *event);

    void initializeBricks();
    void initializeBall();
    void initializePaddle();
    void startGame(float dt);
    void gameLogic(float dt);
    void processCollision(CCSprite* brick);
};

#endif // __HELLOWORLD_SCENE_H__
