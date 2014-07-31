#include "IntroLayer3.h"
#include "Scenes/Levels/LevelGenerator.h"
#include "Scenes/LevelSelector/LevelSelector.h"
#include "Scenes/PlayerSelector/PlayerSelectorScene.h"
#include "SimpleAudioEngine.h"
using namespace cocos2d;
 using namespace CocosDenshion ;
void IntroLayer3::startGamePlay()
{
	CCLOG("Intro complete, asking Game Manager to start the Game play");
	//GameManager::sharedGameManager()->runSceneWithID(kGameLevel1);	
	CCScene *pScene =PlayerSelectorScene::scene(3);
	 CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(2.0f, pScene,ccWHITE));
}

void IntroLayer3::ccTouchesBegan(cocos2d::CCSet *touches, cocos2d::CCEvent *event)
{
	if (hasBeenSkipped == false)
	{
		CCLOG("Touches received, skipping intro");
		this->startGamePlay();
		hasBeenSkipped = true;
	}
}

bool IntroLayer3::init()
{
	bool pRet = false;
	if (CCLayer::init())
	{
		// Accept touch input
		this->setTouchEnabled(true);
		
		hasBeenSkipped = false;

		// Create the intro image
		CCSprite *introImage = CCSprite::create("Menus/Intro/bgIntro31.png");
		introImage->setPosition(ccp(SCREEN_WIDTH/2, SCREEN_HEIGHT/2));
		this->addChild(introImage);
		SimpleAudioEngine::sharedEngine()->playBackgroundMusic(SOUND_MENU) ;
		// Create the intro animation, and load it from intro1 to intro7.png
		CCAnimation *introAnimation = CCAnimation::create();
        introAnimation->setDelayPerUnit(3.5f);

		char frameName[100] = {0};

		for (int frameNumber=1; frameNumber < 4; frameNumber++) 
		{
			CCLOG("Adding image bgIntro%d.png to the introAnimation.",frameNumber);
			sprintf(frameName, "Menus/Intro/bgIntro3%d.png", frameNumber);
			introAnimation->addSpriteFrameWithFileName(frameName);
		}

		// Create the actions to play the intro
		CCFiniteTimeAction *animationAction = CCAnimate::create(introAnimation);
		CCFiniteTimeAction *startGameAction = CCCallFunc::create(this, callfunc_selector(IntroLayer3::startGamePlay));
		CCFiniteTimeAction *introSequence = CCSequence::create(animationAction, startGameAction, NULL);
		
		introImage->runAction(introSequence);
		pRet = true;
	}

	return pRet;
}