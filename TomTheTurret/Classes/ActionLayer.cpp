//
//  ActionLayer.cpp
//  TomTheTurret
//
//  Created by Jaewhan Lee on 13. 1. 21..
//
//

#include "ActionLayer.h"
#include "SimpleAudioEngine.h"
#include "GameState.h"
#include "StoryLayer.h"
#include "GameLevel.h"
#include "Monster.h"

using namespace CocosDenshion;

CCScene* ActionLayer::scene()
{
    CCScene *scene = CCScene::create();
    
    ActionLayer *layer = ActionLayer::create();
    
    // 960 x 640
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    if (winSize.width >= 960) {
        layer->setScale(2.0);
        layer->setAnchorPoint(ccp(0.0, 0.0));
    }

    scene->addChild(layer);
    
    return scene;
}

bool ActionLayer::init()
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
    

    // 현재 게임 레벨 구해오기
    curLevelIndex = GameState::getInstance()->curLevelIndex;

    
    
    // 배치노드 로드 : 이미 있어서 메모리에 있는 텍스쳐를 사용한다.
    batchNode = CCSpriteBatchNode::createWithTexture(
                                                     CCTextureCache::sharedTextureCache()->addImage("Images/sprites.png"));
    this->addChild(batchNode);

    // 백그라운드 이미지
    level_bkgrnd = CCSprite::createWithSpriteFrameName("Level_bkgrnd.png");
    level_bkgrnd->setPosition( ccp(winSize.width/2, winSize.height/2) );
    batchNode->addChild(level_bkgrnd);

    
    // 터렛 탐의 이미지
    player = CCSprite::createWithSpriteFrameName("Level_Tom.png");
    player->setPosition( ccp(player->getContentSize().width/2 + 80,
                             winSize.height/2) );
    batchNode->addChild(player);

    // 게임에 사용될 소리들을 미리 로딩해 놓는다.
//    SimpleAudioEngine::sharedEngine()->preloadBackgroundMusic("Images/background.caf");
    SimpleAudioEngine::sharedEngine()->preloadEffect("Images/femaleHit.wav");
    SimpleAudioEngine::sharedEngine()->preloadEffect("Images/maleHit.wav");
    SimpleAudioEngine::sharedEngine()->preloadEffect("Images/shoot.wav");
    

    return true;
}

void ActionLayer::onExit()
{
    SimpleAudioEngine::sharedEngine()->unloadEffect("Images/femaleHit.wav");
    SimpleAudioEngine::sharedEngine()->unloadEffect("Images/maleHit.wav");
    SimpleAudioEngine::sharedEngine()->unloadEffect("Images/shoot.wav");
//    SimpleAudioEngine::sharedEngine()->stopBackgroundMusic(true);

    monsters->release();
    projectiles->release();

    CCLayer::onExit();
}

void ActionLayer::onEnter()
{
    CCLayer::onEnter();
    
    // 총알, 몬스터 배열 초기화
    monsters = CCArray::create();
    monsters->retain();
    projectiles = CCArray::create();
    projectiles->retain();
    
    // 상태 변수들 초기화
    monsterHit = false;
    nextProjectile = NULL;
    levelBegin = 0;
    lastTimeMonsterAdded = 0;
    inLevel = true;
    
    // 터렛 탐의 눈을 깜박이게 한다.
    CCArray* blinkAnimFrames = CCArray::createWithCapacity(3);
    blinkAnimFrames->addObject( CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("Level_Tom.png") );
    blinkAnimFrames->addObject( CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("Level_Tom_blink.png") );
    blinkAnimFrames->addObject( CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("Level_Tom.png") );
    
    CCAnimation *blinkAnimation = CCAnimation::createWithSpriteFrames(blinkAnimFrames, 0.1f);
    CCAnimate *blinkAnimate = CCAnimate::create(blinkAnimation);
    CCFiniteTimeAction* tomSeqAct = CCSequence::create(blinkAnimate,
                                                       CCDelayTime::create(1.0f),
                                                       NULL);
    CCAction* rep = CCRepeatForever::create((CCActionInterval*)tomSeqAct);
    player->runAction(rep);

    // 주기적으로 상태체크
    this->schedule(schedule_selector(ActionLayer::update), 0.02f);
    this->schedule(schedule_selector(ActionLayer::gameLogic), 0.1f);

}

