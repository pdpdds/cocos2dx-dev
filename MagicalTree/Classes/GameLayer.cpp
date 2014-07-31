#include "GameLayer.h"
#include "Player.h"
#include "PlayerFSM.h"

static GameLayer* _instance = NULL;

GameLayer::GameLayer()
{
}


GameLayer::~GameLayer()
{
	CC_SAFE_RELEASE(_itemLayer);
	CC_SAFE_RELEASE(_itemArray);
}

GameLayer * GameLayer::create(int level, int levelsCompleted) {
	if (!_instance) {
		_instance = new GameLayer();
	}
	else {
		_instance->clearLayer();
	}
	_instance->setLevelsCompleted(levelsCompleted);
	_instance->loadLevel(level);
	_instance->scheduleUpdate();

	_instance->schedule(schedule_selector(GameLayer::update));
	
	return _instance;
}

void GameLayer::loadLevel(int level) {

	clearLayer();

	SimpleAudioEngine::sharedEngine()->playBackgroundMusic("bg.mp3");

	setTouchEnabled(true);

	//CCLayerColor *blueSky = CCLayerColor::create(ccc4(100, 100, 250, 255));

	//this->addChild(blueSky);

	CCSize screenSize = CCDirector::sharedDirector()->getWinSize();

	CCSprite * bg = CCSprite::create("bg.png");
	bg->setPosition(ccp(screenSize.width * 0.5f, screenSize.height * 0.5f));
	this->addChild(bg);

	//Add a lightning bolt
	lightningBolt = CCSprite::create("lightning_bolt.png");
	lightningBolt->setPosition(ccp(140, 160));
	lightningBolt->setOpacity(64);
	lightningBolt->retain();

	//Add a sprite to make it light up other areas.
	//lightningGlow = CCSprite::create("lightning_glow.png");
	//lightningGlow->setColor(ccc3(255, 255, 0));
	//lightningGlow->setPosition(ccp(240, 160));
	//lightningGlow->setOpacity(100);
	//lightningGlow->setBlendFunc( (ccBlendFunc) { GL_ONE, GL_ONE });
	//lightningBolt->addChild(lightningGlow);

	//Set a counter for lightning duration randomization
	lightningRemoveCount = 0;


	_currentLevel = level;

	//Setup Map
	_gameMap = CCTMXTiledMap::create("level1.tmx");
	_gameMap->setAnchorPoint(ccp(0, 0));
	
	// Set all layers to AntiAlias

	CCObject* it = NULL;
	CCARRAY_FOREACH(_gameMap->getChildren(), it)
	{
		CCTMXLayer *child = dynamic_cast<CCTMXLayer*>(it);
		child->getTexture()->setAliasTexParameters();
	}

	// Set up all the map layers
	_treeLayer = _gameMap->layerNamed("TreeLayer");
	_ropeLayer = _gameMap->layerNamed("RopeLayer");
	_toeholdLayer = _gameMap->layerNamed("ToeholdLayer");
	CCTMXObjectGroup* pPlayerLayer = _gameMap->objectGroupNamed("PlayerLayer");

	CCArray* pArray = pPlayerLayer->getObjects();

	CCARRAY_FOREACH(pArray, it)
	{
		CCDictionary *objects = dynamic_cast<CCDictionary*>(it);

		if (objects != NULL)
		{
			// Get values
			CCString* x = (CCString *)objects->valueForKey("x");
			
			CCString* y = (CCString *)objects->valueForKey("y");
			_playerPos.setPoint(x->intValue(), y->intValue());
			
			//CCString* width = (CCString *)objects->valueForKey("width");
			//CCString* height = (CCString *)objects->valueForKey("height");

			//CCLog("Object x=%s y=%s width=%s height=%s", x->getCString(), y->getCString(), width->getCString(), height->getCString());
		}
	}

//////////////////////////////////////////////////////////////////////////////////
//아이템 정보 얻어오기
	
	_itemLayer = _gameMap->objectGroupNamed("ItemLayer");
	_itemLayer->retain();

	_itemArray = CCArray::create();
	_itemArray->retain();

	pArray = _itemLayer->getObjects();

	CCARRAY_FOREACH(pArray, it)
	{
		CCDictionary *objects = dynamic_cast<CCDictionary*>(it);

		if (objects != NULL)
		{
			// Get values
			CCString* itemname = (CCString *)objects->valueForKey("ItemName");
			if (itemname->compare("flower") == 0)
			{
				CCSprite * flower = CCSprite::create("flower.png");
				CCString* x = (CCString *)objects->valueForKey("x");
				CCString* y = (CCString *)objects->valueForKey("y");
				flower->setPosition(ccp(x->intValue(), y->intValue()));
				_gameMap->addChild(flower, 14);
				_itemArray->addObject(flower);

			}
			
		}
	}


//////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////
//플랫폼 정보 얻어오기

	_platformLayer = _gameMap->objectGroupNamed("MovingPlatformLayer");
	_platformLayer->retain();

	_platformArray = CCArray::create();
	_platformArray->retain();

	pArray = _platformLayer->getObjects();
	_platformCount = 0;

	CCARRAY_FOREACH(pArray, it)
	{
		CCDictionary *objects = dynamic_cast<CCDictionary*>(it);

		if (objects != NULL)
		{
			// Get values
			CCString* itemname = (CCString *)objects->valueForKey("name");
			if (itemname->compare("SimplePlatform") == 0)
			{
				CCSprite * platform = CCSprite::create("platform1.png");
				CCString* x = (CCString *)objects->valueForKey("x");
				CCString* y = (CCString *)objects->valueForKey("y");
				platform->setPosition(ccp(x->intValue(), y->intValue()));
				_gameMap->addChild(platform, 14, 20 + _platformCount);
				_platformArray->addObject(platform);
				_platformCount++;

			}

		}
	}


//////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////
//적 정보 얻어오기

	_enemyFreeLayer = _gameMap->objectGroupNamed("EnemyFreeLayer");
	_enemyFreeLayer->retain();

	_enemyArray = CCArray::create();
	_enemyArray->retain();

	pArray = _enemyFreeLayer->getObjects();

	CCARRAY_FOREACH(pArray, it)
	{
		CCDictionary *objects = dynamic_cast<CCDictionary*>(it);

		if (objects != NULL)
		{
			// Get values
			CCString* itemname = (CCString *)objects->valueForKey("name");
			if (itemname->compare("Cloud") == 0)
			{
				CCSprite * cloud = CCSprite::create("cloud1.png");
				CCString* x = (CCString *)objects->valueForKey("x");
				CCString* y = (CCString *)objects->valueForKey("y");
				cloud->setPosition(ccp(x->intValue(), y->intValue()));
				_gameMap->addChild(cloud, 16);
				_platformArray->addObject(cloud);

			}

		}
	}


//////////////////////////////////////////////////////////////////////////////////

	CCParallaxNode *voidNode = CCParallaxNode::create();

	voidNode->addChild(_gameMap, 1, ccp(1.0f, 1.0f), ccp(0, 0));
	addChild(voidNode, 1, 2);

	m_pPlayer = new Player("koalio_stand.png", this);
	m_pPlayer->autorelease();

	m_pPlayer->setPosition(ccp(_playerPos.x, _playerPos.y));
	m_pPlayer->SetDesiredPosition(ccp(_playerPos.x, _playerPos.y));
	//m_pPlayer->setScale(2.0f);
	_gameMap->addChild(m_pPlayer, 15);

	//_gameMap->setScale(2.0f);
	doRain();
}

