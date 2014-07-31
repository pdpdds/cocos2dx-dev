#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"

USING_NS_CC;

class HelloWorld : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();  
    
    CREATE_FUNC(HelloWorld);


    Texture2D*  texture;
    Sprite*     ball;
    Sprite*     paddle;
    Vector<Sprite*> targets;

    int         BRICKS_HEIGHT;
    int         BRICKS_WIDTH;

    bool        isPlaying;
    bool        isPaddleTouched;

    Point       ballMovement;

    ~HelloWorld();
    virtual void onEnter();
    virtual void onExit();
    virtual bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
    virtual void onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event);

    void initializeBricks();
    void initializeBall();
    void initializePaddle();
    void startGame(float dt);
    void gameLogic(float dt);
    void processCollision(Sprite* brick);
};

#endif // __HELLOWORLD_SCENE_H__
