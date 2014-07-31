#include "GameLevelLayer.h"
#include "Player.h"
#include "SimpleAudioEngine.h"
#include "IntVal.h"
#include <algorithm>

using namespace CocosDenshion;

GameLevelLayer::GameLevelLayer()
{
	
}


GameLevelLayer::~GameLevelLayer()
{
}

bool GameLevelLayer::init()
{
	if (!CCLayer::init())
	{
		return false;
	}

	CCLayerColor *blueSky = CCLayerColor::create(ccc4(100, 100, 250, 255));

	this->addChild(blueSky);

	
	SimpleAudioEngine::sharedEngine()->playBackgroundMusic("level1.mp3");

	m_pGameMap = CCTMXTiledMap::create("level1.tmx");
	this->addChild(m_pGameMap);

	this->setTouchEnabled(true);

	m_pWalls = m_pGameMap->layerNamed("walls");
	m_pHazards = m_pGameMap->layerNamed("hazards");

	
	m_pPlayer = new Player("koalio_stand.png");
	m_pPlayer->autorelease();
	
	m_pPlayer->setPosition(ccp(120, 50));
	m_pGameMap->addChild(m_pPlayer, 15);

	gameOver = false;
	//

	this->schedule(schedule_selector(GameLevelLayer::update));

	doRain();

	return true;
}

CCScene* GameLevelLayer::scene()
{
	// 'scene' is an autorelease object
	CCScene *scene = CCScene::create();

	// 'layer' is an autorelease object
	GameLevelLayer *pLayer = GameLevelLayer::create();

	// add layer as a child to scene
	scene->addChild(pLayer);

	// return the scene
	return scene;
}

void GameLevelLayer::update(float dt)
{
	if (gameOver) {
		return;
	}

	m_pPlayer->update(dt);
	checkForAndResolveCollisions(m_pPlayer);
	handleHazardCollisions(m_pPlayer);
	setViewpointCenter(m_pPlayer->getPosition());
}

CCPoint GameLevelLayer::tileCoordForPosition(CCPoint position)
{
	float x = floor(position.x / m_pGameMap->getTileSize().width);
	float levelHeightInPixels = m_pGameMap->getMapSize().height * m_pGameMap->getTileSize().height;
	float y = floor((levelHeightInPixels - position.y) / m_pGameMap->getTileSize().height);
	return ccp(x, y);
}

CCRect GameLevelLayer::tileRectFromTileCoords(CCPoint tileCoords)
{
	float levelHeightInPixels = m_pGameMap->getMapSize().height * m_pGameMap->getTileSize().height;
	CCPoint origin = ccp(tileCoords.x * m_pGameMap->getTileSize().width, levelHeightInPixels - ((tileCoords.y + 1) * m_pGameMap->getTileSize().height));
	return CCRectMake(origin.x, origin.y, m_pGameMap->getTileSize().width, m_pGameMap->getTileSize().height);
}

void GameLevelLayer::checkForAndResolveCollisions(Player *p)
{

	CCArray *tiles = getSurroundingTilesAtPosition(p->getPosition(), m_pWalls);

	
	p->onGround = false;

	CCObject* it = NULL;
	CCARRAY_FOREACH(tiles, it)
	{
		CCDictionary *dic = dynamic_cast<CCDictionary*>(it);
		CCRect pRect = p->collisionBoundingBox();
		
		CCInteger* pValue = (CCInteger*)dic->objectForKey("gid");
		
		if (pValue->getValue()) {
			CCFloat* pValue2 = (CCFloat*)dic->objectForKey("x");
			CCFloat* pValue3 = (CCFloat*)dic->objectForKey("y");
			
			CCRect tileRect = CCRectMake(pValue2->getValue(),
										 pValue3->getValue(),
										 m_pGameMap->getTileSize().width, 
										 m_pGameMap->getTileSize().height);

			if (pRect.intersectsRect(tileRect)) {

				 CCRect intersection = CCRectMake(max(pRect.getMinX(), tileRect.getMinX()), max(pRect.getMinY(), tileRect.getMinY()), 0, 0);
				intersection.size.width = min(pRect.getMaxX(), tileRect.getMaxX()) - intersection.getMinX();
				intersection.size.height = min(pRect.getMaxY(), tileRect.getMaxY()) - intersection.getMinY();
				//CCRect intersection = CCRectIntersection(pRect, tileRect);
				int tileIndx = tiles->indexOfObject(dic);

				if (tileIndx == 0) {
					//tile is directly below player
					p->desiredPosition = ccp(p->desiredPosition.x, p->desiredPosition.y + intersection.size.height);
					p->velocity = ccp(p->velocity.x, 0.0); //////Here
					p->onGround = true; //////Here
				}
				else if (tileIndx == 1) {
					//tile is directly above player
					p->desiredPosition = ccp(p->desiredPosition.x, p->desiredPosition.y - intersection.size.height);
					p->velocity = ccp(p->velocity.x, 0.0); //////Here
				}
				else if (tileIndx == 2) {
					//tile is left of player
					p->desiredPosition = ccp(p->desiredPosition.x + intersection.size.width, p->desiredPosition.y);
				}
				else if (tileIndx == 3) {
					//tile is right of player
					p->desiredPosition = ccp(p->desiredPosition.x - intersection.size.width, p->desiredPosition.y);
				}
				else {
					if (intersection.size.width > intersection.size.height) {
						//tile is diagonal, but resolving collision vertially
						p->velocity = ccp(p->velocity.x, 0.0); //////Here
						float resolutionHeight;
						if (tileIndx > 5) {
							resolutionHeight = intersection.size.height;
							p->onGround = true; //////Here
						}
						else {
							resolutionHeight = -intersection.size.height;
						}
						p->desiredPosition = ccp(p->desiredPosition.x, p->desiredPosition.y + resolutionHeight);

					}
					else {
						float resolutionWidth;
						if (tileIndx == 6 || tileIndx == 4) {
							resolutionWidth = intersection.size.width;
						}
						else {
							resolutionWidth = -intersection.size.width;
						}
						p->desiredPosition = ccp(p->desiredPosition.x + resolutionWidth, p->desiredPosition.y);
					}
				}
			}
		}
	}
	p->setPosition(p->desiredPosition);
}

