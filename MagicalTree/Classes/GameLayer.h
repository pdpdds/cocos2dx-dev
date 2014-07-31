#pragma once
#include "GameDefines.h"
#include "cocos2d.h"
#include "Box2d/Box2D.h"
#include "SimpleAudioEngine.h"

class Player;

enum {
	TAG_CASTLE = 0,
	TAG_LIGHTNING_BOLT = 1,
	TAG_BATS = 2
};

USING_NS_CC;
using namespace CocosDenshion;

class GameLayer : public CCLayer
{
public:
	GameLayer();
	virtual ~GameLayer();

	CC_SYNTHESIZE(int, _levelsCompleted, LevelsCompleted);

	static GameLayer * create(int level, int levelsCompleted);
	static cocos2d::CCScene* scene(int level, int levelsCompleted);

	
	void loadLevel(int level);
	void newLevel(void);

	void checkForAndResolveCollisions(Player *p);
	CCArray* getSurroundingTilesAtPosition(CCPoint position, CCTMXLayer *pLayer);
	CCPoint tileCoordForPosition(CCPoint position);
	CCRect tileRectFromTileCoords(CCPoint tileCoords);

	void update(float dt);
	bool CheckIntersect(CCPoint position, CCTMXLayer *pLayer);

	virtual void ccTouchesBegan(cocos2d::CCSet* touches, cocos2d::CCEvent* event);
	virtual void ccTouchesMoved(CCSet *touches, CCEvent *event);
	virtual void ccTouchesEnded(CCSet *touches, CCEvent *event);

	void setViewpointCenter(CCPoint position);

	CCTMXTiledMap *_gameMap;
	CCTMXLayer *_treeLayer;
	CCTMXLayer *_ropeLayer;
	CCTMXLayer *_toeholdLayer;

	CCTMXObjectGroup *_enemyFreeLayer;
	CCTMXObjectGroup *_platformLayer;
	CCTMXObjectGroup *_itemLayer;
	int _platformCount;

	CCArray* _itemArray;
	CCArray* _platformArray;
	CCArray* _enemyArray;

	CCSprite *lightningBolt;
	CCSprite *lightningGlow;
	int lightningRemoveCount;

	void doRain();

private:
	void createScreen(void);
	void resetLevel(void);
	void levelCompleted(void);
	void clearLayer(void);

	int _currentLevel;

	Player* m_pPlayer;
	CCPoint _playerPos;
};

