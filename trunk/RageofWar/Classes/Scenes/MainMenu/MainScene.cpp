

#include "MainScene.h"
#include "Scenes/Levels/LevelGenerator.h" 
#include "Scenes/Intro/IntroScene.h" 
#include "Scenes/LevelSelector/LevelSelector.h" 

  #include "SimpleAudioEngine.h"
 using namespace cocos2d;
 using namespace CocosDenshion ;


CCScene* MainScene::scene()
{
    CCScene * scene = NULL;
    do 
    {
        // 'scene' is an autorelease object
		scene = CCScene::create();
        CC_BREAK_IF(! scene);

        // 'layer' is an autorelease object
        MainScene *layer = MainScene::node();
        CC_BREAK_IF(! layer);

        // add layer as a child to scene
        scene->addChild(layer);
    } while (0);

    // return the scene
    return scene;
}
void MainScene::MenuDefenceCallback(CCObject* pSender)
{
  /*  Level1 *ModeScene = Level1::node();
	 CCScene *pScene =ModeScene->scene();
	CCDirector::sharedDirector()->replaceScene(CCTransitionFade::transitionWithDuration(2.0f, pScene, ccWHITE));*/
}

void MainScene::MenuAttackCallback(CCObject* pSender)
{
	SimpleAudioEngine::sharedEngine()->playBackgroundMusic("Sound/buttonclick.mp3") ;
    //Level1Scene *ModeScene = Level1Scene::node();
	//CCDirector::sharedDirector()->replaceScene(ModeScene);
	//sceneToRun = IntroScene::node();
	//CCScene *pScene = IntroScene::node();	
	//CCDirector::sharedDirector()->replaceScene(pScene);
	 CCScene *pScene =LevelSelectorScene::scene();
	CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(2.0f, pScene,ccWHITE));
	//CCDirector::sharedDirector()->replaceScene(LevelSelectorScene::scene());
}

// on "init" you need to initialize your instance
bool MainScene::init()
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
            "SpriteImages/StartGame.png",
            "SpriteImages/StartGame2.png",
            this,
            menu_selector(MainScene::MenuAttackCallback));
        CC_BREAK_IF(! pLevel1);
		 // Place the attack menu item in center.
        pLevel1->setPosition(ccp(size.width/2,size.height/2));

        // Create a menu with the "close" menu item, it's an auto release object.
		CCMenu* pMenuLevel1 = CCMenu::createWithItem(pLevel1);
        pMenuLevel1->setPosition(CCPointZero);
        CC_BREAK_IF(! pMenuLevel1);

        // Add the menu to Menu layer as a child layer.
        this->addChild(pMenuLevel1, 1);
		
		  //////////////////////////////////////////////////////////////////////////
        // MenuDefence
        //////////////////////////////////////////////////////////////////////////

      
		
		//////////////////////////////////////////////////////////////////////////
        // Menu
        //////////////////////////////////////////////////////////////////////////
     

        // Get window size and place the label upper. 
        
       

        // 3. Add add a splash screen, show the cocos2d splash image.
        CCSprite* pSprite = CCSprite::create("SpriteImages/bgMainMenu.png");
        CC_BREAK_IF(! pSprite);

        // Place the sprite on the center of the screen
        pSprite->setPosition(ccp(size.width/2, size.height/2));

        // Add the sprite to Menu layer as a child layer.
        this->addChild(pSprite, 0);
		SimpleAudioEngine::sharedEngine()->playBackgroundMusic(SOUND_MENU) ;
		//this->schedule(schedule_selector(MainScene::soundLogic),56.0 );
        bRet = true;
    } while (0);

    return bRet;
}

void MainScene::MenuCloseCallback(CCObject* pSender)
{
    // "close" menu item clicked
    CCDirector::sharedDirector()->end();
}

void MainScene::soundLogic(float dt)
{

	SimpleAudioEngine::sharedEngine()->playEffect("CircusMusic.mp3") ;
}