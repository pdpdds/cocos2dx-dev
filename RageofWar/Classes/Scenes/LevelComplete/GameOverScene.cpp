// cpp with cocos2d-x
 #include "GameOverScene.h"
 #include "Scenes/MainMenu/MainScene.h"
  #include "SimpleAudioEngine.h"
 using namespace cocos2d;
 using namespace CocosDenshion ;
 bool GameOverScene::init()
 {
     if( CCScene::init() )
    {
        this->_layer = GameOverLayer::node();
        this->_layer->retain();
        this->addChild(_layer);

        return true;
    }
    else
    {
        return false;
    }
}

GameOverScene::~GameOverScene()
{
    if (_layer)
    {
        _layer->release();
        _layer = NULL;
    }
}

bool GameOverLayer::init()
{
    if ( CCLayerColor::initWithColor( ccc4(255,255,255,255) ) )
    {
       
		
		CCSize winSize = CCDirector::sharedDirector()->getWinSize();

		 ////////////////////////////////////////
		//Background 
		/////////////////////////////////////////
		CCSprite* bg = CCSprite::create("SpriteImages/MissionInComplete.png");

		bg->setPosition(ccp(winSize.width/2, winSize.height/2));
		this->addChild(bg,0);
		SimpleAudioEngine::sharedEngine()->playEffect("Sound/defeat.mp3") ;	
	/*	CCSprite* sp = CCSprite::spriteWithFile("SpriteImages/gameover.png");

		sp->setPosition(ccp(winSize.width/2, winSize.height/2+50));
		this->addChild(sp,0);*/

   /*     this->_label = CCLabelTTF::labelWithString("","Artial", 32);
        _label->retain();
        _label->setColor( ccc3(0, 0, 0) );
        _label->setPosition(ccp(winSize.width/2, winSize.height/2+50));
        this->addChild(_label);*/

        this->runAction( CCSequence::create(
        CCDelayTime::create(5),
        CCCallFunc::create(this, 
            callfunc_selector(GameOverLayer::gameOverDone)),
            NULL));

        return true;
    }
    else
    {
        return false;
    }
}

void GameOverLayer::gameOverDone()
{
    CCDirector::sharedDirector()->replaceScene(MainScene::scene());
	  
}

GameOverLayer::~GameOverLayer()
{
    if (_label)
    {
        _label->release();
_label = NULL;
    }
}