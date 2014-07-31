#include "ListViewLayer.h"

USING_NS_CC;
USING_NS_CC_EXT;


bool ListViewLayer::init()
{
	bool bRet = false;
	do
	{
		CC_BREAK_IF( !CCLayer::init() );

		CCTableView* pTableView = CCTableView::create(this, CCSizeMake(960, 640));
		pTableView->setDirection(kCCScrollViewDirectionVertical);
		pTableView->setPosition(CCPointZero);
		pTableView->setDelegate(this);
		pTableView->setVerticalFillOrder(kCCTableViewFillTopDown);
		this->addChild(pTableView);
		pTableView->reloadData();

		bRet = true;
	}while(0);

	return bRet;
}



void ListViewLayer::tableCellTouched(CCTableView* table, CCTableViewCell* cell)
{
    CCLog("cell touched at index: %i", cell->getIdx());
}

CCSize ListViewLayer::cellSizeForTable(CCTableView *table)
{
    return CCSizeMake(960, 120);
}

CCTableViewCell* ListViewLayer::tableCellAtIndex(CCTableView *table, unsigned int idx)
{
    CCString *pString = CCString::createWithFormat("%d", idx);
    CCTableViewCell *pCell = table->dequeueCell();
    if (!pCell) {
        pCell = new CCTableViewCell();
        pCell->autorelease();
        CCSprite *pSprite = CCSprite::create("listitem.png");
        pSprite->setAnchorPoint(CCPointZero);
		pSprite->setPosition(CCPointZero);
        pCell->addChild(pSprite);

        CCLabelTTF *pLabel = CCLabelTTF::create(pString->getCString(), "Arial", 20.0);
        pLabel->setPosition(CCPointZero);
		pLabel->setAnchorPoint(CCPointZero);
        pLabel->setTag(123);
        pCell->addChild(pLabel);
    }
    else
    {
        CCLabelTTF *pLabel = (CCLabelTTF*)pCell->getChildByTag(123);
        pLabel->setString(pString->getCString());
    }


    return pCell;
}

unsigned int ListViewLayer::numberOfCellsInTableView(CCTableView *table)
{
    return 20;
}


void ListViewLayer::scrollViewDidScroll(CCScrollView *view)
{
}

void ListViewLayer::scrollViewDidZoom(CCScrollView *view)
{
}