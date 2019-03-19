//
//  LoadingLayer.cpp
//  TomTheTurret
//
//  Created by Jaewhan Lee on 13. 1. 21..
//
//

#include "LoadingLayer.h"
#include "MainMenuLayer.h"


static int MAIN_TITLE_TOP_MARGIN   = 20;
static int TOM_BOTTOM_MARGIN       = 30;
static int TOM_LEFT_MARGIN         = 20;
static int TAPTOCONT_BOTTOM_MARGIN = 30;
static int LOADING_BOTTOM_MARGIN   = 30;

CCScene* LoadingLayer::scene()
{
    CCScene *scene = CCScene::create();
    
    LoadingLayer *layer = LoadingLayer::create();
    
    // 960 x 640
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    if (winSize.width >= 960) {
        layer->setScale(2.0);
        layer->setAnchorPoint(ccp(0.0, 0.0));
    }

    scene->addChild(layer);
    
    return scene;
}

bool LoadingLayer::init()
{
    if ( !CCLayer::init() )
    {
        return false;
    }
    
    /////////////////////////////
    
    // 변수 초기화
    isLoading = true;
    imagesLoaded = false;
    
    // 터치 활성화
    this->setTouchEnabled(true);
    
    // 화면 사이즈 구하기
    //winSize = CCDirector::sharedDirector()->getWinSize();
    winSize = CCSizeMake(480, 320);

    
    // 디폴트 이미지 세로형 화면에 추가하기
    defaultImage = CCSprite::create("Images/DefaultLandscape.png");
    defaultImage->setPosition(ccp(winSize.width/2, winSize.height/2));
    this->addChild(defaultImage);
    
    // 백그라운드에서 사용할 스프라이트 로딩
    // 스프라이트 시트의 위치정보 파일을 읽어들인다.
    CCTextureCache::sharedTextureCache()->addImageAsync("Images/sprites.png",
                                                        this,
                                                        callfuncO_selector(LoadingLayer::spritesLoaded));

    // 주기적으로 상태체크
    this->schedule(schedule_selector(LoadingLayer::tick));
    
    return true;
}

void LoadingLayer::spritesLoaded(CCObject *tex1)
{
    CCTexture2D *tex = (CCTexture2D*)tex1;
    // 이 메서드가 불리우면 스프라이트의 로딩이 끝난 것이다.
    
    // 스프라이트 정보를 읽어 온다.
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("Images/sprites.plist");
    
    // 배치노드 생성
    batchNode = CCSpriteBatchNode::createWithTexture(tex);
    this->addChild(batchNode);
    
    
    // 메인 백그라운드 이미지 추가
    main_bkgrnd = CCSprite::createWithSpriteFrameName("Turret_main_bkgrnd.png");
    main_bkgrnd->setPosition( ccp(winSize.width/2, winSize.height/2) );
    batchNode->addChild(main_bkgrnd);
    
    // 타이틀 이미지 추가
    main_title = CCSprite::createWithSpriteFrameName("Turret_main_title.png");
    main_title->setPosition( ccp(winSize.width/2,
                                 winSize.height
                                 - main_title->getContentSize().height/2
                                 - MAIN_TITLE_TOP_MARGIN) );
    batchNode->addChild(main_title);

    
    // 탐 이미지 추가 : 눈뜬 그림
    tom = CCSprite::createWithSpriteFrameName("Tom.png");
    tom->setPosition( ccp(tom->getContentSize().width/2 + TOM_LEFT_MARGIN,
                          tom->getContentSize().height/2 + TOM_BOTTOM_MARGIN) );
    batchNode->addChild(tom);
    
    
    // 탐이 눈 깜박이게 애니메이션. 1.0초마다 0.1초 사이에 눈 깜박이기
    CCArray* blinkAnimFrames = CCArray::createWithCapacity(3);
    blinkAnimFrames->addObject( CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("Tom.png") );
    blinkAnimFrames->addObject( CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("Tom_blink.png") );
    blinkAnimFrames->addObject( CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("Tom.png") );

    CCAnimation *blinkAnimation = CCAnimation::createWithSpriteFrames(blinkAnimFrames, 0.1f);
    CCAnimate *blinkAnimate = CCAnimate::create(blinkAnimation);
    CCFiniteTimeAction* tomSeqAct = CCSequence::create(blinkAnimate,
                                                      CCDelayTime::create(1.0f),
                                                      NULL);
    CCAction* rep = CCRepeatForever::create((CCActionInterval*)tomSeqAct);
    tom->runAction(rep);

    
    // "Loading..." 글자 이미지 추가
    loading = CCSprite::createWithSpriteFrameName("Turret_loading.png");
    loading->setPosition( ccp(((winSize.width - tom->getContentSize().width) / 2) + tom->getContentSize().width,
                              loading->getContentSize().height/2 + LOADING_BOTTOM_MARGIN) );
    batchNode->addChild(loading);

    
    // "Loading..." 글자 이미지 깜박이게 애니메이션
    CCFiniteTimeAction* loadingSeqAct = CCSequence::create(CCFadeOut::create(1.0f),
                                                           CCFadeIn::create(1.0f),
                                                           NULL);
    CCAction* rep2 = CCRepeatForever::create((CCActionInterval*)loadingSeqAct);
    loading->runAction(rep2);

    
    // 시작할 때 로딩한 디폴트 이미지를 지운다. (메모리 절약)
    this->removeChild(defaultImage, true);
    
    
    // 이미지가 다 로드되었음을 표시
    imagesLoaded = true;
    
}

void LoadingLayer::tick(float dt)
{
    
    // 이미지가 다 로드되었고, 메인에서 백그라운드로 다른 레이어들 로드 준비시켰고,
    // 현재 이미지 로딩중이면
    
    if (imagesLoaded && isLoading) {
        
        // 더이상 로딩할 이미지가 없음 을 표시
        isLoading = false;
        
        // "로딩중 이미지"를 배치노드에서 삭제. (메모리 절약)
        batchNode->removeChild(loading, true);
        
        // "tap to continue" 스프라이트 추가
        tapToCont = CCSprite::createWithSpriteFrameName("Turret_main_taptocont.png");
        tapToCont->setPosition( ccp(((winSize.width - tom->getContentSize().width) / 2)
                                    + tom->getContentSize().width,
                                    tapToCont->getContentSize().height/2
                                    + TAPTOCONT_BOTTOM_MARGIN) );
        batchNode->addChild(tapToCont);

        
        // "tap to continue" 애니메이션 처리.
        // 로딩중 자리에서 대신 나오며 다음 신으로 갈 수 있도록 안내 표시.
        CCFiniteTimeAction* loadingSeqAct = CCSequence::create(CCFadeOut::create(1.0f),
                                                               CCFadeIn::create(1.0f),
                                                               NULL);
        CCAction* rep2 = CCRepeatForever::create((CCActionInterval*)loadingSeqAct);
        tapToCont->runAction(rep2);
    }
    
}

void LoadingLayer::ccTouchesEnded(CCSet *pTouches, CCEvent* event)
{
    if (!isLoading) {
        // 메인메뉴 신으로 이동
        CCScene* pScene = MainMenuLayer::scene();
        CCDirector::sharedDirector()->replaceScene( CCTransitionProgressRadialCW::create(0.5f, pScene) );
    }
}


