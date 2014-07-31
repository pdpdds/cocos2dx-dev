#include "PresentLayer.h"
#include "TitleLayer.h"

using namespace cocos2d;

CCScene* PresentLayer::scene()
{
    CCScene * scene = NULL;
    do 
    {
        // 'scene' is an autorelease object
        scene = CCScene::create();
        CC_BREAK_IF(! scene);

        // 'layer' is an autorelease object
        PresentLayer *layer = PresentLayer::create();
        CC_BREAK_IF(! layer);

        // add layer as a child to scene
        scene->addChild(layer);
    } while (0);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool PresentLayer::init()
{
    bool bRet = false;
    do 
    {
        //////////////////////////////////////////////////////////////////////////
        // super init first
        //////////////////////////////////////////////////////////////////////////

        CC_BREAK_IF(! CCLayer::init());

        // 3. Add add a splash screen, show the cocos2d splash image.
        CCSprite* pSprite = CCSprite::create("4.png");
        CC_BREAK_IF(! pSprite);

		 CCSize size = CCDirector::sharedDirector()->getWinSize();

        // Place the sprite on the center of the screen
        pSprite->setPosition(ccp(size.width/2, size.height/2));

        // Add the sprite to HelloWorld layer as a child layer.
        this->addChild(pSprite, 0);


		this->runAction(CCSequence::create(CCDelayTime::create(3),
			CCCallFunc::create(this, callfunc_selector(PresentLayer::GoTitleScreen)),
										NULL));

        bRet = true;
    } while (0);

    return bRet;
}

void PresentLayer::GoTitleScreen()
{
	CCScene* pScene = TitleLayer::scene();

	if(pScene)
	{
		CCDirector::sharedDirector()->replaceScene(pScene);
	}
}


