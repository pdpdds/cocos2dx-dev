#include "cocos2d.h"
#include "SimpleAudioEngine.h"
#include "Box2d/Box2D.h"
#include "Box2d/Dynamics/b2Body.h"
#include "GameObject.h"


USING_NS_CC;
using namespace CocosDenshion;

class QueuedAction : CCObject 
{
public:
	GameObject* gameObject;
	CCAction* action;

	static QueuedAction* create();
	bool init();
};