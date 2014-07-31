//
//  GSwitch.cpp
//  MiniPool
//
//  Created by Roger Engelbert on 1/13/13.
//
//

#include "GSwitch.h"
#include "GameLayer.h"

GSwitch::~GSwitch() {
}

GSwitch::GSwitch() {
}

GSwitch* GSwitch::create() {
    GSwitch * sprite = new GSwitch();
	if (sprite && sprite->initWithSpriteFrameName("blank.png")) {
		sprite->autorelease();
		return sprite;
	}
	CC_SAFE_DELETE(sprite);
	return NULL;
}


void GSwitch::initGSwitch(int direction, CCPoint position) {
    _direction = direction;
    char szValue[50] = {0};
    sprintf(szValue, "switch_%i.png", direction);
    this->setDisplayFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName (szValue));
    this->setPosition(position);
    setVisible(true);
}
