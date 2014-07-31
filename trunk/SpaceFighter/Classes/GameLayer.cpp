//
//  GameLayer.cpp
//  Asteroids
//
//  Created by Clawoo on 9/16/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#include "GameLayer.h"
#include "GameConfig.h"
#include "Asteroid.h"
#include "Bullet.h"
#include "SSGravityBomb.h"
#include "SSLaser.h"
#include "TitleLayer.h"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include <GLES/gl.h>
#include <GLES/glext.h>
#endif

USING_NS_CC;

GameLayer::~GameLayer()
{
    CC_SAFE_RELEASE_NULL(asteroids_);
    CC_SAFE_RELEASE_NULL(bullets_);
	CC_SAFE_RELEASE_NULL(GravityBomb_);
}

CCScene* GameLayer::scene()
{
	// 'scene' is an autorelease object
	CCScene *scene = CCScene::create();
	
	// 'layer' is an autorelease object
	GameLayer *layer = GameLayer::node();
    
	// add layer as a child to scene
	scene->addChild(layer);
    
	// return the scene
	return scene;
}

bool GameLayer::init()
{
	if ( !CCLayer::init() )
	{
		return false;
	}
    
    setTouchEnabled(true);
    
    // Get window size
    CCSize windowSize = CCDirector::sharedDirector()->getWinSize();

	//Background
	parallax = CCParallaxScrollNode::node();
    
	CCSprite *Universe1 = CCSprite::create("land_green.png");
	CCSprite *Universe2 = CCSprite::create("land_green.png");

	parallax->addInfiniteScrollXWithZ(0, ccp(1,1), ccp(0,0), Universe1,  Universe2, NULL);
	this->addChild(parallax);
 
    // Create ship object, position it, then add to layer
    ship_ = (Ship *)Ship::spriteWithFile("ship.png");
    ship_->setPosition(ccp(windowSize.width / 2, windowSize.height / 2));
    this->addChild(ship_);
    
    asteroids_ = new CCArray();
    bullets_ = new CCArray();
	GravityBomb_ = new CCArray();

    currentLevel_ = 0;

	 _rt = CCRenderTexture::create(windowSize.width, windowSize.height);
	_rt->setPosition(ccp(windowSize.width*0.5f,windowSize.height*0.1f));
	this->addChild(_rt);
	_rt->setVisible(false);
    
    this->startLevel();
    
    this->scheduleUpdate();

////////////////////////////////////////////////////////////////////////////////////////////
	
	
	SSGravityBomb *GravityBomb =  SSGravityBomb::spriteWithFile("exp0.png");
	GravityBomb->Setup(this);
	this->addChild(GravityBomb);        
	GravityBomb_->addObject(GravityBomb);

	//CCMoveBy *MoveBy = CCMoveBy::actionWithDuration(5, ccp(200, 0));  
	//CCRepeatForever *repeat1 = ::CCRepeatForever::actionWithAction((CCSequence *)CCSequence::actions(MoveBy, MoveBy->reverse(), NULL));  

	//farmer->runAction(repeat1);
////////////////////////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////
//Laser
	SSLaser* pLaser = SSLaser::spriteWithFile("laserbeam_blue.png");
	pLaser->Setup(this);
	this->addChild(pLaser);        
    
	return true;
}

