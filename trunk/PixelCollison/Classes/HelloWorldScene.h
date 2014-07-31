#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"

#include "Box2D/Box2D.h"

#include "SimpleAudioEngine.h"

class HelloWorld : public cocos2d::CCLayer
{
public:
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  

    // there's no 'id' in cpp, so we recommand to return the exactly class pointer
    static cocos2d::CCScene* scene();
    
    // a selector callback
    void menuCloseCallback(CCObject* pSender);

    virtual void registerWithTouchDispatcher(void);
    virtual void ccTouchesBegan(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent);
    virtual void ccTouchesMoved(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent);
    virtual void ccTouchesEnded(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent);
    virtual void ccTouchesCancelled(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent); 

    // implement the "static node()" method manually
    CREATE_FUNC(HelloWorld);

	cocos2d::CCSprite *_ship;
	cocos2d::CCSprite *_enemy1;
	cocos2d::CCSprite *_enemy2;
	cocos2d::CCRenderTexture* _rt;
	cocos2d::CCSprite *m_pBrush; 

private:
	cocos2d::CCPoint m_tBeginPos; 
	cocos2d::CCPoint m_tEndPos;

	void	tick(float dt); 
	void	checkCollisions();
	bool	isCollisionBetweenSpriteA(cocos2d::CCSprite* spr1, cocos2d::CCSprite* spr2, bool pp);
};

#endif  // __HELLOWORLD_SCENE_H__