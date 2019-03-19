//
//  PlayFieldLayer.h
//  moles
//
//  Created by breaklee on 6/9/13.
//
//

#ifndef __moles__PlayFieldLayer__
#define __moles__PlayFieldLayer__

#include <iostream>
#include "cocos2d.h"

USING_NS_CC;

class Mole;
class MoleHill;

class PlayFieldLayer : public CCLayer {
    
public :
    virtual bool init();
    virtual bool ccTouchBegan(CCTouch* touch, CCEvent* event);
    CREATE_FUNC(PlayFieldLayer);
    static CCScene* scene();
    
    void incrementScore();
    void addTimeToTimer(int secondsToAdd);
    void updateScore();
 
    CCPoint timerPosition();
    CCPoint scorePosition();
    CCPoint hillPosition(int row, int col);

    void buildAnimations();
    
    void preloadEffects();
    
    void drawGround();
    
    void drawHills();
    
    void generateTimerDisplay();
    
    void generateScoreDisplay();
    
    void deleteMole(CCNode* node, void* hil);
    
    virtual void onEnterTransitionDidFinish();
    
    virtual void update(float dt);
    
    void scoreMole(Mole* mole);
    
    void spawnMole(PlayFieldLayer* layer);
    
    void resetMole(MoleHill* hill);
    
    void raiseMole(MoleHill* aHill);
    
    void gameOver();
    
    

    CCSize winSize; // This is the window size returned from CCDirector
    
    CCSpriteBatchNode *molesheet; // This holds the spritesheet for the game
    
    CCSprite *skybox; // The sky image
    CCSprite *backButton; // simple sprite control to leave the scene
    
    CCArray *moleHillsInPlay; // contains all active Mole Hills
    
    int molesInPlay; // number of active moles
    
    int maxMoles; // maximum number of simultaneous moles
    int maxHillRows; // number of rows of hills
    int maxHillColumns; // number of columns of hills
    int maxHills; // number of hills on the board
    
    float moleRaiseTime; // duration it takes as a mole moves up
    float moleDelayTime; // duration a mole pauses at the top
    float moleDownTime; // duration it takes as a mole moves down
    
    float spawnRest; // delay between mole spawnings
    
    int playerScore; // Current score
    CCLabelTTF *scoreLabel; // Label to display he current score
    
    CCProgressTimer *timerDisplay; // gameplay timer display
    float currentTimerValue; // actual value of time remaining
    float startingTimerValue; // initial value of the timer - we count down
    CCSprite *timerFrame; // The front "face" of the timer
    
    bool isGameOver; // indicates if Game Over has been reached
    

};


#endif /* defined(__moles__PlayFieldLayer__) */
