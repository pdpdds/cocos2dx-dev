

#include "Scenes/PlayerSelector/PlayerSelectorScene.h"
#include "Scenes/Levels/LevelGenerator.h" 
#include "Scenes/MainMenu/MainScene.h" 

  #include "SimpleAudioEngine.h"
 using namespace cocos2d;
 using namespace CocosDenshion ;
     int LevelType=1;

CCScene* PlayerSelectorScene::scene(int leveltype)
{
    CCScene * scene = NULL;
    do 
    {
        // 'scene' is an autorelease object
        scene = CCScene::create();
        CC_BREAK_IF(! scene);
		CCLog("Level Type %d",leveltype);
		LevelType=leveltype;
        // 'layer' is an autorelease object
        PlayerSelectorScene *layer = PlayerSelectorScene::node();
        CC_BREAK_IF(! layer);

        // add layer as a child to scene
        scene->addChild(layer);
    } while (0);

    // return the scene
    return scene;
}
//void PlayerSelectorScene::MenuDefenceCallback(CCObject* pSender)
//{
//    /*Level1 *ModeScene = Level1::node();
//  CCDirector::sharedDirector()->replaceScene(ModeScene->scene());*/
//}
//void PlayerSelectorScene::MenuAttackCallback(CCObject* pSender)
//{
// Player1Scene *ModeScene = Player1Scene::node();
// CCDirector::sharedDirector()->replaceScene(ModeScene);
//   
//}

// on "init" you need to initialize your instance
bool PlayerSelectorScene::init()
{
    bool bRet = false;
    do 
    {
        //////////////////////////////////////////////////////////////////////////
        // super init first
        //////////////////////////////////////////////////////////////////////////

        CC_BREAK_IF(! CCLayer::init());
		CCSize size = CCDirector::sharedDirector()->getWinSize();
		 CCSprite* pSprite = CCSprite::create("SpriteImages/bgEmpty.png");
        CC_BREAK_IF(! pSprite);

        // Place the sprite on the center of the screen
        pSprite->setPosition(ccp(size.width/2, size.height/2));

        // Add the sprite to Menu layer as a child layer.
        this->addChild(pSprite, 0);


		 pSprite = CCSprite::create("SpriteImages/playerselector.png");
        pSprite->setPosition(ccp(size.width/2, size.height-20));
        this->addChild(pSprite, 0);
        //////////////////////////////////////////////////////////////////////////
        // MenuAttack
        //////////////////////////////////////////////////////////////////////////
		
        // 1. Add a menu item with "X" image, which is clicked togo to Attack Mode of game
		CCMenuItemImage *pLevel1 = CCMenuItemImage::create(
            "PlayerType/BlackCommando.png",
            "PlayerType/BlackCommando2.png",
            this,
            menu_selector(PlayerSelectorScene::Player1Selected));
        CC_BREAK_IF(! pLevel1);
		 // Place the attack menu item in center.
        pLevel1->setPosition(ccp(200,150));

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
            "PlayerType/GoldenCommando.png",
            "PlayerType/GoldenCommando2.png",
            this,
            menu_selector(PlayerSelectorScene::Player2Selected));
        CC_BREAK_IF(! pLevel2);
		 // Place the Defence menu item in center.
        pLevel2->setPosition(ccp(500,150));

        // Create a menu with the "close" menu item, it's an auto release object.
		CCMenu* pMenuLevel2 = CCMenu::createWithItem(pLevel2);
        pMenuLevel2->setPosition(CCPointZero);
        CC_BREAK_IF(! pMenuLevel2);

        // Add the menu to Menu layer as a child layer.
        this->addChild(pMenuLevel2, 1);


		  // 1. Add a menu item with "X" image, which is clicked to quit the program.
		CCMenuItemImage *pLevel3 = CCMenuItemImage::create(
            "PlayerType/GreenCommando.png",
            "PlayerType/GreenCommando2.png",
            this,
            menu_selector(PlayerSelectorScene::Player3Selected));
        CC_BREAK_IF(! pLevel3);
		 // Place the Defence menu item in center.
        pLevel3->setPosition(ccp(800,150));

        // Create a menu with the "close" menu item, it's an auto release object.
        CCMenu* pMenuLevel3 = CCMenu::createWithItem(pLevel3);
        pMenuLevel3->setPosition(CCPointZero);
        CC_BREAK_IF(! pMenuLevel3);

        // Add the menu to Menu layer as a child layer.
        this->addChild(pMenuLevel3, 1);

		
		//////////////////////////////////////////////////////////////////////////
        // Menu
        //////////////////////////////////////////////////////////////////////////
     

  //      // Get window size and place the label upper. 
  //      CCSize size = CCDirector::sharedDirector()->getWinSize();
  //     
		//// 3. Add add a splash screen, show the cocos2d splash image.
  //      CCSprite* pSprite = CCSprite::spriteWithFile("SpriteImages/bgHome.png");
  //      CC_BREAK_IF(! pSprite);

  //      // Place the sprite on the center of the screen
  //      pSprite->setPosition(ccp(size.width/2, size.height/2));

  //      // Add the sprite to Menu layer as a child layer.
  //      this->addChild(pSprite, 0);
       
SimpleAudioEngine::sharedEngine()->playBackgroundMusic(SOUND_MENU) ;
		//this->schedule(schedule_selector(PlayerSelectorScene::soundLogic),56.0 );
        bRet = true;
    } while (0);

    return bRet;
}

void PlayerSelectorScene::Player1Selected(CCObject* pSender)
{
    // "close" menu item clicked
  SimpleAudioEngine::sharedEngine()->playBackgroundMusic("Sound/buttonclick.mp3") ;
	LevelGeneratorScene *ModeScene = LevelGeneratorScene::node();
  ModeScene->setPlayerType(1,LevelType);
//CCDirector::sharedDirector()->replaceScene(cocos2d::CCTransitionFlipY::transitionWithDuration(2.0f, ModeScene));
 CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(1.0f, ModeScene,ccWHITE));
}
void PlayerSelectorScene::Player2Selected(CCObject* pSender)
{
	SimpleAudioEngine::sharedEngine()->playBackgroundMusic("Sound/buttonclick.mp3") ;
    // "close" menu item clicked
	LevelGeneratorScene *ModeScene = LevelGeneratorScene::node();
	ModeScene->setPlayerType(2,LevelType);
	//CCDirector::sharedDirector()->replaceScene(cocos2d::CCTransitionJumpZoom::transitionWithDuration(2.0f, ModeScene));
 CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(1.0f, ModeScene,ccWHITE));

}
void PlayerSelectorScene::Player3Selected(CCObject* pSender)
{
	SimpleAudioEngine::sharedEngine()->playBackgroundMusic("Sound/buttonclick.mp3") ;
	LevelGeneratorScene *ModeScene = LevelGeneratorScene::node();
ModeScene->setPlayerType(3,LevelType);
//CCDirector::sharedDirector()->replaceScene(cocos2d::CCTransitionCrossFade::transitionWithDuration(2.0f, ModeScene));

 CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(1.0f, ModeScene,ccWHITE));
}

void PlayerSelectorScene::soundLogic(float dt)
{

	SimpleAudioEngine::sharedEngine()->playEffect("CircusMusic.mp3") ;
}