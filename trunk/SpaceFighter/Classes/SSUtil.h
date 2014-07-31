#ifndef _SSUTIL_H_
#define _SSUTIL_H_

#include "cocos2d.h"

USING_NS_CC;

class SSUtil
{
public:
	SSUtil(void);
	~SSUtil(void);


	static float radiansBetweenLines(CCPoint l1p1, CCPoint l1p2, CCPoint l2p1, CCPoint l2p2);
};

#endif
