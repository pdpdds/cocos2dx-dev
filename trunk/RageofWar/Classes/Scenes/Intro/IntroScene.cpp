#include "IntroScene.h"

using namespace cocos2d;

bool IntroScene::init()
{
	bool pRet = false;
	if (CCScene::init())
	{
		IntroLayer *myLayer = IntroLayer::node();
		this->addChild(myLayer);
		pRet = true;
	}

	return pRet;
}