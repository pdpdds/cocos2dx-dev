#include "HelloWorldScene.h"
#include "TouchTrailLayer.h"
#include "RTLayer.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include <GLES/gl.h>
#include <GLES/glext.h>
#endif

using namespace cocos2d;

#ifndef max
#define max(a,b) (((a) > (b)) ? (a) : (b))
#endif
#ifndef min
#define min(a,b) (((a) < (b)) ? (a) : (b))
#endif

enum {
    kTagSprite1,
    kTagSprite2,
    kTagSprite3,
    kTagSprite4,
    kTagSprite5,
    kTagSprite6,
    kTagSprite7,
    kTagSprite8,
};

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
        CCMenu* pMenu = CCMenu::create(pCloseItem, NULL);
        pMenu->setPosition(CCPointZero);
        CC_BREAK_IF(! pMenu);

        // Add the menu to HelloWorld layer as a child layer.
        this->addChild(pMenu, 1);

        // 2. Add a label shows "Hello World".
        setTouchEnabled(true);
        
        CCSize winSize = CCDirector::sharedDirector()->getWinSize();

		// Background from Mosquito's Insomnia title screen :D
#if 0
		CCSprite *bg = CCSprite::create("images/bg.png");
		bg->getTexture()->setAliasTexParameters();
		bg->setAnchorPoint( ccp(0,0));
		bg->setPosition( ccp(0,0));
		bg->setOpacity( 100);
        this->addChild(bg);
#endif
        
        // Add text
        CCLabelTTF* pLabel = CCLabelTTF::create("PIXEL PERFECT COLLISION DETECTION", "Arial", 14);
        CC_BREAK_IF(! pLabel);

        // Get window size and place the label upper. 
		pLabel->setAnchorPoint( ccp(0.5f,1));
        pLabel->setPosition(ccp(winSize.width*0.5f,winSize.height*0.95f));

        // Add the label to HelloWorld layer as a child layer.
        this->addChild(pLabel, 0, kTagSprite1);

        CCLabelTTF* pLabel2 = CCLabelTTF::create("Touch to move the player ship", "Arial", 14);
        CC_BREAK_IF(! pLabel2);

        // Get window size and place the label upper. 
		pLabel2->setAnchorPoint( ccp(0.5f,1));
        pLabel2->setPosition(ccp(winSize.width*0.5f,winSize.height*0.9f));

        // Add the label to HelloWorld layer as a child layer.
        this->addChild(pLabel2, 0, kTagSprite2);
        

        //CCLabelTTF* pLabel3 = CCLabelTTF::create("Log message", "Arial", 12);
		CCLabelTTF* pLabel3 = CCLabelTTF::create("Alignment 0\nnew line", "Helvetica", 14, CCSizeMake(256, 32), kCCTextAlignmentLeft);
        CC_BREAK_IF(! pLabel3);

        // Get window size and place the label upper. 
        pLabel3->setPosition(ccp( winSize.width*0.5f, winSize.height-20));
		pLabel3->setAnchorPoint(ccp(0.5f,0.5f));
		//pLabel3->setPosition(ccp( 0, winSize.height-10));

        // Add the label to HelloWorld layer as a child layer.
        addChild(pLabel3, 0, kTagSprite3);

		CCLabelAtlas* label1 = (CCLabelAtlas*)getChildByTag(kTagSprite3);


		//TouchTrailLayer *layer = TouchTrailLayer::create();
		//this->addChild(layer);

		//RTLayer *layer = RTLayer::create();
		//this->addChild(layer);

        // create render texture and make it visible for testing purposes
        _rt = CCRenderTexture::create(winSize.width, winSize.height);
		_rt->setPosition(ccp(winSize.width*0.5f,winSize.height*0.1f));
		//_rt->setPosition(ccp(winSize.width*0.5f,winSize.height*0.5f));
        this->addChild(_rt);
		_rt->setVisible(true);

		// create a brush image to draw into the texture with
		m_pBrush = CCSprite::create("images/streak1.png");
		m_pBrush->retain();
		m_pBrush->setColor(ccRED);
		m_pBrush->setOpacity(20);
        
        // create player ship
        _ship = CCSprite::create("images/ship1.png");
		_ship->getTexture()->setAliasTexParameters();
		_ship->setRotation(45);
		_ship->setPosition(ccp(winSize.width*0.5f, winSize.height*0.4));
		//_ship->setDrawTextureBox(true);
        this->addChild(_ship, 1);
        
        // create some enemies
        _enemy1 = CCSprite::create("images/ship2.png");
		_enemy1->getTexture()->setAliasTexParameters();
		_enemy1->setRotation(0);
		_enemy1->setPosition(ccp(winSize.width*0.4f, winSize.height*0.65));
		//_enemy1->setDrawTextureBox(true);
        this->addChild(_enemy1, 1);
        
        _enemy2 = CCSprite::create("images/ship2.png");
		_enemy2->getTexture()->setAliasTexParameters();
		_enemy2->setRotation(270);
		_enemy2->setPosition(ccp(winSize.width*0.6f, winSize.height*0.65));
		//_enemy2->setDrawTextureBox(true);
        this->addChild(_enemy2, 1);

        // schedule update method
		schedule( schedule_selector(HelloWorld::tick) ); 
		//this->scheduleUpdate();

        bRet = true;
    } while (0);

    return bRet;
}

