#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "cocos-ext.h"

using namespace cocos2d;
using namespace extension;

class HelloWorld : public cocos2d::CCLayer, public CCTableViewDataSource, public CCTableViewDelegate
{
public:
	// Method 'init' in cocos2d-x returns bool, instead of 'id' in cocos2d-iphone (an object pointer)
	virtual bool init();

	// there's no 'id' in cpp, so we recommend to return the class instance pointer
	static cocos2d::CCScene* scene();

	// a selector callback
	void menuCloseCallback(CCObject* pSender);

	// preprocessor macro for "static create()" constructor ( node() deprecated )
	CREATE_FUNC(HelloWorld);

public:
	void scrollViewDidScroll(CCScrollView* view);
	void scrollViewDidZoom(CCScrollView* view);

public:
	unsigned int numberOfCellsInTableView(CCTableView *table);
	CCTableViewCell* tableCellAtIndex(CCTableView *table, unsigned int idx);
	CCSize cellSizeForTable(CCTableView *table);
	CCSize tableCellSizeForIndex(CCTableView *table, unsigned int idx);

public:
	void tableCellTouched(CCTableView* table, CCTableViewCell* cell);
};

#endif // __HELLOWORLD_SCENE_H__
