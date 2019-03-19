//
//  PlayFieldLayer.cpp
//  moles
//
//  Created by breaklee on 6/9/13.
//
//

#include "PlayFieldLayer.h"
#include "Mole.h"
#include "MoleHill.h"
#include "SimpleAudioEngine.h"
#include "MenuLayer.h"

CCScene* PlayFieldLayer::scene() {

    CCScene* scene = CCScene::create();
    PlayFieldLayer* layer = PlayFieldLayer::create();
    scene->addChild(layer);
    return scene;
    
}

bool PlayFieldLayer::init() {
    
    if(CCLayer::init()== false) {
        return false;
    }
    
    this->setTouchMode(kCCTouchesOneByOne);
    this->setTouchEnabled(true);
    
    this->winSize = CCDirector::sharedDirector()->getWinSize();
   
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("molesheet.plist");

    this->molesheet = CCSpriteBatchNode::create("molesheet.png");
    this->addChild(this->molesheet);
    
    this->backButton = CCSprite::createWithSpriteFrameName("backbutton.png");
    this->backButton->setAnchorPoint(ccp(0,1));
    this->backButton->setScale(0.7);
    this->backButton->setPosition(ccp(10, winSize.height-10));
    this->molesheet->addChild(this->backButton, 100);
    
    this->moleHillsInPlay = CCArray::create();
    this->moleHillsInPlay->retain();

    maxHillRows = 4;
    maxHillColumns = 3;
    maxHills = maxHillRows * maxHillColumns;
    maxMoles = 3;
    spawnRest = 0;
    isGameOver = false;
    startingTimerValue = 60;
    currentTimerValue = startingTimerValue;
    playerScore = 0;

    this->buildAnimations();
    
    this->preloadEffects();
    
    this->drawGround();
    
    this->drawHills();
    
    this->generateTimerDisplay();
    
    this->generateScoreDisplay();
    
    float tempTimeVal = 1.5 - (20 * 0.025);
    
    if (tempTimeVal < 0.33f) { tempTimeVal = 0.33f; }
    
    this->moleRaiseTime = tempTimeVal;
    this->moleDownTime = tempTimeVal - 0.25;
    this->moleDownTime = tempTimeVal;

    return true;
    
}

void PlayFieldLayer::onEnterTransitionDidFinish() {

    this->scheduleUpdate();
    
}

void PlayFieldLayer::preloadEffects() {
    
    CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect(SND_BUTTON);
    CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect(SND_MOLE_NORMAL);
    CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect(SND_MOLE_SPECIAL);
    
}

void PlayFieldLayer::update(float dt) {
    
    CCObject* obj;
    CCARRAY_FOREACH(moleHillsInPlay, obj) {
        MoleHill *aHill = (MoleHill*) obj;
        
		if (aHill->hillMole->moleState == kMoleHit) {
            aHill->hillMole->moleState = kMoleMoving;
            this->scoreMole(aHill->hillMole);
		}
	}
    
    if (molesInPlay < maxMoles && spawnRest > 10) {
       this->spawnMole(this);
        spawnRest = 0;
    } else {
        spawnRest++;
    }
    
    currentTimerValue = currentTimerValue - dt;
    
    if (currentTimerValue > startingTimerValue) {
        currentTimerValue = startingTimerValue;
    }
    
    timerDisplay->setPercentage((currentTimerValue /
                                startingTimerValue) * 100);
    
    if (currentTimerValue <= 3.0) {
        skybox->setColor(ccRED);
    }
    
    if (currentTimerValue <= 0) {
        this->unscheduleUpdate();
        
        CCObject* obj = NULL;
        CCARRAY_FOREACH(moleHillsInPlay, obj) {
            MoleHill* moleHill = (MoleHill*)obj;
            this->resetMole(moleHill);
        }
        isGameOver = true;
        this->gameOver();
    }
}

