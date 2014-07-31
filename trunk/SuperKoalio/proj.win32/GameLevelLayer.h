#ifndef __GAMELEVEL_LAYER_H__
#define __GAMELEVEL_LAYER_H__

#include "cocos2d.h"
#include "tilemap_parallax_nodes/CCTMXTiledMap.h"

USING_NS_CC;

class Player;

class GameLevelLayer : public cocos2d::CCLayer
{
public:
	GameLevelLayer();
	virtual ~GameLevelLayer();

	virtual bool init();
	static cocos2d::CCScene* scene();

	CREATE_FUNC(GameLevelLayer);
	void update(float dt);

	virtual void ccTouchesBegan(cocos2d::CCSet* touches, cocos2d::CCEvent* event);
	virtual void ccTouchesMoved(CCSet *touches, CCEvent *event);
	virtual void ccTouchesEnded(CCSet *touches, CCEvent *event);
	void setViewpointCenter(CCPoint position);
	void handleHazardCollisions(Player *p);
	void gameover(bool won);
	void menuCallback(CCObject * pSender);

	CCArray* getSurroundingTilesAtPosition(CCPoint position, CCTMXLayer *pLayer);
	void checkForAndResolveCollisions(Player *p);
	CCPoint tileCoordForPosition(CCPoint position);
	CCRect tileRectFromTileCoords(CCPoint tileCoords);

	void doRain();

protected:

private:
	CCTMXTiledMap* m_pGameMap;

	Player *m_pPlayer;
	CCTMXLayer *m_pWalls;
	CCTMXLayer *m_pHazards;
	bool gameOver;
};

#endif