void HelloWorld::menuCloseCallback(CCObject* pSender)
{
    // "close" menu item clicked
    CCDirector::sharedDirector()->end();
}

void HelloWorld::tick(float delta)
{
    char string[128] = {0};
	CCRect r1 = _ship->boundingBox();
    sprintf(string, "ship rect=(%03.1f,%03.1f)-(%03.1f,%03.1f)", r1.getMinX(), r1.getMinY(), \
												r1.getMaxX(), r1.getMaxY());
    //string.format("%2.2f Test", m_time);

    CCLabelTTF* label1 = (CCLabelTTF*)getChildByTag(kTagSprite3);
    label1->setString(string);

	checkCollisions();
}

void HelloWorld::checkCollisions()
{
    // let's make it in a hard way :D
    
    if (this->isCollisionBetweenSpriteA(_ship, _enemy1, true) )
    {
		_enemy1->setColor(ccc3(255,0,0));
    }
    else
    {
		_enemy1->setColor(ccc3(255,255,255));
    }
    
    if ( this->isCollisionBetweenSpriteA( _ship, _enemy2, true))
    {
		_enemy2->setColor(ccc3(255,0,0));
    }
    else
    {
		_enemy2->setColor(ccc3(255,255,255));
    }
}

/*--------------------------------------------------*/

bool
HelloWorld::isCollisionBetweenSpriteA(CCSprite* spr1, CCSprite* spr2, bool pp)
{
    bool isCollision = false; 
    //CCRect intersection = CGRectIntersection([spr1 boundingBox], [spr2 boundingBox]);
	CCRect intersection;
	CCRect r1 = spr1->boundingBox();
	CCRect r2 = spr2->boundingBox();
	intersection = CCRectMake(max(r1.getMinX(),r2.getMinX()), max(r1.getMinY(),r2.getMinY()),0,0);
	intersection.size.width = min(r1.getMaxX(), r2.getMaxX()) - intersection.getMinX();
	intersection.size.height = min(r1.getMaxY(), r2.getMaxY()) - intersection.getMinY();
    
    // Look for simple bounding box collision
	//CCRect::CCRectIntersectsRect
    //if (!GRectIsEmpty(intersection))
	if ( (intersection.size.width>0) && (intersection.size.height>0) )
    {
        // If we're not checking for pixel perfect collisions, return true
        if (!pp) {return true;}
        
        // Get intersection info
        unsigned int x = intersection.origin.x;
        unsigned int y = intersection.origin.y;
        unsigned int w = intersection.size.width;
        unsigned int h = intersection.size.height;
        unsigned int numPixels = w * h;
        
        //NSLog(@"\nintersection = (%u,%u,%u,%u), area = %u",x,y,w,h,numPixels);
        
        // Draw into the RenderTexture
        _rt->beginWithClear( 0, 0, 0, 0);
        
        // Render both sprites: first one in RED and second one in GREEN
        glColorMask(1, 0, 0, 1);
        spr1->visit();
        glColorMask(0, 1, 0, 1);
        spr2->visit();
        glColorMask(1, 1, 1, 1);
        
        // Get color values of intersection area
        ccColor4B *buffer = (ccColor4B *)malloc( sizeof(ccColor4B) * numPixels );
        glReadPixels(x, y, w, h, GL_RGBA, GL_UNSIGNED_BYTE, buffer);
        
        /******* All this is for testing purposes *********/
         
        // Draw the first sprite bounding box
        //CRect r1 = [spr1 boundingBox];
        glColor4f(1, 0, 0, 1);
        glLineWidth(0.5f);
        ccDrawLine(ccp(r1.origin.x,r1.origin.y), ccp(r1.origin.x+r1.size.width,r1.origin.y));
        ccDrawLine(ccp(r1.origin.x,r1.origin.y), ccp(r1.origin.x,r1.origin.y+r1.size.height));
        ccDrawLine(ccp(r1.origin.x+r1.size.width,r1.origin.y), ccp(r1.origin.x+r1.size.width,r1.origin.y+r1.size.height));
        ccDrawLine(ccp(r1.origin.x,r1.origin.y+r1.size.height), ccp(r1.origin.x+r1.size.width,r1.origin.y+r1.size.height));
        
        // Draw the second sprite bounding box
        //CGRect r2 = [spr2 boundingBox];
        glColor4f(0, 1, 0, 1);
        glLineWidth(0.5f);
        ccDrawLine(ccp(r2.origin.x,r2.origin.y), ccp(r2.origin.x+r2.size.width,r2.origin.y));
        ccDrawLine(ccp(r2.origin.x,r2.origin.y), ccp(r2.origin.x,r2.origin.y+r2.size.height));
        ccDrawLine(ccp(r2.origin.x+r2.size.width,r2.origin.y), ccp(r2.origin.x+r2.size.width,r2.origin.y+r2.size.height));
        ccDrawLine(ccp(r2.origin.x,r2.origin.y+r2.size.height), ccp(r2.origin.x+r2.size.width,r2.origin.y+r2.size.height));
        
        // Draw the intersection rectangle in BLUE (testing purposes)
        glColor4f(0, 0, 1, 1);
        glLineWidth(0.5f);
        ccDrawLine(ccp(x,y), ccp(x+w,y));
        ccDrawLine(ccp(x,y), ccp(x,y+h));
        ccDrawLine(ccp(x+w,y), ccp(x+w,y+h));
        ccDrawLine(ccp(x,y+h), ccp(x+w,y+h));
        
        /**************************************************/
        
        _rt->end();
        
        // Read buffer
        unsigned int step = 1;
        for(unsigned int i=0; i<numPixels; i+=step)
        {
            ccColor4B color = buffer[i];
                        
            if (color.r > 0 && color.g > 0)
            {
                isCollision = true;
                break;
            }
        }
       
        // Free buffer memory
        free(buffer);
    }
    
    return isCollision;
} 

