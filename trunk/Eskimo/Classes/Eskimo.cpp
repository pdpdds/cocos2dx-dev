//
//  Eskimo.cpp
//  MiniPool
//
//  Created by Roger Engelbert on 1/13/13.
//
//

#include "Eskimo.h"
#include "GameLayer.h"

Eskimo::~Eskimo() {
    
}

Eskimo::Eskimo (GameLayer * game) : b2Sprite (game) {
    //create body
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    
    _body = game->getWorld()->CreateBody(&bodyDef);
    _body->SetSleepingAllowed(false);
    _body->SetUserData(this);
    this->initWithSpriteFrameName("player_circle.png");
    makeCircleShape();
    _switchShape = false;
}

Eskimo* Eskimo::create(GameLayer * game) {
    Eskimo * sprite = new Eskimo(game);
	if (sprite) {
		sprite->autorelease();
		return sprite;
	}
	CC_SAFE_DELETE(sprite);
	return NULL;
}

void Eskimo::reset(){
    if (_state != kStateCircle) {
        makeCircleShape();
    }
    _body->SetTransform(_body->GetPosition(),0.0);
    _body->SetLinearVelocity(b2Vec2_zero);
    
    setVisible(true);
    setRotation(0.0);
}

void Eskimo::update () {
    
    if (_switchShape) {
        SimpleAudioEngine::sharedEngine()->playEffect("shape.wav");
        if (_state == kStateBox) {
            //switch to circle
            makeCircleShape();
        } else {
            //switch to box
            makeBoxShape();
        }
        _switchShape = false;
    }
    
    b2Sprite::update();
}

void Eskimo::makeCircleShape() {
    
    if (_body->GetFixtureList() ) {
        _body->DestroyFixture(_body->GetFixtureList());
    }
    //Define shape
    b2CircleShape  circle;
    circle.m_radius = PLAYER_RADIUS/PTM_RATIO;
    
    //Define fixture
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &circle;
    fixtureDef.density = 1;
    fixtureDef.restitution = 0.4;
    fixtureDef.friction = 10;
    
    _body->CreateFixture(&fixtureDef);
    _state = kStateCircle;
    
    this->setDisplayFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName ("player_circle.png"));

}

void Eskimo::makeBoxShape() {
    
    if (_body->GetFixtureList() ) {
        _body->DestroyFixture(_body->GetFixtureList());
    }
    
    //Define shape
    b2PolygonShape box;
    box.SetAsBox(PLAYER_RADIUS /PTM_RATIO, PLAYER_RADIUS/ PTM_RATIO);
    
    //Define fixture
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &box;
    fixtureDef.density = 10;
    fixtureDef.restitution = 0;
    fixtureDef.friction = 0;
    
    _body->CreateFixture(&fixtureDef);
    _state = kStateBox;
    this->setDisplayFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName ("player_box.png"));
    
}