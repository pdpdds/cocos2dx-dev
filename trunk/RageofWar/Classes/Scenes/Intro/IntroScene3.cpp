#include "IntroScene3.h"

using namespace cocos2d;

bool IntroScene3::init()
{
	bool pRet = false;
	if (CCScene::init())
	{
		IntroLayer3 *myLayer = new IntroLayer3();
		this->addChild(myLayer);
		myLayer->autorelease();
		pRet = true;
	}

	return pRet;
}