#ifndef _TITLELAYER_H_
#define _TITLELAYER_H_

#include "cocos2d.h"

class TitleLayer : public cocos2d::CCLayer
{
public:
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  

    // there's no 'id' in cpp, so we recommand to return the exactly class pointer
    static cocos2d::CCScene* scene();

	void MenuStartCallback(CCObject* pSender);

    // implement the "static node()" method manually
    CREATE_FUNC(TitleLayer);

};

#endif  