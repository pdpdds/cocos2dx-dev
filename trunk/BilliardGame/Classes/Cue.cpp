//
//  Cue.cpp
//  MiniPool
//
//  Created by Roger Engelbert on 1/13/13.
//
//

#include "Cue.h"
#include "GameLayer.h"

Cue::~Cue() {
}

Cue::Cue(GameLayer * game):b2Sprite(game, kSpriteCue) {
}

Cue* Cue::create(GameLayer * game) {
    Cue * sprite = new Cue(game);
	if (sprite) {
        sprite->initCue();
		sprite->autorelease();
		return sprite;
	}
	CC_SAFE_DELETE(sprite);
	return NULL;
}


void Cue::initCue() {
    
    //create body
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    
    _body = _game->getWorld()->CreateBody(&bodyDef);
    _body->SetSleepingAllowed(true);
    _body->SetLinearDamping(8);
    _body->SetAngularDamping(5);
    
    //Define shape
    b2PolygonShape box;
    box.SetAsBox(BALL_RADIUS * 21 /PTM_RATIO, BALL_RADIUS * 0.2 / PTM_RATIO);
    
    //Define fixture
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &box;
    fixtureDef.filter.maskBits = 0x0100;
    fixtureDef.density = 10;
    fixtureDef.restitution = 1;
    _body->CreateFixture(&fixtureDef);
    _body->SetUserData(this);
    
    
    //set sprite texture
    this->initWithSpriteFrameName("cue.png");
}