void ActionLayer::update(float dt)
{
    if (!inLevel) return;

    CCArray* projectilesToDelete = CCArray::create();
    projectilesToDelete->retain();

    CCObject* pObjA = NULL;
    CCARRAY_FOREACH(projectiles, pObjA)
    {
        CCSprite *projectile = (CCSprite*)pObjA;
        
        CCRect projectileRect = projectile->boundingBox();
        

        Monster *monster = NULL;
        CCObject* pObjB = NULL;
        CCARRAY_FOREACH(monsters, pObjB)
        {
//            CCSprite *curMonster = (CCSprite*)pObjB;
            Monster *curMonster = (Monster*)pObjB;
            CCRect monsterRect = curMonster->boundingBox();
            
            if ( projectileRect.intersectsRect(monsterRect) ) {
//                monster = (Monster *)curMonster;
                monster = curMonster;
                break;
            }
        }
        
        if (monster != NULL) {
            
            // Subtract HP
            monsterHit = true;
            monster->hp--;
            
            // Play the hit sound effect
            if (monster->hitEffectSoundId == 2001) {
                SimpleAudioEngine::sharedEngine()->playEffect("Images/femaleHit.wav");
            } else {
                SimpleAudioEngine::sharedEngine()->playEffect("Images/maleHit.wav");
            }
            
            // 몬스터가 죽었으면 제거한다.
            if (monster->hp <= 0) {
                // 배열에서 제거
                monsters->removeObject(monster);
                // 배치노드(화면)에서 제거
                batchNode->removeChild(monster, true);
            }
            
            // Add the projectile to the list to delete
            // 제거할 총알 배열에 저장
            projectilesToDelete->addObject(projectile);
        }
    }
    
    // 총알 제거
    CCObject* pObjC = NULL;
    CCARRAY_FOREACH(projectilesToDelete, pObjC)
    {
        CCSprite *projectile = (CCSprite*)pObjC;

        projectiles->removeObject(projectile);
        batchNode->removeChild(projectile, true);
    }

    projectilesToDelete->release();
}

void ActionLayer::gameLogic(float dt)
{
    if (!inLevel) return;
    
    GameLevel *curLevel = GameState::getInstance()->curLevel;
    
    timeval nowTime;
    gettimeofday(&nowTime,  0);
    double now = nowTime.tv_sec;

    // Check to see if level is over
    
    if (levelBegin == 0) {
        //Start background music
//        SimpleAudioEngine::sharedEngine()->playBackgroundMusic("Images/background.caf", true);

        levelBegin = now;
        return;
    } else {

        // 몬스터 생성 시간 체크 : 더이상 몬스터를 만들지 않는다.
        if (now - levelBegin >= curLevel->spawnSeconds) {
            
            if (monsters->count() == 0) {
                // 몬스터를 다 잡았다.
                inLevel = false;
                this->unschedule(schedule_selector(ActionLayer::update));
                this->unschedule(schedule_selector(ActionLayer::gameLogic));
                
                // 이겼다.
                this->clearData(1);
                
            } else {
                // 몬스터가 벽에 닿으면 그 때 진 것으로 처리한다.
            }
            return;

        }
    }
    
    // 몬스터를 만든다.
    if(lastTimeMonsterAdded == 0 || now - lastTimeMonsterAdded >= curLevel->spawnRate) {
        this->addMonsters();
        lastTimeMonsterAdded = now;
    }
    
}

void ActionLayer::addMonsters()
{
    GameLevel *curLevel = GameState::getInstance()->curLevel;

    CCObject* pObj = NULL;
    CCARRAY_FOREACH(curLevel->spawnIds, pObj)
    {
        int monsterId = ((CCString *)pObj)->intValue();
        
        Monster *monster = Monster::monsterWithType(monsterId);
        
        if (monster != NULL) {
            this->addMonster(monster);
        }
    }
}

void ActionLayer::addMonster(Monster* monster)
{
    // 몬스터가 생성될 때 어디서 생성될 지 Y축의 값을 결정한다.
    srand((int)time(NULL));
    
//    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    CCSize winSize = CCSizeMake(480, 320);
    int minY = monster->getContentSize().height/2;
    int maxY = winSize.height - monster->getContentSize().height/2;
    int rangeY = maxY - minY;
    int actualY = (rand() % rangeY) + minY;
    
    monster->setPosition( ccp(winSize.width + (monster->getContentSize().width/2), actualY) );
    monster->setTag(1);
    
    // 배치노드에 추가
    batchNode->addChild(monster);
    // monsters 배열에 추가
    monsters->addObject(monster);
    
    // 몬스터가 이동할 시간을 랜덤하게 구한다.
    int minDuration = monster->minMoveDuration; //2.0;
    int maxDuration = monster->maxMoveDuration; //4.0;
    int rangeDuration = maxDuration - minDuration;
    int actualDuration = (rand() % rangeDuration) + minDuration;
    
    // 반복이동 액션을 만든다.
    static int X_OFFSET = 40;
    CCActionInterval* actionMove = CCMoveTo::create(actualDuration,
                                                    ccp(X_OFFSET+monster->getContentSize().width/2, actualY));
    CCActionInterval* actionPause = CCDelayTime::create(0.5f);
    CCActionInterval* actionMoveBack = CCMoveTo::create(actualDuration,
                                                        monster->getPosition());
//    CCFiniteTimeAction* actionMoveDone = CCCallFuncN::create(this, callfuncN_selector(ActionLayer::spriteMoveFinished));
    CCFiniteTimeAction* actionMoveDone = CCCallFuncO::create(this, callfuncO_selector(ActionLayer::spriteMoveFinished), monster);
    
    CCFiniteTimeAction* seqAct = CCSequence::create(actionMove,
                                                    actionPause,
                                                    actionMoveBack,
                                                    actionMoveDone,
                                                    NULL);
    // 반복 이동 액션 실행
    monster->runAction(seqAct);
}

