/////////////////////////////////////////////////////////////////////////////////////////////////
//
// Gavin M Thornton 2012
//
/////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _INTVAL_H_
#define _INTVAL_H_

#include "cocos2d.h"
namespace   cocos2d {

class CIntVal : CCObject
{
public:
	//virtual bool init();  
	CIntVal(void);
	CIntVal(int i){m_i=i;};
	~CIntVal(void){};
	
	void release();
	void retain();
	
	// Constructors & Destructors
	int m_i;
};
	
}//namespace   cocos2d {

#endif // _INTVAL_H_
