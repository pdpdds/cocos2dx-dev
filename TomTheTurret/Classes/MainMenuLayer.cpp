//
//  MainMenuLayer.cpp
//  TomTheTurret
//
//  Created by Jaewhan Lee on 13. 1. 21..
//
//

#include "MainMenuLayer.h"
#include "GameState.h"
#include "StoryLayer.h"


CCScene* MainMenuLayer::scene()
{
    CCScene *scene = CCScene::create();
    
    MainMenuLayer *layer = MainMenuLayer::create();
    
    // 960 x 640
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    if (winSize.width >= 960) {
        layer->setScale(2.0);
        layer->setAnchorPoint(ccp(0.0, 0.0));
    }

    scene->addChild(layer);
    
    return scene;
}

bool MainMenuLayer::init()
{
    if ( !CCLayer::init() )
    {
        return false;
    }
    
    
    // 화면 사이즈 구하기
//    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    CCSize winSize = CCSizeMake(480, 320);

    // 배치노드 로드 : 이미 있어서 메모리에 있는 텍스쳐가 사용된다.
    batchNode = CCSpriteBatchNode::createWithTexture(
                                                     CCTextureCache::sharedTextureCache()->addImage("Images/sprites.png"));
    this->addChild(batchNode);
    
    
    // 백그라운드 이미지
    main_bkgrnd = CCSprite::createWithSpriteFrameName("Turret_main_bkgrnd.png");
    main_bkgrnd->setPosition( ccp(winSize.width/2, winSize.height/2) );
    batchNode->addChild(main_bkgrnd);


    // 메인메뉴 추가
    CCSprite *newGameSprite1 = CCSprite::createWithSpriteFrameName("Turret_newgame.png");
    CCSprite *newGameSprite2 = CCSprite::createWithSpriteFrameName("Turret_newgame.png");
    newGameSprite2->setScale((float)1.1);
    CCMenuItem *newGameItem = CCMenuItemSprite::create(newGameSprite1,
                                                       newGameSprite2,
                                                       this,
                                                       menu_selector(MainMenuLayer::newGameSpriteTapped) );
    CCMenu* menu = CCMenu::create(newGameItem, NULL);
    menu->setPosition( ccp(winSize.width/2, winSize.height/2) );
    this->addChild(menu);

    
    return true;
}

void MainMenuLayer::newGameSpriteTapped(CCObject* sender)
{
    GameState::getInstance()->reset();
    
    // 새로운게임 스토리 신으로 이동
    CCScene* pScene = StoryLayer::scene();
    CCDirector::sharedDirector()->replaceScene( CCTransitionProgressRadialCW::create(0.5f, pScene) );
}