void ActionLayer::spriteMoveFinished(CCObject* sender)
{
    if (!inLevel) return;
    
    CCSprite *sprite = (CCSprite *)sender;
    batchNode->removeChild(sprite, true);
    
    if (sprite->getTag() == 1) { // monster
        monsters->removeObject(sprite);
        
        // 졌다. 몬스터가 출발지로 다시 도착.
        CCLog("몬스터 도착");

        inLevel = false;
        this->unschedule(schedule_selector(ActionLayer::update));
        this->unschedule(schedule_selector(ActionLayer::gameLogic));
        this->clearData(2);

    } else if (sprite->getTag() == 2) { // projectile
        projectiles->removeObject(sprite);
    }
    
}

void ActionLayer::clearData(int victory)
{
    // victory 1: Win 2: Lose
    
    projectiles->removeAllObjects();
    monsters->removeAllObjects();
    batchNode->removeAllChildrenWithCleanup(true);
    
    if (victory == 2) {
        // 졌다.
        GameState::getInstance()->loseGame();

        CCScene* pScene = StoryLayer::scene();
        CCDirector::sharedDirector()->replaceScene( CCTransitionProgressRadialCW::create(0.5f, pScene) );

        return;

    } else {
    
        // 이겼다.
        if (curLevelIndex == 2) {
            
            GameState::getInstance()->winGame();
            
            CCScene* pScene = StoryLayer::scene();
            CCDirector::sharedDirector()->replaceScene( CCTransitionProgressRadialCW::create(0.5f, pScene) );
            
        } else {
            GameState::getInstance()->nextLevel();
            
            CCScene* pScene = StoryLayer::scene();
            CCDirector::sharedDirector()->replaceScene( CCTransitionProgressRadialCW::create(0.5f, pScene) );
        }
    }
}

void ActionLayer::ccTouchesEnded(CCSet *pTouches, CCEvent* event)
{
    if (nextProjectile != NULL) return;
    
    // Get current touch location
    CCSetIterator it = pTouches->begin();
    CCTouch* touch = (CCTouch*)(*it);
    CCPoint location = touch->getLocation();

    
    
    // Play a sound!
    SimpleAudioEngine::sharedEngine()->playEffect("Images/shoot.wav");
    
    // Set up initial location of projectile
    nextProjectile = CCSprite::createWithSpriteFrameName("Level_bullet.png");

    // Determine the angle to rotate
    CCPoint shootVector = ccpSub(location, player->getPosition());
    float shootAngle = ccpToAngle(shootVector);
    float cocosAngle = CC_RADIANS_TO_DEGREES(-1 * shootAngle);
    
    // Determine how long it should take to rotate to that angle
    float curAngle = player->getRotation();
    float rotateDiff = cocosAngle - curAngle;
    if (rotateDiff > 180)
        rotateDiff -= 360;
    if (rotateDiff < -180)
        rotateDiff += 360;
    float rotateSpeed = 0.5f / 180; // Would take 0.5 seconds to rotate half a circle
    float rotateDuration = fabsf(rotateDiff * rotateSpeed);
    
    // Actually set up the actions
    CCActionInterval* actRotate1 = CCRotateTo::create(rotateDuration, cocosAngle);
//    CCFiniteTimeAction* actDone1 = CCCallFuncN::create(this, callfuncN_selector(ActionLayer::finishShoot));
    CCFiniteTimeAction* actDone1 = CCCallFuncO::create(this, callfuncO_selector(ActionLayer::finishShoot), nextProjectile);
    
    CCFiniteTimeAction* seqAct1 = CCSequence::create(actRotate1,
                                                     actDone1,
                                                     NULL);
    // 액션 실행
    player->runAction(seqAct1);
    
    // Make sure projectile is rotated at same angle
    nextProjectile->setRotation(cocosAngle);
    
    // Move projectile offscreen
    float delta = 1.0f;
    CCPoint normalizedShootVector = ccpNormalize(shootVector);
    CCPoint overshotVector = ccpMult(normalizedShootVector, 420);
    
    
    CCActionInterval* actMove2 = CCMoveBy::create(delta, overshotVector);
//    CCFiniteTimeAction* actDone2 = CCCallFuncN::create(this, callfuncN_selector(ActionLayer::spriteMoveFinished));
    CCFiniteTimeAction* actDone2 = CCCallFuncO::create(this, callfuncO_selector(ActionLayer::spriteMoveFinished), nextProjectile);
    
    CCFiniteTimeAction* seqAct2 = CCSequence::create(actMove2,
                                                     actDone2,
                                                     NULL);
    nextProjectile->runAction(seqAct2);
    
    // Add to projectiles array
    nextProjectile->setTag(2);

}

void ActionLayer::finishShoot(CCObject* pSender)
{
    // Ok to add now - we've finished rotation!
    CCPoint pPoint = player->convertToWorldSpace( ccp(player->getContentSize().width,
                                                      player->getContentSize().height/2) );
    nextProjectile->setPosition( pPoint );
    
    batchNode->addChild(nextProjectile);
    projectiles->addObject(nextProjectile);
    
    // Release
    nextProjectile = NULL;
    
}












