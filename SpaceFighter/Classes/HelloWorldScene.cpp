#include "HelloWorldScene.h"
#include "LogoScene.h"
#include "BlackFadeLayer.h"
#include "Transition.h"

using namespace cocos2d;

CCScene* HelloWorld::scene()
{
    CCScene * scene = NULL;
    do 
    {
        // 'scene' is an autorelease object
        scene = CCScene::create();
        CC_BREAK_IF(! scene);

        // 'layer' is an autorelease object
        HelloWorld *layer = HelloWorld::create();
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

        // 3. Add add a splash screen, show the cocos2d splash image.
        CCSprite* pSprite = CCSprite::create("HelloWorld.png");
        CC_BREAK_IF(! pSprite);

		 CCSize size = CCDirector::sharedDirector()->getWinSize();

        // Place the sprite on the center of the screen
        pSprite->setPosition(ccp(size.width/2, size.height/2));

        // Add the sprite to HelloWorld layer as a child layer.
        this->addChild(pSprite, 0);


		this->runAction(CCSequence::create(CCDelayTime::create(3),
			CCCallFunc::create(this, callfunc_selector(HelloWorld::ChangeLogo)),
										NULL));

        bRet = true;
    } while (0);

    return bRet;
}

void HelloWorld::ChangeLogo()
{
 
	CCScene* pLogoScene = new LogoScene();

    CCLayer* pLayer = new BlackFadeLayer();
    pLogoScene->addChild(pLayer);
    
	CCScene* pScene = FadeWhiteTransition::create(TRANSITION_DURATION, pLogoScene); 
	
	pLayer->release();
	pLogoScene->release();

    if (pScene)
    {
        CCDirector::sharedDirector()->replaceScene(pScene);
   } 
   
}
