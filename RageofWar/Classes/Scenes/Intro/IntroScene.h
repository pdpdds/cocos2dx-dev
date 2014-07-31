#ifndef __INTRO_SCENE_H__
#define __INTRO_SCENE_H__

#include "cocos2d.h"
#include "IntroLayer.h"

class IntroScene : public cocos2d::CCScene
{
public:
	NODE_FUNC(IntroScene);
	bool init();
};
#endif