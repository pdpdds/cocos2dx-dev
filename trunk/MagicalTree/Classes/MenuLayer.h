#pragma once
#include "GameDefines.h"
#include "cocos2d.h"
#include "Box2d/Box2D.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;
using namespace CocosDenshion;

class MenuLayer : public CCLayer
{
public:
	MenuLayer();
	virtual ~MenuLayer();

	static cocos2d::CCScene* scene();

	void update(float dt);
	void showLevels(CCObject* pSender);

private:
	void createScreen(void);

	CCParticleSystem * _snow;
	CCSize _screenSize;
	float _msgCntDown;

	CCSpriteBatchNode * _layerBatchNode;
	CCMenu* _mainMenu;
};

