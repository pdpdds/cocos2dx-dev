#ifndef __LINECONTAINER_H__
#define __LINECONTAINER_H__


#include "cocos2d.h"

using namespace cocos2d;

class LineContainer : public CCNode {
    
public:
    
    CC_SYNTHESIZE(CCPoint, _ballPoint, BallPoint);
	CC_SYNTHESIZE(CCPoint, _cuePoint, CuePoint);
    CC_SYNTHESIZE(bool, _drawing, Drawing);
	
    LineContainer();
	static LineContainer * create();
    virtual void draw();
    
    
private:
	
    int _dash;
    int _dashSpace;
	CCSize _screenSize;
    
};

#endif // __LINECONTAINER_H__


