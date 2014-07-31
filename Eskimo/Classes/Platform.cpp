//
//  Platform.cpp
//  MiniPool
//
//  Created by Roger Engelbert on 1/13/13.
//
//

#include "Platform.h"
#include "GameLayer.h"


Platform::~Platform() {
    
    CC_SAFE_RELEASE(_tiles);
    CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, NOTIFICATION_GRAVITY_SWITCH);
}

Platform::Platform (GameLayer * game) : b2Sprite (game) {
    
    //create body
    b2BodyDef bodyDef;
    bodyDef.type = b2_staticBody;
    
    _body = game->getWorld()->CreateBody(&bodyDef);
    _body->SetSleepingAllowed(true);
    _body->SetUserData(this);
    
    //register game notifications
    CCNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(Platform::onGravityChanged), NOTIFICATION_GRAVITY_SWITCH, NULL);
    

}

Platform* Platform::create(GameLayer * game) {
    
    Platform * sprite = new Platform(game);
	if (sprite && sprite->initWithSpriteFrameName("blank.png")) {
		sprite->autorelease();
        sprite->createTiles();
		return sprite;
	}
	CC_SAFE_DELETE(sprite);
	return NULL;
}


void Platform::onGravityChanged(CCObject * note) {
    if (this->isVisible()) {
        switchTexture();
    }
}

void Platform::initPlatform(int width, float angle, CCPoint position) {
    
    //Define shape
    b2PolygonShape box;
    box.SetAsBox(width * 0.5f /PTM_RATIO, PLATFORM_HEIGHT * 0.5f / PTM_RATIO);
    
    //Define fixture
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &box;
    fixtureDef.density = 1;
    fixtureDef.restitution = 0;
    
    //reutilize body from the pool: so destroy any existent fixture
    if (_body->GetFixtureList()) {
        _body->DestroyFixture(_body->GetFixtureList());
    }
    
    _body->CreateFixture(&fixtureDef);
    _body->SetTransform(b2Vec2(position.x / PTM_RATIO, position.y / PTM_RATIO), CC_DEGREES_TO_RADIANS(-angle));
    _body->SetActive(true);
    CCSprite * block;
    float startX = -width * 0.5f + TILE * 0.5f;
    
   //set unused tiles in the platform invisible
    for (int i = 0; i < TILES_PER_PLATFORM; i++) {
        
        block = (CCSprite *) _tiles->objectAtIndex(i);
        
        if (i >= width/TILE) {
            block->setVisible(false);
        } else {
            block->setVisible(true);
            block->setPosition(ccp(startX + i * TILE,
                                   0));
            block->setFlipY(false);
            block->setFlipX(false);
            
            if (angle == 0.0) {
                if (position.y >= TILE * 13) block->setFlipY(true);
            }
            if (angle == 90.0) {
                if (position.x > TILE * 5) block->setFlipY(true);
            }
        }
    }
    switchTexture();
    
    this->setRotation(angle);
    this->setPosition(position);
    setVisible(true);
}

//texture platform with tiles
void Platform::switchTexture() {
    CCSprite * block;
    for (int i = 0; i < TILES_PER_PLATFORM; i++) {
        block = (CCSprite *) _tiles->objectAtIndex(i);
        if (block->isVisible()) {
            char szValue[50] = {0};
            sprintf(szValue, "block_%i.png", _game->getGravity());
            block->setDisplayFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName (szValue));
        }
        
    }
}

//create platform with maximum number of tiles a platform can have (larger side / tile size). 
void Platform::createTiles() {
    _tiles = CCArray::createWithCapacity(TILES_PER_PLATFORM);
    _tiles->retain();
    CCSprite * block;
    for (int i = 0; i < TILES_PER_PLATFORM; i++) {
        block = CCSprite::createWithSpriteFrameName("block_1.png");
        block->setVisible(false);
        _tiles->addObject(block);
        this->addChild(block, kBackground, kSpriteBlock);
    }
}