void PlayFieldLayer::drawGround() {

    CCString* groundName(NULL);
    int pick = CCRANDOM_0_1() * 2;
    switch(pick) {
        case 1 :
            groundName = CCString::create("ground1.png");
            break;
        default :
            groundName = CCString::create("ground2.png");
            break;
    }
    
    for(int i = 0; i < 5; ++i) {
        CCSprite* strip = CCSprite::createWithSpriteFrameName(groundName->getCString());
        strip->setAnchorPoint(ccp(0.5, 0));
        strip->setPosition(ccp(winSize.width/2, i*82));
        molesheet->addChild(strip, 4 + ((5-i) * 5));
    }
    
    skybox = CCSprite::createWithSpriteFrameName("skybox1.png");
    skybox->setPosition(ccp(winSize.width/2,5*82 ));
    skybox->setAnchorPoint(ccp(0.5, 0));
    molesheet->addChild(skybox, 1);
    
}

void PlayFieldLayer::drawHills() {

    int hillCounter = 0;
    int newHillZ = 6;
    
    for(int row = 1; row <= 4; ++row) {
        newHillZ--;
        for( int col = 1; col <= 3; ++col) {
            hillCounter++;
            
            MoleHill* hill = MoleHill::create();
            hill->setPosition(this->hillPosition(row, col));
            hill->moleHillBaseZ = newHillZ;
            hill->moleHillTop = CCSprite::createWithSpriteFrameName("pileTop.png");
            hill->moleHillBottom = CCSprite::createWithSpriteFrameName("pileBottom.png");
            hill->moleHillID = hillCounter;
            
            
            CCPoint pt = hill->getPosition();
            hill->moleHillTop->setPosition(ccp(pt.x,
                                               pt.y +
                                               hill->moleHillTop->getContentSize().height/2 ));

            hill->moleHillBottom->setPosition(ccp(pt.x,
                                               pt.y +
                                               hill->moleHillTop->getContentSize().height/2 ));

            molesheet->addChild(hill->moleHillTop, (2 + (newHillZ * 5)));
            molesheet->addChild(hill->moleHillBottom, (5 + (newHillZ * 5)));

            Mole* mole = Mole::create();
            mole->retain();
            hill->hillMole = mole;
            mole->parentHill = hill;
            hill->retain();
            
            moleHillsInPlay->addObject(hill);
            
            this->resetMole(hill);
            
        }
    }
}

void PlayFieldLayer::buildAnimations() {
    
    CCArray* frameArray = CCArray::create();
    frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("penguin_forward.png"));
    frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("penguin_left.png"));
    frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("penguin_forward.png"));
    frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("penguin_right.png"));
    frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("penguin_forward.png"));
    frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("penguin_forward.png"));
    
    CCAnimation *newAnim = CCAnimation::createWithSpriteFrames(frameArray);
    newAnim->setDelayPerUnit(0.4);
    CCAnimationCache::sharedAnimationCache()->addAnimation(newAnim, "penguinAnim");
    
}


void PlayFieldLayer::resetMole(MoleHill* hill) {
    
    hill->isOccupied = false;

    if ( hill->hillMole == NULL ) return;
    hill->hillMole->stopAllActions();
    
    hill->hillMole->moleGroundY = 0.0f;
    hill->hillMole->moleState = kMoleDead;
    hill->hillMole->isSpecial = false;
    
    if( hill->hillMole->moleSprite != NULL) {
        hill->hillMole->moleSprite->removeFromParentAndCleanup(false);
    }
}

void PlayFieldLayer::spawnMole(PlayFieldLayer* layer) {
    
    int newMoleHill;
    bool isApprovedHole = false;
    int randI;
    
    if(molesInPlay == moleHillsInPlay->count() ||
       molesInPlay == maxMoles) {
        
    }
    else {
        do {
            randI = CCRANDOM_0_1() * maxHills;
            if(randI > maxHills) { randI = maxHills; }
            
            MoleHill* testHill = (MoleHill*)moleHillsInPlay->objectAtIndex(randI);
            
            if(testHill->isOccupied == false){
                newMoleHill = randI;
                isApprovedHole = true;
                testHill->isOccupied = true;
            }
        }while(isApprovedHole == false);
        
        molesInPlay++;
        
        MoleHill *thisHill = (MoleHill*)moleHillsInPlay->objectAtIndex(newMoleHill);
        
        int hillZ = thisHill->moleHillBaseZ;
        
        CCSprite *newMoleSprite = CCSprite::createWithSpriteFrameName("penguin_forward.png");
        
        thisHill->hillMole->moleSprite = newMoleSprite;

        thisHill->hillMole->moleState = kMoleAlive;
        
        thisHill->hillMole->moleGroundY = thisHill->getPosition().y;
        
        float newMolePosX = thisHill->getPosition().x;
        float newMolePosY = thisHill->getPosition().y -
        (newMoleSprite->getContentSize().height/2);
        
        newMoleSprite->setPosition(ccp(newMolePosX,
                                       newMolePosY));
        
        
        int moleRandomizer = CCRANDOM_0_1() * 100;
        
        if (moleRandomizer < 5) {
            thisHill->hillMole->isSpecial = true;
        }
        
        molesheet->addChild(newMoleSprite, (3 + ( hillZ * 5 )));
        this->raiseMole(thisHill);
        
    }
}

