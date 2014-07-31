#include "IntroScene2.h"

using namespace cocos2d;

bool IntroScene2::init()
{
	bool pRet = false;
	if (CCScene::init())
	{
		IntroLayer2 *myLayer = IntroLayer2::node();
		this->addChild(myLayer);
		pRet = true;
	}

	return pRet;
}