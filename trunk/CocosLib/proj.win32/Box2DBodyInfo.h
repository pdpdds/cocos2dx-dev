#include "cocos2d.h"
#include "SimpleAudioEngine.h"
#include "Box2d/Box2D.h"
#include "Box2d/Dynamics/b2Body.h"
#include "GameObject.h"


USING_NS_CC;
using namespace CocosDenshion;

class Box2DBodyInfo : CCObject 
{
public:
	CC_SYNTHESIZE(b2BodyDef*, bodyDef, BodyDef);
	CC_SYNTHESIZE(b2FixtureDef*, fixtureDef, FixtureDef);
	CC_SYNTHESIZE(GameObject*, gameObject, GameObject);

	static Box2DBodyInfo* create();
	bool init();
};