void PlayFieldLayer::raiseMole(MoleHill* aHill) {
    
    CCSprite* aMole = aHill->hillMole->moleSprite;
    float moleHeight = aMole->getContentSize().height;
    
    CCMoveBy* wobbleHillLeft = CCMoveBy::create(0.1, ccp(-3,0));
    CCMoveBy* wobbleHillRight = CCMoveBy::create(0.1, ccp(0, 0));
    
    aHill->moleHillBottom->runAction(CCSequence::create(wobbleHillLeft, wobbleHillRight, wobbleHillLeft, wobbleHillRight, NULL));
    
    CCMoveBy* moveUp = CCMoveBy::create(moleRaiseTime, ccp(0, moleHeight*0.8));
    CCMoveBy* moveUpHalf = CCMoveBy::create(moleRaiseTime, ccp(0, moleHeight*0.4));
    CCDelayTime* moleDelay = CCDelayTime::create(moleDelayTime);
    CCMoveBy* moveDown = CCMoveBy::create(moleDownTime, ccp(0, -moleHeight*0.8));
    CCCallFuncND* delMole = CCCallFuncND::create(this, (SEL_CallFuncND)&PlayFieldLayer::deleteMole, (void*)aHill);

    CCLOG("aHill address : %x", aHill);
    
    CCAnimate* anim = CCAnimate::create(CCAnimationCache::sharedAnimationCache()->animationByName("penguinAnim"));
    
    
    CCRotateBy *rot1 = CCRotateBy::create(moleDelayTime/3, -20);
    CCRotateBy *rot2 = CCRotateBy::create(moleDelayTime/3, 40);
    CCRotateBy *rot3 = CCRotateBy::create(moleDelayTime/3, -20);
    
    
    int behaviorPick = CCRANDOM_0_1() * 6;
    
    if(aHill->hillMole->isSpecial) {
        
        CCTintTo *tintR = CCTintTo::create(0.2, 255.0, 0.2, 0.2);
        CCTintTo *tintB = CCTintTo::create(0.2, 0.2, 0.2, 255.0);
        CCTintTo *tintG = CCTintTo::create(0.2, 0.2, 255.0, 0.2);
        
        aMole->runAction(CCRepeatForever::create(CCSequence::create(tintR, tintB, tintG, NULL)));
        aMole->runAction(CCSequence::create(moveUp, rot1, rot2, rot3, rot1, rot2, rot3, moveDown, delMole, NULL));

    }
    else {
        switch (behaviorPick) {
            case 1:
                aMole->runAction(CCSequence::create(moveUp, rot1, rot2, rot3, moveDown, delMole, NULL));
                break;
            case 2:
                aMole->runAction(CCSequence::create(moveUp, moveDown, delMole, NULL));
                break;
            case 3:
                aMole->runAction(CCSequence::create(moveUpHalf, moleDelay, moveDown, delMole, NULL));
                break;
            case 4:
                aMole->runAction(CCSequence::create(moveUpHalf, moveDown, delMole, NULL));
                break;
            case 5:
                 aMole->runAction(CCSequence::create(moveUpHalf, anim, moveDown, delMole, NULL));
                break;
            default:
                 aMole->runAction(anim);
                aMole->runAction(CCSequence::create(moveUp, moleDelay, moveDown, delMole, NULL));
                break;
        }
        
    }
}

void PlayFieldLayer::deleteMole(CCNode* obj, void* hill) {

    molesInPlay--;
    this->resetMole((MoleHill*)hill);

}

