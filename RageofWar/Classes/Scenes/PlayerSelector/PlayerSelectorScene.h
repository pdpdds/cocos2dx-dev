#ifndef __PlayerSelector_SCENE_H__
#define __PlayerSelector_SCENE_H__

#include "cocos2d.h"



#include "Scenes/Levels/LevelGenerator.h" 
#include "Scenes/MainMenu/MainScene.h" 

#include "SimpleAudioEngine.h"

class PlayerSelectorScene : public cocos2d::CCLayer
{
public:
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  

    // there's no 'id' in cpp, so we recommand to return the exactly class pointer
    static cocos2d::CCScene* scene(int leveltype);

    // a selector callback
    virtual void Player1Selected(CCObject* pSender);
	virtual void Player2Selected(CCObject* pSender);
	virtual void Player3Selected(CCObject* pSender);
	void soundLogic(float dt);
    // implement the "static node()" method manually
    NODE_FUNC(PlayerSelectorScene);
};

#endif  // __Menu_SCENE_H__