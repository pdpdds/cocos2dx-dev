#pragma once
#include "GameDefines.h"
#include "cocos2d.h"
#include "Box2d/Box2D.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;
using namespace CocosDenshion;

enum {
	kTagButtonOn,
	kTagButtonOff
};

class LevelSelectLayer : public CCLayer {
public:

	~LevelSelectLayer();
	LevelSelectLayer();

	static cocos2d::CCScene* scene();

	virtual void ccTouchesBegan(cocos2d::CCSet* touches, cocos2d::CCEvent* event);
	virtual void ccTouchesEnded(cocos2d::CCSet* touches, cocos2d::CCEvent* event);

private:
	CCSpriteBatchNode * _layerBatchNode;
	CCSize _screenSize;
	CCMenu * _levelMenu;
	CCArray * _levels;
	CCArray * _levelLabels;

	CCSprite * _btnBack;
	CCSprite * _btnNext;


	CCParticleSystem * _snow;

	int _levelsCompleted;
	int _firstIndex;

	void createScreen(void);
	void createMenu(void);

};

