#ifndef __HELLO_WORLD_H__
#define __HELLO_WORLD_H__

#include "cocos2d.h"
#include "tilemap_parallax_nodes/CCTMXTiledMap.h"

USING_NS_CC;
// HelloWorld Layer
class HelloWorld : public CCLayer
{
public:
	CCTMXTiledMap *collisionMap;
	CCTMXTiledMap *foregroundMap;
	CCTMXLayer *colLayer;
	CCTMXLayer *flowerLayer;
	CCTMXLayer *baddiesLayer;
	BOOL mapMovingX, mapMovingY, playerHit;
	CCSprite *ball;
	CCSprite *baddie;
	CCSprite *flyBaddie;
	int ballWidth;
	int ballHeight;
	int tblX, tblY;
	int flowerCount;
	int c;
	
	int pos1X;
	int pos1Y; 
	int pos2X;
		int pos2Y;
		int pos3X; 
		int pos3Y;
		int pos4X;
	int pos4Y;
	CCLabelTTF *scoreLabel;
	CCLabelTTF *healthLabel;
	
	CCArray *_baddies;
	int health;
	CCRect ballRect;
	int enemyStartX, enemyStartY;

	void addFlyEnemy(CCPoint aPosition);
	void addEnemy(CCPoint aPosition);
	void playerHitDone(CCNode* pSender);
	bool init();

	static cocos2d::CCScene* scene();
	CREATE_FUNC(HelloWorld);
	void update(float dt);

	bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
	void registerWithTouchDispatcher();

	
};

#endif