void PlayFieldLayer::scoreMole(Mole* mole) {
    
    if( mole->moleState == kMoleDead) {
        return;
    }
    
    MoleHill* hill = mole->parentHill;
    
    if(mole->isSpecial) {
        playerScore = playerScore + 5;
        this->addTimeToTimer(5);
    }
    else {
        playerScore++;
    }
    
    this->updateScore();
    
    CCMoveTo* moveMole = CCMoveTo::create(0.2f, this->scorePosition());
    CCScaleTo* shrinkMole = CCScaleTo::create(0.2f, 0.5f);
    CCSpawn* shirinkAndScore = CCSpawn::create(moveMole, shrinkMole, NULL);
    CCCallFuncND* delMole = CCCallFuncND::create(this, callfuncND_selector(PlayFieldLayer::deleteMole), hill);
    
    hill->hillMole->moleSprite->stopAllActions();
    hill->hillMole->moleSprite->runAction(CCSequence::create(shirinkAndScore, delMole, NULL));
    
}

void PlayFieldLayer::generateScoreDisplay() {
    
    CCLabelTTF *scoreTitleLbl = CCLabelTTF::create("SCORE", "AnuDaw", 20);
    scoreTitleLbl->setPosition(ccpAdd(this->scorePosition(), ccp(0, 20)));
    this->addChild(scoreTitleLbl, 50);
    
    scoreLabel = CCLabelTTF::create(CCString::createWithFormat("%i", playerScore)->getCString(), "AnuDaw", 18);
    scoreLabel->setPosition(this->scorePosition());
    this->addChild(scoreLabel, 50);
    
}


void PlayFieldLayer::incrementScore() {
    
    playerScore++;
    this->updateScore();
    
}

void PlayFieldLayer::updateScore() {
    
    scoreLabel->setString(CCString::createWithFormat("%i", playerScore)->getCString());
    
}


void PlayFieldLayer::generateTimerDisplay() {
    
    timerFrame = CCSprite::create("timer.png");
    timerFrame->setPosition(this->timerPosition());
    this->addChild(timerFrame, 8);
    
    CCSprite *timerImage = CCSprite::create("timer_back.png");
    timerDisplay = CCProgressTimer::create(timerImage);
    timerDisplay->setPosition(this->timerPosition());
    timerDisplay->setType(kCCProgressTimerTypeRadial);
    this->addChild(timerDisplay, 4);
    timerDisplay->setPercentage(100.0);
    
}

void PlayFieldLayer::addTimeToTimer(int secondsToAdd) {
    
    currentTimerValue = currentTimerValue + secondsToAdd;
}

void PlayFieldLayer::gameOver() {

    CCLabelTTF *gameOverLabel = CCLabelTTF::create("Game Over", "Marker Felt", 60);
    gameOverLabel->setPosition(ccp(winSize.width/2, winSize.height/2));
    this->addChild(gameOverLabel, 50);
    

    CCLabelTTF *gameOverLabelShadow = CCLabelTTF::create("Game Over", "Marker Felt", 60);
    gameOverLabel->setPosition(ccp(winSize.width/2-4, winSize.height/2-4));
    gameOverLabelShadow->setColor(ccBLACK);
    this->addChild(gameOverLabelShadow, 49);
    
}

CCPoint PlayFieldLayer::hillPosition(int row, int col) {
    
    float rowPos = row * 82;
    float colPos = 54 + ((col - 1) * 104);
    return ccp(colPos, rowPos);
    
}

CCPoint PlayFieldLayer::timerPosition() {
    
    return ccp(winSize.width/2, winSize.height-30);
    
}

CCPoint PlayFieldLayer::scorePosition() {
    
    return ccp(winSize.width - 60, winSize.height - 40);
}

bool PlayFieldLayer::ccTouchBegan(CCTouch* touch, CCEvent* event) {
    
    CCPoint location = touch->getLocationInView();
    CCPoint convLoc = CCDirector::sharedDirector()->convertToGL(location);
    
    if (isGameOver || this->backButton->boundingBox().containsPoint(convLoc)) {
        CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect(SND_BUTTON);
        CCDirector::sharedDirector()->replaceScene(MenuLayer::scene());
        return true;
    }
    return false;

}
