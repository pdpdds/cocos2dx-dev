
#ifndef __LEVEL_GENERATOR_HUD_H__
#define __LEVEL_GENERATOR_HUD_H__

#include "cocos2d.h"

#include "JoystickClasses\SneakyButton.h"
#include "JoystickClasses\SneakyButtonSkinnedBase.h"
#include "JoystickClasses\SneakyJoystick.h"
#include "JoystickClasses\SneakyJoystickSkinnedBase.h"
#include "Constants\Constants.h"

#include "Scenes\MainMenu\MainScene.h"
class LevelGenerator;

class LevelGeneratorHud : public cocos2d::CCLayer
{
	cocos2d::CCLabelTTF *_label;
	
	cocos2d::CCLabelTTF* _lblTimer;
public:
	virtual bool init();
	cocos2d::CCLabelTTF* _lblScore;
	cocos2d::CCLabelTTF* _lblEnemyScore;
	cocos2d::CCLabelTTF* _lblZombieScore;
	cocos2d::CCLabelTTF* _lblCarScore;
	cocos2d::CCLabelTTF* _lblTruckScore;
	cocos2d::CCLabelTTF* _lblHeliScore;
	void numCollectedChanged(int numCollected);
	void projectileButtonTapped(CCObject *sender);
	
	NODE_FUNC(LevelGeneratorHud);

	CC_SYNTHESIZE(LevelGenerator *, _gameLayer, GameLayer);
	CC_SYNTHESIZE(int, sectime, SecTimeVal);
	void updateHealthBar(int health);
	//health bar
	float scalefactor;
	int hbxloc;
	//power up icon
	cocos2d::CCSprite* powerup;
	cocos2d::CCSprite* life;
	cocos2d::CCMenu* pMenuPause;
	cocos2d::CCMenu* pMenuPlay;
	void update(float deltaTime);
	//gameplay layer stuff
	void initJoystickAndButtons();

	SneakyJoystick *leftJoystick;
	//SneakyButton *jumpButton;
	//SneakyButton *attackButton;

	CC_SYNTHESIZE(SneakyJoystick *, joystick, Joystick);
	CC_SYNTHESIZE(SneakyButton *, jumpButton, JumpButton);
	CC_SYNTHESIZE(SneakyButton *, attackButton, AttackButton);

	void applyJoystick(SneakyJoystick *aJoystick, float deltaTime);
	float getxval();
	void updateScore(int score);
	void updateEnemyScore(int score);
	void updateZombieScore(int score);
	void updateCarScore(int score);
	void updateTruckScore(int score);
	void updateHeliScore(int score);
	void showHealthPackAlert();
	void hideHealthPackAlert();
	void showTimer();

	bool updateTimer();
	//void applyJoystick(SneakyJoystick *aJoystick, cocos2d::CCNode *tempNode, float deltaTime);
	virtual void menuBackCallback(CCObject* pSender);
	virtual void PauseGameCallback(CCObject* pSender);
	virtual void PlayGameCallback(CCObject* pSender);
	
	protected:
	/*SneakyJoystick *lefJoystick;
	SneakyButton *jumpButton;
	SneakyButton *attackButton;*/
	//CCSpriteBatchNode *sceneSpriteBatchNode;
	//CCLabelBMFont *gameBeginLabel;
	
};

#endif