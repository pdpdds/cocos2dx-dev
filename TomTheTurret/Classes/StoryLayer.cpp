//
//  StoryLayer.cpp
//  TomTheTurret
//
//  Created by Jaewhan Lee on 13. 1. 21..
//
//

#include "StoryLayer.h"
#include "MainMenuLayer.h"
#include "GameState.h"
#include "ActionLayer.h"

static int LABEL_MARGIN            = 20;
static int LABEL_MARGIN_BOTTOM     = 50;
static int TAPTOCONT_BOTTOM_MARGIN = 30;
static int NEWGAME_BOTTOM_MARGIN   = 30;


CCScene* StoryLayer::scene()
{
    CCScene *scene = CCScene::create();
    
    StoryLayer *layer = StoryLayer::create();
    
    // 960 x 640
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    if (winSize.width >= 960) {
        layer->setScale(2.0);
        layer->setAnchorPoint(ccp(0.0, 0.0));
    }

    scene->addChild(layer);
    
    return scene;
}

bool StoryLayer::init()
{
    if ( !CCLayer::init() )
    {
        return false;
    }
    

    // 화면 사이즈 구하기
//    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    CCSize winSize = CCSizeMake(480, 320);

    // 터치 활성화
    this->setTouchEnabled(true);
    
    // 배치노드 로드
    // 배치노드 로드 : 이미 있어서 메모리에 있는 텍스쳐가 사용된다.
    batchNode = CCSpriteBatchNode::createWithTexture(
                                                     CCTextureCache::sharedTextureCache()->addImage("Images/sprites.png"));
    this->addChild(batchNode);
    
    // 백그라운드 이미지
    main_bkgrnd = CCSprite::createWithSpriteFrameName("Turret_main_bkgrnd.png");
    main_bkgrnd->setPosition( ccp(winSize.width/2, winSize.height/2) );
    batchNode->addChild(main_bkgrnd);

    // 내용 추가
    label = CCLabelTTF::create("",
                               "Arial",
                               24,
                               CCSizeMake(winSize.width - LABEL_MARGIN*2,
                                          winSize.height - LABEL_MARGIN - LABEL_MARGIN_BOTTOM),
                               kCCTextAlignmentCenter,
                               kCCVerticalTextAlignmentCenter);

    label->setPosition( ccp(winSize.width/2, winSize.height/2) );
    label->setColor( ccc3(0, 0, 0) );
    this->addChild(label);
    
    
    // 상황에 따라 아래 두 메뉴 중 하나를 보여 준다.
    
    // "tap to continue" 메뉴 추가
    tapToCont = CCSprite::createWithSpriteFrameName("Turret_main_taptocont.png");
    tapToCont->setPosition( ccp(winSize.width / 2,
                                tapToCont->getContentSize().height/2 + TAPTOCONT_BOTTOM_MARGIN) );
    tapToCont->setVisible(false);
    batchNode->addChild(tapToCont);
    
    // "new game" 메뉴 추가
    spriteNewGame = CCSprite::createWithSpriteFrameName("Turret_newgame.png");

    spriteNewGame->setPosition( ccp(winSize.width / 2,
                                    tapToCont->getContentSize().height/2 + NEWGAME_BOTTOM_MARGIN) );
    spriteNewGame->setVisible(false);
    batchNode->addChild(spriteNewGame);
    
    
    
    // Story 1
    story1 = CCArray::create();
    story1->retain();
    story1->addObject( CCString::createWithFormat("%s",
             "Tom is a turret - and a good one at that.  He has a 100% success rate at fulfilling his primary directive:\n\nProtect the supplies from intruders.") );
    story1->addObject( CCString::createWithFormat("%s",
             "Tom has to complete his directive, or he will self-destruct.\n\nBut for the first time, Tom wonders where this directive came from...") );
    
    
    // Story 2
    story2 = CCArray::create();
    story2->retain();
    story2->addObject( CCString::createWithFormat("%s",
             "Tom has noticed the intruders are larger than they used to be.\n\nThey continue to march on, despite the rain of bullets.") );
    story2->addObject( CCString::createWithFormat("%s",
             "Why is this?  Something does not compute.  But Tom must continue to fulfill his directive...\n\n...mustn't he?") );
    
    
    // Story 3
    story3 = CCArray::create();
    story3->retain();
    story3->addObject( CCString::createWithFormat("%s",
             "A strange intruder approaches the kill zone.\n\nHe tells Tom that there has been a terrible mistake, that he must let him pass, for the good of humanity.") );
    story3->addObject( CCString::createWithFormat("%s",
             "Tom doesn't know what to think.\n\nTruth, lies, knowledge, uncertainty, directive.") );
    
    
    // Story 4 - Kill ending
    story4 = CCArray::create();
    story4->retain();
    story4->addObject( CCString::createWithFormat("%s",
             "Tom is a turret - and a good one at that.\n\nNo one ever got by Tom, not ever again.") );
    story4->addObject( CCString::createWithFormat("%s",
             "In fact, that was the last intruder Tom ever saw.") );
    
    
    // Story 5 - Lose ending
    story5 = CCArray::create();
    story5->retain();
    story5->addObject( CCString::createWithFormat("%s",
             "The last thing Tom saw was an intruder running off into the distance, cackling with glee.\n") );
    story5->addObject( CCString::createWithFormat("%s",
             "Primary directive: FAILED.\n") );

    return true;
}

