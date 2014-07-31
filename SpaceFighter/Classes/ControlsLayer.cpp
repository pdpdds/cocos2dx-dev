//
//  ControlsLayer.cpp
//  Asteroids
//
//  Created by Clawoo on 9/18/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//
#pragma warning(disable : 4996)
#include "ControlsLayer.h"
#include "TitleLayer.h"

USING_NS_CC;

CCScene* ControlsLayer::scene()
{
	// 'scene' is an autorelease object
	CCScene *scene = CCScene::create();
	
	// 'layer' is an autorelease object
	ControlsLayer *layer = ControlsLayer::node();
    
	// add layer as a child to scene
	scene->addChild(layer);
    
	// return the scene
	return scene;
}

bool ControlsLayer::init()
{
	if ( !CCLayer::init() )
	{
		return false;
	}
    
    // Get window size
    CCSize windowSize = CCDirector::sharedDirector()->getWinSize();
    
    CCLabelTTF *title = CCLabelTTF::create("how to play", "Courier", 32.0);
    
    title->setPosition(ccp(windowSize.width / 2, windowSize.height - title->getContentSize().height));
    
    // Add to layer
    this->addChild(title, 1);
    
	CCLabelTTF *controlsLabel = CCLabelTTF::create("tap = shoot\npinch = rotate\nswipe = move",
															"Courier", 16.0,
                                                            CCSizeMake(windowSize.width, windowSize.height / 3), 
                                                            kCCTextAlignmentCenter);
    controlsLabel->setPosition(CCPointMake(windowSize.width/2, windowSize.height/2));
    this->addChild(controlsLabel);
    
	CCMenuItemFont *backButton = CCMenuItemFont::create("back", this, menu_selector(ControlsLayer::backButtonAction));
    
	CCMenu *menu = CCMenu::create(backButton, NULL);
    menu->setPosition(ccp(windowSize.width/2, controlsLabel->getPosition().y - controlsLabel->getContentSize().height));
    
    this->addChild(menu, 2);
    
    return true;
}

void ControlsLayer::backButtonAction(CCObject* pSender)
{
    CCDirector::sharedDirector()->replaceScene(TitleLayer::scene());
}