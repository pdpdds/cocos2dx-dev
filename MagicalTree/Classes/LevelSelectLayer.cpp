//
//  LevelSelectLayer.cpp
//  Eskimo
//
//  Created by Roger Engelbert on 1/22/13.
//
//

#include "LevelSelectLayer.h"
#include "MenuLayer.h"
#include "GameLayer.h"

//this is for testing purposes... so imagine we have 100 levels!
#define TOTAL_LEVELS 100
#define LEVELS_PER_SCREEN 25


LevelSelectLayer::~LevelSelectLayer()
{
	CC_SAFE_RELEASE(_levels);
	CC_SAFE_RELEASE(_levelLabels);
}

LevelSelectLayer::LevelSelectLayer()
{
	_screenSize = CCDirector::sharedDirector()->getWinSize();

	//load user data for number of levels completed
	_levelsCompleted = CCUserDefault::sharedUserDefault()->getIntegerForKey("levelsCompleted");
	if (_levelsCompleted == 0) {
		_levelsCompleted = 1;
		CCUserDefault::sharedUserDefault()->setIntegerForKey("levelsCompleted", 1);
		CCUserDefault::sharedUserDefault()->flush();
	}

	_firstIndex = 1 + (floor(_levelsCompleted / LEVELS_PER_SCREEN) * LEVELS_PER_SCREEN);

	createScreen();

	setTouchEnabled(true);

}


void LevelSelectLayer::ccTouchesBegan(CCSet* touches, CCEvent* event) {

	CCTouch *touch = (CCTouch *)touches->anyObject();

	if (touch) {
		CCPoint tap = touch->getLocation();
		int count = _levels->count();
		CCSprite * btn;
		//track touch on level buttons
		for (int i = 0; i < count; i++) {
			btn = (CCSprite *)_levels->objectAtIndex(i);
			if (ccpDistance(tap, btn->getPosition()) <= btn->boundingBox().size.width * 0.8f) {
				if (btn->getTag() == kTagButtonOff) {
					btn->setDisplayFrame(
						CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("btn_num_on.png")
						);
					return;
				}

			}
		}
	}
}

void LevelSelectLayer::ccTouchesEnded(CCSet* touches, CCEvent* event) {

	CCTouch *touch = (CCTouch *)touches->anyObject();

	if (touch) {
		CCPoint tap = touch->getLocation();
		int count = _levels->count();
		CCSprite * btn;
		//track touch on level buttons
		for (int i = 0; i < count; i++) {
			btn = (CCSprite *)_levels->objectAtIndex(i);
			if (ccpDistance(tap, btn->getPosition()) <= btn->boundingBox().size.width * 0.8f) {
				if (btn->getTag() == kTagButtonOff) {
					SimpleAudioEngine::sharedEngine()->playEffect("button.wav");
					btn->setDisplayFrame(
						CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("btn_num_off.png")
						);

					CCScene* newScene = CCTransitionFade::create(0.2f, GameLayer::scene(_firstIndex + i, _levelsCompleted));
					CCDirector::sharedDirector()->replaceScene(newScene);

					return;
				}

			}
		}
		//track touch on back button button
		if (ccpDistance(tap, _btnBack->getPosition()) <= _btnBack->getTextureRect().size.width) {
			SimpleAudioEngine::sharedEngine()->playEffect("button.wav");
			if (_firstIndex == 1) {
				CCScene* newScene = CCTransitionFade::create(0.2f, MenuLayer::scene());
				CCDirector::sharedDirector()->replaceScene(newScene);
			}
			else {
				_firstIndex -= LEVELS_PER_SCREEN;
				createMenu();
				_btnNext->setVisible(true);

			}
			//track touch on next button button
		}
		else if (ccpDistance(tap, _btnNext->getPosition()) <= _btnNext->getTextureRect().size.width) {
			SimpleAudioEngine::sharedEngine()->playEffect("button.wav");
			if (_firstIndex < TOTAL_LEVELS - LEVELS_PER_SCREEN) {
				_firstIndex += LEVELS_PER_SCREEN;
				createMenu();
			}
		}

	}
}

CCScene* LevelSelectLayer::scene()
{
	// 'scene' is an autorelease object
	CCScene *scene = CCScene::create();

	// add layer as a child to scene
	CCLayer* layer = new LevelSelectLayer();
	scene->addChild(layer);
	layer->release();

	return scene;
}


