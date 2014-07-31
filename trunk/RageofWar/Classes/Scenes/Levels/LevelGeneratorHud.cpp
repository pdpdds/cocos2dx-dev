/*
 *  LevelGeneratorHud.cpp
 *  TileBasedGame
 *
 *  Created by Clawoo on 8/21/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "LevelGeneratorHud.h"
#include "LevelGenerator.h"

USING_NS_CC;
float LevelGeneratorHud::getxval()
{
	return joystick->getVelocity().x;
}

bool LevelGeneratorHud::init()
{
	joystick = NULL;
		jumpButton = NULL;
		attackButton = NULL;
		sectime=0;
	scalefactor=1.0;

	CCSprite *sprite  = CCSprite::create("SpriteImages/ScoreBar.png");
    sprite->setPosition(ccp(250.0f, 305));
    this->addChild(sprite, 0);


	sprite  = CCSprite::create("SpriteImages/emptyBar.png");
    sprite->setPosition(ccp(700.0f, 305));
    this->addChild(sprite, 0);

	CCSize winSize = CCDirector::sharedDirector()->getWinSize();
	CCSize screenSize = CCDirector::sharedDirector()->getWinSize();
	_label = CCLabelTTF::create("0", "Verdana-Bold", 18.0, CCSizeMake(50, 20), kCCTextAlignmentRight);
	_label->setColor(ccc3(0, 0, 0));
	_label->setVisible(false);

	this->_lblScore = CCLabelTTF::create("000","Verdana", 20);
	_lblScore ->retain();
	_lblScore ->setColor( ccc3(250,250, 250) );
	_lblScore ->setPosition(ccp(190,305));
	this->addChild(_lblScore ,0);

	//Enemy Destroyed Score Label
	this->_lblEnemyScore = CCLabelTTF::create("0","Verdana", 20);
	_lblEnemyScore ->retain();
	_lblEnemyScore ->setColor( ccc3(250,250, 250) );
	_lblEnemyScore ->setPosition(ccp(280,305));
	this->addChild(_lblEnemyScore ,0);
	//Zombie Destroyed Score Label
	this->_lblZombieScore = CCLabelTTF::create("0","Verdana", 20);
	_lblZombieScore ->retain();
	_lblZombieScore ->setColor( ccc3(250,250, 250) );
	_lblZombieScore ->setPosition(ccp(340,305));
	this->addChild(_lblZombieScore ,0);
	//Car Destroyed Score Label
	this->_lblCarScore = CCLabelTTF::create("0","Verdana", 20);
	_lblCarScore ->retain();
	_lblCarScore ->setColor( ccc3(250,250, 250) );
	_lblCarScore ->setPosition(ccp(400,305));
	this->addChild(_lblCarScore ,0);

 
	this->_lblTimer = CCLabelTTF::create("00:00","Verdana", 20);
	_lblTimer ->retain();
	_lblTimer ->setColor( ccc3(250,250, 250) );
	_lblTimer ->setPosition(ccp(475,305));
	_lblTimer->setVisible(false);
	this->addChild(_lblTimer ,0);


	int margin = 10;
	
	_label->setPosition(ccp(winSize.width - (_label->getContentSize().width / 2) - margin, 
							_label->getContentSize().height / 2 + margin));
	
	this->addChild(_label);
	
	/*CCMenuItem *on = CCMenuItemImage::itemFromNormalImage("projectile-button-on.png", "projectile-button-on.png", NULL, NULL);
	CCMenuItem *off = CCMenuItemImage::itemFromNormalImage("projectile-button-off.png", "projectile-button-off.png", NULL, NULL);
	
	CCMenuItemToggle *toggleItem = CCMenuItemToggle::itemWithTarget(this, menu_selector(LevelGeneratorHud::projectileButtonTapped), off, on, NULL);
	
	CCMenu *toggleMenu = CCMenu::menuWithItems(toggleItem, NULL);
	toggleMenu->setPosition(ccp(100, 32));
	this->addChild(toggleMenu);
	*/
		////////////////////////////////////////
		//health bar border and bar
		/////////////////////////////////////////
		CCSprite* hbb = CCSprite::create("SpriteImages/heathbarborder.png");

		hbb->setPosition(ccp(winSize.width-300,winSize.height-13));
		this->addChild(hbb,+3);//background green

		//CCSprite* hbb2 = CCSprite::spriteWithFile("SpriteImages/healthbar2.png");

		//hbb2->setPosition(ccp(winSize.width-185,winSize.height-20));
		//this->addChild(hbb2,+3);//background green

		CCSprite* hb = CCSprite::create("SpriteImages/healthbar.png");
		
		hbxloc=winSize.width-300;
		hb->setPosition(ccp(hbxloc,winSize.height-13));
		
		this->addChild(hb,+2,34);//health bar
		//////////////////////////////////////////////////////
		//power up

		//powerup = CCSprite::spriteWithFile("bolt.png");

		//powerup->setPosition(ccp(18, winSize.height-20));
		//this->addChild(powerup,+3);//background green
		//this->powerup->setIsVisible(false);

		//////////////////////////////////////////////////////
		//life

		//life = CCSprite::spriteWithFile("life.png");

		//life->setPosition(ccp(98, winSize.height-20));
		//this->addChild(life,+3);//background green
		//this->life->setIsVisible(false);
		////////////////////////////////////////////////

		//gamelayer stuff
		this->setTouchEnabled(true);

		srand(time(NULL));
		CCLOG("Loading Assets");

		CCLOG("Loading Done");


		this->initJoystickAndButtons();
		this->setJoystick(leftJoystick);
		this->setAttackButton(attackButton);
		this->setJumpButton(jumpButton);
	
		cocos2d::CCLabelTTF *gameBeginLabel = cocos2d::CCLabelTTF::create("Game Start", "Arial",20);
		gameBeginLabel->setPosition(ccp(SCREEN_WIDTH/2, SCREEN_HEIGHT/2));

		this->addChild(gameBeginLabel);
		CCFiniteTimeAction *labelAction = CCSpawn::create(CCScaleBy::create(0.6f, 2), CCFadeOut::create(0.6f), NULL);
		gameBeginLabel->runAction(labelAction);


		// Create a "close" menu item with close icon, it's an auto release object.
		CCMenuItemImage *pCloseItem = CCMenuItemImage::create(
		"SpriteImages/backbutton.png",
		"SpriteImages/backbutton.png",
		this,
		menu_selector(LevelGeneratorHud::menuBackCallback));
     

		// Place the menu item bottom-right conner.
		pCloseItem->setPosition(ccp(CCDirector::sharedDirector()->getWinSize().width - 120,CCDirector::sharedDirector()->getWinSize().height -15));

		// Create a menu with the "close" menu item, it's an auto release object.
		CCMenu* pMenu = CCMenu::createWithItem(pCloseItem);
		//pMenu->setPosition(ccp(CCDirector::sharedDirector()->getWinSize().width - 20,CCDirector::sharedDirector()->getWinSize().height -20));
		pMenu->setPosition(CCPointZero);
		// Add the menu to HelloWorld layer as a child layer.
		this->addChild(pMenu, 20,1);


	/*	CCSprite *spLevel = CCSprite::spriteWithFile("SpriteImages/level1.png");
		spLevel->setAnchorPoint(CCPointZero);
		spLevel->setPosition(CCPointMake(440,290));
		addChild(spLevel,+10);*/

		//Pause Button
		// Create a "pause" menu item with close icon, it's an auto release object.
		CCMenuItemImage *pPauseGame = CCMenuItemImage::create(
		"pause button.png",
		"pause button.png",
		
		this,
		menu_selector(LevelGeneratorHud::PauseGameCallback));
     
		//pPauseGame->set
		// Place the menu item 
		pPauseGame->setPosition(CCPointMake(780,305));

		
		pMenuPause = CCMenu::createWithItem(pPauseGame);
		//pMenu->setPosition(ccp(CCDirector::sharedDirector()->getWinSize().width - 20,CCDirector::sharedDirector()->getWinSize().height -20));
		pMenuPause->setPosition(CCPointZero);
		// Add the menu to HelloWorld layer as a child layer.
		this->addChild(pMenuPause, 20,1);
		
		
		//Play Button
		// Create a "pause" menu item with close icon, it's an auto release object.
		CCMenuItemImage *pPlayGame = CCMenuItemImage::create(
		"play button.png",
		"play button.png",
		
		this,
		menu_selector(LevelGeneratorHud::PlayGameCallback));
     
		//pPauseGame->set
		// Place the menu item 
		pPlayGame->setPosition(CCPointMake(780,305));

		
		pMenuPlay = CCMenu::createWithItem(pPlayGame);
		//pMenu->setPosition(ccp(CCDirector::sharedDirector()->getWinSize().width - 20,CCDirector::sharedDirector()->getWinSize().height -20));
		pMenuPlay->setPosition(CCPointZero);
		// Add the menu to HelloWorld layer as a child layer.
		this->addChild(pMenuPlay, 20,1);


		pMenuPause->setVisible(true);
	pMenuPlay->setVisible(false);


		this->scheduleUpdate();
		this->setKeypadEnabled(true);
	return true;
}

