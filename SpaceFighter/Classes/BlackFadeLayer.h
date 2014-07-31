#ifndef _BLACKFADELAYER_H_
#define _BLACKFADELAYER_H_

#include "cocos2d.h"

USING_NS_CC;
using namespace std;

class BlackFadeLayer : public CCLayer
{
public: 
    BlackFadeLayer();
	~BlackFadeLayer(void);

	virtual void onEnter();
    virtual void onEnterTransitionDidFinish();
    virtual void onExitTransitionDidStart();
    virtual void onExit();

	void ChangeLogo();

};

#endif