void LevelSelectLayer::createScreen() {
	//if we load this scene first, load sprite frames
	//CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("sprite_sheet.plist");

	CCSprite * bg = CCSprite::create("bg.jpg");
	bg->setPosition(ccp(_screenSize.width * 0.5f, _screenSize.height * 0.5f));
	this->addChild(bg);

	//add snow particle
	_snow = CCParticleSystemQuad::create("snow.plist");
	_snow->setPosition(ccp(_screenSize.width * 0.5, _screenSize.height));
	this->addChild(_snow, kBackground);

	_layerBatchNode = CCSpriteBatchNode::create("sprite_sheet.png", 60);
	this->addChild(_layerBatchNode, kMiddleground);

	/*CCSprite * ground = CCSprite::createWithSpriteFrameName("intro_igloo.png");
	ground->setAnchorPoint(ccp(0.5f, 0.05f));
	ground->setPosition(ccp(_screenSize.width * 0.5f, 0.0));
	_layerBatchNode->addChild(ground);

	CCSprite * logo = CCSprite::createWithSpriteFrameName("logo.png");
	logo->setPosition(ccp(_screenSize.width * 0.5f, _screenSize.height * 0.82f));
	_layerBatchNode->addChild(logo);*/


	//build level picker menu
	int cols = 5;
	int rows = 5;
	int i = _firstIndex;

	CCSprite * menuItem;
	CCLabelBMFont * label;
	CCPoint position;

	_levelLabels = CCArray::createWithCapacity(LEVELS_PER_SCREEN);
	_levels = CCArray::createWithCapacity(LEVELS_PER_SCREEN);

	_levelLabels->retain();
	_levels->retain();

	for (int r = 0; r < rows; r++) {
		for (int c = 0; c < cols; c++) {
			position = ccp(_screenSize.width * 0.2f +
				c * _screenSize.width * 0.15f,
				_screenSize.height * 0.65f -
				_screenSize.width * 0.15f * r);
			if (i > _levelsCompleted) {
				menuItem = CCSprite::createWithSpriteFrameName("btn_num_on.png");
				menuItem->setTag(kTagButtonOn);
			}
			else {
				menuItem = CCSprite::createWithSpriteFrameName("btn_num_off.png");
				menuItem->setTag(kTagButtonOff);
			}
			menuItem->setPosition(position);
			_layerBatchNode->addChild(menuItem);
			//add the background sprite to an array so we track touch collision
			_levels->addObject(menuItem);

			char szValue[10] = { 0 };
			sprintf(szValue, "%i", (int)i);
			label = CCLabelBMFont::create(szValue, "font_levels.fnt", _screenSize.width * 0.1f);
			label->setPosition(position);
			//add the labels to an array so we can update them
			_levelLabels->addObject(label);

			this->addChild(label, kForeground);
			i++;
		}
	}

	//the nav buttons
	_btnBack = CCSprite::createWithSpriteFrameName("switch_3.png");
	_btnBack->setPosition(ccp(_screenSize.width * 0.15f, _screenSize.height * 0.1f));
	_layerBatchNode->addChild(_btnBack);

	_btnNext = CCSprite::createWithSpriteFrameName("switch_2.png");
	_btnNext->setPosition(ccp(_screenSize.width * 0.85f, _screenSize.height * 0.1f));
	_layerBatchNode->addChild(_btnNext);

	if (_firstIndex == TOTAL_LEVELS - LEVELS_PER_SCREEN + 1) {
		_btnNext->setVisible(false);
	}
}

//update the level labels with the current displayed range (1-25, 26-50, 51-75, 76-100)
void LevelSelectLayer::createMenu() {

	int count = _levelLabels->count();
	CCSprite * btn;
	CCLabelBMFont * label;
	int index = _firstIndex;
	for (int i = 0; i < count; i++) {
		label = (CCLabelBMFont *)_levelLabels->objectAtIndex(i);
		btn = (CCSprite *)_levels->objectAtIndex(i);
		if (index > _levelsCompleted) {
			btn->setDisplayFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("btn_num_on.png"));
			btn->setTag(kTagButtonOn);
		}
		else {
			btn->setDisplayFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("btn_num_off.png"));
			btn->setTag(kTagButtonOff);
		}

		char szValue[10] = { 0 };
		sprintf(szValue, "%i", index);
		label->setString(szValue);
		index++;

	}
	if (_firstIndex == TOTAL_LEVELS - LEVELS_PER_SCREEN + 1) {
		_btnNext->setVisible(false);
	}
}