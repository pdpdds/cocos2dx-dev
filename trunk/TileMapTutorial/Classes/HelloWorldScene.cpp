
#include "HelloWorldScene.h"

enum {
	kTagNode =1,
	kTagFore =2,
};


CCScene* HelloWorld::scene()
{
	// 'scene' is an autorelease object.
	CCScene *scene = CCScene::create();
	
	// 'layer' is an autorelease object.
	HelloWorld *layer = HelloWorld::create();
	
	// add layer as a child to scene
	scene->addChild(layer);
	
	// return the scene
	return scene;
}



void HelloWorld::addEnemy(CCPoint aPosition)
{
	baddie = CCSprite::create("Target.png");
	baddie->setAnchorPoint(ccp(0, 0));
	baddie->setPosition(ccp(enemyStartX*collisionMap->getTileSize().width, 
		                ((collisionMap->getMapSize().height*collisionMap->getTileSize().height) - collisionMap->getTileSize().height) - 
						 (enemyStartY*collisionMap->getTileSize().height)));
	collisionMap->addChild(baddie);

	baddie->setTag(2);
	_baddies->addObject(baddie);
	// Create the actions for baddie
	CCMoveBy* actionMove = CCMoveBy::create(3, ccp(-100, 0));
	baddie->runAction(CCRepeatForever::create(CCSequence::create(actionMove, actionMove->reverse(), NULL)));
}

void HelloWorld::addFlyEnemy(CCPoint aPosition)
{
	flyBaddie = CCSprite::create("Target.png");
	flyBaddie->setAnchorPoint( ccp(0, 0));

	flyBaddie->setPosition(ccp(enemyStartX*collisionMap->getTileSize().width,
		((collisionMap->getMapSize().height*collisionMap->getTileSize().height) - collisionMap->getTileSize().height) -
		(enemyStartY*collisionMap->getTileSize().height)));


	collisionMap->addChild(flyBaddie);

	flyBaddie->setTag(3);
	_baddies->addObject(flyBaddie);

	// Create the actions for baddie
	CCMoveBy* actionMove1 = CCMoveBy::create(1, ccp(-30, 30));
	CCMoveBy* actionMove2 = CCMoveBy::create(1, ccp(-30, -30));
	CCMoveBy* actionMove3 = CCMoveBy::create(1, ccp(-30, 30));
	flyBaddie->runAction(CCRepeatForever::create(CCSequence::create(actionMove1, actionMove2, actionMove3, actionMove1->reverse(), actionMove2->reverse(), actionMove3->reverse(), NULL)));
}

