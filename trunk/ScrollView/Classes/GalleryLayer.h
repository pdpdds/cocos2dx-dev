#ifndef __HELP_SCENE_H__
#define __HELP_SCENE_H__

#include "cocos2d.h"
#include "SimpleAudioEngine.h"
#include "cocos-ext.h"

USING_NS_CC;
USING_NS_CC_EXT;


class GalleryLayer : public cocos2d::CCLayer ,public CCScrollViewDelegate
{
public:
    virtual bool init();  

    void menuCloseCallback(CCObject* pSender);

    CREATE_FUNC(GalleryLayer);

public:
	//scrollview滚动的时候会调用
	void scrollViewDidScroll(CCScrollView* view);
	//scrollview缩放的时候会调用
	void scrollViewDidZoom(CCScrollView* view);

	virtual void onEnter();
	virtual void onExit();

	virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
	virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
	virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
	virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent);

private:
	//根据手势滑动的距离和方向滚动图层
	 void adjustScrollView(float offset);
	 CCScrollView *m_pScrollView;
	 CCPoint m_touchPoint;
	 int m_nCurPage;
};

#endif  