void StoryLayer::onEnter()
{
	CCLayer::onEnter();
    
    // 현재 레벨의 첫번째 스토리를 보여준다.
    curStoryIndex = 0;
    this->displayCurStoryString();
    
    
    // "tap to continue" 메뉴를 애니메이션 시킨다.
    CCFiniteTimeAction* seqAct = CCSequence::create(CCFadeOut::create(1.0f),
                                                    CCFadeIn::create(1.0f),
                                                    NULL);
    CCAction* rep = CCRepeatForever::create((CCActionInterval*)seqAct);
    tapToCont->runAction(rep);
}

void StoryLayer::displayCurStoryString()
{
    curLevelIndex = GameState::getInstance()->curLevelIndex;    
    
    int storyCount = 0;
    CCString* curStoryString = CCString::create("");
    
    switch (curLevelIndex) {
        case 0:
            storyCount     = story1->count();
            curStoryString = (CCString *)story1->objectAtIndex(curStoryIndex);
            break;
        case 1:
            storyCount     = story2->count();
            curStoryString = (CCString *)story2->objectAtIndex(curStoryIndex);
            break;
        case 2:
            storyCount     = story3->count();
            curStoryString = (CCString *)story3->objectAtIndex(curStoryIndex);
            break;
        case 3:
            storyCount     = story4->count();
            curStoryString = (CCString *)story4->objectAtIndex(curStoryIndex);
            break;
        case 4:
            storyCount     = story5->count();
            curStoryString = (CCString *)story5->objectAtIndex(curStoryIndex);
            break;
            
        default:
            break;
    }
    
    label->setString(curStoryString->getCString());
    
    if ( (curLevelIndex > 2) && (curStoryIndex == storyCount - 1) ) {
        spriteNewGame->setVisible(true);
        tapToCont->setVisible(false);
    } else {
        spriteNewGame->setVisible(false);
        tapToCont->setVisible(true);
    }
    
}

void StoryLayer::ccTouchesEnded(CCSet *pTouches, CCEvent* event)
{
    curStoryIndex++;
    
    int storyCount = 0;
    switch (curLevelIndex) {
        case 0:
            storyCount = story1->count();
            break;
        case 1:
            storyCount = story2->count();
            break;
        case 2:
            storyCount = story3->count();
            break;
        case 3:
            storyCount = story4->count();
            break;
        case 4:
            storyCount = story5->count();
            break;
            
        default:
            break;
    }
    
    if (curStoryIndex < storyCount) {
        this->displayCurStoryString();
    } else {
        if (curLevelIndex > 2) {

            // 메인메뉴 신으로 이동
            CCScene* pScene = MainMenuLayer::scene();
            CCDirector::sharedDirector()->replaceScene( CCTransitionProgressRadialCW::create(0.5f, pScene) );
        } else {
            // 새로운게임 ... 신으로 이동
            CCScene* pScene = ActionLayer::scene();
            CCDirector::sharedDirector()->replaceScene( CCTransitionProgressRadialCW::create(0.5f, pScene) );
        }
    }
}







