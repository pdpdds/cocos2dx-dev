#include "AGGameIntro.h"
#include "cocos2d.h"
#include "HelloWorldScene.h"

USING_NS_CC;

AGGameIntro::AGGameIntro(void)
{
}

AGGameIntro::~AGGameIntro(void)
{
}

bool AGGameIntro::OnEnter()
{
	 CCDirector *pDirector = CCDirector::sharedDirector();

	// create a scene. it's an autorelease object
	CCScene *pScene = HelloWorld::scene();

	// run
	pDirector->runWithScene(pScene);

	return true;
}

bool AGGameIntro::OnLeave()
{
	return true;
}