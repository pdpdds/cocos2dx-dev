#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"
#include "cocos-ext.h"


using namespace cocos2d;
using namespace CocosDenshion;

CCScene* HelloWorld::scene()
{
	// 'scene' is an autorelease object
	CCScene *scene = CCScene::create();

	// 'layer' is an autorelease object
	HelloWorld *layer = HelloWorld::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
	//////////////////////////////
	// 1. super init first
	if (!CCLayer::init())
	{
		return false;
	}

	/////////////////////////////
	// 2. add a menu item with "X" image, which is clicked to quit the program
	//    you may modify it.

	// add a "close" icon to exit the progress. it's an autorelease object
	CCMenuItemImage *pCloseItem = CCMenuItemImage::create(
		"CloseNormal.png",
		"CloseSelected.png",
		this,
		menu_selector(HelloWorld::menuCloseCallback));
	pCloseItem->setPosition(ccp(CCDirector::sharedDirector()->getWinSize().width - 20, 20));

	// create menu, it's an autorelease object
	CCMenu* pMenu = CCMenu::create(pCloseItem, NULL);
	pMenu->setPosition(CCPointZero);
	this->addChild(pMenu, 1);

	/////////////////////////////
	// 3. add your codes below...

	// add a label shows "Hello World"
	// create and initialize a label
	CCLabelTTF* pLabel = CCLabelTTF::create("By Cooki", "Thonburi", 40);

	// ask director the window size
	CCSize size = CCDirector::sharedDirector()->getWinSize();

	// position the label on the center of the screen
	pLabel->setPosition(ccp(size.width / 2, size.height - 20));

	// add the label as a child to this layer
	this->addChild(pLabel, 1);

	// add "HelloWorld" splash screen"
	CCSprite* pSprite = CCSprite::create("HelloWorld.png");

	// position the sprite on the center of the screen
	pSprite->setPosition(ccp(size.width / 2, size.height / 2));

	// add the sprite as a child to this layer
	this->addChild(pSprite, 0);

	CCTableView * tableView = CCTableView::create(this, CCSizeMake(480, 260));
	tableView->setDirection(kCCScrollViewDirectionVertical);
	tableView->setPosition(ccp(0, 10));
	tableView->setDelegate(this);
	tableView->setVerticalFillOrder(kCCTableViewFillTopDown);

	this->addChild(tableView);

	return true;
}

#pragma mark - CCTableViewDataSource
unsigned int HelloWorld::numberOfCellsInTableView(CCTableView *table) {
	return 8;
}

CCTableViewCell* HelloWorld::tableCellAtIndex(CCTableView *table, unsigned int idx) {

	CCTableViewCell* cell = table->cellAtIndex(idx);
	if (!cell) {
		cell = new CCTableViewCell();
		cell->autorelease();

		CCSprite* item = CCSprite::create("market_item_2.png");
		item->setPosition(ccp(240, 28));

		cell->addChild(item);
	}


	return cell;
}


CCSize HelloWorld::cellSizeForTable(CCTableView *table) {
	return CCSizeMake(480, 51);
}

CCSize HelloWorld::tableCellSizeForIndex(CCTableView *table, unsigned int idx){
	//    if (idx == (isOpen - 1)) {
	//        return CCSizeMake(960, 162);
	//    }
	//    else
	return CCSizeMake(480, 56);
}

#pragma mark - CCTableViewDelegate
void HelloWorld::tableCellTouched(CCTableView* table, CCTableViewCell* cell) {

		CCLog("you touch cell at %d", cell->getIdx());

}

#pragma mark - CCScrollViewDelegate
void HelloWorld::scrollViewDidScroll(CCScrollView* view) {

}

void HelloWorld::scrollViewDidZoom(CCScrollView* view) {

}

void HelloWorld::menuCloseCallback(CCObject* pSender)
{
	CCDirector::sharedDirector()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
}