CCArray* GameLevelLayer::getSurroundingTilesAtPosition(CCPoint position, CCTMXLayer *pLayer)
{

	CCPoint plPos = tileCoordForPosition(position);

	CCArray* gids = CCArray::create();
		
	for (int i = 0; i < 9; i++) { //3
		int c = i % 3;
		int r = (int)(i / 3);
		CCPoint tilePos = ccp(plPos.x + (c - 1), plPos.y + (r - 1));

		if (tilePos.y >(m_pGameMap->getMapSize().height - 1)) {
			//fallen in a hole
			gameover(0);
			return NULL;
		}

		int tgid = pLayer->tileGIDAt(tilePos);
			

		CCRect tileRect = tileRectFromTileCoords(tilePos);
		
		CCDictionary *tileDict = CCDictionary::create();
		CCInteger* pValue1 = CCInteger::create(tgid);
		CCFloat* pValue2 = CCFloat::create(tileRect.origin.x);
		CCFloat* pValue3 = CCFloat::create(tileRect.origin.y);

		CCFloat* pValue4 = CCFloat::create(tilePos.x);
		CCFloat* pValue5 = CCFloat::create(tilePos.y);
		
		tileDict->setObject(pValue1, "gid");
		tileDict->setObject(pValue2, "x");
		tileDict->setObject(pValue3, "y");
		tileDict->setObject(pValue4, "tilePos.x");
		tileDict->setObject(pValue5, "tilePos.y");
		

		gids->addObject(tileDict);

	}
	
	gids->removeObjectAtIndex(4);
	gids->insertObject(gids->objectAtIndex(2), 6);
	gids->removeObjectAtIndex(2);
	gids->exchangeObjectAtIndex(4, 6);
	gids->exchangeObjectAtIndex(0, 4);


	return gids;
}


void GameLevelLayer::ccTouchesBegan(CCSet *touches, CCEvent *event)
{
	
	CCSetIterator i;
	for (i = touches->begin(); i != touches->end(); i++)
	{
		CCTouch *t = (CCTouch*)*i;
		CCPoint touchLocation = convertTouchToNodeSpace(t);
		if (touchLocation.x > 240) {
			m_pPlayer->mightAsWellJump = true;
		}
		else {
			m_pPlayer->forwardMarch = true;
		}
	}
	
}

void GameLevelLayer::ccTouchesMoved(CCSet *touches, CCEvent *event)
{
	CCSetIterator i;
	for (i = touches->begin(); i != touches->end(); i++)
	{
		CCTouch *t = (CCTouch*)*i;

		CCPoint touchLocation = convertTouchToNodeSpace(t);

		//get previous touch and convert it to node space
		CCPoint previousTouchLocation = t->getPreviousLocationInView();
		CCSize screenSize = CCDirector::sharedDirector()->getWinSize();
		previousTouchLocation = ccp(previousTouchLocation.x, screenSize.height - previousTouchLocation.y);

		if (touchLocation.x > 240 && previousTouchLocation.x <= 240) {
			m_pPlayer->forwardMarch = false;
			m_pPlayer->mightAsWellJump = true;
		}
		else if (previousTouchLocation.x > 240 && touchLocation.x <= 240) {
			m_pPlayer->forwardMarch = true;
			m_pPlayer->mightAsWellJump = false;
		}
	}
}

