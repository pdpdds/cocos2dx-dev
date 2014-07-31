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
	//scrollview������ʱ������
	void scrollViewDidScroll(CCScrollView* view);
	//scrollview���ŵ�ʱ������
	void scrollViewDidZoom(CCScrollView* view);

	virtual void onEnter();
	virtual void onExit();

	virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
	virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
	virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
	virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent);

private:
	//�������ƻ����ľ���ͷ������ͼ��
	 void adjustScrollView(float offset);
	 CCScrollView *m_pScrollView;
	 CCPoint m_touchPoint;
	 int m_nCurPage;
};

#endif  