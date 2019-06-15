#include "GameObject.h"

bool GameObject::init() 
{
    if( (this->init()) ) 
	{
		this->initBox2D();
		markedForDestruction = false;
    }
    return true;
}

void GameObject::initBox2D() {
	bodyDef = new b2BodyDef();
	fixtureDef = new b2FixtureDef();

	//Initial fixture settings
	fixtureDef->density = 1.0f;
	fixtureDef->friction = 0.5f;
	fixtureDef->restitution = 0.3f;

	bodyDef->userData = this;
}




