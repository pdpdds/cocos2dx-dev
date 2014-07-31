//
//  GameOver.cpp
//  BrickEx
//
//  Created by Jaewhan Lee on 12. 12. 20..
//
//

#include "GameOver.h"
#include "HelloWorldScene.h"

CCScene* GameOver::createScene()
{
    auto scene = CCScene::create();

    auto layer = GameOver::create();

    scene->addChild(layer);

    return scene;
}

bool GameOver::init()
{
    if ( !CCLayer::init() )
    {
        return false;
    }
    
    /////////////////////////////
    
    // 메뉴 아이템 생성 및 초기화    
	auto item1 = CCMenuItemFont::create("New Game", this, menu_selector(GameOver::doClose));
                    
	ccColor3B color = { 255, 255, 255 };
	item1->setColor(color);
    
    // 메뉴 생성
    auto pMenu = CCMenu::create( item1, NULL );
    
    // 메뉴 위치
    pMenu->setPosition(CCPoint(160, 240));
    
    // 레이어에 메뉴 객체 추가
    this->addChild(pMenu);
    
    
    return true;
}

void GameOver::doClose(CCObject* pSender)
{
    CCLog("aaaa");
    // 새로운게임을 시작하게 처음 신으로 이동
    CCScene* pScene = HelloWorld::createScene();
    CCDirector::sharedDirector()->replaceScene( pScene );
}
