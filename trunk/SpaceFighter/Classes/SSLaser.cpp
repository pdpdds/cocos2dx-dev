#include "SSLaser.h"
#include "GameLayer.h"
#include "SSUtil.h"

SSLaser* SSLaser::spriteWithFile(const char *pszFileName)
{
    SSLaser *pobSprite = new SSLaser();
    if (pobSprite && pobSprite->initWithFile(pszFileName))
    {
        pobSprite->scheduleUpdate();
        pobSprite->autorelease();
        return pobSprite;
    }
    CC_SAFE_DELETE(pobSprite);
	return NULL;
}

void SSLaser::Setup(CCLayer* pOwner)
{
	m_pOwner = pOwner;
	setInteract(false);
	State_ = STATE_LASER_WAIT;
	ScaleFactor_ = 0.5f;
	WaitTime_ = 0.0f;
	setScaleX(ScaleFactor_);
	setPosition(ccp(0,20));
	setVisible(false);

	/*CCSize windowSize = CCDirector::sharedDirector()->getWinSize();

	CCAnimation *GravityBombAni = CCAnimation::animation();
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
	
	CCAnimate *animate = CCAnimate::actionWithAnimation(GravityBombAni);
	CCRepeatForever *repeat = CCRepeatForever::actionWithAction(animate);

	runAction(repeat);*/
}

void SSLaser::update(float dt)
{
    if(STATE_LASER_WAIT == State_)
	{
		WaitTime_ += dt;

		if(WaitTime_ > 2.0f)
		{
			State_ = STATE_LASER;

			float x;
			float y;
			((GameLayer*)(m_pOwner))->GetShip()->getPosition(&x, &y);

			float rad = SSUtil::radiansBetweenLines(CCPoint(0,0), CCPoint(1,0), CCPoint(0,20), CCPoint(x,y));
		//	radiansBetweenLines()
			

			this->setRotation(this->getRotation() - CC_RADIANS_TO_DEGREES(rad));
			WaitTime_ = 0;
			setVisible(true);
		}	
	}
	else if(STATE_LASER == State_)
	{
		//ScaleFactor_ = ScaleFactor_ + (dt * 16.0f);

		//if(ScaleFactor_ >= 16.0f)
		{
			ScaleFactor_ = 16.0f;
			State_ = STATE_LASER_FINAL;
		}

		setScaleX(ScaleFactor_);
	}
	else if(STATE_LASER_FINAL == State_)
	{
		WaitTime_ += dt;

		if(WaitTime_ > 2.0f)
		{
			m_pOwner->removeChild(this, false);
			State_ = STATE_LASER_DONE;
			WaitTime_ = 0;
		}
	}
}