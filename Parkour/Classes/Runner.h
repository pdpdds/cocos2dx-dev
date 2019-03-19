//
//  Runner.h
//  PaoKu
//
//  Created by YTB on 13-11-21.
//
//

#ifndef __PaoKu__Runner__
#define __PaoKu__Runner__

#include "cocos2d.h"
#include "chipmunk.h"
#include "cocos-ext.h"

USING_NS_CC;

enum RunnerStat
{
    RunnerStatRunning = 0,
    RunnerStatJumpUp = 1,
    RunnerStatJumpDown = 2,
    RunnerStatCrouch = 3,
    RunnerStatIncredible = 4
};

class Runner : public extension::CCPhysicsSprite
{
private:
    CCSize  runningSize;
    CCSize crouchSize;

    cpSpace *space;//current space;
    cpBody *body;// runner chipmunk body
    cpShape *shape;// runner chipmunk shape
    RunnerStat m_stat;// init with running status
    CCAction *runningAction;
    CCAction *jumpUpAction;
    CCAction *jumpDownAction;
    CCAction *crouchAction;

    CCParticleFlower *_emitter;

    float m_offsetPx;

public:
    static Runner *create(cpSpace *space);
    Runner(cpSpace *space);
    ~Runner();

    float getoffsetPx() {return m_offsetPx;}
    
    void initAction();
    void initBody();
    void levelUp();
    void initShape(const char* type);
    void jump();
    void crouch();
    void step(float dt);
    void loadNormal(float dt);

	static const CCSize *getCrouchContentSize()
	{
        CCPhysicsSprite *sprite = CCPhysicsSprite::createWithSpriteFrameName("runnerCrouch0.png");
        return &sprite->getContentSize();
	}
};

#endif /* defined(__PaoKu__Runner__) */
