//
//  b2Sprite.cpp
//  MiniPool
//
//  Created by Roger Engelbert on 1/13/13.
//
//

#include "b2Sprite.h"
#include "GameLayer.h"

b2Sprite::b2Sprite (GameLayer * game) {
    _game = game;
}

void b2Sprite::setSpritePosition (CCPoint position) {
    
    setPosition(position);
   
    if (_body) {
        _body->SetTransform(b2Vec2(
                            position.x / PTM_RATIO,
                            position.y/ PTM_RATIO),
                            _body->GetAngle());
    }
     
}

void b2Sprite::update() {
    
    if (_body) {
        setPositionX(_body->GetPosition().x * PTM_RATIO);
        setPositionY(_body->GetPosition().y * PTM_RATIO);
        setRotation(CC_RADIANS_TO_DEGREES(-1 * _body->GetAngle()));
    }
}

void b2Sprite::hide(void) {
    if (_body) {
        _body->SetLinearVelocity(b2Vec2_zero);
        _body->SetAngularVelocity(0);
        _body->SetTransform(b2Vec2_zero, 0.0);
        _body->SetAwake(false);
    }
}

void b2Sprite::reset() {
    
}

