#ifndef __INTRO_LAYER_H__
#define __INTRO_LAYER_H__

#include "cocos2d.h"
#include "Constants\Constants.h"


class IntroLayer : public cocos2d::CCLayer
{
protected:
	// IMPORTANT: check if the intro has been skipped, if it has then don't run startGameplay again.
	// This is to prevent the player from tapping too many times as this leads to startGameplay being called again and again
	// Slowing down the starting and potentially crashing the game
	bool hasBeenSkipped;
public:
	NODE_FUNC(IntroLayer);
	void startGamePlay();
	void ccTouchesBegan(cocos2d::CCSet *touches, cocos2d::CCEvent *event);
	bool init();
};

#endif