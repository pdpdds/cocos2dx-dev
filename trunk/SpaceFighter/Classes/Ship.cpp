//
//  Ship.cpp
//  Asteroids
//
//  Created by Clawoo on 9/16/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#include "Ship.h"

USING_NS_CC;

Ship* Ship::spriteWithFile(const char *pszFileName)
{
    Ship *pobSprite = new Ship();
    if (pobSprite && pobSprite->initWithFile(pszFileName))
    {
        pobSprite->scheduleUpdate();
        pobSprite->autorelease();
        return pobSprite;
    }
    CC_SAFE_DELETE(pobSprite);
	return NULL;
}

void Ship::update(float dt)
{
    this->setPosition(ccp(this->getPosition().x + velocity_.x, this->getPosition().y + velocity_.y));
    
    CCSize windowSize = CCDirector::sharedDirector()->getWinSize();
    
    if (this->getPosition().x < 0)
    {
        this->setPosition(ccp(windowSize.width, this->getPosition().y));
    }
    else if (this->getPosition().x > windowSize.width)
    {
        this->setPosition(ccp(0, this->getPosition().y));
    }
    
    if (this->getPosition().y < 0)
    {
        this->setPosition(ccp(this->getPosition().x, windowSize.height));
    }
    else if (this->getPosition().y > windowSize.height)
    {
        this->setPosition(ccp(this->getPosition().x, 0));
    }
}

void Ship::Destroy(cocos2d::CCLayer* pOwner)
{
	CCTexture2D* pTexture = CCTextureCache::sharedTextureCache()->addImage("explosion.png");
	m_pOwner = pOwner;

	int ImageWidth = 59;
	int ImageHeight = 64;
	CCSpriteFrame* pFrame0 = CCSpriteFrame::createWithTexture(pTexture, CCRectMake(0, 0, ImageWidth, ImageHeight));
	CCSpriteFrame* pFrame1 = CCSpriteFrame::createWithTexture(pTexture, CCRectMake(ImageWidth, 0, ImageWidth, ImageHeight));
	CCSpriteFrame* pFrame2 = CCSpriteFrame::createWithTexture(pTexture, CCRectMake(ImageWidth * 2, 0, ImageWidth, ImageHeight));
	CCSpriteFrame* pFrame3 = CCSpriteFrame::createWithTexture(pTexture, CCRectMake(ImageWidth * 3, 0, ImageWidth, ImageHeight));
	CCSpriteFrame* pFrame4 = CCSpriteFrame::createWithTexture(pTexture, CCRectMake(ImageWidth * 4, 0, ImageWidth, ImageHeight));

	CCSpriteFrame* pFrame5 = CCSpriteFrame::createWithTexture(pTexture, CCRectMake(0, ImageHeight, ImageWidth, ImageHeight));
	CCSpriteFrame* pFrame6 = CCSpriteFrame::createWithTexture(pTexture, CCRectMake(ImageWidth, ImageHeight, ImageWidth, ImageHeight));
	CCSpriteFrame* pFrame7 = CCSpriteFrame::createWithTexture(pTexture, CCRectMake(ImageWidth * 2, ImageHeight, ImageWidth, ImageHeight));
	CCSpriteFrame* pFrame8 = CCSpriteFrame::createWithTexture(pTexture, CCRectMake(ImageWidth * 3, ImageHeight, ImageWidth, ImageHeight));
	CCSpriteFrame* pFrame9 = CCSpriteFrame::createWithTexture(pTexture, CCRectMake(ImageWidth * 4, ImageHeight, ImageWidth, ImageHeight));
	
	
	CCSprite* pExplosion = CCSprite::createWithSpriteFrame(pFrame0);
	pExplosion->setPosition(getPosition());
	pOwner->addChild(pExplosion);

	CCAnimation *pExplosionAni = CCAnimation::create();
	pExplosionAni->setDelayPerUnit(0.05);
	pExplosionAni->addSpriteFrame(pFrame0);
	pExplosionAni->addSpriteFrame(pFrame1);
	pExplosionAni->addSpriteFrame(pFrame2);
	pExplosionAni->addSpriteFrame(pFrame3);
	pExplosionAni->addSpriteFrame(pFrame4);
	pExplosionAni->addSpriteFrame(pFrame5);
	pExplosionAni->addSpriteFrame(pFrame6);
	pExplosionAni->addSpriteFrame(pFrame7);
	pExplosionAni->addSpriteFrame(pFrame8);
	pExplosionAni->addSpriteFrame(pFrame9);

	CCAnimate *animate = CCAnimate::create(pExplosionAni);
	CCRepeatForever *repeat = CCRepeatForever::create(animate);

	//pExplosion->runAction(repeat);]
	CCFiniteTimeAction* actionDone = CCCallFuncN::create(this, callfuncN_selector(Ship::ExplosionFinished));
	pExplosion->runAction(CCSequence::create(animate, actionDone, NULL));

}

void Ship::ExplosionFinished(CCNode* pSender)
{

	CCSprite* pSprite = (CCSprite*)pSender;
	m_pOwner->removeChild(pSprite, true);
}