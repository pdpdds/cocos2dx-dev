#pragma once

#include "cocos2d.h"

using namespace cocos2d;

class AGTextLayer : public CCLayer
{
public:
	AGTextLayer(void);
	virtual ~AGTextLayer(void);

	virtual bool init(); 

	// implement the "static node()" method manually
	CREATE_FUNC(AGTextLayer);

protected:

private:

};
