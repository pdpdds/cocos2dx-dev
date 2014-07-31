//
//  CollisionListener.cpp
//  MiniPool
//
//  Created by Roger Engelbert on 1/14/13.
//
//

#include "CollisionListener.h"
#include "GameLayer.h"
#include "SimpleAudioEngine.h"

using namespace CocosDenshion;

void CollisionListener::BeginContact(b2Contact* contact) {
    b2Body * bodyA = contact->GetFixtureA()->GetBody();
    b2Body * bodyB = contact->GetFixtureB()->GetBody();
    
    b2Sprite * spriteA = (b2Sprite *) bodyA->GetUserData();
    b2Sprite * spriteB = (b2Sprite *) bodyB->GetUserData();
    
    if (spriteA && spriteB) {
        
        //track collision between balls and pockets
        if (spriteA->getType() == kSpritePocket) {
            spriteB->setVisible(false);
        
        } else if (spriteB->getType() == kSpritePocket) {
            spriteA->setVisible(false);
            
        
        } else if (spriteA->getType() == kSpriteBall &&
                spriteB->getType() == kSpriteBall) {
            if (spriteA->mag() > 10 || spriteB->mag() > 10) {
                SimpleAudioEngine::sharedEngine()->playEffect("ball.wav");
            }
        } else if ((spriteA->getType() == kSpriteBall &&
                    spriteB->getType() == kSpritePlayer) ||
                   (spriteB->getType() == kSpriteBall &&
                    spriteA->getType() == kSpritePlayer)) {
            if (spriteA->mag() > 10 || spriteB->mag() > 10) {
                SimpleAudioEngine::sharedEngine()->playEffect("ball.wav");
            }
        }
    }
}


void CollisionListener::PreSolve(b2Contact* contact, const b2Manifold* oldManifold)  {
    
    b2Body * bodyA = contact->GetFixtureA()->GetBody();
    b2Body * bodyB = contact->GetFixtureB()->GetBody();
    
    b2Sprite * spriteA = (b2Sprite *) bodyA->GetUserData();
    b2Sprite * spriteB = (b2Sprite *) bodyB->GetUserData();
    
        
    if (spriteA && spriteB) {
        
        //track collision between player and cue ball
        if (spriteA->getType() == kSpriteCue && spriteA->mag() > 2) {
            if (spriteB->getType() == kSpritePlayer && spriteA->isVisible()) {
                SimpleAudioEngine::sharedEngine()->playEffect("hit.wav");
                spriteA->setVisible(false);
                spriteB->getGame()->setCanShoot(false);
            }
        } else if (spriteB->getType() == kSpriteCue && spriteA->mag() > 2) {
            if (spriteA->getType() == kSpritePlayer && spriteB->isVisible()) {
                SimpleAudioEngine::sharedEngine()->playEffect("hit.wav");
                spriteB->setVisible(false);
                spriteA->getGame()->setCanShoot(false);
            }
        } 
    
    }
    
}

