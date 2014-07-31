#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"

#include "Box2D/Box2D.h"

#include "SimpleAudioEngine.h"

class HelloWorld : public cocos2d::CCLayer
{
private:
	cocos2d::CCSprite* hulk;
public:
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  

    // there's no 'id' in cpp, so we recommand to return the exactly class pointer
    static cocos2d::CCScene* scene();
    
    // a selector callback
    virtual void menuCloseCallback(CCObject* pSender);

    // implement the "static node()" method manually
    NODE_FUNC(HelloWorld);
	void GenerateHulkAnimation(char *Name,int Max,cocos2d::CCSprite* sp);
	void generateHulkMenu();
	void runPunchAnimation(CCObject* pSender);
	void runNamasteAnimation(CCObject* pSender);
	void runFistAnimation(CCObject* pSender);
	void runThrowAnimation(CCObject* pSender);
};

#endif  // __HELLOWORLD_SCENE_H__