bool HelloWorld::init()
{
	if (!CCLayer::init())
	{
		return false;
	}

	setTouchEnabled(true);

	// ask Director for the window size
	CCSize winSize = CCDirector::sharedDirector()->getWinSize();

	_baddies = CCArray::create();
	_baddies->init();
		
	health = 5;

	//Setup Map
	collisionMap = CCTMXTiledMap::create("TileMapTest.tmx");
	collisionMap->setAnchorPoint(ccp(0, 0));
	// Set all layers to AntiAlias

	CCObject* it = NULL;
	CCARRAY_FOREACH(collisionMap->getChildren(), it)
	{
		CCTMXLayer *child = dynamic_cast<CCTMXLayer*>(it);
		child->getTexture()->setAliasTexParameters();
	}	

	// Set up all the map layers
	colLayer = collisionMap->layerNamed("Tiles");
	flowerLayer = collisionMap->layerNamed("Flowers");
	/*baddiesLayer = collisionMap->layerNamed ("Baddies");


	for (int y = 0; y < baddiesLayer->getLayerSize().height; y++){
		for (int x = 0; x < baddiesLayer->getLayerSize().width; x++){
			baddiesLayer->tileAt(ccp(x, y))->setOpacity(0);
		}
	}*/
	mapMovingX = true;
	mapMovingY = true;

	// Background Image
	CCSprite *background = CCSprite::create("background.png");
	background->setScale(1.0f);
	background->setAnchorPoint(ccp(0, 0));

	// Setup foreground Map
	foregroundMap = CCTMXTiledMap::create("ForeGroundTest.tmx");
	foregroundMap->setAnchorPoint(ccp(0, 0));
	
	
	// Set all layers to AntiAlias
	CCObject* iter = NULL;
	CCARRAY_FOREACH(foregroundMap->getChildren(), iter)
	{
		CCTMXLayer *child = dynamic_cast<CCTMXLayer*>(iter);
		child->getTexture()->setAliasTexParameters();
	}

	// Set up all the map layers
	CCTMXLayer *layer = foregroundMap->layerNamed("Layer 0");
	for (int y = 0; y < layer->getLayerSize().height; y++){
		for (int x = 0; x < layer->getLayerSize().width; x++){
			CCSprite* pSprite = layer->tileAt(ccp(x, y));
			if (pSprite)
				pSprite->setOpacity(255 * 0.7);
		}
	}


	// Setup player
	ball = CCSprite::create("ball.png");
	//ball.position = ccp(240,50);
	ball->setAnchorPoint(ccp(0, 0));
	ballRect = CCRectMake(0, 0, 32, 32);
	ball->setScaleX(1);
	ball->setScaleY(1);
	ballWidth = 32;
	ballHeight = 32;
	tblX = 0;
	tblX = 0;
	for (int y = 0; y < colLayer->getLayerSize().height; y++){
		for (int x = 0; x < colLayer->getLayerSize().width; x++){
			unsigned int gid = colLayer->tileGIDAt(ccp(x, y));
			if (gid == 193){
				ball->setPosition(ccp(x*collisionMap->getTileSize().width, (((collisionMap->getMapSize().height*collisionMap->getTileSize().height)
					- collisionMap->getTileSize().height) - (y*collisionMap->getTileSize().height))));
			}
		}
	}

	// Create a void Node, parent Node
	CCParallaxNode *voidNode = CCParallaxNode::create();

	// Now we add our children "layers" to void Node

	// Background Image
	voidNode->addChild(background, -1, ccp(0.4f, 0.5f), ccp(0, 0));

	// TileMap
	voidNode->addChild(collisionMap, 1, ccp(1.0f, 1.0f), ccp(0, 0));

	// Create the Baddies
	/*for (int y = 0; y < baddiesLayer->getLayerSize().height; y++){
		for (int x = 0; x < baddiesLayer->getLayerSize().width; x++){
			unsigned int gid = baddiesLayer->tileGIDAt(ccp(x, y));
			if (gid == 169){
				enemyStartX = x;
				enemyStartY = y;
				addEnemy( ccp(enemyStartX, enemyStartY));
			}
			if (gid == 151){
				enemyStartX = x;
				enemyStartY = y;
				addFlyEnemy(ccp(enemyStartX, enemyStartY));
			}
		}
	}
	*/
	// Setup a label for display of score
	//scoreLabel = CCLabelTTF::create[NSString stringWithFormat : @"Flowers: 0"] dimensions : CGSizeMake(130, 25) alignment : UITextAlignmentCenter fontName : @"Marker Felt" fontSize:20.0] retain];
	//scoreLabel->setPosition(ccp(winSize.width - (scoreLabel.contentSize.width), winSize.height - (scoreLabel.contentSize.height / 2)));

	// Setup a label for display of score
	//healthLabel = CCLabelTTF::create[NSString stringWithFormat : @"Health: %d", health] dimensions : CGSizeMake(130, 25) alignment : UITextAlignmentCenter fontName : @"Marker Felt" fontSize:20.0] retain];
	//healthLabel.position = ccp(50, winSize.height - (scoreLabel.contentSize.height / 2));

	//Add everything to self
	//addChild(scoreLabel ,10);
	//addChild(healthLabel, 10);

	addChild(voidNode, 1 , kTagNode);
	addChild(ball, 2);
	addChild(foregroundMap, 10, kTagFore);


	this->schedule(schedule_selector(HelloWorld::update));

	return true;
}

