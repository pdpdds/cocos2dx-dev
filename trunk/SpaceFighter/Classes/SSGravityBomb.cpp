#include "SSGravityBomb.h"

SSGravityBomb* SSGravityBomb::spriteWithFile(const char *pszFileName)
{
    SSGravityBomb *pobSprite = new SSGravityBomb();
    if (pobSprite && pobSprite->initWithFile(pszFileName))
    {
        pobSprite->scheduleUpdate();
        pobSprite->autorelease();
        return pobSprite;
    }
    CC_SAFE_DELETE(pobSprite);
	return NULL;
}

void SSGravityBomb::Setup(CCLayer* pOwner)
{
	m_pOwner = pOwner;
	setInteract(false);
	State_ = STATE_DISABLE;
	ScaleFactor_ = 0.5f;
	WaitTime_ = 0.0f;
	setScale(ScaleFactor_);

	CCSize windowSize = CCDirector::sharedDirector()->getWinSize();

	CCAnimation *GravityBombAni = CCAnimation::create();
	GravityBombAni->setDelayPerUnit(0.05);
	
	setPosition(ccp(windowSize.width / 2 + 100, windowSize.height/2));

	GravityBombAni->addSpriteFrameWithFileName("exp0.png");
	GravityBombAni->addSpriteFrameWithFileName("exp1.png");
	GravityBombAni->addSpriteFrameWithFileName("exp2.png");
	GravityBombAni->addSpriteFrameWithFileName("exp3.png");
	GravityBombAni->addSpriteFrameWithFileName("exp4.png");
	GravityBombAni->addSpriteFrameWithFileName("exp5.png");
	GravityBombAni->addSpriteFrameWithFileName("exp6.png");
	GravityBombAni->addSpriteFrameWithFileName("exp7.png");
	
	CCAnimate *animate = CCAnimate::create(GravityBombAni);
	CCRepeatForever *repeat = CCRepeatForever::create(animate);

	runAction(repeat);
}

void SSGravityBomb::update(float dt)
{
    if(STATE_DISABLE == State_)
	{
		WaitTime_ += dt;

		if(WaitTime_ > 2.0f)
		{
			State_ = STATE_ENABLE;
			WaitTime_ = 0;
		}	
	}
	else if(STATE_ENABLE == State_)
	{
		ScaleFactor_ = ScaleFactor_ + (dt * 4.0f);

		if(ScaleFactor_ >= 2.0f)
		{
			ScaleFactor_ = 2.0f;
			State_ = STATE_FINAL;
		}

		setScale(ScaleFactor_);
	}
	else if(STATE_FINAL == State_)
	{
		WaitTime_ += dt;

		if(WaitTime_ > 2.0f)
		{
			m_pOwner->removeChild(this, false);
			State_ = STATE_DONE;
			WaitTime_ = 0;
		}
	}
}