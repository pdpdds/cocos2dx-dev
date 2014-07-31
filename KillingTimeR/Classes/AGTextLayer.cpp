#include "AGTextLayer.h"

AGTextLayer::AGTextLayer(void)
{
}

AGTextLayer::~AGTextLayer(void)
{
}

bool AGTextLayer::init()
{
	bool bRet = false;
	do 
	{
		//////////////////////////////////////////////////////////////////////////
		// super init first
		//////////////////////////////////////////////////////////////////////////

		CC_BREAK_IF(! CCLayer::init());

		// Get window size
		CCSize windowSize = CCDirector::sharedDirector()->getWinSize();

		// 3. Add add a splash screen, show the cocos2d splash image.
		CCSprite* pSprite = CCSprite::create("Scene/Outer.PNG");
		CC_BREAK_IF(! pSprite);

		//pSprite->setContentSize(windowSize);
		pSprite->setScaleX(windowSize.width / pSprite->getContentSize().width);
		pSprite->setScaleY(windowSize.height / pSprite->getContentSize().height);

		// Place the sprite on the center of the screen
		pSprite->setPosition(ccp(windowSize.width/2, windowSize.height/2));

		// Add the sprite to HelloWorld layer as a child layer.
		this->addChild(pSprite, 0);


		// 3. Add add a splash screen, show the cocos2d splash image.
		pSprite = CCSprite::create("/Text/Text_Layer.png");
		CC_BREAK_IF(! pSprite);

		//pSprite->setContentSize(windowSize);
		pSprite->setScaleX(windowSize.width / pSprite->getContentSize().width);
		pSprite->setScaleY(windowSize.height / pSprite->getContentSize().height);

		// Place the sprite on the center of the screen
		pSprite->setPosition(ccp(windowSize.width/2, windowSize.height/2));
		pSprite->setOpacity(50);

		// Add the sprite to HelloWorld layer as a child layer.
		this->addChild(pSprite, 0);

		pSprite->setOpacity( 0 );
		CCActionInterval*  action1 = CCFadeTo::create(0.5f, 50);
		
		pSprite->runAction( CCSequence::create( action1, NULL, NULL));

		
		// Create a label and initialize with string "Hello World".
		CCLabelTTF* pLabel = CCLabelTTF::create("문자열 테스트", "Arial", 16);
		CC_BREAK_IF(! pLabel);

		// Get window size and place the label upper. 
		CCSize size = CCDirector::sharedDirector()->getWinSize();
		pLabel->setPosition(ccp(pLabel->getContentSize().width/2, pLabel->getContentSize().height/2));

		// Add the label to HelloWorld layer as a child layer.
		this->addChild(pLabel, 1);

		bRet = true;
	} while (0);

	return bRet;
}
