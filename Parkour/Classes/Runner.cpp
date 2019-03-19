//
//  Runner.cpp
//  PaoKu
//
//  Created by YTB on 13-11-21.
//
//

#include "Runner.h"
#include "MapManager.h"
#include "Constant.h"
#include "SimpleAudioEngine.h"

#define RUNING_MODE "running"

Runner *Runner::create(cpSpace *space)
{
    Runner *runner = new Runner(space);
    runner->autorelease();
    return runner;
}

Runner::Runner(cpSpace *space) : m_offsetPx(100.0)
{
    this->space = space;
    this->shape = NULL;
    
    this->initWithSpriteFrameName("runner0.png");
    this->runningSize = this->getContentSize();
    
    // init crouchSize
    cocos2d::extension::CCPhysicsSprite *tmpSprite = cocos2d::extension::CCPhysicsSprite::createWithSpriteFrameName("runnerCrouch0.png");
    this->crouchSize = tmpSprite->getContentSize();
    
    initAction();
    initBody();
    initShape(RUNING_MODE);// start with running shape
    this->setCPBody(this->body);
    
    m_stat = RunnerStatRunning;
    this->runAction(this->runningAction);
}

Runner::~Runner()
{
    this->runningAction->release();
    this->jumpUpAction->release();
    this->jumpDownAction->release();
    this->crouchAction->release();
    cpBodyFree(this->body);
    cpShapeFree(this->shape);
}

void Runner::initAction()
{
    // init runningAction
    CCArray *animFrames = CCArray::create();
    for (int i = 0; i < 8; i++)
    {
        CCString *name = CCString::createWithFormat("runner%d.png",i);
        CCSpriteFrame *frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(name->getCString());
        animFrames->addObject(frame);
    }

    CCAnimation *animation = CCAnimation::createWithSpriteFrames(animFrames, 0.1);
    this->runningAction =CCRepeatForever::create(CCAnimate::create(animation));
    this->runningAction->retain();

    // init jumpUpAction
    animFrames = CCArray::create();
    for (int i=0; i<4; i++) {
        CCString *name = CCString::createWithFormat("runnerJumpUp%d.png",i);
        CCSpriteFrame *frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(name->getCString());
        animFrames->addObject(frame);
    }

    animation = CCAnimation::createWithSpriteFrames(animFrames, 0.2);
    this->jumpUpAction = CCAnimate::create(animation);
    this->jumpUpAction->retain();

    // init jumpDownAction
    animFrames->removeAllObjects();
    for (int i=0; i<2; i++) {
        CCString *name = CCString::createWithFormat("runnerJumpDown%d.png",i);
        CCSpriteFrame *frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(name->getCString());
        animFrames->addObject(frame);
    }
    animation = CCAnimation::createWithSpriteFrames(animFrames, 0.3);
    this->jumpDownAction = CCAnimate::create(animation);
    this->jumpDownAction->retain();

    // init crouchAction
    animFrames->removeAllObjects();
    for (int i=0; i<1; i++) {
        CCString *name = CCString::createWithFormat("runnerCrouch%d.png",i);
        CCSpriteFrame *frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(name->getCString());
        animFrames->addObject(frame);
    }
    animation = CCAnimation::createWithSpriteFrames(animFrames, 0.3);
    this->crouchAction = CCAnimate::create(animation);
    this->crouchAction->retain();
}

void Runner::initBody()
{
    // create chipmunk body
    this->body = cpBodyNew(1, cpMomentForBox(1,this->runningSize.width, this->runningSize.height));
    this->body->p = cpv(m_offsetPx, MapManager::getGroundHeight() + this->runningSize.height / 2);
    this->body->v = cpv(150, 0);//run speed
    cpSpaceAddBody(this->space, this->body);
}

void Runner::levelUp()
{
    // run faster
    cpBodyApplyImpulse(this->body, cpv(10, 0), cpv(0, 0));
}

void Runner::initShape(const char* type)
{
    if (this->shape) {
        cpSpaceRemoveShape(this->space, this->shape);
    }
    
    if (0 == strcmp(type, RUNING_MODE)) {
        this->shape = cpBoxShapeNew(this->body, this->runningSize.width-14, this->runningSize.height);
    } else {
        // crouch
        this->shape = cpBoxShapeNew(this->body, this->crouchSize.width, this->crouchSize.height);
    }
    cpSpaceAddShape(this->space, this->shape);
    cpShapeSetCollisionType(this->shape, SpriteTagrunner);
}

void Runner::jump()
{
    if (m_stat == RunnerStatRunning) {
        m_stat = RunnerStatJumpUp;
        cpBodyApplyImpulse(this->body, cpv(0, 250), cpv(0, 0));
        this->stopAllActions();
        this->runAction(this->jumpUpAction);
        
        CocosDenshion::SimpleAudioEngine *audioEngine = CocosDenshion::SimpleAudioEngine::sharedEngine();
        audioEngine->playEffect("jump.mp3");
    }
}

void Runner::crouch()
{
    m_stat = RunnerStatCrouch;
    this->initShape("crouch");
    this->stopAllActions();
    this->runAction(this->crouchAction);
    scheduleOnce(schedule_selector(Runner::loadNormal), 1.5);
    CocosDenshion::SimpleAudioEngine *audioEngine = CocosDenshion::SimpleAudioEngine::sharedEngine();
    audioEngine->playEffect("crouch.mp3");
}

void Runner::loadNormal(float dt)
{
	initShape(RUNING_MODE);
	this->stopAllActions();
	this->runAction(runningAction);
	m_stat = RunnerStatRunning;
}

void Runner::step(float dt)
{
    cpVect vel = cpBodyGetVel(this->body);
    if (m_stat == RunnerStatJumpUp) {
        if (vel.y < 0.1) {
            m_stat = RunnerStatJumpDown;
            this->stopAllActions();
            this->runAction(this->jumpDownAction);
        }
        return;
    }
    if (m_stat == RunnerStatJumpDown) {
        if (vel.y == 0) {
            m_stat = RunnerStatRunning;
            this->stopAllActions();
            this->runAction(this->runningAction);
        }
        return;
    }
    
}