void LevelGeneratorHud::updateScore(int score)
{
	const int labelLength = 100;
	char scoreLabelText[labelLength];
	sprintf_s(scoreLabelText, labelLength, "%d", score);
	_lblScore ->setString(scoreLabelText);

}
void LevelGeneratorHud::showHealthPackAlert()
{
	CCSprite *sprite = CCSprite::create("SpriteImages/power.png");
	sprite->setPosition(ccp(475,285));
    this->addChild(sprite, 1,66);
	CCActionInterval *blink = CCBlink::create(80.0, 60.0);
			sprite->runAction(blink);
}
void LevelGeneratorHud::hideHealthPackAlert()
{
	
	this->removeChildByTag(66,true);
}
void LevelGeneratorHud::updateEnemyScore(int score)
{
	const int labelLength = 100;
	char scoreLabelText[labelLength];
	sprintf_s(scoreLabelText, labelLength, "%d", score);
	_lblEnemyScore ->setString(scoreLabelText);

}
void LevelGeneratorHud::updateZombieScore(int score)
{
	const int labelLength = 100;
	char scoreLabelText[labelLength];
	sprintf_s(scoreLabelText, labelLength, "%d", score);
	_lblZombieScore ->setString(scoreLabelText);

}
void LevelGeneratorHud::updateCarScore(int score)
{
	const int labelLength = 100;
	char scoreLabelText[labelLength];
	sprintf_s(scoreLabelText, labelLength, "%d", score);
	_lblCarScore ->setString(scoreLabelText);

}
void LevelGeneratorHud::updateTruckScore(int score)
{
	const int labelLength = 100;
	char scoreLabelText[labelLength];
	sprintf_s(scoreLabelText, labelLength, "T :%d", score);
	_lblTruckScore ->setString(scoreLabelText);

}
void LevelGeneratorHud::updateHeliScore(int score)
{
	const int labelLength = 100;
	char scoreLabelText[labelLength];
	sprintf_s(scoreLabelText, labelLength, "H :%d", score);
	_lblHeliScore ->setString(scoreLabelText);

}
void LevelGeneratorHud::menuBackCallback(CCObject* pSender)
{
   CCDirector::sharedDirector()->replaceScene(MainScene::scene());

}
void LevelGeneratorHud::PauseGameCallback(CCObject* pSender)
{
	
	CCDirector::sharedDirector()->pause();
	pMenuPause->setVisible(false);
	pMenuPlay->setVisible(true);
}
void LevelGeneratorHud::PlayGameCallback(CCObject* pSender)
{
		CCDirector::sharedDirector()->resume();
	pMenuPause->setVisible(true);
	pMenuPlay->setVisible(false);

}
bool LevelGeneratorHud::updateTimer()
{
	const int labelLength = 100;
	char timerLabelText[labelLength];
	if(sectime<10)
	{
		sprintf_s(timerLabelText, labelLength, "00:0%d", sectime);
	}
	else if(sectime<=60)
	{
		sprintf_s(timerLabelText, labelLength, "00:%d", sectime);
	}
	else
	{
		int sec=sectime%60;
		int min=sectime/60;
		//sprintf_s(timerLabelText, labelLength, "0%d:%d",min ,sec);
		if(sec<10)
		{
			sprintf_s(timerLabelText, labelLength, "0%d:0%d",min ,sec);
		}
		else if(sec<=60)
		{
			sprintf_s(timerLabelText, labelLength, "0%d:%d",min ,sec);
		}
	}
	sectime+=1;
	
	_lblTimer ->setString(timerLabelText);

	if(sectime>=60)
	return true;
	else
		return false;
}
void LevelGeneratorHud::numCollectedChanged(int numCollected)
{
	char *str = (char *)malloc(sizeof(str) * 10);
	sprintf(str, "%d", numCollected);
	_label->setString(str);
	free(str);
}

