#include "TitleLayer.h"
#include "GameLayer.h"

using namespace cocos2d;

CCScene* TitleLayer::scene()
{
    CCScene * scene = NULL;
    do 
    {
        // 'scene' is an autorelease object
        scene = CCScene::create();
        CC_BREAK_IF(! scene);

        // 'layer' is an autorelease object
        TitleLayer *layer = TitleLayer::create();
        CC_BREAK_IF(! layer);

        // add layer as a child to scene
        scene->addChild(layer);
    } while (0);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool TitleLayer::init()
{
    bool bRet = false;
    do 
    {
        //////////////////////////////////////////////////////////////////////////
        // super init first
        //////////////////////////////////////////////////////////////////////////

        CC_BREAK_IF(! CCLayer::init());

        // 3. Add add a splash screen, show the cocos2d splash image.
		CCSprite* pSprite = CCSprite::create("Mountain2.png");
        CC_BREAK_IF(! pSprite);

		 CCSize size = CCDirector::sharedDirector()->getWinSize();

        // Place the sprite on the center of the screen
        pSprite->setPosition(ccp(size.width/2, size.height/2));

		pSprite->setScaleX(size.width / pSprite->getContentSize().width);
		pSprite->setScaleY(size.height / pSprite->getContentSize().height);

        // Add the sprite to HelloWorld layer as a child layer.
        this->addChild(pSprite, 0);

		// 1. Add a menu item with "X" image, which is clicked togo to Attack Mode of game
		CCMenuItemImage *pLevel1 = CCMenuItemImage::create(
            "StartGame.png",
            "StartGame2.png",
            this,
            menu_selector(TitleLayer::MenuStartCallback));
        CC_BREAK_IF(! pLevel1);
		 // Place the attack menu item in center.
        pLevel1->setPosition(ccp(size.width/2,size.height/2));

        // Create a menu with the "close" menu item, it's an auto release object.
		CCMenu* pMenuLevel1 = CCMenu::create(pLevel1, NULL);
        pMenuLevel1->setPosition(CCPointZero);
        CC_BREAK_IF(! pMenuLevel1);

        // Add the menu to Menu layer as a child layer.
        this->addChild(pMenuLevel1, 1);


		//this->runAction(CCSequence::actions(CCDelayTime::actionWithDuration(3),
								//		CCCallFunc::actionWithTarget(this, callfunc_selector(PresentLayer::GoTitleScreen)),
									//	NULL));

        bRet = true;
    } while (0);

    return bRet;
}

void TitleLayer::MenuStartCallback(CCObject* pSender)
{
	CCDirector::sharedDirector()->replaceScene(GameLayer::scene());
}
