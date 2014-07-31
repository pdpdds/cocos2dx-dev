#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"

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
    if ( !CCLayer::init() )
    {
        return false;
    }
    
    CCMenuItemImage *item0 = CCMenuItemImage::create("Play-0.png", "Play-1.png");
    CCMenuItemImage *item1 = CCMenuItemImage::create("Help-0.png", "Help-1.png");
    
    CCMenuItem *item_0 = CCMenuItemToggle::createWithTarget(this, menu_selector(HelloWorld::menuCallback), item0, item1, NULL);
    item_0->setTag(0);
    CCMenuItem *item_1 = CCMenuItemImage::create("Option-0.png", "Option-1.png", this, menu_selector(HelloWorld::menuCallback));
    item_1->setTag(1);
    
    CCMenu *menu = CCMenu::create(item_0, item_1, NULL);
    menu->alignItemsVertically();
    this->addChild(menu);

    
    
    
    return true;
}

void HelloWorld::menuCallback(CCObject* pSender)
{
    CCLog("menuCallback");
    
    CCMenuItem *item = (CCMenuItem *)pSender;
    
    if (item->getTag()==0) {
        CCLog("menuCallback : TOGGLE");
        
        CCMenuItemToggle *item_toggle = (CCMenuItemToggle *)item;
        
        int index = item_toggle->getSelectedIndex();
        CCLog("menuCallback : TOGGLE = %d", index);
    }
    else if (item->getTag()==1) {
        CCLog("menuCallback : OPTION");
    }
}






