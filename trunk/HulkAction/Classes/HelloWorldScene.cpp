#include "HelloWorldScene.h"

using namespace cocos2d;
using namespace CocosDenshion ;

CCScene* HelloWorld::scene()
{
    CCScene * scene = NULL;
    do 
    {
        // 'scene' is an autorelease object
        scene = CCScene::create();
        CC_BREAK_IF(! scene);

        // 'layer' is an autorelease object
        HelloWorld *layer = HelloWorld::node();
        CC_BREAK_IF(! layer);

        // add layer as a child to scene
        scene->addChild(layer);
    } while (0);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
	bool bRet = false;
	do 
	{
		//////////////////////////////////////////////////////////////////////////
		// super init first
		//////////////////////////////////////////////////////////////////////////

		CC_BREAK_IF(! CCLayer::init());

		//////////////////////////////////////////////////////////////////////////
		// add your codes below...
		//////////////////////////////////////////////////////////////////////////

		// 1. Add a menu item with "X" image, which is clicked to quit the program.

		// Create a "close" menu item with close icon, it's an auto release object.
		CCMenuItemImage *pCloseItem = CCMenuItemImage::create(
			"CloseNormal.png",
			"CloseSelected.png",
			this,
			menu_selector(HelloWorld::menuCloseCallback));
		CC_BREAK_IF(! pCloseItem);

		// Place the menu item bottom-right conner.
		pCloseItem->setPosition(ccp(CCDirector::sharedDirector()->getWinSize().width - 20, 20));

		// Create a menu with the "close" menu item, it's an auto release object.
		CCMenu* pMenu = CCMenu::createWithItem(pCloseItem);
		pMenu->setPosition(CCPointZero);
		CC_BREAK_IF(! pMenu);

		// Add the menu to HelloWorld layer as a child layer.
		this->addChild(pMenu, 1);

		// 2. Add a label shows "Hello World".

		// Create a label and initialize with string "Hello World".
		CCLabelTTF* pLabel = CCLabelTTF::create("THE HULK", "Thonburi", 34);
		CC_BREAK_IF(! pLabel);

		// Get window size and place the label upper. 
		CCSize size = CCDirector::sharedDirector()->getWinSize();
		pLabel->setPosition(ccp(size.width / 2, size.height - 20));

		// Add the label to HelloWorld layer as a child layer.
		this->addChild(pLabel, 1);

		// add "HelloWorld" splash screen"
		CCSprite* pSprite = CCSprite::create("streetbackground.jpg");

		// position the sprite on the center of the screen
		pSprite->setPosition( ccp(size.width/2, size.height/2) );

		// add the sprite as a child to this layer
		this->addChild(pSprite, 0);

		///////////////////////////////////////////////////////////////////////////////
		//Generate Menu bar for hulk actions
		generateHulkMenu();
		///////////////////////////////////////////////////////////////////////////////
		CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("hulkmoves.plist");
		CCSpriteBatchNode * sceneSpriteBatchNode = CCSpriteBatchNode::create("hulkmoves.png");
		this->addChild(sceneSpriteBatchNode, 0);

		CCSize winSize = CCDirector::sharedDirector()->getWinSize();
		//hulk=  CCSprite::spriteWithFile("slice_0_1.png.png");
		hulk=CCSprite::createWithSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("slice_0_1.png"));

		hulk->setScale(0.8);
		hulk->setPosition(ccp(winSize.width/2,winSize.height*0.4));
		addChild(hulk, 1);




		bRet = true;
	} while (0);

	return bRet;
}
 void HelloWorld::GenerateHulkAnimation(char *Name,int Max,cocos2d::CCSprite* sp)
 {
	 char *frameName = new char[1024];
	 cocos2d::CCAnimate *_curAnimate;
	 CCAnimation* animaiton = CCAnimation::create();

	 for(int i = 1; i <= Max; ++i){

		 sprintf(frameName, "%s%d.png", Name, i);
		 animaiton->addSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(frameName)); 



	 }
	 for(int i = 1; i <= Max; ++i){

		 animaiton->addSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(frameName)); 

	 }
	 animaiton->setDelayPerUnit(0.1);
	 _curAnimate = CCAnimate::create(animaiton);
	 //CCActionInterval*   action =   CCAnimate::actionWithDuration(1);
	 //CCRepeatForever*    repeatAction   =   CCRepeatForever::actionWithAction(action);

	 //sp->runAction(repeatAction);
	 sp->runAction(_curAnimate);
		
       
}
 void HelloWorld::generateHulkMenu()
 {
	  CCMenuItemImage *punchItem = CCMenuItemImage::create(
            "punchbutton.png",
            "punchbutton.png",
            this,
            menu_selector(HelloWorld::runPunchAnimation));        
        punchItem->setPosition((ccp(CCDirector::sharedDirector()->getWinSize().width /2 -200,CCDirector::sharedDirector()->getWinSize().height -30)));

        
        CCMenu* pMenu = CCMenu::createWithItem(punchItem);
        pMenu->setPosition(CCPointZero);        
        this->addChild(pMenu, 1);

		/////////////////////////////////////////////////////////////////////////////////
		CCMenuItemImage *namasteItem = CCMenuItemImage::create(
            "namastebutton.png",
            "namastebutton.png",
            this,
            menu_selector(HelloWorld::runNamasteAnimation));        
        namasteItem->setPosition((ccp(CCDirector::sharedDirector()->getWinSize().width /2 -200,CCDirector::sharedDirector()->getWinSize().height -80)));

        
        CCMenu* pMenu2 = CCMenu::createWithItem(namasteItem);
        pMenu2->setPosition(CCPointZero);        
        this->addChild(pMenu2, 1);
		/////////////////////////////////////////////////////////////////////////////////
		CCMenuItemImage *fistItem = CCMenuItemImage::create(
            "fistbutton.png",
            "fistbutton.png",
            this,
            menu_selector(HelloWorld::runFistAnimation));        
        fistItem->setPosition((ccp(CCDirector::sharedDirector()->getWinSize().width /2 -200,CCDirector::sharedDirector()->getWinSize().height -130)));

        
        CCMenu* pMenu3 = CCMenu::createWithItem(fistItem);
        pMenu3->setPosition(CCPointZero);        
        this->addChild(pMenu3, 1);
		/////////////////////////////////////////////////////////////////////////////////
		CCMenuItemImage *throwItem = CCMenuItemImage::create(
            "throwbutton.png",
            "throwbutton.png",
            this,
            menu_selector(HelloWorld::runThrowAnimation));        
        throwItem->setPosition((ccp(CCDirector::sharedDirector()->getWinSize().width /2 -200,CCDirector::sharedDirector()->getWinSize().height -180)));

        
        CCMenu* pMenu4 = CCMenu::createWithItem(throwItem);
        pMenu4->setPosition(CCPointZero);        
        this->addChild(pMenu4, 1);
 }
 void HelloWorld::runPunchAnimation(CCObject* pSender)
{
	SimpleAudioEngine::sharedEngine()->playEffect("angrypunch.mp3");
    GenerateHulkAnimation("slice_0_",7,hulk);
	
}
 void HelloWorld::runFistAnimation(CCObject* pSender)
{
   // GenerateHulkAnimation("fist_thump_1_",6,hulk);
	SimpleAudioEngine::sharedEngine()->playEffect("angrypunch.mp3");
	cocos2d::CCAnimate *_curAnimate;
	CCAnimation* animaiton = CCAnimation::create();

	for(int i = 1; i <= 6; ++i){
	char *frameName = new char[1024];
	sprintf(frameName, "%s%d.png", "fist_thump_1_", i);
	animaiton->addSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(frameName)); 
		


	}
	for(int i = 1; i <= 6; ++i){
	
		animaiton->addSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("fist_thump_1_6.png")); 
	
	}
		animaiton->setDelayPerUnit(0.1);
		_curAnimate = CCAnimate::create(animaiton);
		// CCActionInterval*   action =   CCAnimate::actionWithDuration(1);
		//CCRepeatForever*    repeatAction   =   CCRepeatForever::actionWithAction(action);

		//sp->runAction(repeatAction);
		hulk->runAction(_curAnimate);
}

 void HelloWorld::runThrowAnimation(CCObject* pSender)
{
	SimpleAudioEngine::sharedEngine()->playEffect("angrypunch.mp3");
    GenerateHulkAnimation("stone_throw_3_",8,hulk);
}

 void HelloWorld::runNamasteAnimation(CCObject* pSender)
{
	SimpleAudioEngine::sharedEngine()->playEffect("angrypunch.mp3");
    //GenerateHulkAnimation("namaste_1_",4,hulk);
	cocos2d::CCAnimate *_curAnimate;
	CCAnimation* animaiton = CCAnimation::create();

	for(int i = 1; i <= 4; ++i){
	char *frameName = new char[1024];
	sprintf(frameName, "%s%d.png", "namaste_1_", i);
	animaiton->addSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(frameName)); 
		


	}
	for(int i = 1; i <= 8; ++i){
	
		animaiton->addSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("namaste_1_4.png")); 
	
	}
		animaiton->setDelayPerUnit(0.1);
		_curAnimate = CCAnimate::create(animaiton);
		// CCActionInterval*   action =   CCAnimate::actionWithDuration(1);
		//CCRepeatForever*    repeatAction   =   CCRepeatForever::actionWithAction(action);

		//sp->runAction(repeatAction);
		hulk->runAction(_curAnimate);
	
}

 void HelloWorld::menuCloseCallback(CCObject* pSender)
{
    // "close" menu item clicked
    CCDirector::sharedDirector()->end();
}