void GameLevelLayer::ccTouchesEnded(CCSet *touches, CCEvent *event)
{

	CCSetIterator i;
	for (i = touches->begin(); i != touches->end(); i++)
	{
		CCTouch *t = (CCTouch*)*i;
		CCPoint touchLocation = convertTouchToNodeSpace(t);
		if (touchLocation.x < 240) {
			m_pPlayer->forwardMarch = false;
		}
		else {
			m_pPlayer->mightAsWellJump = false;
		}
	}
}

void GameLevelLayer::setViewpointCenter(CCPoint position)
{

	CCSize winSize = CCDirector::sharedDirector()->getWinSize();

	int x = MAX(position.x, winSize.width / 2);
	int y = MAX(position.y, winSize.height / 2);
	x = MIN(x, (m_pGameMap->getMapSize().width * m_pGameMap->getTileSize().width)
		- winSize.width / 2);
	y = MIN(y, (m_pGameMap->getMapSize().height * m_pGameMap->getTileSize().height)
		- winSize.height / 2);
	CCPoint actualPosition = ccp(x, y);

	CCPoint centerOfView = ccp(winSize.width / 2, winSize.height / 2);
	CCPoint viewPoint = ccpSub(centerOfView, actualPosition);
	m_pGameMap->setPosition(viewPoint);	
}

void GameLevelLayer::handleHazardCollisions(Player *p)
{
	CCArray *tiles = getSurroundingTilesAtPosition(p->getPosition(), m_pHazards);
	CCObject* it = NULL;
	CCARRAY_FOREACH(tiles, it)
	{
		CCDictionary *dic = dynamic_cast<CCDictionary*>(it);

		CCFloat* pValue2 = (CCFloat*)dic->objectForKey("x");
		CCFloat* pValue3 = (CCFloat*)dic->objectForKey("y");

		CCRect tileRect = CCRectMake(pValue2->getValue(),
			pValue3->getValue(),
			m_pGameMap->getTileSize().width,
			m_pGameMap->getTileSize().height);

		CCRect pRect = p->collisionBoundingBox();

		CCInteger* pValue = (CCInteger*)dic->objectForKey("gid");
		if (pValue->getValue() != 0 && pRect.intersectsRect(tileRect))
		{
			gameover(0);
			break;
		}
		

	}
}

void GameLevelLayer::gameover(bool won)
{
	gameOver = true;
	CCString gameText;

	if (won) {
		gameText = "You Won!";
	}
	else {
		gameText = "You have Died!";
		SimpleAudioEngine::sharedEngine()->playEffect("hurt.wav");
	}
	
	CCLabelTTF *diedLabel = CCLabelTTF::create(gameText.getCString(), "Marker Felt", 40);
	diedLabel->setPosition(ccp(240, 200));
	CCMoveBy *slideIn = CCMoveBy::create(1.0f, ccp(0, 250));
	CCMenuItemImage *replay = CCMenuItemImage::create("replay.png",
		"replay.png",
		"replay.png", this, menu_selector(GameLevelLayer::menuCallback));
													

	
	CCArray *menuItems = CCArray::createWithObject(replay);
	CCMenu *menu = CCMenu::createWithArray(menuItems);
	menu->setPosition(ccp(240, -100));

	addChild(menu);
	addChild(diedLabel);

	menu->runAction(slideIn);
}

void GameLevelLayer::menuCallback(CCObject * pSender)
{
	CCMenuItem* pMenuItem = (CCMenuItem *)(pSender);
	//int tag = (int)pMenuItem->getTag();

	CCDirector::sharedDirector()->replaceScene(GameLevelLayer::scene());

	
}

void GameLevelLayer::doRain()
{
	CCParticleRain* m_emitter = CCParticleRain::create();
	m_emitter->retain();
	CCPoint p = m_emitter->getPosition();
	//m_emitter->setPosition(ccp(p.x, p.y - 100));
	m_emitter->setLife(4);

	m_emitter->setTexture((CCTextureCache::sharedTextureCache()->addImage("fire.png")));
	if (m_emitter != NULL)
	{
		CCSize s = CCDirector::sharedDirector()->getWinSize();
		m_emitter->setPosition(ccp(s.width/2, s.height));

		this->addChild(m_emitter);
	}
}