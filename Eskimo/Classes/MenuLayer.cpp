//
//  MenuLayer.cpp
//  Eskimo
//
//  Created by Roger Engelbert on 1/22/13.
//
//

#include "MenuLayer.h"
#include "LevelSelectLayer.h"
#include "GameLayer.h"

MenuLayer::~MenuLayer(){}

MenuLayer::MenuLayer()
{
    _screenSize = CCDirector::sharedDirector()->getWinSize();
    //interval between balloon messages
    _msgCntDown = 5;
    createScreen();
    
    setTouchEnabled( true );
    scheduleUpdate();
    
}

void MenuLayer::update(float dt) {
	
	
	//update balloon messages
    if (!_caption2->isVisible()) _msgCntDown += dt;
    
    if (_msgCntDown > 6) {
        if (!_balloon->isVisible()) {
            SimpleAudioEngine::sharedEngine()->playEffect("cap.wav");
            _msgCntDown = 0;
            _balloon->setVisible(true);
            _caption1->setVisible(true);
        } else {
            if (!_caption2->isVisible()) {
                SimpleAudioEngine::sharedEngine()->playEffect("cap.wav");
                _caption1->setVisible(false);
                _caption2->setVisible(true);
            }
        }
    }
}


CCScene* MenuLayer::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // add layer as a child to scene
    CCLayer* layer = new MenuLayer();
    scene->addChild(layer);
    layer->release();
    
    return scene;
}

void MenuLayer::showLevels(CCObject* pSender) {
     SimpleAudioEngine::sharedEngine()->playEffect("button.wav");
    CCScene* newScene = CCTransitionMoveInR::create(0.2f, LevelSelectLayer::scene());
    CCDirector::sharedDirector()->replaceScene(newScene);
}
void MenuLayer::showHelp(CCObject* pSender) {
    SimpleAudioEngine::sharedEngine()->playEffect("button.wav");
    int levelsCompleted = CCUserDefault::sharedUserDefault()->getIntegerForKey("levelsCompleted");
    CCScene* newScene = CCTransitionMoveInR::create(0.2f, GameLayer::scene(0, levelsCompleted));
    CCDirector::sharedDirector()->replaceScene(newScene);
}

void MenuLayer::createScreen() {
    
    CCSprite * bg = CCSprite::create("bg.jpg");
    bg->setPosition(ccp(_screenSize.width * 0.5f, _screenSize.height * 0.5f));
    this->addChild(bg);
    
    //load a particle system for snow
    _snow = CCParticleSystemQuad::create("snow.plist");
    _snow->setPosition(ccp(_screenSize.width * 0.5, _screenSize.height));
    this->addChild(_snow, kBackground);
    
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("sprite_sheet.plist");
    _layerBatchNode = CCSpriteBatchNode::create("sprite_sheet.png", 50);
    this->addChild(_layerBatchNode, kMiddleground);
    
    CCSprite * ground = CCSprite::createWithSpriteFrameName("intro_igloo.png");
    ground->setAnchorPoint(ccp(0.5f, 0.05f));
    ground->setPosition(ccp(_screenSize.width * 0.5f, 0.0));
    _layerBatchNode->addChild(ground);
    
    CCSprite * logo = CCSprite::createWithSpriteFrameName("logo.png");
    logo->setPosition(ccp(_screenSize.width * 0.5f, _screenSize.height * 0.75f));
    _layerBatchNode->addChild(logo);
    
    //create menu
    CCSprite * menuItemOn;
    CCSprite * menuItemOff;
    menuItemOn = CCSprite::createWithSpriteFrameName("btn_play_on.png");
    menuItemOff = CCSprite::createWithSpriteFrameName("btn_play_off.png");
    
    CCMenuItemSprite * playItem = CCMenuItemSprite::create(
                               menuItemOff,
                               menuItemOn,
                               this,
                               menu_selector(MenuLayer::showLevels));
    
    menuItemOn = CCSprite::createWithSpriteFrameName("btn_help_on.png");
    menuItemOff = CCSprite::createWithSpriteFrameName("btn_help_off.png");
    CCMenuItemSprite * helpItem = CCMenuItemSprite::create(
                               menuItemOff,
                               menuItemOn,
                               this,
                               menu_selector(MenuLayer::showHelp));
   
    _mainMenu = CCMenu::create(playItem, helpItem, NULL);
    _mainMenu->alignItemsVerticallyWithPadding(10);
    _mainMenu->setPosition(ccp(_screenSize.width * 0.5f, _screenSize.height * 0.5f));
    this->addChild(_mainMenu, kForeground);
    
    
    //add balloon caption
    _balloon = CCSprite::createWithSpriteFrameName("cap.png");
    _balloon->setPosition(ccp(_screenSize.width * 0.36f, _screenSize.height * 0.25f));
    _layerBatchNode->addChild(_balloon, kForeground);
    _balloon->setVisible(false);
    
    _caption1 = CCSprite::createWithSpriteFrameName("cap_supper.png");
    _caption2 = CCSprite::createWithSpriteFrameName("cap_where.png");

    _caption1->setVisible(false);
    _caption2->setVisible(false);
    
    _balloon->addChild(_caption1);
    _balloon->addChild(_caption2);
    
    CCRect rect = _balloon->getTextureRect();
    _caption1->setPosition(ccp(rect.size.width * 0.42f, rect.size.height * 0.45f));
    _caption2->setPosition(ccp(rect.size.width * 0.42f, rect.size.height * 0.45f));
    
    
}