

#include "Scenes/LevelSelector/LevelSelector.h"
# include "Scenes/Intro/IntroScene.h"
# include "Scenes/Intro/IntroScene2.h"
# include "Scenes/Intro/IntroScene3.h"
#include "Scenes/PlayerSelector/PlayerSelectorScene.h"
#include "Scenes/Levels/LevelGenerator.h" 
#include "Scenes/MainMenu/MainScene.h" 

  #include "SimpleAudioEngine.h"
 using namespace cocos2d;
 using namespace CocosDenshion ;


CCScene* LevelSelectorScene::scene()
{
    CCScene * scene = NULL;
    do 
    {
        // 'scene' is an autorelease object
        scene = CCScene::create();
        CC_BREAK_IF(! scene);

        // 'layer' is an autorelease object
        LevelSelectorScene *layer = LevelSelectorScene::node();
        CC_BREAK_IF(! layer);

        // add layer as a child to scene
        scene->addChild(layer);
    } while (0);

    // return the scene
    return scene;
}

void LevelSelectorScene::Menulevel3Callback(CCObject* pSender)
{
	SimpleAudioEngine::sharedEngine()->playBackgroundMusic("Sound/buttonclick.mp3") ;
  CCScene *pScene =IntroScene3::node();
	
 CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(2.0f, pScene,ccWHITE));
}
void LevelSelectorScene::MenuDefenceCallback(CCObject* pSender)
{
	SimpleAudioEngine::sharedEngine()->playBackgroundMusic("Sound/buttonclick.mp3") ;
  CCScene *pScene =IntroScene2::node();
	
 CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(2.0f, pScene,ccWHITE));
}
void LevelSelectorScene::MenuAttackCallback(CCObject* pSender)
{
 SimpleAudioEngine::sharedEngine()->playBackgroundMusic("Sound/buttonclick.mp3") ;
	 CCScene *pScene =IntroScene::node();
	
 CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(2.0f, pScene,ccWHITE));
}
// on "init" you need to initialize your instance
bool LevelSelectorScene::init()
{
    bool bRet = false;
    do 
    {
        //////////////////////////////////////////////////////////////////////////
        // super init first
        //////////////////////////////////////////////////////////////////////////

        CC_BREAK_IF(! CCLayer::init());
		CCSize size = CCDirector::sharedDirector()->getWinSize();
       
        //////////////////////////////////////////////////////////////////////////
        // MenuAttack
        //////////////////////////////////////////////////////////////////////////

        // 1. Add a menu item with "X" image, which is clicked togo to Attack Mode of game
		CCMenuItemImage *pLevel1 = CCMenuItemImage::create(
            "SpriteImages/SurvivalMode.png",
            "SpriteImages/SurvivalMode2.png",
            this,
            menu_selector(LevelSelectorScene::MenuAttackCallback));
        CC_BREAK_IF(! pLevel1);
		 // Place the attack menu item in center.
        pLevel1->setPosition(ccp(250,150));

        // Create a menu with the "close" menu item, it's an auto release object.
        CCMenu* pMenuLevel1 = CCMenu::createWithItem(pLevel1);
        pMenuLevel1->setPosition(CCPointZero);
        CC_BREAK_IF(! pMenuLevel1);

        // Add the menu to Menu layer as a child layer.
        this->addChild(pMenuLevel1, 1);
		
		  //////////////////////////////////////////////////////////////////////////
        // MenuDefence
        //////////////////////////////////////////////////////////////////////////

         // 1. Add a menu item with "X" image, which is clicked to quit the program.
		CCMenuItemImage *pLevel2 = CCMenuItemImage::create(
            "SpriteImages/TimeMode.png",
            "SpriteImages/TimeMode2.png",
            this,
            menu_selector(LevelSelectorScene::MenuDefenceCallback));
        CC_BREAK_IF(! pLevel2);
		 // Place the Defence menu item in center.
        pLevel2->setPosition(ccp(size.width/2+3,150));

        // Create a menu with the "close" menu item, it's an auto release object.
		CCMenu* pMenuLevel2 = CCMenu::createWithItem(pLevel2);
        pMenuLevel2->setPosition(CCPointZero);
        CC_BREAK_IF(! pMenuLevel2);

        // Add the menu to Menu layer as a child layer.
        this->addChild(pMenuLevel2, 1);


		
				  //////////////////////////////////////////////////////////////////////////
        // MenuDefence
        //////////////////////////////////////////////////////////////////////////

         // 1. Add a menu item with "X" image, which is clicked to quit the program.
		CCMenuItemImage *pLevel3 = CCMenuItemImage::create(
            "SpriteImages/EndlessMode.png",
            "SpriteImages/EndlessMode2.png",
            this,
            menu_selector(LevelSelectorScene::Menulevel3Callback));
        CC_BREAK_IF(! pLevel3);
		 // Place the Defence menu item in center.
        pLevel3->setPosition(ccp(size.width/2+230,150));

        // Create a menu with the "close" menu item, it's an auto release object.
        CCMenu* pMenuLevel3 = CCMenu::createWithItem(pLevel3);
        pMenuLevel3->setPosition(CCPointZero);
        CC_BREAK_IF(! pMenuLevel3);

        // Add the menu to Menu layer as a child layer.
        this->addChild(pMenuLevel3, 1);

		
		//////////////////////////////////////////////////////////////////////////
        // Menu
        //////////////////////////////////////////////////////////////////////////
     

        // Get window size and place the label upper. 
//        CCSize size = CCDirector::sharedDirector()->getWinSize();
       
		// 3. Add add a splash screen, show the cocos2d splash image.
        CCSprite* pSprite = CCSprite::create("SpriteImages/bgEmpty.png");
        CC_BREAK_IF(! pSprite);

        // Place the sprite on the center of the screen
        pSprite->setPosition(ccp(size.width/2, size.height/2));

        // Add the sprite to Menu layer as a child layer.
        this->addChild(pSprite, 0);


		 pSprite = CCSprite::create("SpriteImages/Gamemode.png");
        pSprite->setPosition(ccp(size.width/2, size.height-20));
        this->addChild(pSprite, 0);

       SimpleAudioEngine::sharedEngine()->playBackgroundMusic(SOUND_MENU) ;
		
		//this->schedule(schedule_selector(LevelSelectorScene::soundLogic),56.0 );
        bRet = true;
    } while (0);

    return bRet;
}

void LevelSelectorScene::MenuCloseCallback(CCObject* pSender)
{
    // "close" menu item clicked
    CCDirector::sharedDirector()->end();
}

void LevelSelectorScene::soundLogic(float dt)
{

	SimpleAudioEngine::sharedEngine()->playEffect("CircusMusic.mp3") ;
}