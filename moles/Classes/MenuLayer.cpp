//
//  IntroLayer.cpp
//  moles
//
//  Created by breaklee on 6/9/13.
//
//

#include "MenuLayer.h"
#include "PlayFieldLayer.h"

CCScene* MenuLayer::scene() {
    
    CCScene* scene = CCScene::create();
    MenuLayer* layer = MenuLayer::create();
    scene->addChild(layer);
    return scene;
    
}

bool MenuLayer::init() {
    
    if(CCLayer::init() == false) {
        return false;
    }
    started = false;
    return true;
    
}

void MenuLayer::onEnter() {
    
    CCLayer::onEnter();
        
    CCSize size = CCDirector::sharedDirector()->getWinSize();
    
    CCLabelTTF* label = CCLabelTTF::create("Mole Thumper", "Times New Roman", 44);
    label->setColor(ccWHITE);
    label->setPosition(ccp(size.width/2, size.height/2 - 20));
    this->addChild(label);
    
    CCLabelTTF* start = CCLabelTTF::create("start", "Times New Romanq", 22);
    start->setColor(ccBLUE);
    CCMenuItemLabel* item = CCMenuItemLabel::create(start, this, menu_selector(MenuLayer::startGame));	

    
    CCMenu* menu = CCMenu::create(item, NULL);
    this->addChild(menu);
    menu->setPosition(ccp(size.width/2, size.height/2 - 50));
    
}

void MenuLayer::startGame(CCObject* pObject) {
    
    CCDirector::sharedDirector()->replaceScene(PlayFieldLayer::scene());
    
}