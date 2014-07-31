#include "GalleryLayer.h"
#include "ListViewLayer.h"

using namespace cocos2d;
using namespace cocos2d::extension;

bool GalleryLayer::init()
{
	bool bRet = false;
	do
	{
       CC_BREAK_IF( !CCLayer::init() );

	   m_nCurPage = 1;
	   CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
	   CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();

	   CCLayer *pLayer = CCLayer::create();
	   char helpstr[30] = {0};
	   for (int i = 1; i <= 3; ++ i)
	   {
		   memset(helpstr, 0, sizeof(helpstr));
		   sprintf(helpstr,"bg_%02d.png",i);
		   CCSprite *pSprite = CCSprite::create(helpstr);
		   pSprite->setPosition(ccp(visibleSize.width * (i-0.5f), visibleSize.height / 2));
		   pLayer->addChild(pSprite);
	   }
	   
	   m_pScrollView = CCScrollView::create(CCSizeMake(960, 640), pLayer);
	   m_pScrollView->setContentOffset(CCPointZero);
	   m_pScrollView->setTouchEnabled(false);
	   m_pScrollView->setDelegate(this);
	   m_pScrollView->setDirection(kCCScrollViewDirectionHorizontal);
	   pLayer->setContentSize(CCSizeMake(960*3, 640));

	   this->addChild(m_pScrollView);

	   CCSpriteFrameCache *pCache =  CCSpriteFrameCache::sharedSpriteFrameCache();
   
	   pCache->addSpriteFrame(CCSpriteFrame::create("button_normal.png",CCRectMake(0, 0, 64, 64)),"button_normal.png");
	   pCache->addSpriteFrame(CCSpriteFrame::create("button_selected.png",CCRectMake(0, 0, 64, 64)),"button_selected.png");
	   for (int i = 1; i <= 3; ++ i)
	   {
		   CCSprite *pPoint = CCSprite::createWithSpriteFrameName("button_normal.png");
		   pPoint->setTag(i);
		   pPoint->setPosition(ccp( origin.x + (visibleSize.width - 3 * pPoint->getContentSize().width)/2 + pPoint->getContentSize().width * (i-1), origin.y + 30));
		   this->addChild(pPoint);
	   }
		CCSprite *pPoint = (CCSprite *)this->getChildByTag(1);
		pPoint->setDisplayFrame(pCache->spriteFrameByName("button_selected.png"));
	
		bRet = true;
	}while(0);

	return bRet;

}

void GalleryLayer::menuCloseCallback(CCObject* pSender)
{

}

void GalleryLayer::scrollViewDidScroll(cocos2d::extension::CCScrollView *view)
{
	CCLOG("scroll");
}

void GalleryLayer::scrollViewDidZoom(cocos2d::extension::CCScrollView *view)
{
	CCLOG("zoom");
}

void GalleryLayer::onEnter()
{
	CCLayer::onEnter();
	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 1, false);
}

void GalleryLayer::onExit()
{
	CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
	CCLayer::onExit();
	CCSpriteFrameCache::sharedSpriteFrameCache()->removeUnusedSpriteFrames();
}


bool GalleryLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
	m_touchPoint = CCDirector::sharedDirector()->convertToGL(pTouch->getLocationInView());
	return true;
}

void GalleryLayer::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
	
}

void GalleryLayer::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
	CCPoint endPoint = CCDirector::sharedDirector()->convertToGL(pTouch->getLocationInView());
	float distance = endPoint.x - m_touchPoint.x;
	if(fabs(distance) > 50)
	{
		adjustScrollView(distance);
	}
}

void GalleryLayer::ccTouchCancelled(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
	CCPoint endPoint = CCDirector::sharedDirector()->convertToGL(pTouch->getLocationInView());
	float distance = endPoint.x - m_touchPoint.x;
	if(fabs(distance) > 50)
	{
		adjustScrollView(distance);
	}
}

void GalleryLayer::adjustScrollView(float offset)
{
	CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
	CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();
	CCSpriteFrameCache *pCache =  CCSpriteFrameCache::sharedSpriteFrameCache();
	CCSprite *pPoint = (CCSprite *)this->getChildByTag(m_nCurPage);
	pPoint->setDisplayFrame(pCache->spriteFrameByName("button_normal.png"));
	if (offset<0)
	{
		m_nCurPage ++;
	}else
	{
		m_nCurPage --;
	}

	if (m_nCurPage <1)
	{
		m_nCurPage = 1;
	}

	if(m_nCurPage > 3)
	{
		CCLayer *pLayer = ListViewLayer::create();
		CCScene *pScene = CCScene::create();
		pScene->addChild(pLayer);
		CCDirector::sharedDirector()->replaceScene(pScene);
	}
	else
	{
		pPoint = (CCSprite *)this->getChildByTag(m_nCurPage);
		pPoint->setDisplayFrame(pCache->spriteFrameByName("button_selected.png"));
		CCPoint  adjustPos = ccp(origin.x - visibleSize.width * (m_nCurPage-1), 0);
		m_pScrollView->setContentOffset(adjustPos, true);
	}
}