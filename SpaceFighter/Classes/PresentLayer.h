#ifndef _PRESENTLAYER_H_
#define _PRESENTLAYER_H_

#include "cocos2d.h"

class PresentLayer : public cocos2d::CCLayer
{
public:
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  

    // there's no 'id' in cpp, so we recommand to return the exactly class pointer
    static cocos2d::CCScene* scene();

	void GoTitleScreen();

    // implement the "static node()" method manually
    CREATE_FUNC(PresentLayer);

	
};

#endif  