//
//  StatusLayer.cpp
//  Parkour
//
//  Created by YTB on 13-11-20.
//
//

#include "StatusLayer.h"

bool StatusLayer::init()
{
    if (!CCLayer::init())
        return false;
    
    coins = 0;
    this->setTag(1);
    this->labelCoin = CCLabelTTF::create("Coins:0", "Helvetica", 25);
    this->labelCoin->setPosition(ccp(70, 300));
    this->addChild(this->labelCoin);
    
    this->labelMeter = CCLabelTTF::create("0M", "Helvetica", 25);
    this->labelMeter->setPosition(ccp(300,300));
    this->addChild(this->labelMeter);
    
    return true;
}

void StatusLayer::addCoin(int num)
{
    coins += num;
    this->labelCoin->setString(CCString::createWithFormat("Coins:%ld",coins)->getCString());
}

void StatusLayer::updateMeter(int px)
{
    this->labelMeter->setString(CCString::createWithFormat("%d:%s", px/10, "M")->getCString());
}