void GameLayer::update(float dt)
{
	parallax->updateWithVelocity(ccp(-.5,0), dt);

    if (asteroids_->count() == 0)
    {
        currentLevel_++;
        this->startLevel();
    }
    
    // Array that keeps asteroids that need to be removed
    CCArray *asteroidsToDelete = new CCArray();
    asteroidsToDelete->autorelease();
    
    // Array that keeps asteroids that need to be split in half
    CCArray *asteroidsToSplit = new CCArray();
    asteroidsToSplit->autorelease();
////////////////////////////////////////////////////////////////////////////////////////
	// Check for collisions vs. asteroids
    CCObject* pGravityBomb = NULL;
	 CCArray *GravityBombToDelete = new CCArray();
	 GravityBombToDelete->autorelease();
	CCARRAY_FOREACH(GravityBomb_, pGravityBomb)
    {
        SSGravityBomb *a = (SSGravityBomb *)pGravityBomb;
        
		if(a->getState() == STATE_DONE)
		{
			GravityBombToDelete->addObject(a);
			continue;
		}
       
        // Check if asteroid hits ship
        //if (a->collidesWith(ship_))
        {
			if(isCollisionBetweenSpriteA(a, ship_,true))
			{
				// Game over, man!
				this->gameOver();
				return;
			}
        }
	}
////////////////////////////////////////////////////////////////////////////////////////
    
    // Array that keeps expired or otherwise exploded bullets that need to be removed
    CCArray *bulletsToDelete = new CCArray();
    bulletsToDelete->autorelease();
   
	int i=0;

	// Check for collisions vs. asteroids
    CCObject* pObject = NULL;
	CCARRAY_FOREACH(asteroids_, pObject)
    {
        Asteroid *a = (Asteroid *)pObject;
       // CCLog("asteroid #%d", i++);
        
        // Check if asteroid hits ship
        if (a->collidesWith(ship_))
        {
			if(isCollisionBetweenSpriteA(a, ship_,true))
			{
				// Game over, man!
				this->gameOver();
				return;
			}
            
        }
       
		  // Check if asteroid hits bullet, or if bullet is expired
		CCObject* pObject2 = NULL;
		CCARRAY_FOREACH(bullets_, pObject2)
		{
            Bullet *b = (Bullet *)pObject2;
            
            if (b->getExpired())
            {
                // Remove the bullet from organizational array
                bulletsToDelete->addObject(b);
                
                // Remove bullet sprite from layer
                this->removeChild(b, false);
            }
            else if (a->collidesWith(b))
            {
                // Remove the asteroid the bullet collided with
                asteroidsToDelete->addObject(a);
                
                // Remove asteroid sprite from layer
                this->removeChild(a, false);
                
                // Remove the bullet the asteroid collided with
                bulletsToDelete->addObject(b);
                
                // Remove bullet sprite from layer
                this->removeChild(b, false);

                if (a->getSize() < kAsteroidSmall)
                {
                    asteroidsToSplit->addObject(a);
                }
            }
        }
    }
    
	// split the larger asteroids that were hit by bullets
	CCObject* pObject3 = NULL;
	CCARRAY_FOREACH(asteroidsToSplit, pObject3)
	{
		Asteroid *a = (Asteroid *)pObject3;
        for (int i = 0; i < 2; i++)
        {
            this->createAsteroidAt(a->getPosition(), a->getSize()+1);
        }
    }
    asteroids_->removeObjectsInArray(asteroidsToDelete);
    bullets_->removeObjectsInArray(bulletsToDelete);
	GravityBomb_->removeObjectsInArray(GravityBombToDelete);
}

CCArray* GameLayer::allTouchesFromSet(CCSet *touches)
{
    CCArray *arr = new CCArray();
    
    CCSetIterator it;
    
	for( it = touches->begin(); it != touches->end(); it++) 
    {
        arr->addObject((CCTouch *)*it);
    }
    return arr;
}

