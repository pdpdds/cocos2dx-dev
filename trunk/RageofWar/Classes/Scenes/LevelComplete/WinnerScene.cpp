// cpp with cocos2d-x
 #include "WinnerScene.h" ;
 #include "Scenes/MainMenu/MainScene.h" ;
  #include "SimpleAudioEngine.h"
 using namespace cocos2d;
 using namespace CocosDenshion ;
 bool WinnerScene::init()
 {
     if( CCScene::init() )
    {
        this->_layer = WinnerLayer::node();
        this->_layer->retain();
        this->addChild(_layer);

        return true;
    }
    else
    {
        return false;
    }
}

WinnerScene::~WinnerScene()
{
    if (_layer)
    {
        _layer->release();
        _layer = NULL;
    }
}

bool WinnerLayer::init()
{
    if ( CCLayerColor::initWithColor( ccc4(255,255,255,255) ) )
    {
       
		
		CCSize winSize = CCDirector::sharedDirector()->getWinSize();

		 ////////////////////////////////////////
		//Background 
		/////////////////////////////////////////
		CCSprite* bg = CCSprite::create("SpriteImages/MissionComplete.png");

		bg->setPosition(ccp(winSize.width/2, winSize.height/2));
		this->addChild(bg,0);
			SimpleAudioEngine::sharedEngine()->playEffect("Sound/won.mp3") ;	


	/*	CCSprite* sp = CCSprite::spriteWithFile("SpriteImages/youwin.png");

		sp->setPosition(ccp(winSize.width/2, winSize.height/2+50));
		this->addChild(sp,0);*/

     /*   this->_label = CCLabelTTF::labelWithString("","Artial", 32);
        _label->retain();
        _label->setColor( ccc3(0, 0, 0) );
        _label->setPosition(ccp(winSize.width/2, winSize.height/2+50));
        this->addChild(_label);*/
		ExplosionAnimation(ccp(winSize.width/2-200, winSize.height/2+50),"SpriteImages/flag1.png" );
		ExplosionAnimation(ccp(winSize.width/2-100, winSize.height/2+50),"SpriteImages/flag2.png" );
		ExplosionAnimation(ccp(winSize.width/2+100, winSize.height/2+50),"SpriteImages/flag3.png" );
		ExplosionAnimation(ccp(winSize.width/2+200, winSize.height/2+50),"SpriteImages/flag4.png" );

        this->runAction( CCSequence::create(
        CCDelayTime::create(5),
        CCCallFunc::create(this, 
            callfunc_selector(WinnerLayer::WinnerDone)),
            NULL));

        return true;
    }
    else
    {
        return false;
    }
}

void WinnerLayer::WinnerDone()
{
    CCDirector::sharedDirector()->replaceScene(MainScene::scene());
	 // SimpleAudioEngine::sharedEngine()->stopAllEffects();
}

WinnerLayer::~WinnerLayer()
{
    if (_label)
    {
        _label->release();
_label = NULL;
    }
}

void WinnerLayer::ExplosionAnimation(cocos2d::CCPoint position,const char* imgName )
{

	CCParticleExplosion * emitter = new CCParticleExplosion();
	emitter->setTexture(CCTextureCache::sharedTextureCache()->addImage(imgName));
	emitter->initWithTotalParticles(20);      
	emitter->setStartSize(20.0f);
	emitter->setSpeed(50.0f);
	emitter->setAnchorPoint(ccp(0.0f,0.0f));
	emitter->setPosition(position);
	emitter->setDuration(0.5f);
	ccColor4F startColor = {219.5f, 147.5f, 29.5f, 1.0f};
	emitter->setStartColor(startColor);
	
	ccColor4F startColorVar = {219.5f, 147.5f, 29.5f, 1.0f};
	emitter->setStartColorVar(startColorVar);
	
	ccColor4F endColor = {167.1f, 91.1f, 39.1f, 1.0f};
	emitter->setEndColor(endColor);
	
	ccColor4F endColorVar = {167.1f, 91.1f, 39.1f, 1.0f};    
	emitter->setEndColorVar(endColorVar);
	//emitter->setIsBlendAdditive(true);
	emitter->setAutoRemoveOnFinish(true);
	emitter->retain();
	addChild(emitter, 1);
	emitter->release();

}