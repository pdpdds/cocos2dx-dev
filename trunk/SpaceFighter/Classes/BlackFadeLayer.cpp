#include "BlackFadeLayer.h"
#include "PresentLayer.h"
#include "Transition.h"

BlackFadeLayer::BlackFadeLayer(void)
{
}


BlackFadeLayer::~BlackFadeLayer(void)
{
}

void BlackFadeLayer::onEnter()
{
    CCLayer::onEnter();
    CCLog("Scene 2 onEnter");
}

void BlackFadeLayer::onEnterTransitionDidFinish()
{
	CCLayer::onEnterTransitionDidFinish();
	CCLog("Scene 2: onEnterTransitionDidFinish");

	this->runAction(CCSequence::create(CCDelayTime::create(3),
		CCCallFunc::create(this, callfunc_selector(BlackFadeLayer::ChangeLogo)),
										NULL));

	
}

void BlackFadeLayer::onExitTransitionDidStart()
{
    CCLayer::onExitTransitionDidStart();
    CCLog("Scene 2: onExitTransitionDidStart");

	
}

void BlackFadeLayer::onExit()
{
    CCLayer::onExit();
    CCLog("Scene 2 onExit");
}

void BlackFadeLayer::ChangeLogo()
{
    CCScene *pPresentScene = PresentLayer::scene();

	//CCScene* pScene = FadeWhiteTransition::create(TRANSITION_DURATION, pPresentScene); 

	//if (pScene)
	{
		CCDirector::sharedDirector()->replaceScene(pPresentScene);
	}
}