void HelloWorld::update(float adelta)
{
	CCPoint nodeSpace0 = collisionMap->convertToNodeSpace(ccp(ball->getPosition().x, ball->getPosition().y));

	ballRect = CCRectMake(nodeSpace0.x, nodeSpace0.y, ball->getContentSize().width, ball->getContentSize().height);
	
	CCObject* it = NULL;
	/*CCARRAY_FOREACH(_baddies, it)
	{
		CCSprite *baddies = dynamic_cast<CCSprite*>(it);

		CCRect baddiesRect = CCRectMake(
			baddies->getPosition().x,
			baddies->getPosition().y,
			baddies->getContentSize().width,
			baddies->getContentSize().height);

		if (ballRect.intersectsRect(baddiesRect)) {
			if (!playerHit) {
				playerHit = true;
				health--;
				//[healthLabel setString : [NSString stringWithFormat : @"Health: %d", health]];
				ball->runAction(CCSequence::create(
					CCFadeTo::create(0.3,0.7),
					CCFadeIn::create(0.3),
					CCFadeTo::create(0.3, 0.7),
					CCFadeIn::create(0.3),
					CCFadeTo::create(0.3, 0.7),
					CCFadeIn::create(0.3),
					CCFadeTo::create(0.3, 0.7),
					CCFadeIn::create(0.3),
					CCFadeTo::create(0.3, 0.7),
					CCFadeIn::create(0.3),
					CCFadeTo::create(0.3, 0.7),
					CCFadeIn::create(0.3),
					CCDelayTime::create(1.0),
					CCCallFuncN::create(this, callfuncN_selector(HelloWorld::playerHitDone)),
					NULL));
			}
		}
	}*/


	// Move the TileMap
	CCPoint diff = ccp(-tblX*adelta, -tblY*adelta);
	CCNode* node = getChildByTag(kTagNode);
	CCNode* foreNode = getChildByTag(kTagFore);

	CCPoint currentPos = node->getPosition();
	CCPoint newPos = ccpAdd(currentPos, diff);
	if (newPos.x > 0 || newPos.x < -160) {
		newPos.x = currentPos.x;
		mapMovingX = false;
	}
	else {
		mapMovingX = true;
	}
	if (newPos.y > 0 || newPos.y < -160) {
		newPos.y = currentPos.y;
		mapMovingY = false;
	}
	else {
		mapMovingY = true;
	}
	node->setPosition(newPos);
	newPos.x = newPos.x * 1.35;
	foreNode->setPosition(newPos);


	// Move the Player
	if (mapMovingY && mapMovingX) {
		ball->setPosition(ccp(ball->getPosition().x + ((tblX*adelta)*0.75), ball->getPosition().y + ((tblY*adelta)*0.75)));
	}
	else if (!mapMovingX){
		ball->setPosition(ccp(ball->getPosition().x + ((tblX*adelta) * 2), ball->getPosition().y + ((tblY*adelta)*0.75)));
	}
	else if (!mapMovingY){
		ball->setPosition(ccp(ball->getPosition().x + ((tblX*adelta)*0.75), ball->getPosition().y + ((tblY*adelta) * 2)));
	}
	else if (!mapMovingY && !mapMovingX){
		ball->setPosition(ccp(ball->getPosition().x + ((tblX*adelta) * 2), ball->getPosition().y + ((tblY*adelta) * 2)));
	}

	// Determine the four corners of my player
	int tlXright = floor(ball->getPosition().x + ballWidth);
	int tlXleft = floor(ball->getPosition().x);
	int tlYup = floor(ball->getPosition().y + ballHeight);
	int tlYdown = floor(ball->getPosition().y);

	//Convert our Map points due to us scrolling the map
	CCPoint nodeSpace1 = collisionMap->convertToNodeSpace(ccp(tlXright, tlYdown));
	pos1X = floor(nodeSpace1.x / collisionMap->getTileSize().width);
	pos1Y = floor(collisionMap->getMapSize().height - (nodeSpace1.y / collisionMap->getTileSize().height));

	CCPoint nodeSpace2 = collisionMap->convertToNodeSpace(ccp(tlXright, tlYup));
	pos2X = floor(nodeSpace2.x / collisionMap->getTileSize().width);
	pos2Y = floor(collisionMap->getMapSize().height - (nodeSpace2.y / collisionMap->getTileSize().height));

	CCPoint nodeSpace3 = collisionMap->convertToNodeSpace(ccp(tlXleft, tlYdown));
	pos3X = floor(nodeSpace3.x / collisionMap->getTileSize().width);
	pos3Y = floor(collisionMap->getMapSize().height - (nodeSpace3.y / collisionMap->getTileSize().height));

	CCPoint nodeSpace4 = collisionMap->convertToNodeSpace(ccp(tlXleft, tlYup));
	pos4X = floor(nodeSpace4.x / collisionMap->getTileSize().width);
	pos4Y = floor(collisionMap->getMapSize().height - (nodeSpace4.y / collisionMap->getTileSize().height));

	if (pos1X <= 19 && pos1Y <= 15 && pos2X <= 19 && pos2Y >= 0 && pos3X >= 0 && pos3Y <= 15 && pos4X >= 0 && pos4Y >= 0) {

		//Check if tile is blocked (collision)
		if (tblX != 0 && tblY != 0) {
			unsigned int gid1 = colLayer->tileGIDAt(ccp(pos1X, pos1Y));
			unsigned int gid2 = colLayer->tileGIDAt(ccp(pos2X, pos2Y));
			unsigned int gid3 = colLayer->tileGIDAt(ccp(pos3X, pos3Y));
			unsigned int gid4 = colLayer->tileGIDAt(ccp(pos4X, pos4Y));

			if (gid1 == 3 || gid1 == 5 || gid2 == 3 || gid2 == 5 || gid3 == 3 || gid3 == 5 || gid4 == 3 || gid4 == 5) {
				tblX = 0;
				tblY = 0;
			}

			unsigned int gid5 = flowerLayer->tileGIDAt(ccp(pos1X, pos1Y));
			if (gid5 == 31) {
				flowerLayer->removeTileAt(ccp(pos1X, pos1Y));
				flowerCount++;
				//[scoreLabel setString : [NSString stringWithFormat : @"Flowers: %d", flowerCount]];
			}
			unsigned int gid6 = flowerLayer->tileGIDAt(ccp(pos2X, pos2Y));
			if (gid6 == 31) {
				flowerLayer->removeTileAt(ccp(pos2X, pos2Y));
				flowerCount++;
				//[scoreLabel setString : [NSString stringWithFormat : @"Flowers: %d", flowerCount]];
			}
			unsigned int gid7 = flowerLayer->tileGIDAt(ccp(pos3X, pos3Y));
			if (gid7 == 31) {
				flowerLayer->removeTileAt(ccp(pos3X, pos3Y));
				flowerCount++;
				//[scoreLabel setString : [NSString stringWithFormat : @"Flowers: %d", flowerCount]];
			}
			unsigned int gid8 = flowerLayer->tileGIDAt(ccp(pos4X, pos4Y));
			if (gid8 == 31) {
				flowerLayer->removeTileAt(ccp(pos4X, pos4Y));
				flowerCount++;
				//[scoreLabel setString : [NSString stringWithFormat : @"Flowers: %d", flowerCount]];
			}

		}
	}
	else {
		tblX = 0;
		tblY = 0;
	}

}

void HelloWorld::playerHitDone(CCNode* pSender)
{
	playerHit = false;
}


void HelloWorld::registerWithTouchDispatcher()
{
	CCDirector* pDirector = CCDirector::sharedDirector();
	pDirector->getTouchDispatcher()->addTargetedDelegate(this, 0, true);
}

bool HelloWorld::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{

	if (pTouch)
	{



		CCPoint location = pTouch->getLocationInView();

		location = CCDirector::sharedDirector()->convertToGL(location);

		//NSLog(@"Touched");

		if (location.x > ball->getPosition().x) {
			tblX = 32;
		}
		if (location.x < ball->getPosition().x) {
			tblX = -32;
		}
		if (location.y > ball->getPosition().y) {
			tblY = 32;
		}
		if (location.y < ball->getPosition().y) {
			tblY = -32;
		}
		return true;


	}

	
	return false;
}