/*void GameLayer::ccTouchesBegan(cocos2d::CCSet* touches, cocos2d::CCEvent* event)
{
    // This method is passed an NSSet of touches called (of course) "touches"
    // We need to convert it to an array first
    CCArray *allTouches = this->allTouchesFromSet(touches);
	CCTouch* fingerOne = (CCTouch *)allTouches->objectAtIndex(0);
    
    // Convert each UITouch object to a CGPoint, which has x/y coordinates we can actually use
    CCPoint pointOne = fingerOne->locationInView();
    
    // The touch points are always in "portrait" coordinates - convert to landscape
    pointOne = CCDirector::sharedDirector()->convertToGL(pointOne);
    
    // We store the starting point of the touch so we can determine whether the touch is a swipe or tap.
    // A tap shouldn't move, so we compare the distance of the starting/ending touches, and if the distance is
    // small enough (we account for a bit of movement, just in case), the input is considered a tap
    startTouchPoint_ = pointOne;
    if (allTouches->count() > 1)
    {
        CCTouch *fingerTwo = (CCTouch *)allTouches->objectAtIndex(1);
        
        // Convert each UITouch object to a CGPoint, which has x/y coordinates we can actually use
        CCPoint pointTwo = fingerTwo->locationInView();
        
        // The touch points are always in "portrait" coordinates - convert to landscape
        pointTwo = CCDirector::sharedDirector()->convertToGL(pointTwo);
        
        // Initialize the variables used to store the angle of rotation derived from the user's fingers
        currentTouchAngle_ = previousTouchAngle_ = CC_RADIANS_TO_DEGREES(atan2(pointOne.x - pointTwo.x, pointOne.y - pointTwo.y));
    }
}

void GameLayer::ccTouchesMoved(CCSet* touches, CCEvent* event)
{
    // This method is passed an NSSet of touches called (of course) "touches"
    // We need to convert it to an array first
    CCArray *allTouches = this->allTouchesFromSet(touches);

    // Only run the following code if there is more than one touch
    if (allTouches->count() > 1)
    {
        // We're going to track the first two touches (i.e. first two fingers)
        // Create "UITouch" objects representing each touch
        CCTouch *fingerOne = (CCTouch *)allTouches->objectAtIndex(0);
		CCTouch *fingerTwo = (CCTouch *)allTouches->objectAtIndex(1);
        
        // Convert each UITouch object to a CGPoint, which has x/y coordinates we can actually use
        CCPoint pointOne = fingerOne->locationInView();
        CCPoint pointTwo = fingerTwo->locationInView();
        
        // The touch points are always in "portrait" coordinates - you will need to convert them if in landscape (which we are)
        pointOne = CCDirector::sharedDirector()->convertToGL(pointOne);
        pointTwo = CCDirector::sharedDirector()->convertToGL(pointTwo);
        
        // Get the angle that's created by the user's two fingers
        currentTouchAngle_ = CC_RADIANS_TO_DEGREES(atan2(pointOne.x - pointTwo.x, pointOne.y - pointTwo.y));
        
        // Compare with the previous angle, to decide whether the change is positive or negative.
        float difference = currentTouchAngle_ - previousTouchAngle_;
        
        // The ship is then rotated by that difference
        ship_->setRotation(ship_->getRotation() + difference);
        
        // Store the current angle variable to be used again on the next loop iteration
        previousTouchAngle_ = currentTouchAngle_;
    }
}

void GameLayer::ccTouchesEnded(CCSet* touches, CCEvent* event)
{
    // This method is passed an NSSet of touches called (of course) "touches"
    // We need to convert it to an array first
    CCArray *allTouches = this->allTouchesFromSet(touches);
    
    if (allTouches->count() == 1)
    {
		CCTouch* fingerOne = (CCTouch *)allTouches->objectAtIndex(0);
        
        // Convert each UITouch object to a CGPoint, which has x/y coordinates we can actually use
        CCPoint pointOne = fingerOne->locationInView();
        
        // The touch points are always in "portrait" coordinates - convert to landscape
        pointOne = CCDirector::sharedDirector()->convertToGL(pointOne);
        
        // Set the variable that stores the ending touch point
        endTouchPoint_ = pointOne;

        // Get the distance that the user's finger moved during this touch
        float distance = sqrt(pow(endTouchPoint_.x - startTouchPoint_.x, 2) + pow(endTouchPoint_.y - startTouchPoint_.y, 2));
        
        // If the distance moved (in pixels) is small enough, consider the gesture a tap
        if (distance < 5)
        {
            this->createBullet();
        }
        // Otherwise, it's a swipe
        else
        {
            // Use distance of swipe as a multiplier for the ship velocity (longer swipe, go faster)
            ship_->setVelocity(ccp(cos(CC_DEGREES_TO_RADIANS(ship_->getRotation())) * distance / 100, 
                                   -sin(CC_DEGREES_TO_RADIANS(ship_->getRotation())) * distance / 100));
        }
    }
}*/

void GameLayer::createAsteroidAt(cocos2d::CCPoint position , int size)
{
    const char *imageFile;
    
    switch (size) {
        case kAsteroidLarge:
            imageFile = "asteroid-large.png";
            break;
        case kAsteroidMedium:
            imageFile = "asteroid-medium.png";
            break;
        case kAsteroidSmall:
            imageFile = "asteroid-small.png";
            break;
            
        default:
            break;
    }
    
    Asteroid *a = Asteroid::spriteWithFile(imageFile);
    a->setSize(size);
    a->setPosition(position);
    

	if(position.x == 0)
		a->setVelocity(ccp(1 - (float)(rand() % 100) / 100, (float)(rand() % 100) / 100 - 1));
	else if(position.y == 0)
		a->setVelocity(ccp(1 - (float)(rand() % 100) / 100, 1 - (float)(rand() % 100) / 100 ));
	else
    // Random numbers 
    a->setVelocity(ccp((float)(rand() % 100) / 100 - 1, (float)(rand() % 100) / 100 - 1));
    
    // Add asteroid to organization array
    asteroids_->addObject(a);
    
    // Add asteroid to layer
    this->addChild(a);
}