void LevelGeneratorHud::projectileButtonTapped(CCObject *sender)
{
	/*if (_gameLayer->getMode() == 0)
	{
		_gameLayer->setMode(1);
	}
	else {
		_gameLayer->setMode(0);
	}*/

}
void LevelGeneratorHud::updateHealthBar(int health)
{
	         scalefactor= health/100.0f;
			  if(health==100)
			  {
				  CCSize winSize = CCDirector::sharedDirector()->getWinSize();
				  hbxloc=winSize.width-300;
				  scalefactor=1.0;
				  this->getChildByTag(34)->setScaleX(scalefactor);
				  this->getChildByTag(34)->setPosition(ccp(hbxloc,winSize.height-13));
			  }
			  else
				if(scalefactor>=0)
				{
					this->getChildByTag(34)->setScaleX(scalefactor);
					CCSize winSize = CCDirector::sharedDirector()->getWinSize();
					//
					hbxloc=(hbxloc-5);
					this->getChildByTag(34)->setPosition(ccp(hbxloc,winSize.height-13));
					
				}
}
void LevelGeneratorHud::initJoystickAndButtons()
{
	CCSize screenSize = CCDirector::sharedDirector()->getWinSize();
	CCRect joystickBaseDimensions = CCRectMake(0, 0, 128.0f, 128.0f); 
	CCRect jumpButtonDimensions = CCRectMake(0, 0, 64.0f, 64.0f); 
	CCRect attackButtonDimensions = CCRectMake(0, 0, 64.0f, 64.0f);

	CCPoint joystickBasePosition;
	CCPoint jumpButtonPosition;
	CCPoint attackButtonPosition;

	joystickBasePosition = ccp(screenSize.width*0.07f, screenSize.height*0.11f);
	jumpButtonPosition = ccp(screenSize.width*0.93f, screenSize.height*0.11f);
	// Set the attackButtonPosition to be 80 points above the jump button
	// So that their positions are fixed and do not change because of different screen resolutions
	attackButtonPosition = ccp(screenSize.width*0.93f, screenSize.height*0.11f + 80.0f);

	//Joystick
	SneakyJoystickSkinnedBase *joystickBase = SneakyJoystickSkinnedBase::node(); 
	joystickBase->setPosition(joystickBasePosition);
	joystickBase->setBackgroundSprite(CCSprite::create("dpadDown.png"));
	joystickBase->setThumbSprite(CCSprite::create("joystickDown.png"));
	SneakyJoystick *aJoystick = new SneakyJoystick();
	aJoystick->initWithRect(joystickBaseDimensions);
	aJoystick->autorelease();

	joystickBase->setJoystick(aJoystick);
	/* IMPORTANT: do not need to retain here like in the book, because the setJoystick method does retain as well
	   In the book the setJoystick method is not called, instead the joystick is set directly, which we can't do in cocos2d-x
	   Same thing applies for the buttons below */

	//joystickBase->getJoystick()->retain();
	leftJoystick = joystickBase->getJoystick();
	this->addChild(joystickBase);

	//Jump button
	SneakyButtonSkinnedBase *jumpButtonBase = SneakyButtonSkinnedBase::node(); 
	jumpButtonBase->setPosition(jumpButtonPosition); // 13
	jumpButtonBase->setDefaultSprite(CCSprite::create("jumpUp.png")); // 14
	jumpButtonBase->setActivatedSprite(CCSprite::create("jumpDown.png")); // 15
	jumpButtonBase->setPressSprite(CCSprite::create("jumpDown.png")); // 16
	SneakyButton *aButton = new SneakyButton();
	aButton->initWithRect(jumpButtonDimensions);
	aButton->autorelease();

	jumpButtonBase->setButton(aButton); // 17
	//jumpButtonBase->getButton()->retain(); // 18
	jumpButton = jumpButtonBase->getButton();
	jumpButton->setIsToggleable(false); // 19
	this->addChild(jumpButtonBase); // 20

	//Attack button
	SneakyButtonSkinnedBase *attackButtonBase = SneakyButtonSkinnedBase::node(); 
	attackButtonBase->setPosition(attackButtonPosition); // 13
	attackButtonBase->setDefaultSprite(CCSprite::create("handUp.png")); // 14
	attackButtonBase->setActivatedSprite(CCSprite::create("handDown.png")); // 15
	attackButtonBase->setPressSprite(CCSprite::create("handDown.png")); // 16
	SneakyButton *aButton1 = new SneakyButton();
	aButton1->initWithRect(attackButtonDimensions);
	aButton1->autorelease();

	attackButtonBase->setButton(aButton1); // 17
	//attackButtonBase->getButton()->retain(); // 18
	attackButton = attackButtonBase->getButton();
	attackButton->setIsToggleable(false); // 19
	this->addChild(attackButtonBase); // 20

}

void  LevelGeneratorHud::update(float deltaTime)
{
	/*CCArray *listOfGameObjects = sceneSpriteBatchNode->getChildren();
	CCObject *object = NULL;
	CCARRAY_FOREACH(listOfGameObjects, object)
	{
		GameCharacter *tempChar = (GameCharacter *) object; 
		tempChar->updateStateWithDeltaTime(deltaTime, listOfGameObjects);
	}*/
}
void  LevelGeneratorHud::showTimer()
{
	_lblTimer->setVisible(true);
}