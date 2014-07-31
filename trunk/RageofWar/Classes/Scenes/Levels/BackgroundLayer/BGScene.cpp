#include "BGScene.h"

using namespace cocos2d;

CCScene* BG::scene()
{
    CCScene * scene = NULL;
    do 
    {
        // 'scene' is an autorelease object
        scene = CCScene::create();
        CC_BREAK_IF(! scene);

        // 'layer' is an autorelease object
        BG *layer = BG::node();
        CC_BREAK_IF(! layer);

        // add layer as a child to scene
        scene->addChild(layer);
    } while (0);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool BG::init()
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
            menu_selector(BG::menuCloseCallback));
        CC_BREAK_IF(! pCloseItem);

        // Place the menu item bottom-right conner.
        pCloseItem->setPosition(ccp(CCDirector::sharedDirector()->getWinSize().width - 20, 20));

        // Create a menu with the "close" menu item, it's an auto release object.
        CCMenu* pMenu = CCMenu::createWithItem(pCloseItem);
        pMenu->setPosition(CCPointZero);
        CC_BREAK_IF(! pMenu);

        // Add the menu to BG layer as a child layer.
        this->addChild(pMenu, 1);

        //// 2. Add a label shows "Hello World".

        //// Create a label and initialize with string "Hello World".
        //CCLabelTTF* pLabel = CCLabelTTF::labelWithString("Hello World", "Thonburi", 64);
        //CC_BREAK_IF(! pLabel);

        //// Get window size and place the label upper. 
        CCSize size = CCDirector::sharedDirector()->getWinSize();
        //pLabel->setPosition(ccp(size.width / 2, size.height - 20));

        //// Add the label to BG layer as a child layer.
        //this->addChild(pLabel, 1);
		
		/////////////////////////////////////////////////////////////////////////////////////
		// 3. The Background Layer
		/////////////////////////////////////////////////////////////////////////////////////
        
		CCSprite* pSprite = CCSprite::create("Mountain.jpeg");
        CC_BREAK_IF(! pSprite);

        // Place the sprite on the center of the screen
		pSprite->setAnchorPoint(ccp(0,0));
		pSprite->setPosition(ccp(0, size.height/2));

        // Add the sprite to BG layer as a child layer.
        this->addChild(pSprite, 0);
		

		 CCSprite* pSprite2 = CCSprite::create("Mountain2.jpeg");
        CC_BREAK_IF(! pSprite2);
		//float x=size.width/2;
		pSprite2->setAnchorPoint(ccp(0,0));
        // Place the sprite on the center of the screen
        pSprite2->setPosition(ccp(size.width/2-1, size.height/2));

        // Add the sprite to BG layer as a child layer.
        this->addChild(pSprite2, 0);

		/////////////

		 CCSprite* pSprite3 = CCSprite::create("Mountain.jpeg");
        CC_BREAK_IF(! pSprite3);
		//x=x+size.width;
		pSprite3->setAnchorPoint(ccp(0,0));
        // Place the sprite on the center of the screen
        pSprite3->setPosition(ccp(size.width-2, size.height/2));

        // Add the sprite to BG layer as a child layer.
        this->addChild(pSprite3, 0);

		 CCSprite* pSprite4 = CCSprite::create("Mountain2.jpeg");
        CC_BREAK_IF(! pSprite4);
		//x=x+size.width;
		pSprite4->setAnchorPoint(ccp(0,0));
        // Place the sprite on the center of the screen
        pSprite4->setPosition(ccp((size.width/2)*3-3, size.height/2));

        // Add the sprite to BG layer as a child layer.
        this->addChild(pSprite4, 0);

		 CCSprite* pSprite42 = CCSprite::create("Mountain.jpeg");
        CC_BREAK_IF(! pSprite42);
		//x=x+size.width;
		pSprite42->setAnchorPoint(ccp(0,0));
        // Place the sprite on the center of the screen
        pSprite42->setPosition(ccp((size.width*2)-4, size.height/2));

        // Add the sprite to BG layer as a child layer.
        this->addChild(pSprite42, 0);
		 ///////////////////////////////////////////////////////////////////
		//CCSprite* pSprite21 = CCSprite::spriteWithFile("Balloon.png");
  //      CC_BREAK_IF(! pSprite21);

  //      // Place the sprite on the center of the screen
  //      pSprite21->setPosition(ccp(size.width/2, size.height/2));

  //      // Add the sprite to BG layer as a child layer.
  //      this->addChild(pSprite21, 0);
		/////////////////////////////////////////////////////////////////////////////////////
		// 3. The Ground Layer
		/////////////////////////////////////////////////////////////////////////////////////
		CCSprite* pSpritebg = CCSprite::create("bg.png");
        CC_BREAK_IF(! pSpritebg);

        // Place the sprite on the center of the screen
		pSpritebg->setAnchorPoint(ccp(0,0));
		pSpritebg->setPosition(ccp(0, -40));

        // Add the sprite to BG layer as a child layer.
        this->addChild(pSpritebg, 0);
		

		 CCSprite* pSprite2bg = CCSprite::create("bg2.png");
        CC_BREAK_IF(! pSprite2bg);
		//float x=size.width/2;
		pSprite2bg->setAnchorPoint(ccp(0,0));
        // Place the sprite on the center of the screen
        pSprite2bg->setPosition(ccp((size.width/2)-1, -40));

        // Add the sprite to BG layer as a child layer.
        this->addChild(pSprite2bg, 0);

		///////////////

		 CCSprite* pSprite3bg = CCSprite::create("bg.png");
        CC_BREAK_IF(! pSprite3bg);
		//x=x+size.width;
		pSprite3bg->setAnchorPoint(ccp(0,0));
        // Place the sprite on the center of the screen
        pSprite3bg->setPosition(ccp(size.width-2, -40));

        // Add the sprite to BG layer as a child layer.
        this->addChild(pSprite3bg, 0);

		 CCSprite* pSprite4bg = CCSprite::create("bg2.png");
        CC_BREAK_IF(! pSprite4bg);
		//x=x+size.width;
		pSprite4bg->setAnchorPoint(ccp(0,0));
        // Place the sprite on the center of the screen
        pSprite4bg->setPosition(ccp((size.width/2)*3-3, -40));

        // Add the sprite to BG layer as a child layer.
        this->addChild(pSprite4bg, 0);


		 CCSprite* pSprite3bg2 = CCSprite::create("bg.png");
        CC_BREAK_IF(! pSprite3bg2);
		//x=x+size.width;
		pSprite3bg2->setAnchorPoint(ccp(0,0));
        // Place the sprite on the center of the screen
        pSprite3bg2->setPosition(ccp(size.width*2-4, -40));

        // Add the sprite to BG layer as a child layer.
        this->addChild(pSprite3bg2, 0);

		/////////////////////////////////////////////////////////////////////////////////////
		// 3. The Tent Layer
		/////////////////////////////////////////////////////////////////////////////////////
		
		CCSprite* pSpritetent = CCSprite::create("bunker.png");
        CC_BREAK_IF(! pSpritetent);

        // Place the sprite on the center of the screen
		pSpritetent->setAnchorPoint(ccp(0,0));
		pSpritetent->setPosition(ccp(-20, size.height/3));

        // Add the sprite to BG layer as a child layer.
        this->addChild(pSpritetent, 1);
		

		CCSprite* pSprite2tent = CCSprite::create("bluetent4.png");
        CC_BREAK_IF(! pSprite2tent);
		//float x=size.width/2;
		pSprite2tent->setAnchorPoint(ccp(0,0));
        // Place the sprite on the center of the screen
        pSprite2tent->setPosition(ccp((size.width/2)-60, 60));

        // Add the sprite to BG layer as a child layer.
        this->addChild(pSprite2tent, 1);

		///////////////

		CCSprite* pSprite3tent = CCSprite::create("bluetent2tone.png");
        CC_BREAK_IF(! pSprite3tent);
		//x=x+size.width;
		pSprite3tent->setAnchorPoint(ccp(0,0));
        // Place the sprite on the center of the screen
        pSprite3tent->setPosition(ccp(size.width, 60));

        // Add the sprite to BG layer as a child layer.
        this->addChild(pSprite3tent, 1);

		 CCSprite* pSprite4tent = CCSprite::create("greytent2.png");
        CC_BREAK_IF(! pSprite4tent);
		//x=x+size.width;
		pSprite4tent->setAnchorPoint(ccp(0,0));
        // Place the sprite on the center of the screen
        pSprite4tent->setPosition(ccp((size.width/2)*3, 60));
		
		////////////////////////////////////////////////////////////////////////////////////
		//Jeep layer

		////////////////////////////////////////////////////////////////////////////////////
		CCSprite* jeep = CCSprite::create("jeep2.png");
        CC_BREAK_IF(! jeep);
		//float x=size.width/2;
		jeep->setAnchorPoint(ccp(0,0));
		
        // Place the sprite on the center of the screen
        jeep->setPosition(ccp(size.width*2-190, size.height/3));

        // Add the sprite to BG layer as a child layer.
        this->addChild(jeep, 0);

		CCSprite* bunker = CCSprite::create("bunker.png");
        CC_BREAK_IF(! bunker);

        // Place the sprite on the center of the screen
		bunker->setAnchorPoint(ccp(0,0));
		bunker->setPosition(ccp((size.width/2)*3, size.height/3));

        // Add the sprite to BG layer as a child layer.
        this->addChild(bunker, 1);
		
		////////////////////////////////////////////////////////////////////////////////////
		//Fence layer

		////////////////////////////////////////////////////////////////////////////////////
		CCSprite* fence  = CCSprite::create("fence.png");
        CC_BREAK_IF(! fence);
		//float x=size.width/2;
		fence->setAnchorPoint(ccp(0,0));
		
        // Place the sprite on the center of the screen
        fence->setPosition(ccp(size.width/4, size.height/3));

        // Add the sprite to BG layer as a child layer.
        this->addChild(fence, 3);

		CCSprite* fence2  = CCSprite::create("fence.png");
        CC_BREAK_IF(! fence2);
		//float x=size.width/2;
		fence2->setAnchorPoint(ccp(0,0));
		
        // Place the sprite on the center of the screen
        fence2->setPosition(ccp(size.width/4+100, size.height/3+10));

        // Add the sprite to BG layer as a child layer.
        this->addChild(fence2, 3);

		CCSprite* fence3  = CCSprite::create("fence.png");
        CC_BREAK_IF(! fence3);
		//float x=size.width/2;
		fence3->setAnchorPoint(ccp(0,0));
		
        // Place the sprite on the center of the screen
        fence3->setPosition(ccp((size.width/2)+200, size.height/3-20));

        // Add the sprite to BG layer as a child layer.
        this->addChild(fence3, 3);
		///////////////////////////////////////////////
		//Fire 2
		////////////////////////////////////////////////
		CCSprite* fire2 = CCSprite::create("fire1.png");
        CC_BREAK_IF(!fire2);
		//x=x+size.width;
		fire2->setAnchorPoint(ccp(0,0));
        // Place the sprite on the center of the screen
        fire2->setPosition(ccp((size.width/2)+300,size.height/3-60));

        // Add the sprite to BG layer as a child layer.
        this->addChild(fire2, 9);
		
		CCParticleSystem*	m_emitter2;
		
		m_emitter2 = CCParticleSmoke::create();
		m_emitter2->retain();
		this->addChild(m_emitter2, 10);
		m_emitter2->setStartSize(20.0f);
		//m_emitter->setTotalParticles(300);
		m_emitter2->setSpeed(30.0f);
		m_emitter2->setTexture( CCTextureCache::sharedTextureCache()->addImage("fire.png") );//.pvr"];
		
		m_emitter2->setPosition( CCPointMake((size.width/2)+350,size.height/3));

		////////////////////////////////////////////////////////////////////////////////////
		//Tree layer (Fire and Smoke)

		////////////////////////////////////////////////////////////////////////////////////
		CCSprite* fire = CCSprite::create("fire1.png");
        CC_BREAK_IF(!fire);
		//x=x+size.width;
		fire->setAnchorPoint(ccp(0,0));
        // Place the sprite on the center of the screen
        fire->setPosition(ccp((size.width/4)-50,size.height/3-60));

        // Add the sprite to BG layer as a child layer.
        this->addChild(fire, 9);
		
		CCParticleSystem*	m_emitter;
		CCSprite*			m_background;
		m_emitter = CCParticleSmoke::create();
		m_emitter->retain();
		this->addChild(m_emitter, 10);
		m_emitter->setStartSize(20.0f);
		//m_emitter->setTotalParticles(300);
		m_emitter->setSpeed(30.0f);
		m_emitter->setTexture( CCTextureCache::sharedTextureCache()->addImage("fire.png") );//.pvr"];
		
		m_emitter->setPosition( CCPointMake(size.width/4,size.height/3));
		/////////////////

		CCSprite* pSprite0tree = CCSprite::create("tree1.png");
        CC_BREAK_IF(! pSprite0tree);
		//x=x+size.width;
		pSprite0tree->setAnchorPoint(ccp(0,0));
        // Place the sprite on the center of the screen
        pSprite0tree->setPosition(ccp(40, size.height/3-10));

        // Add the sprite to BG layer as a child layer.
        this->addChild(pSprite0tree, 3);
		
		CCSprite* pSpritetree = CCSprite::create("bushytree.png");
        CC_BREAK_IF(! pSpritetree);

        // Place the sprite on the center of the screen
		pSpritetree->setAnchorPoint(ccp(0,0));
		pSpritetree->setPosition(ccp(size.width/4, size.height/3));

        // Add the sprite to BG layer as a child layer.
        this->addChild(pSpritetree, 2);
		

		CCSprite* pSprite2tree = CCSprite::create("tree1.png");
        CC_BREAK_IF(! pSprite2tree);
		//float x=size.width/2;
		pSprite2tree->setAnchorPoint(ccp(0,0));
		
        // Place the sprite on the center of the screen
        pSprite2tree->setPosition(ccp((size.width/2)+100, size.height/2-40));

        // Add the sprite to BG layer as a child layer.
        this->addChild(pSprite2tree, 0);

		/////////////////

		CCSprite* pSprite3tree = CCSprite::create("treetilt.png");
        CC_BREAK_IF(! pSprite3tree);
		//x=x+size.width;
		pSprite3tree->setAnchorPoint(ccp(0,0));
        // Place the sprite on the center of the screen
        pSprite3tree->setPosition(ccp(size.width+50, size.height/3));

        // Add the sprite to BG layer as a child layer.
        this->addChild(pSprite3tree, 0);


		CCSprite* pSprite31tree = CCSprite::create("treetilt.png");
        CC_BREAK_IF(! pSprite31tree);
		//x=x+size.width;
		pSprite31tree->setAnchorPoint(ccp(0,0));
        // Place the sprite on the center of the screen
        pSprite31tree->setPosition(ccp((size.width/2)*3-70, size.height/3));

        // Add the sprite to BG layer as a child layer.
        this->addChild(pSprite31tree, 0);

		 CCSprite* pSprite4tree = CCSprite::create("tree1.png");
        CC_BREAK_IF(! pSprite4tree);
		//x=x+size.width;
		pSprite4tree->setAnchorPoint(ccp(0,0));
        // Place the sprite on the center of the screen
        pSprite4tree->setPosition(ccp((size.width/2)*3-40, size.height/3));
        // Add the sprite to BG layer as a child layer.
        this->addChild(pSprite4tree, 1);


		CCSprite* pSprite41tree = CCSprite::create("treetilt.png");
        CC_BREAK_IF(! pSprite41tree);
		//x=x+size.width;
		pSprite41tree->setAnchorPoint(ccp(0,0));
        // Place the sprite on the center of the screen
        pSprite41tree->setPosition(ccp(size.width*2-70, size.height/3));

        // Add the sprite to BG layer as a child layer.
        this->addChild(pSprite41tree, 1);

		 CCSprite* pSprite5tree = CCSprite::create("tree1.png");
        CC_BREAK_IF(! pSprite5tree);
		//x=x+size.width;
		pSprite5tree->setAnchorPoint(ccp(0,0));
        // Place the sprite on the center of the screen
        pSprite5tree->setPosition(ccp(size.width*2-40, size.height/3));
        // Add the sprite to BG layer as a child layer.
        this->addChild(pSprite5tree, 0);
		this->setTouchEnabled(true);
	
        bRet = true;
    } while (0);

    return bRet;
}
void BG::ccTouchesEnded(CCSet* touches, CCEvent* event)
{/*
		runAction(CCSequence::actions(CCMoveTo::actionWithDuration(6.5f, CCPointMake(-480.0f,0.0f)) , 
			CCMoveTo::actionWithDuration(6.5f, CCPointMake(-960.0f,0.0f)) ,
			CCMoveTo::actionWithDuration(6.5f, CCPointMake(-1440.0f,0.0f)) ,
								 
								  NULL));*/
		
}
//void BG::setViewpointCenter(CCPoint position)
//{
//	CCSize winSize = CCDirector::sharedDirector()->getWinSize();
//	
//	int x = MAX(position.x, winSize.width / 2);
//	int y = MAX(position.y, winSize.height / 2);
//	
//	x = MIN(x, (_tileMap->getMapSize().width * _tileMap->getTileSize().width) - winSize.width/2);
//	y = MIN(y, (_tileMap->getMapSize().height * _tileMap->getTileSize().height) - winSize.height/2);
//	
//	CCPoint actualPosition = ccp(x, y);
//	
//	CCPoint centerOfView = ccp(winSize.width/2, winSize.height/2);
//	
//	CCPoint viewPoint = ccpSub(centerOfView, actualPosition);
//	
//	this->setPosition(viewPoint);
//}

void BG::menuCloseCallback(CCObject* pSender)
{
    // "close" menu item clicked
    CCDirector::sharedDirector()->end();
}

