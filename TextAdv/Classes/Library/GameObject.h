#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "cocos2d.h"
USING_NS_CC;
#include <Box2D/Box2D.h>
#include <Box2D/Dynamics/b2Body.h>
//#include "GameObjectTypes.h"

class GameArea2D;

class GameObject : public CCNode {
	
public:
	GameArea2D *gameArea;
	b2Body *body;
	b2BodyDef *bodyDef;
	b2FixtureDef *fixtureDef;
	b2PolygonShape *polygonShape;
	b2CircleShape *circleShape;
	CCSprite *sprite;
	int typeTag;
	bool markedForDestruction;

	virtual bool init();
	void initBox2D();

	CC_SYNTHESIZE(GameArea2D*, m_gameArea, gameArea)
	CC_SYNTHESIZE(b2Body*, m_body, body)
	CC_SYNTHESIZE(b2BodyDef*, m_bodyDef, bodyDef)
	CC_SYNTHESIZE(b2FixtureDef*, m_fixtureDef, fixtureDef)
	CC_SYNTHESIZE(b2PolygonShape*, m_polygonShape, polygonShape)
	CC_SYNTHESIZE(b2CircleShape*, m_circleShape, circleShape)
	CC_SYNTHESIZE_RETAIN(CCSprite*, m_sprite, sprite)
	CC_SYNTHESIZE(int, m_typeTag, typeTag)
	CC_SYNTHESIZE(bool, m_markedForDestruction, markedForDestruction)
	
	int type;
};


#endif //GAMEOBJECT_H