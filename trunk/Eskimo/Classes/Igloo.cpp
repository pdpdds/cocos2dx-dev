//
//  Igloo.cpp
//  Eskimo
//
//  Created by Roger Engelbert on 1/23/13.
//
//

#include "Igloo.h"

Igloo::~Igloo() {
    CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, NOTIFICATION_GRAVITY_SWITCH);
    CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, NOTIFICATION_LEVEL_COMPLETED);
}

Igloo::Igloo() {
    //register game notifications
    CCNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(Igloo::onGravityChanged), NOTIFICATION_GRAVITY_SWITCH, NULL);
    
    CCNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(Igloo::onLevelCompleted), NOTIFICATION_LEVEL_COMPLETED, NULL);
}

Igloo* Igloo::create() {
    Igloo * sprite = new Igloo();
	if (sprite && sprite->initWithSpriteFrameName("igloo_off.png")) {
		sprite->autorelease();
        sprite->addBlock();
		return sprite;
	}
	CC_SAFE_DELETE(sprite);
	return NULL;
}
void Igloo::onGravityChanged(CCObject * note) {
    char szValue[50] = {0};
    sprintf(szValue, "block_large_%i.png", (int) note);
    _block->setDisplayFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName (szValue));

}

void Igloo::onLevelCompleted(CCObject * note) {
    this->setDisplayFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName ("igloo_on.png"));
}

void Igloo::initIgloo(int gravity, CCPoint position) {

    char szValue[50] = {0};
    sprintf(szValue, "block_large_%i.png", gravity);
    _block->setDisplayFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName (szValue));
    this->setPosition(position);
    this->setDisplayFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName ("igloo_off.png"));
    setVisible(true);
    
}