void GameLayer::newLevel(void)
{

}

void GameLayer::createScreen(void)
{

}

void GameLayer::resetLevel(void)
{

}

void GameLayer::doRain()
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
		m_emitter->setPosition(ccp(s.width / 2, s.height));

		this->addChild(m_emitter, 16);
	}
}

void GameLayer::levelCompleted(void)
{

}
void GameLayer::clearLayer(void)
{

}

CCScene* GameLayer::scene(int level, int levelsCompleted)
{
	// 'scene' is an autorelease object
	CCScene *scene = CCScene::create();

	// add layer as a child to scene
	scene->addChild(GameLayer::create(level, levelsCompleted));

	return scene;
}

bool GameLayer::CheckIntersect(CCPoint position, CCTMXLayer *pLayer)
{
	CCPoint plPos = tileCoordForPosition(position);

	int tgid = pLayer->tileGIDAt(plPos);

	if (tgid > 0)
		return true;

	return false;
}

CCArray* GameLayer::getSurroundingTilesAtPosition(CCPoint position, CCTMXLayer *pLayer)
{

	CCPoint plPos = tileCoordForPosition(position);

	CCArray* gids = CCArray::create();

	for (int i = 0; i < 9; i++) { //3
		int c = i % 3;
		int r = (int)(i / 3);
		CCPoint tilePos = ccp(plPos.x + (c - 1), plPos.y + (r - 1));

		if (tilePos.y >(_gameMap->getMapSize().height - 1)) {
			//fallen in a hole
		//	gameover(0);
			return NULL;
		}


		if (tilePos.x < 0 || tilePos.x >= _gameMap->getMapSize().width)
		{
	
			//CCRect tileRect = tileRectFromTileCoords(tilePos);

			CCDictionary *tileDict = CCDictionary::create();
			CCInteger* pValue1 = CCInteger::create(-1);
			CCFloat* pValue2 = CCFloat::create(-1);
			CCFloat* pValue3 = CCFloat::create(-1);

			CCFloat* pValue4 = CCFloat::create(-1);
			CCFloat* pValue5 = CCFloat::create(-1);

			tileDict->setObject(pValue1, "gid");
			tileDict->setObject(pValue2, "x");
			tileDict->setObject(pValue3, "y");
			tileDict->setObject(pValue4, "tilePos.x");
			tileDict->setObject(pValue5, "tilePos.y");


			gids->addObject(tileDict);

			continue;
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


CCPoint GameLayer::tileCoordForPosition(CCPoint position)
{
	float x = floor(position.x / _gameMap->getTileSize().width);
	float levelHeightInPixels = _gameMap->getMapSize().height * _gameMap->getTileSize().height;
	float y = floor((levelHeightInPixels - position.y) / _gameMap->getTileSize().height);
	return ccp(x, y);
}

CCRect GameLayer::tileRectFromTileCoords(CCPoint tileCoords)
{
	float levelHeightInPixels = _gameMap->getMapSize().height * _gameMap->getTileSize().height;
	CCPoint origin = ccp(tileCoords.x * _gameMap->getTileSize().width, levelHeightInPixels - ((tileCoords.y + 1) * _gameMap->getTileSize().height));
	return CCRectMake(origin.x, origin.y, _gameMap->getTileSize().width, _gameMap->getTileSize().height);
}

#ifdef _WIN32
//this implementation till I find (or create) better
int arc4random() {
	return rand();
}
#endif

void GameLayer::update(float dt)
{

	m_pPlayer->update(dt);
	m_pPlayer->CheckCollisonWithItem(this);
	m_pPlayer->CheckCollison(this);
	//checkForAndResolveCollisions(m_pPlayer);
	setViewpointCenter(m_pPlayer->getPosition());

	//Randomly make lightning strike
	if (arc4random() % 70 == 7){
		if (lightningRemoveCount < 0){
			this->addChild(lightningBolt, 1, TAG_LIGHTNING_BOLT);
			lightningRemoveCount = arc4random() % 5 + 5;
		}
	}

	//Count down
	lightningRemoveCount -= 1;

	//Clean up any old lightning bolts
	if (lightningRemoveCount == 0){
		this->removeChildByTag(TAG_LIGHTNING_BOLT, false);
	}
}

void GameLayer::checkForAndResolveCollisions(Player *p)
{
	CCArray *tiles = getSurroundingTilesAtPosition(p->getPosition(), _toeholdLayer);

	//p->onGround = false;

	CCObject* it = NULL;
	CCARRAY_FOREACH(tiles, it)
	{
		CCDictionary *dic = dynamic_cast<CCDictionary*>(it);
		CCRect pRect = p->boundingBox();

		CCInteger* pValue = (CCInteger*)dic->objectForKey("gid");

		if (pValue->getValue() && pValue->getValue() != -1) {
			CCFloat* pValue2 = (CCFloat*)dic->objectForKey("x");
			CCFloat* pValue3 = (CCFloat*)dic->objectForKey("y");

			CCRect tileRect = CCRectMake(pValue2->getValue(),
				pValue3->getValue(),
				_gameMap->getTileSize().width,
				_gameMap->getTileSize().height);

			if (pRect.intersectsRect(tileRect)) {

				CCRect intersection = CCRectMake(max(pRect.getMinX(), tileRect.getMinX()), max(pRect.getMinY(), tileRect.getMinY()), 0, 0);
				intersection.size.width = min(pRect.getMaxX(), tileRect.getMaxX()) - intersection.getMinX();
				intersection.size.height = min(pRect.getMaxY(), tileRect.getMaxY()) - intersection.getMinY();
				//CCRect intersection = CCRectIntersection(pRect, tileRect);
				int tileIndx = tiles->indexOfObject(dic);

				if (tileIndx == 0) {
					//tile is directly below player
					p->SetDesiredPosition(ccp(p->desiredPosition.x, p->desiredPosition.y + intersection.size.height));
					
					if (p->velocity.y <= 0)
					{

						p->velocity = ccp(p->velocity.x, 0.0); //////Here

						if (p->GetFSM()->GetState() != PLAYER_GROUND)
							p->GetFSM()->ChangeState(PLAYER_GROUND);
					}
				}
				else if (tileIndx == 1) {
					//tile is directly above player
				//	p->desiredPosition = ccp(p->desiredPosition.x, p->desiredPosition.y - intersection.size.height);
				//	p->velocity = ccp(p->velocity.x, 0.0); //////Here
				}
				else if (tileIndx == 2) {
					//tile is left of player
			//		p->desiredPosition = ccp(p->desiredPosition.x + intersection.size.width, p->desiredPosition.y);
				}
				else if (tileIndx == 3) {
					//tile is right of player
				//	p->desiredPosition = ccp(p->desiredPosition.x - intersection.size.width, p->desiredPosition.y);
				}
				else {
					/*if (intersection.size.width > intersection.size.height) {
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
					}*/
				}
			}
		}
	}
	p->setPosition(p->desiredPosition);
}

void GameLayer::ccTouchesBegan(CCSet *touches, CCEvent *event)
{
	
	CCSetIterator i;
	for (i = touches->begin(); i != touches->end(); i++)
	{
		CCTouch *t = (CCTouch*)*i;
		CCPoint touchLocation = convertTouchToNodeSpace(t);

		CCSize winSize = CCDirector::sharedDirector()->getWinSize();

		int x = MAX(m_pPlayer->getPosition().x, winSize.width / 2);
		int y = MAX(m_pPlayer->getPosition().y, winSize.height / 2);
		x = MIN(x, (_gameMap->getMapSize().width * _gameMap->getTileSize().width)
			- winSize.width / 2);
		y = MIN(y, (_gameMap->getMapSize().height * _gameMap->getTileSize().height)
			- winSize.height / 2);
		CCPoint actualPosition = ccp(x, y);

		CCPoint centerOfView = ccp(winSize.width / 2, winSize.height / 2);
		CCPoint viewPoint = ccpSub(centerOfView, actualPosition);

		touchLocation = ccpAdd(touchLocation, -viewPoint);

		m_pPlayer->GetFSM()->OnTouch(touchLocation);
		return;
	}
}

void GameLayer::ccTouchesMoved(CCSet *touches, CCEvent *event)
{
	CCSetIterator i;
	for (i = touches->begin(); i != touches->end(); i++)
	{
		CCTouch *t = (CCTouch*)*i;
		CCPoint touchLocation = convertTouchToNodeSpace(t);
		m_pPlayer->GetFSM()->OnMoved(touchLocation);
		return;
	}

	/*CCSetIterator i;
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
	}*/
}

void GameLayer::ccTouchesEnded(CCSet *touches, CCEvent *event)
{
	CCSetIterator i;
	for (i = touches->begin(); i != touches->end(); i++)
	{
		CCTouch *t = (CCTouch*)*i;
		CCPoint touchLocation = convertTouchToNodeSpace(t);
		m_pPlayer->GetFSM()->OnTouchEnd(touchLocation);
		return;
	}

	m_pPlayer->mightAsWellJump = false;
	m_pPlayer->forwardMarch = 0;
	/*CCSetIterator i;
	for (i = touches->begin(); i != touches->end(); i++)
	{
		CCTouch *t = (CCTouch*)*i;
		CCPoint touchLocation = convertTouchToNodeSpace(t);
		if (touchLocation.x < 240) {
			m_pPlayer->forwardMarch = false;
		}
		else {
			
		}
	}

	m_pPlayer->mightAsWellJump = false;*/
}

void GameLayer::setViewpointCenter(CCPoint position)
{

	CCSize winSize = CCDirector::sharedDirector()->getWinSize();

	int x = MAX(position.x, winSize.width / 2);
	int y = MAX(position.y, winSize.height / 2);
	x = MIN(x, (_gameMap->getMapSize().width * _gameMap->getTileSize().width)
		- winSize.width / 2);
	y = MIN(y, (_gameMap->getMapSize().height * _gameMap->getTileSize().height)
		- winSize.height / 2);
	CCPoint actualPosition = ccp(x, y);

	CCPoint centerOfView = ccp(winSize.width / 2, winSize.height / 2);
	CCPoint viewPoint = ccpSub(centerOfView, actualPosition);
	_gameMap->setPosition(viewPoint);
}