#include "GameObject.h"

bool GameObject::init()
{
	if (CCNode::init())
	{
		
		initBox2D();
		markedForDestruction = false;
		return true;
    }
    return false;
}

void GameObject::initBox2D()
{
	bodyDef = new b2BodyDef();
	fixtureDef = new b2FixtureDef();

	//Initial fixture settings
	fixtureDef->density = 1.0f;
	fixtureDef->friction = 0.5f;
	fixtureDef->restitution = 0.3f;

	bodyDef->userData = this;
}