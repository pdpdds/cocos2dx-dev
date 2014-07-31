//
//  Ball.cpp
//  MiniPool
//
//  Created by Roger Engelbert on 1/13/13.
//
//

#include "Ball.h"
#include "GameLayer.h"

Ball::~Ball() {   
}

Ball::Ball (GameLayer * game, int type, CCPoint position, int color) : b2Sprite (game, type) {
    _type = type;
    _startPosition = position;
    _color = color;
    _inPlay = true;
}

Ball* Ball::create(GameLayer * game, int type, CCPoint position, int color) {
    Ball * sprite = new Ball(game, type, position, color);
	if (sprite) {
        sprite->initBall();
		sprite->autorelease();
		return sprite;
	}
	CC_SAFE_DELETE(sprite);
	return NULL;
}

void Ball::initBall() {
    
    //create box2d body
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;

    _body = _game->getWorld()->CreateBody(&bodyDef);
    _body->SetSleepingAllowed(true);
    _body->SetLinearDamping(1.2);
    _body->SetAngularDamping(0.8);
    
    //create circle shape
    b2CircleShape  circle;
    circle.m_radius = BALL_RADIUS/PTM_RATIO;
    
    //define fixture
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &circle;
    fixtureDef.density = 5;
    fixtureDef.restitution = 0.7;
    
	//add collision filters so only white ball can be hit by cue
    if (_type == kSpriteBall) {
        fixtureDef.filter.categoryBits = 0x0010;
    } else if (_type == kSpritePlayer) {
    //white ball is tracked as bullet by simulation
        _body->SetBullet(true);
        fixtureDef.filter.categoryBits = 0x0100;
    }
    
    //set sprite texture
    switch (_color) {
        case kColorBlack:
            this->initWithSpriteFrameName("ball_black.png");
            break;
        case kColorRed:
            this->initWithSpriteFrameName("ball_red.png");
            break;
        case kColorYellow:
            this->initWithSpriteFrameName("ball_yellow.png");
            break;
        case kColorWhite:
            this->initWithSpriteFrameName("ball_white.png");
            break;
    }
    
    _body->CreateFixture(&fixtureDef);
    _body->SetUserData(this);
    
    setSpritePosition(_startPosition);
}

void Ball::reset() {
    if (_body) {
        _body->SetLinearVelocity(b2Vec2_zero);
        _body->SetAngularVelocity(0);
       _body->SetAwake(true);
    }
    setSpritePosition(_startPosition);
    _inPlay = true;
    setVisible(true);
}

void Ball::update (float dt) {
    
    if (_body && isVisible()) {
        setPositionX(_body->GetPosition().x * PTM_RATIO);
        setPositionY(_body->GetPosition().y * PTM_RATIO);
    }
}