void GameLayer::createBullet()
{
    Bullet *b = Bullet::spriteWithFile("bullet.png");
    
    // Set the bullet's position by starting w/ the ship's position, then adding the rotation vector, so the bullet appears to come from the ship's nose
    
    b->setPosition(ccp(ship_->getPosition().x + cos(CC_DEGREES_TO_RADIANS(ship_->getRotation())) * ship_->getContentSize().width, 
                       ship_->getPosition().y - sin(CC_DEGREES_TO_RADIANS(ship_->getRotation())) * ship_->getContentSize().height));
    
    // Set the bullet's velocity to be in the same direction as the ship is pointing, plus whatever the ship's velocity is
    b->setVelocity(ccp(cos(CC_DEGREES_TO_RADIANS(ship_->getRotation())) * 2 + ship_->getVelocity().x, 
                       -sin(CC_DEGREES_TO_RADIANS(ship_->getRotation())) * 2 + ship_->getVelocity().y));
	b->setExpired(false);
    
    // Add bullet to organizational array
    bullets_->addObject(b);
     
    // Add bullet to layer
    this->addChild(b);
}

void GameLayer::startLevel()
{
    // Reset the ship's position, which also removes all bullets
    this->resetShip();
    
    // Get window size
    CCSize windowSize = CCDirector::sharedDirector()->getWinSize();
    
    // Create asteroids based on level number
	for (int i = 0; i < (currentLevel_ + 8); i++)
	{
		// Random numbers
		//CCPoint randomPointOnScreen = ccp((float)(rand() % 100) / 100 * windowSize.width, (float)(rand() % 100) / 100 * windowSize.height);

		int Pos = rand() % 4;

		CCPoint randomPointOnScreen;
		if(Pos == 0)
			randomPointOnScreen = ccp(0.0f, (float)(rand() % 100) / 100 * windowSize.height);
		else if(Pos == 1)
			randomPointOnScreen = ccp((float)(rand() % 100) / 100 * windowSize.width, windowSize.height);
		else if(Pos == 2)
			randomPointOnScreen = ccp( windowSize.width, (float)(rand() % 100) / 100 * windowSize.height);
		else
			randomPointOnScreen = ccp((float)(rand() % 100) / 100 * windowSize.width, 0.0f);

		//CCPoint randomPointOnScreen = ccp((float)(rand() % 100) / 100 * windowSize.width, (float)(rand() % 100) / 100 * windowSize.height);
        
        this->createAsteroidAt(randomPointOnScreen, kAsteroidLarge);
	}
}

void GameLayer::resetShip()
{
    CCObject* pObject = NULL;
	CCARRAY_FOREACH(bullets_, pObject)
    {
        Bullet *b = (Bullet *)pObject;
        this->removeChild(b, true);
    }
    
    bullets_->removeAllObjects();
}

void GameLayer::gameOver()
{
	ship_->Destroy(this);
    this->resetShip();
    this->unscheduleUpdate();
    
    ship_->setVisible(false);
    
    CCSize windowSize = CCDirector::sharedDirector()->getWinSize();
    
    CCLabelTTF *title = CCLabelTTF::create("game over", "Courier", 64.0);
    title->setPosition(ccp(windowSize.width / 2, windowSize.height/2));
    this->addChild(title, 1);
    
	CCMenuItemFont *backButton = CCMenuItemFont::create("back to title", this, menu_selector(GameLayer::backButtonAction));
    
	CCMenu *menu = CCMenu::create(backButton, NULL);
    menu->setPosition(ccp(windowSize.width/2, title->getPosition().y - title->getContentSize().height));
    


    this->addChild(menu, 2);
}

void GameLayer::backButtonAction(CCObject* pSender)
{
    CCDirector::sharedDirector()->replaceScene(TitleLayer::scene());
}
////////////////////////////////////////////////////////////////////////
void GameLayer::ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent)
{
    CCSetIterator it = pTouches->begin();
    CCTouch* touch = (CCTouch*)(*it);

    m_tBeginPos = touch->getLocationInView();    
    m_tBeginPos = CCDirector::sharedDirector()->convertToGL( m_tBeginPos );
	//ship_->setPosition( m_tBeginPos); 


///////////////////////////////////////////////////////////////
//particle effect
///////////////////////////////////////////////////////////////
	show_particle(m_tBeginPos);
}

