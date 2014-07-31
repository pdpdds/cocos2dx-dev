#include "MenuLayer.h"
#include "LevelSelectLayer.h"

MenuLayer::MenuLayer()
{
	_screenSize = CCDirector::sharedDirector()->getWinSize();
	//interval between balloon messages
	_msgCntDown = 5;
	createScreen();

	setTouchEnabled(true);
	scheduleUpdate();


}


MenuLayer::~MenuLayer()
{
}

void MenuLayer::createScreen() {

	CCSprite * bg = CCSprite::create("bg.jpg");
	bg->setPosition(ccp(_screenSize.width * 0.5f, _screenSize.height * 0.5f));
	this->addChild(bg);

	//load a particle system for snow
	_snow = CCParticleSystemQuad::create("snow.plist");
	_snow->setPosition(ccp(_screenSize.width * 0.5, _screenSize.height));
	this->addChild(_snow, kBackground);

	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("sprite_sheet.plist");
	_layerBatchNode = CCSpriteBatchNode::create("sprite_sheet.png", 50);
	this->addChild(_layerBatchNode, kMiddleground);

	/*CCSprite * logo = CCSprite::createWithSpriteFrameName("logo.png");
	logo->setPosition(ccp(_screenSize.width * 0.5f, _screenSize.height * 0.75f));
	_layerBatchNode->addChild(logo);*/

	//create menu
	CCSprite * menuItemOn;
	CCSprite * menuItemOff;
	menuItemOn = CCSprite::createWithSpriteFrameName("btn_play_on.png");
	menuItemOff = CCSprite::createWithSpriteFrameName("btn_play_off.png");

	CCMenuItemSprite * playItem = CCMenuItemSprite::create(
		menuItemOff,
		menuItemOn,
		this,
		menu_selector(MenuLayer::showLevels));

	menuItemOn = CCSprite::createWithSpriteFrameName("btn_help_on.png");
	/*menuItemOff = CCSprite::createWithSpriteFrameName("btn_help_off.png");
	CCMenuItemSprite * helpItem = CCMenuItemSprite::create(
		menuItemOff,
		menuItemOn,
		this,
		menu_selector(MenuLayer::showHelp));*/

	_mainMenu = CCMenu::create(playItem, NULL);
	_mainMenu->alignItemsVerticallyWithPadding(10);
	_mainMenu->setPosition(ccp(_screenSize.width * 0.5f, _screenSize.height * 0.5f));
	this->addChild(_mainMenu, kForeground);
}

void MenuLayer::update(float dt) {


	
}


CCScene* MenuLayer::scene()
{
	// 'scene' is an autorelease object
	CCScene *scene = CCScene::create();

	// add layer as a child to scene
	CCLayer* layer = new MenuLayer();
	scene->addChild(layer);
	layer->release();

	return scene;
}

void MenuLayer::showLevels(CCObject* pSender) {
	//SimpleAudioEngine::sharedEngine()->playEffect("button.wav");
	CCScene* newScene = CCTransitionFade::create(0.2f, LevelSelectLayer::scene());
	CCDirector::sharedDirector()->replaceScene(newScene);
}