//
//  MainScene.cpp
//  Parkour
//
//  Created by YTB on 13-11-19.
//
//

#include "MainScene.h"
#include "PlayScene.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;

CCScene* MainLayer::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();

    // 'layer' is an autorelease object
    MainLayer *layer = MainLayer::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

bool MainLayer::init()
{
    if (!CCLayer::init()) {
        return false;
    }

    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();
    CCPoint centerPos = CCPoint(visibleSize.width / 2, visibleSize.height / 2);

    //add backgroud
    CCSprite *spriteBG = CCSprite::create("MainBG.png");
    spriteBG->setPosition(ccp(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));
    this->addChild(spriteBG, 0);

    //add menu item
    CCMenuItemFont::setFontSize(60);
    CCMenuItemImage *menuItemPlay = CCMenuItemImage::create("start_n.png", "start_s.png", this, menu_selector(MainLayer::onPlay));
    menuItemPlay->setPosition(centerPos);

    // create menu, it's an autorelease object
    CCMenu* pMenu = CCMenu::create(menuItemPlay, NULL);
    pMenu->setPosition(CCPointZero);
    this->addChild(pMenu, 1);
    
    // preload music
    CocosDenshion::SimpleAudioEngine *audioEngine = CocosDenshion::SimpleAudioEngine::sharedEngine();
    audioEngine->preloadBackgroundMusic("background.mp3");
    audioEngine->preloadEffect("jump.mp3");
    audioEngine->preloadEffect("crouch.mp3");
    audioEngine->setBackgroundMusicVolume(0.3);
    audioEngine->setEffectsVolume(0.3);
    
    return true;
}

void MainLayer::onPlay(CCObject* pSender)
{
    CocosDenshion::SimpleAudioEngine *audioEngine = CocosDenshion::SimpleAudioEngine::sharedEngine();
    audioEngine->playBackgroundMusic("background.mp3",true);

    CCDirector::sharedDirector()->replaceScene(PlayLayer::scene());
}
