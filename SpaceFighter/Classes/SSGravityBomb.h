#ifndef __SSGravityBomb_H__
#define __SSGravityBomb_H__

#include "cocos2d.h"

#define STATE_DISABLE 0
#define STATE_ENABLE 1
#define STATE_FINAL 2
#define STATE_DONE 3

USING_NS_CC;

class SSGravityBomb : public cocos2d::CCSprite
{
public:
    virtual void update(float dt);
	
	static SSGravityBomb* spriteWithFile(const char *pszFileName);
	void Setup(CCLayer* pOwner);

    CC_SYNTHESIZE(cocos2d::CCPoint, velocity_, Velocity);
    CC_SYNTHESIZE(int, size_, Size);
	CC_SYNTHESIZE(bool, interact_, Interact);
	CC_SYNTHESIZE(int, State_, State);

	// implement the "static node()" method manually
	NODE_FUNC(SSGravityBomb);

protected:

private:
	float ScaleFactor_;
	float WaitTime_;
	CCLayer* m_pOwner;
};

#endif //