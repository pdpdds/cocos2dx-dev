#include "LineContainer.h"

using namespace cocos2d;

LineContainer::LineContainer() {
	
	_dash = 5;
	_dashSpace = 5;
	_screenSize = CCDirector::sharedDirector()->getWinSize();
    _drawing = false;
    ccPointSize(0.2);
}

LineContainer* LineContainer::create () {
	LineContainer * node = new LineContainer();
	if (node) {
		node->autorelease();
		return node;
	}
	CC_SAFE_DELETE(node);
	return NULL;
}

void LineContainer::draw () {
	
    if (_drawing) {
        
        ccDrawColor4F(1.0, 1.0, 1.0, 1.0);
        
        float length = ccpDistance(_ballPoint, _cuePoint);
        int segments = length / (_dash + _dashSpace);
        
        float t = 0.0f;
        float x_;
        float y_;
        
        for (int i = 0; i < segments + 1; i++) {
            
            x_ = _cuePoint.x + t * (_ballPoint.x - _cuePoint.x);
            y_ = _cuePoint.y + t * (_ballPoint.y - _cuePoint.y);
            
            ccDrawCircle(ccp ( x_, y_ ), 3, M_PI, 4, false);
            
            t += (float) 1 / segments;
        }
    }
}