static CCDictionary s_dic;

void HelloWorld::registerWithTouchDispatcher(void)
{
    CCDirector::sharedDirector()->getTouchDispatcher()->addStandardDelegate(this, 0);
}

void HelloWorld::ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent)
{
    CCSetIterator it = pTouches->begin();
    CCTouch* touch = (CCTouch*)(*it);

    m_tBeginPos = touch->getLocationInView();    
    m_tBeginPos = CCDirector::sharedDirector()->convertToGL( m_tBeginPos );
	_ship->setPosition( m_tBeginPos); 
}

void HelloWorld::ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent)
{
    CCSetIterator it = pTouches->begin();
    CCTouch* touch = (CCTouch*)(*it);

    m_tBeginPos = touch->getLocationInView();    
    m_tBeginPos = CCDirector::sharedDirector()->convertToGL( m_tBeginPos );
    m_tEndPos = touch->getPreviousLocationInView();    
    m_tEndPos = CCDirector::sharedDirector()->convertToGL( m_tEndPos );
	_ship->setPosition( m_tBeginPos); 

#if 1
    // begin drawing to the render texture
    _rt->begin();

    // for extra points, we'll draw this smoothly from the last position and vary the sprite's
    // scale/rotation/offset
    float distance = ccpDistance(m_tBeginPos, m_tEndPos);
    if (distance > 1)
    {
        int d = (int)distance;
        for (int i = 0; i < d; i++)
        {
            float difx = m_tEndPos.x - m_tBeginPos.x;
            float dify = m_tEndPos.y - m_tBeginPos.y;
            float delta = (float)i / distance;
            m_pBrush->setPosition(ccp(m_tBeginPos.x + (difx * delta), m_tBeginPos.y + (dify * delta)));
            //m_pBrush->setRotation(rand() % 360);
            float r = (float)(rand() % 50 / 50.f) + 0.25f;
            m_pBrush->setScale(r);
            /*m_pBrush->setColor(ccc3(CCRANDOM_0_1() * 127 + 128, 255, 255));*/
            // Use CCRANDOM_0_1() will cause error when loading libtests.so on android, I don't know why.
            m_pBrush->setColor(ccc3(rand() % 127 + 128, 255, 255));
            // Call visit to draw the brush, don't call draw..
            m_pBrush->visit();
        }
    }

    // finish drawing and return context back to the screen
    _rt->end(); 
#endif
}

void HelloWorld::ccTouchesEnded(CCSet *pTouches, CCEvent *pEvent)
{
}

void HelloWorld::ccTouchesCancelled(CCSet *pTouches, CCEvent *pEvent)
{
    ccTouchesEnded(pTouches, pEvent);
} 