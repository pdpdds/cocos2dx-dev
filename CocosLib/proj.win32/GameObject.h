#include "cocos2d.h"
#include "SimpleAudioEngine.h"
#include "Box2d/Box2D.h"
#include "Box2d/Dynamics/b2Body.h"
#include "GameObjectTypes.h"

class GameArea2D;

USING_NS_CC;
using namespace CocosDenshion;

class GameObject : public CCNode
{
public:
	CC_SYNTHESIZE(GameArea2D*, gameArea, GameArea);
	CC_SYNTHESIZE(b2Body*, body, Body);
	CC_SYNTHESIZE(b2BodyDef*, bodyDef, BodyDef);
	CC_SYNTHESIZE(b2FixtureDef*, fixtureDef, FixtureDef);
	CC_SYNTHESIZE(b2PolygonShape*, polygonShape, PolygonShape);
	CC_SYNTHESIZE(b2CircleShape*, circleShape, CircleShape);
	CC_SYNTHESIZE(CCSprite*, sprite, Sprite);
	CC_SYNTHESIZE(int, type, Type);
	CC_SYNTHESIZE(int, typeTag, TypeTag);
	CC_SYNTHESIZE(bool, markedForDestruction, MarkedForDestruction);

	bool init();
	void initBox2D();

private:

};