void GameLayer::show_particle(CCPoint& Pos)
{
	CCParticleSystem* Emitter = new CCParticleSystemQuad();

	char* pFileName = "ExplodingRing.plist";
	Emitter->initWithFile(pFileName);
	Emitter->setPosition(Pos);
	addChild(Emitter);
}

void GameLayer::ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent)
{
	static float m_temp = 0; 
    CCSetIterator it = pTouches->begin();
    CCTouch* touch = (CCTouch*)(*it);

    m_tBeginPos = touch->getLocationInView();    
    m_tBeginPos = CCDirector::sharedDirector()->convertToGL( m_tBeginPos );
    m_tEndPos = touch->getPreviousLocationInView();    
    m_tEndPos = CCDirector::sharedDirector()->convertToGL( m_tEndPos );
	
	CCPoint CurPos = ship_->getPosition();
	CCPoint Dir;
	Dir.x = m_tBeginPos.x - CurPos.x;
	Dir.y = m_tBeginPos.y - CurPos.y;
	float temp = sqrtf(Dir.x * Dir.x + Dir.y * Dir.y);
	
	if(temp == 0)
		temp = m_temp;
	
	float x = Dir.x / temp;
	float y = Dir.y / temp;

	CCPoint offset = CCPoint(x,y);

	CurPos.x += (offset.x)*4;
	CurPos.y += (offset.y)*4;

	ship_->setPosition( CurPos); 

	m_temp = temp;
/*
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
            
            // Use CCRANDOM_0_1() will cause error when loading libtests.so on android, I don't know why.
            m_pBrush->setColor(ccc3(rand() % 127 + 128, 255, 255));
            // Call visit to draw the brush, don't call draw..
            m_pBrush->visit();
        }
    }

    // finish drawing and return context back to the screen
    _rt->end(); 
#endif*/
}

void GameLayer::ccTouchesEnded(CCSet *pTouches, CCEvent *pEvent)
{
}

void GameLayer::ccTouchesCancelled(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent)
{
    ccTouchesEnded(pTouches, pEvent);
} 

bool
GameLayer::isCollisionBetweenSpriteA(CCSprite* spr1, CCSprite* spr2, bool pp)
{
    bool isCollision = false; 
    //CCRect intersection = CGRectIntersection([spr1 boundingBox], [spr2 boundingBox]);
	CCRect intersection;
	CCRect r1 = spr1->boundingBox();
	CCRect r2 = spr2->boundingBox();
	intersection = CCRectMake(MAX(r1.getMinX(),r2.getMinX()), MAX(r1.getMinY(),r2.getMinY()),0,0);
	intersection.size.width = MIN(r1.getMaxX(), r2.getMaxX()) - intersection.getMinX();
	intersection.size.height = MIN(r1.getMaxY(), r2.getMaxY()) - intersection.getMinY();
    
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
       // glColor4f(1, 0, 0, 1);
        glLineWidth(0.5f);
        ccDrawLine(ccp(r1.origin.x,r1.origin.y), ccp(r1.origin.x+r1.size.width,r1.origin.y));
        ccDrawLine(ccp(r1.origin.x,r1.origin.y), ccp(r1.origin.x,r1.origin.y+r1.size.height));
        ccDrawLine(ccp(r1.origin.x+r1.size.width,r1.origin.y), ccp(r1.origin.x+r1.size.width,r1.origin.y+r1.size.height));
        ccDrawLine(ccp(r1.origin.x,r1.origin.y+r1.size.height), ccp(r1.origin.x+r1.size.width,r1.origin.y+r1.size.height));
        
        // Draw the second sprite bounding box
        //CGRect r2 = [spr2 boundingBox];
        //glColor4f(0, 1, 0, 1);
        glLineWidth(0.5f);
        ccDrawLine(ccp(r2.origin.x,r2.origin.y), ccp(r2.origin.x+r2.size.width,r2.origin.y));
        ccDrawLine(ccp(r2.origin.x,r2.origin.y), ccp(r2.origin.x,r2.origin.y+r2.size.height));
        ccDrawLine(ccp(r2.origin.x+r2.size.width,r2.origin.y), ccp(r2.origin.x+r2.size.width,r2.origin.y+r2.size.height));
        ccDrawLine(ccp(r2.origin.x,r2.origin.y+r2.size.height), ccp(r2.origin.x+r2.size.width,r2.origin.y+r2.size.height));
        
        // Draw the intersection rectangle in BLUE (testing purposes)
       // glColor4f(0, 0, 1, 1);
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