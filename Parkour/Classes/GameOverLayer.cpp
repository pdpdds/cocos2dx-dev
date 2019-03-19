//
//  GameOverLayer.cpp
//  Parkour
//
//  Created by YTB on 13-11-20.
//
//

#include "GameOverLayer.h"
#include "PlayScene.h"

GameOverLayer* GameOverLayer::create(const ccColor4B& color)
{
    GameOverLayer *pRet = new GameOverLayer();
    pRet->initWithColor(color);
    pRet->autorelease();
    return pRet;
}

bool GameOverLayer::initWithColor(const ccColor4B& color)
{
    if (!CCLayerColor::initWithColor(color)) {
        return false;
    }

    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    CCPoint centerPos = CCPoint(winSize.width / 2, winSize.height / 2);
    
    CCMenuItemFont::setFontSize(30);
    CCMenuItemSprite *menuItemRestart = CCMenuItemSprite::create(CCSprite::create("restart_n.png"), CCSprite::create("restart_s.png"),this, menu_selector(GameOverLayer::onRestart));
    CCMenu *menu = CCMenu::create(menuItemRestart, NULL);
    menu->setPosition(centerPos);
    this->addChild(menu);
    
    return true;
}

void GameOverLayer::onRestart(CCObject* pSender)
{
    CCDirector::sharedDirector()->replaceScene(PlayLayer::scene());
    CCDirector::sharedDirector()->resume();
}