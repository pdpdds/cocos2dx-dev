//
//  ScoresLayer.cpp
//  Asteroids
//
//  Created by Clawoo on 9/18/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//
#pragma warning(disable : 4996)
#include "ScoresLayer.h"
#include "TitleLayer.h"

USING_NS_CC;

CCScene* ScoresLayer::scene()
{
	// 'scene' is an autorelease object
	CCScene *scene = CCScene::create();
	
	// 'layer' is an autorelease object
	ScoresLayer *layer = ScoresLayer::node();
    
	// add layer as a child to scene
	scene->addChild(layer);
    
	// return the scene
	return scene;
}

bool ScoresLayer::init()
{
	if ( !CCLayer::init() )
	{
		return false;
	}
    
    // Get window size
    CCSize windowSize = CCDirector::sharedDirector()->getWinSize();
    
	CCLabelTTF *title = CCLabelTTF::create("high scores", "Courier", 32.0);
    
    title->setPosition(ccp(windowSize.width / 2, windowSize.height - title->getContentSize().height));
    
    // Add to layer
    this->addChild(title, 1);
    
	CCLabelTTF *scoresLabel = CCLabelTTF::create("None", "Courier", 16.0, CCSizeMake(windowSize.width, windowSize.height / 3), kCCTextAlignmentCenter);
    scoresLabel->setPosition(CCPointMake(windowSize.width/2, windowSize.height/2));

    CCMenuItemFont *backButton = CCMenuItemFont::create("back", this, menu_selector(ScoresLayer::backButtonAction));
    
	CCMenu *menu = CCMenu::create(backButton, NULL);
    menu->setPosition(ccp(windowSize.width/2, scoresLabel->getPosition().y - scoresLabel->getContentSize().height));
    
    this->addChild(menu, 2);
    
    return true;
}

void ScoresLayer::backButtonAction(CCObject* pSender)
{
    CCDirector::sharedDirector()->replaceScene(TitleLayer::scene());
}