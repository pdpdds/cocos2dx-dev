//
//  IntroLayer.cpp
//  moles
//
//  Created by breaklee on 6/9/13.
//
//

#include "IntroLayer.h"
#include "MenuLayer.h"



CCScene* IntroLayer::scene() {
    
    CCScene* scene = CCScene::create();
    IntroLayer* layer = IntroLayer::create();
    scene->addChild(layer);
    return scene;
    
}

bool IntroLayer::init() {
    
    if(CCLayer::init() == false) {
        return false;
    }
    return true;
    
}

void IntroLayer::onEnter() {
    
    CCLayer::onEnter();
    
    CCSize size = CCDirector::sharedDirector()->getWinSize();
    CCSprite* background = CCSprite::create("Default.png");
    
    background->setPosition(ccp(size.width/2, size.height/2));
    this->addChild(background);

    this->scheduleOnce(schedule_selector(IntroLayer::loadMenu), 1.0f);
    
}

void IntroLayer::loadMenu(float dt) {
    
    CCDirector::sharedDirector()->replaceScene(MenuLayer::scene());
    
}