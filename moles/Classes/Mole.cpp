//
//  Mole.cpp
//  moles
//
//  Created by breaklee on 6/9/13.
//
//

#include "Mole.h"
#include "SimpleAudioEngine.h"


bool Mole::init() {
    
    if( CCNode::init() == false) {
        return false;
    }

    this->moleState = kMoleDead;
   CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, false);

    return true;
}

bool Mole::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent) {
    
    CCPoint location = pTouch->getLocationInView();
    CCPoint locConv = CCDirector::sharedDirector()->convertToGL(location);

    if( this->moleState == kMoleDead) {
        
    }
    else if (this->moleSprite->getPosition().y + (this->moleSprite->getContentSize().height/2) <= moleGroundY) {
        this->moleState = kMoleHidden;
        return false;
	} else  {
        
        if(this->moleSprite->boundingBox().containsPoint(locConv) && locConv.y >= moleGroundY) {
            
            this->moleState = kMoleHit;
            
            if(isSpecial) {
                CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect(SND_MOLE_SPECIAL);
            }
            else {
                CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect(SND_MOLE_NORMAL);
            }
        }
    }
    return true;
}


