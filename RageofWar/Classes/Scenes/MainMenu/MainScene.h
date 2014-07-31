#ifndef __MENU_SCENE_H__
#define __MENU_SCENE_H__

#include "cocos2d.h"



#include "SimpleAudioEngine.h"

class MainScene : public cocos2d::CCLayer
{
public:
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  

    // there's no 'id' in cpp, so we recommand to return the exactly class pointer
    static cocos2d::CCScene* scene();
    
    // a selector callback
    virtual void MenuCloseCallback(CCObject* pSender);
	virtual void MenuAttackCallback(CCObject* pSender);
	virtual void MenuDefenceCallback(CCObject* pSender);
	void soundLogic(float dt);
    // implement the "static node()" method manually
    NODE_FUNC(MainScene);
};

#endif  // __Menu_SCENE_H__