#ifndef __SSLASER_H__
#define __SSLASER_H__

#include "cocos2d.h"

#define STATE_LASER_WAIT 0
#define STATE_LASER 1
#define STATE_LASER_FINAL 2
#define STATE_LASER_DONE 3

USING_NS_CC;

class SSLaser : public cocos2d::CCSprite
{
public:
    virtual void update(float dt);
	
	static SSLaser* spriteWithFile(const char *pszFileName);
	void Setup(CCLayer* pOwner);

    CC_SYNTHESIZE(cocos2d::CCPoint, velocity_, Velocity);
    CC_SYNTHESIZE(int, size_, Size);
	CC_SYNTHESIZE(bool, interact_, Interact);
	CC_SYNTHESIZE(int, State_, State);

	// implement the "static node()" method manually
	NODE_FUNC(SSLaser);

protected:

private:
	float ScaleFactor_;
	float WaitTime_;
	CCLayer* m_pOwner;
};

#endif //