
#include "LevelGenerator.h"
#include "LevelGeneratorHud.h"
using namespace CocosDenshion ;
using namespace cocos2d;
USING_NS_CC ;
#define PTM_RATIO 32
int lp=1;
int levelT=1;
#define DEGTORAD 0.0174532925199432957f

#pragma warning(disable : 4996)

bool LevelGeneratorScene::init()
{
	if( CCScene::init() )
	{
		pt=0;


		return true;
	}
	else
	{
		return false;
	}
}
void LevelGeneratorScene::setPlayerType(int type,int level)
{
	pt=type;
	lp=type;
	levelT=level;
	lt=level;
	this->_layer = LevelGenerator::node();
	//_layer->playerType=type;
	this->_layer->retain();
	this->addChild(_layer);




	LevelGeneratorHud *hud = LevelGeneratorHud::node();

	this->addChild(hud,5);
	_layer->setHud(hud);
	if(levelT==2)
	{
		hud->showTimer();
	}
	hud->setGameLayer(_layer);

}
LevelGeneratorScene::~LevelGeneratorScene()
{
	if (_layer)
	{
		_layer->release();
		_layer = NULL;
	}
}
CCScene* LevelGenerator::scene()
{
	CCScene * scene = NULL;
	do 
	{
		// 'scene' is an autorelease object
		scene = CCScene::create();
		CC_BREAK_IF(! scene);

		// 'layer' is an autorelease object
		LevelGenerator *layer = LevelGenerator::node();
		CC_BREAK_IF(! layer);

		// add layer as a child to scene
		scene->addChild(layer);
	} while (0);

	// return the scene
	return scene;
}
//
//void LevelGenerator::draw(void) {
//	CCLayer::draw();
//	
//	if( _world )
//	{
//	glDisable(GL_TEXTURE_2D);
//	glDisableClientState(GL_COLOR_ARRAY);
//	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
//	glEnableClientState(GL_VERTEX_ARRAY);
//	
//	_world->DrawDebugData();
//	
//	glDisableClientState(GL_VERTEX_ARRAY);
//	
//	
//	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
//	glEnableClientState(GL_COLOR_ARRAY);
//	glEnable(GL_TEXTURE_2D);
//	
//	}
//
//}
void LevelGenerator::preloadSounds()
{
	SimpleAudioEngine::sharedEngine()->setBackgroundMusicVolume(20.0f);
	SimpleAudioEngine::sharedEngine()->setEffectsVolume(0.02f);

	SimpleAudioEngine::sharedEngine()->preloadBackgroundMusic("Sound/DesertMode.mp3");
	SimpleAudioEngine::sharedEngine()->preloadBackgroundMusic("Sound/ModernMode.mp3");
	SimpleAudioEngine::sharedEngine()->preloadEffect("Sound/Jeep.mp3");
	SimpleAudioEngine::sharedEngine()->preloadEffect("Sound/truck.mp3");
	SimpleAudioEngine::sharedEngine()->preloadEffect("Sound/GunFire.mp3");
	SimpleAudioEngine::sharedEngine()->preloadEffect("Sound/Jeepblast.mp3");
	SimpleAudioEngine::sharedEngine()->preloadEffect("Sound/Campblast1.mp3");
	SimpleAudioEngine::sharedEngine()->preloadEffect("Sound/Rifle.mp3");
	SimpleAudioEngine::sharedEngine()->preloadEffect("Sound/Campblast2.mp3");
	SimpleAudioEngine::sharedEngine()->preloadEffect("Sound/shotgun.mp3");
	SimpleAudioEngine::sharedEngine()->preloadEffect("Sound/doublegun.mp3") ;
	SimpleAudioEngine::sharedEngine()->preloadEffect("Sound/snipershot.mp3") ;
	SimpleAudioEngine::sharedEngine()->preloadEffect("Sound/barreta.mp3") ;
	SimpleAudioEngine::sharedEngine()->preloadEffect("Sound/Helicopter.mp3") ;

	SimpleAudioEngine::sharedEngine()->preloadEffect("Sound/dying.wav") ;

	SimpleAudioEngine::sharedEngine()->preloadEffect("Sound/hit.wav") ;
	SimpleAudioEngine::sharedEngine()->preloadEffect("Sound/enemydying.wav") ;
	SimpleAudioEngine::sharedEngine()->preloadEffect("Sound/jumping.wav") ;
	SimpleAudioEngine::sharedEngine()->preloadEffect("Sound/walking.wav") ;
	SimpleAudioEngine::sharedEngine()->preloadEffect("Sound/FireBurning.mp3");
	SimpleAudioEngine::sharedEngine()->preloadEffect("Sound/powerup.mp3");
	SimpleAudioEngine::sharedEngine()->preloadEffect("Sound/vehiclehit.wav") ;
	SimpleAudioEngine::sharedEngine()->preloadEffect("Sound/rock.mp3") ;
	SimpleAudioEngine::sharedEngine()->preloadEffect("Sound/woodexplosion.mp3") ;
	SimpleAudioEngine::sharedEngine()->preloadEffect("Sound/shotgun2.mp3") ;

}
/////////////////////////////////////////////////////////////////////////////////////////////////
//   INIT
////////////////////////////////////////////////////////////////////////////////////////////////
bool LevelGenerator::init()
{

	preloadSounds();
	joystick = NULL;
	jumpButton = NULL;
	attackButton = NULL;
	state_Player=0;

	//JeepDestroyedCount=0;
	EnemyDestroyedCount=0;
	ZombieDestroyedCount=0;
	VehicleDestroyedCount=0;
	//TruckDestroyedCount=0;
	//HeliDestroyedCount=0;
	//Blayer = BG::node();
	//	Blayer->retain();
	//  this->addChild(Blayer,-1);
	bool bRet = false;
	do 
	{
		setTouchEnabled( true );
		scoreCount = 0;
		isPlayerLive=true;
		health=100;
		isEnemyFire=false;
		isEnemyKill=false;
		isEnemyZombieKill=false;
		isEnemyZombieFire=false;
		isPowerGive=false;
		heliSoundOn=false;
		Id_JeepSound=-1;
		Id_TruckSound=-1;
		EnemyHealth=100;
		EnemyZombieHealth=100;
		healthJeep=0;
		healthTruck=0;
		playerType=lp;


		CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("plist1.plist");
		sceneSpriteBatchNode = CCSpriteBatchNode::create("plist1.png");
		this->addChild(sceneSpriteBatchNode, 0);
		// load physics shapes
		GB2ShapeCache::sharedGB2ShapeCache()->addShapesWithFile("Box2dPlist.plist");
		CCSize winSize = CCDirector::sharedDirector()->getWinSize();


		b2Vec2 gravity = b2Vec2(0.0f, -6.0f); 
		_world = new b2World(gravity);
		_world->SetContinuousPhysics(true);


		m_debugDraw = new GLESDebugDraw( PTM_RATIO );
		_world->SetDebugDraw(m_debugDraw);

		uint32 flags = 0;
		flags += b2Draw::e_shapeBit;
		flags += b2Draw::e_jointBit;
		flags += b2Draw::e_aabbBit;
		flags += b2Draw::e_pairBit;
		flags += b2Draw::e_centerOfMassBit;
		m_debugDraw->SetFlags(flags);



		b2BodyDef _bodyDef;
		_bodyDef.position.Set(0, 0); 
		//CCSprite *sp = CCSprite::spriteWithFile("SpriteImages/ground.png");

		//this->addChild(sp, 1);
		//_bodyDef.type = b2_staticBody;
		//_bodyDef.userData=sp;
		_body = _world->CreateBody(&_bodyDef);

		b2EdgeShape groundEdge; 
		b2FixtureDef boxShapeDef; 
		boxShapeDef.shape = &groundEdge;

		groundEdge.Set(b2Vec2(-200,0), 
			b2Vec2((winSize.width*2)/PTM_RATIO, 0));

		_body->CreateFixture(&boxShapeDef);

		//groundEdge.Set(b2Vec2(0,0), b2Vec2(0, winSize.height/PTM_RATIO)); 
		//_body->CreateFixture(&boxShapeDef);

		groundEdge.Set(b2Vec2(0, winSize.height/PTM_RATIO),                      
			b2Vec2(winSize.width/PTM_RATIO*2, winSize.height/PTM_RATIO)); 
		_body->CreateFixture(&boxShapeDef); 

		//	groundEdge.Set(b2Vec2(winSize.width/PTM_RATIO, winSize.height/PTM_RATIO), b2Vec2(winSize.width/PTM_RATIO, 0)); 
		//	_body->CreateFixture(&boxShapeDef); 

		if(levelT==1)
		{

			CCSprite *sprite = CCSprite::create("SpriteImages/BgDesert.png");
			sprite->setAnchorPoint(CCPointZero);
			sprite->setPosition(CCPointMake(0,0));
			addChild(sprite,0,0);
			CocosDenshion::SimpleAudioEngine::sharedEngine()->setBackgroundMusicVolume(0.5f);
			SimpleAudioEngine::sharedEngine()->playBackgroundMusic("Sound/DesertMode.mp3") ;

		}
		if(levelT==2)
		{

			CCSprite *sprite = CCSprite::create("SpriteImages/BgSunSet.png");
			sprite->setAnchorPoint(CCPointZero);
			sprite->setPosition(CCPointMake(0,0));
			addChild(sprite,0,0);
			CocosDenshion::SimpleAudioEngine::sharedEngine()->setBackgroundMusicVolume(0.5f);
			SimpleAudioEngine::sharedEngine()->playBackgroundMusic("Sound/Level2.mp3") ;

		}

		if(levelT==3)
		{
			tex =TextureBackground::node();

			tex->init();

			CCSize s = CCDirector::sharedDirector()->getWinSize();
			RainAnimation(ccp(s.width / 2, s.height));
			RainAnimation(ccp((s.width*2)-(s.width / 2), s.height));
			this->addChild(tex,1);

			//CCSprite *sprite = CCSprite::spriteWithFile("SpriteImages/bg2x.png");
			//sprite->setAnchorPoint(CCPointZero);
			//sprite->setPosition(CCPointMake(0,0));
			//addChild(sprite,0,0);
			CocosDenshion::SimpleAudioEngine::sharedEngine()->setBackgroundMusicVolume(0.5f);
			SimpleAudioEngine::sharedEngine()->playBackgroundMusic("Sound/ModernMode.mp3") ;

		}

		//createBackground();

		//CCSprite *spLevel = CCSprite::spriteWithFile("SpriteImages/level1.png");
		//spLevel->setAnchorPoint(CCPointZero);
		//spLevel->setPosition(CCPointMake(440,290));
		//addChild(spLevel,+10);

		_backgroundNode = CCParallaxNodeExtras::node() ;
		this->addChild(_backgroundNode,+3) ;

		// 2) Create the sprites we'll add to the CCParallaxNode
		_cloud1 = CCSprite::create("SpriteImages/clowdsmall.png");
		_cloud2 = CCSprite::create("SpriteImages/clowdsmall.png");
		_cloud3 = CCSprite::create("SpriteImages/clowdsmall.png");
		_cloud4 = CCSprite::create("SpriteImages/clowdsmall.png");

		_bird1 = CCSprite::create("SpriteImages/bird1.png");
		_bird2 = CCSprite::create("SpriteImages/bird1.png");
		_bird3 = CCSprite::create("SpriteImages/bird1.png");
		_bird4 = CCSprite::create("SpriteImages/bird1.png");
		birdindex=1;

		CCPoint dustSpeed = ccp(0.1, 0.1);
		CCPoint bgSpeed = ccp(0.05, 0.05);

		_backgroundNode->addChild(_cloud1, +6, dustSpeed , ccp(300,250) ); 
		_backgroundNode->addChild(_cloud3, +6 , dustSpeed , ccp(700,280)); 
		_backgroundNode->addChild(_cloud2, +6, dustSpeed , ccp(100,220) ); 
		_backgroundNode->addChild(_cloud4, +6 , dustSpeed , ccp(400,200));

		_backgroundNode->addChild(_bird1, +7 , dustSpeed , ccp(200,270));
		_backgroundNode->addChild(_bird2, +7 , dustSpeed , ccp(600,280));
		_backgroundNode->addChild(_bird3, +7 , dustSpeed , ccp(500,260));
		_backgroundNode->addChild(_bird4, +7 , dustSpeed , ccp(800,220));



		_enemyzombieplayers= new vector<b2Body*>;
		_enemyplayers= new vector<b2Body*>;
		_players= new vector<b2Body*>;
		_jeeps = new vector<b2Body*>;
		_baseCamp = new vector<b2Body*>;
		_tnt = new vector<b2Body*>;
		_tanks = new vector<b2Body*>;
		_helicopters = new vector<b2Body*>;
		_trucks = new vector<b2Body*>;
		_rocks = new vector<b2Body*>;
		_bullets = new vector<b2Body*>;
		_powers = new vector<b2Body*>;
		_woods = new vector<b2Body*>;





		GameSetup();
		//this = new LevelGeneratorContactListener();
		_world->SetContactListener(this);
		//SimpleAudioEngine::sharedEngine()->playEffect("CircusMusic.mp3") ;
		//this->schedule(schedule_selector(MainScene::soundLogic),56.0 );
		schedule( schedule_selector(LevelGenerator::tick) );
		schedule( schedule_selector(LevelGenerator::update) );
		schedule( schedule_selector(LevelGenerator::updateKeyBoard) );
		schedule( schedule_selector(LevelGenerator::updateHelicopter),10.0 );
		schedule( schedule_selector(LevelGenerator::updateTruck),30.0 );
		schedule( schedule_selector(LevelGenerator::spawnEnemies),10.0 );
		schedule( schedule_selector(LevelGenerator::updateFire),1.0 );
		schedule( schedule_selector(LevelGenerator::updateTimer),1.0 );
		schedule( schedule_selector(LevelGenerator::updateEnemyPostion),0.5 );




		bRet = true;
	} while (0);

	return bRet;
}
/////////////////////////////////////////////////////////////////////////////////////////////////
//   ~LevelGenerator
////////////////////////////////////////////////////////////////////////////////////////////////
LevelGenerator::~LevelGenerator()
{

	//SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();
	//SimpleAudioEngine::sharedEngine()->stopAllEffects();




	delete _powers;
	delete _rocks;
	delete _enemyzombieplayers;
	delete _enemyplayers;
	delete _players;
	delete _bullets;
	delete _helicopters;
	delete _jeeps;
	delete _tanks;
	delete _baseCamp;
	delete _tnt;
	delete _trucks;
	delete _woods;


}
/////////////////////////////////////////////////////////////////////////////////////////////////
//   GAME SETUP
////////////////////////////////////////////////////////////////////////////////////////////////

void LevelGenerator::createBackground()
{
	CCParallaxNode *parallax = CCParallaxNode::create();
	CCTexture2D::setDefaultAlphaPixelFormat(kCCTexture2DPixelFormat_RGB565);
	CCSprite *background;
	background = CCSprite::create("bgempty.png");
	background->setAnchorPoint(ccp(0,0));
	CCTexture2D::setDefaultAlphaPixelFormat(kCCTexture2DPixelFormat_Default);
	parallax->addChild(background, -10, ccp(0.05f, 0.05f), ccp(0,0));
	this->addChild(parallax, -10);
}
void LevelGenerator::GameSetup()
{

	generateMap();

}

/////////////////////////////////////////////////////////////////////////////////////////////////
//   MAP GENERATION
////////////////////////////////////////////////////////////////////////////////////////////////
void LevelGenerator::generateMap()
{

	if(playerType==1)
		AddPlayers(CCPointMake(150.0f, 0.0f));
	else if (playerType==2) 
		AddGoldenPlayers(CCPointMake(150.0f, 0.0f));
	else if (playerType==3) 
		AddBrownPlayers(CCPointMake(150.0f, 0.0f));

	AddEnemyZombiePlayers(CCPointMake(1450.0f, 0.0f));
	AddEnemyPlayers(CCPointMake(400.0f, 0.0f));
	AddHelicopters(CCPointMake(1800.0f, 200.0f));
	if(levelT==1)

		AddTrucks(CCPointMake(-180.0f, 0.0f));
	else
		AddTrucks(CCPointMake(1180.0f, 0.0f));
	if(levelT==1)
		AddTanks(CCPointMake(1030.0f, 0.0f));
	else
		AddTanks(CCPointMake(930.0f, 0.0f));
	AddJeeps(CCPointMake(700.0f, 0.0f));	
	AddBaseCamp(CCPointMake(1750.0f, 0.0f));
	AddLongFixRocks(CCPointMake(1600.0f, 120.0f));
	AddBridge(CCPointMake(800.0f, 120.0f));

	if(levelT==1)
	{
		for(int i=0;i<5;i++)
			AddRocks(CCPointMake(300.0f, 0.0f+i*10));
		for(int i=0;i<5;i++)
			AddRocks(CCPointMake(320.0f, 0.0f+i*10));

		for(int i=0;i<12;i++)
			AddRocks(CCPointMake(1500.0f, 0.0f+i*10));
		for(int i=0;i<12;i++)
			AddRocks(CCPointMake(1520.0f, 0.0f+i*10));
		for(int i=0;i<12;i++)
			AddRocks(CCPointMake(1540.0f, 0.0f+i*10));
		for(int i=0;i<12;i++)
			AddRocks(CCPointMake(1560.0f, 0.0f+i*10));
	}

	/////Level3
	if(levelT==3)
	{
		for(int i=0;i<5;i++)
			AddRocks(CCPointMake(300.0f, 0.0f+i*10));
		for(int i=0;i<5;i++)
			AddWoods(CCPointMake(320.0f, 0.0f+i*12));
		for(int i=0;i<10;i++)
			AddWoods(CCPointMake(1480.0f, 0.0f+i*12));
		for(int i=0;i<12;i++)
			AddRocks(CCPointMake(1522.0f, 0.0f+i*10));
		for(int i=0;i<10;i++)
			AddWoods(CCPointMake(1542.0f, 0.0f+i*12));

	}


	/////Level3
	if(levelT==2)
	{
		for(int i=0;i<5;i++)
			AddWoods(CCPointMake(300.0f, 0.0f+i*12));
		for(int i=0;i<10;i++)
			AddWoods(CCPointMake(1500.0f, 0.0f+i*12));
		for(int i=0;i<10;i++)
			AddWoods(CCPointMake(1542.0f, 0.0f+i*12));

	}
}
/////////////////////////////////////////////////////////////////////////////////////////////////
//  CREATE  GAME OBJECTS 
////////////////////////////////////////////////////////////////////////////////////////////////
float LevelGenerator::ptm(float value)
{

	return value/PTM_RATIO;

}
void LevelGenerator::AddRagDollPlayers(CCPoint position)
{

	CCPoint _ragDollPosition;
	_ragDollPosition.x=position.x;
	_ragDollPosition.y=position.y;
	// -------------------------
	// Bodies ------------------
	// -------------------------
	//   
	//   // Set these to dynamics bodies
	//   b2BodyDef bd;
	//   bd.type = b2_dynamicBody;

	//   //b2PolygonShape box;
	//   b2FixtureDef fixtureDef;
	//   
	//   // Head ------
	//CCSprite *sp = CCSprite::spriteWithFile("PlayerBody2\\Head.png");
	//   b2CircleShape headShape;
	//   headShape.m_radius = sp->getContentSize().width/2.0f/PTM_RATIO;
	//   fixtureDef.shape = &headShape;
	//   fixtureDef.density = 1.0f;
	//   fixtureDef.friction = 0.4f;
	//   fixtureDef.restitution = 0.3f;
	//
	//addChild(sp, 1);
	//bd.userData=sp;
	//   bd.position.Set(ptm(_ragDollPosition.x), ptm(_ragDollPosition.y));
	//   b2Body *head = _world->CreateBody(&bd);
	//   head->CreateFixture(&fixtureDef);
	//   //head->ApplyLinearImpulse(b2Vec2(rand() % 100 - 50.0f, rand() % 100 - 50.0f), head->GetWorldCenter());
	//   
	//   // -----------
	//	b2PolygonShape box;
	//   // Torso1 ----
	//sp = CCSprite::spriteWithFile("PlayerBody2\\Torso.png");
	//box.SetAsBox(sp->getContentSize().width/2.0f/PTM_RATIO, sp->getContentSize().height/2.0f/PTM_RATIO);
	//   fixtureDef.shape = &box;
	//   fixtureDef.density = 1.0f;
	//   fixtureDef.friction = 0.4f;
	//   fixtureDef.restitution = 0.1f;
	//
	//addChild(sp, +10);
	//bd.userData=sp;
	//   bd.position.Set(ptm(_ragDollPosition.x), ptm(_ragDollPosition.y + 22.0f));
	//   b2Body *torso1 = _world->CreateBody(&bd);
	//   torso1->CreateFixture(&fixtureDef);
	//   torso1->ApplyLinearImpulse(b2Vec2(-5, 5), head->GetWorldCenter());
	//   
	//   // -----------

	////   // Torso2 ----
	////sp = CCSprite::spriteWithFile("Torso.png");
	////  box.SetAsBox(sp->getContentSize().width/2.0f/PTM_RATIO, sp->getContentSize().height/2.0f/PTM_RATIO);
	////   fixtureDef.shape = &box;
	////	
	////addChild(sp, 1);
	////bd.userData=sp;
	////   bd.position.Set(ptm(_ragDollPosition.x), ptm(_ragDollPosition.y + 20.0f));
	////   b2Body *torso2 = _world->CreateBody(&bd);
	////   torso2->CreateFixture(&fixtureDef);
	////   
	////   // -----------

	////   // Torso3 ----
	////sp = CCSprite::spriteWithFile("Torso.png");
	////    box.SetAsBox(sp->getContentSize().width/2.0f/PTM_RATIO, sp->getContentSize().height/2.0f/PTM_RATIO);
	////   fixtureDef.shape = &box;
	////	
	////addChild(sp, 1);
	////bd.userData=sp;
	////   bd.position.Set(ptm(_ragDollPosition.x), ptm(_ragDollPosition.y + 30.0f));
	////   b2Body *torso3 = _world->CreateBody(&bd);
	////   torso3->CreateFixture(&fixtureDef);
	////   
	//   // -----------

	//   // UpperArm --
	//   fixtureDef.density = 1.0f;
	//   fixtureDef.friction = 0.14f;
	//   fixtureDef.restitution = 0.1f;

	//   // Left
	//sp = CCSprite::spriteWithFile("PlayerBody2\\LeftUpperArm.png");
	//   box.SetAsBox(sp->getContentSize().width/2.0f/PTM_RATIO, sp->getContentSize().height/2.0f/PTM_RATIO);
	//   fixtureDef.shape = &box;
	//
	//addChild(sp, 1);
	//bd.userData=sp;
	//   bd.position.Set(ptm(_ragDollPosition.x - 14.0f), ptm(_ragDollPosition.y + 18.0f));
	//   b2Body *upperArmL = _world->CreateBody(&bd);
	//   upperArmL->CreateFixture(&fixtureDef);

	//   // Right
	//sp = CCSprite::spriteWithFile("PlayerBody2\\RightUpperArm.png");
	//   box.SetAsBox(sp->getContentSize().width/2.0f/PTM_RATIO, sp->getContentSize().height/2.0f/PTM_RATIO);
	//   fixtureDef.shape = &box;
	//addChild(sp, 1);
	//bd.userData=sp;
	//   bd.position.Set(ptm(_ragDollPosition.x + 13.0f), ptm(_ragDollPosition.y + 18.0f));
	//   b2Body *upperArmR = _world->CreateBody(&bd);
	//   upperArmR->CreateFixture(&fixtureDef);
	//   
	//   // -----------

	//   // Lower Arm
	//   fixtureDef.density = 1.0f;
	//   fixtureDef.friction = 0.4f;
	//   fixtureDef.restitution = 0.1f;
	//   
	//   // Left
	//sp = CCSprite::spriteWithFile("PlayerBody2\\LeftLowerArm.png");
	//   box.SetAsBox(sp->getContentSize().width/2.0f/PTM_RATIO, sp->getContentSize().height/2.0f/PTM_RATIO);
	//   fixtureDef.shape = &box;
	//
	//addChild(sp, 1);
	//bd.userData=sp;
	//   bd.position.Set(ptm(_ragDollPosition.x - 20.0f), ptm(_ragDollPosition.y + 18.0f));
	//   b2Body *lowerArmL = _world->CreateBody(&bd);
	//   lowerArmL->CreateFixture(&fixtureDef);
	//   
	//   // Right
	//sp = CCSprite::spriteWithFile("PlayerBody2\\RightLowerArm.png");
	//   box.SetAsBox(sp->getContentSize().width/2.0f/PTM_RATIO, sp->getContentSize().height/2.0f/PTM_RATIO);
	//   fixtureDef.shape = &box;
	//
	//addChild(sp, 1);
	//bd.userData=sp;
	//   bd.position.Set(ptm(_ragDollPosition.x + 20.0f), ptm(_ragDollPosition.y + 18.0f));
	//   b2Body *lowerArmR = _world->CreateBody(&bd);
	//   lowerArmR->CreateFixture(&fixtureDef);
	////   
	//   // -----------

	//   // UpperLeg --
	//   fixtureDef.density = 1.0f;
	//   fixtureDef.friction = 0.1f;
	//   fixtureDef.restitution = 0.1f;
	//   
	//   // Left
	//	sp = CCSprite::spriteWithFile("PlayerBody2\\LeftLeg.png");
	//    box.SetAsBox(sp->getContentSize().width/2.0f/PTM_RATIO, sp->getContentSize().height/2.0f/PTM_RATIO);
	//   fixtureDef.shape = &box;

	//addChild(sp, 1);
	//bd.userData=sp;
	//   bd.position.Set(ptm(_ragDollPosition.x - 0.0f), ptm(_ragDollPosition.y + 50.0f));
	//   b2Body *upperLegL = _world->CreateBody(&bd);
	//   upperLegL->CreateFixture(&fixtureDef);
	//   
	//   // Right
	//sp = CCSprite::spriteWithFile("PlayerBody2\\RightLeg.png");
	//    box.SetAsBox(sp->getContentSize().width/2.0f/PTM_RATIO, sp->getContentSize().height/2.0f/PTM_RATIO);
	//   fixtureDef.shape = &box;
	//
	//addChild(sp, 1);
	//bd.userData=sp;
	//   bd.position.Set(ptm(_ragDollPosition.x + 0.0f), ptm(_ragDollPosition.y + 50.0f));
	//   b2Body *upperLegR = _world->CreateBody(&bd);
	//   upperLegR->CreateFixture(&fixtureDef);
	//   
	//   // -----------

	////   // LowerLeg --
	////   fixtureDef.density = 1.0f;
	////   fixtureDef.friction = 0.4f;
	////   fixtureDef.restitution = 0.1f;
	////   
	////   // Left
	////   box.SetAsBox(ptm(6.0f), ptm(20.0f));
	////   fixtureDef.shape = &box;
	////sp = CCSprite::spriteWithFile("UpperLeg.png");
	////addChild(sp, 1);
	////bd.userData=sp;
	////   bd.position.Set(ptm(_ragDollPosition.x - 8.0f), ptm(_ragDollPosition.y + 74.0f));
	////   b2Body *lowerLegL = _world->CreateBody(&bd);
	////   lowerLegL->CreateFixture(&fixtureDef);
	////   
	////   // Right
	////   box.SetAsBox(ptm(6.0f), ptm(20.0f));
	////   fixtureDef.shape = &box;
	////sp = CCSprite::spriteWithFile("UpperLeg.png");
	////addChild(sp, 1);
	////bd.userData=sp;
	////   bd.position.Set(ptm(_ragDollPosition.x + 8.0f), ptm(_ragDollPosition.y + 74.0f));
	////   b2Body *lowerLegR = _world->CreateBody(&bd);
	////   lowerLegR->CreateFixture(&fixtureDef);
	////   // -----------

	//   // -------------------------
	//   // Joints ------------------
	//   // -------------------------
	//   
	//   b2RevoluteJointDef jd;
	//   jd.enableLimit = true;
	//   
	//   // Head to shoulders
	//   jd.lowerAngle = -20.0f / (180.0f / M_PI);
	//   jd.upperAngle = 20.0f / (180.0f / M_PI);
	//   jd.Initialize(torso1, head, b2Vec2(ptm(_ragDollPosition.x), ptm(_ragDollPosition.y + 20.0f)));
	//   _world->CreateJoint(&jd);
	//   
	//   // Upper arm to shoulders --
	//   // Left
	//   jd.lowerAngle = -85.0f / (180.0f / M_PI);
	//   jd.upperAngle = 130.0f / (180.0f / M_PI);
	//   jd.Initialize(torso1, upperArmL, b2Vec2(ptm(_ragDollPosition.x - 14.0f), ptm(_ragDollPosition.y + 14.0f)));
	//   _world->CreateJoint(&jd);
	//   
	//   // Right
	//   jd.lowerAngle = -130.0f / (180.0f / M_PI);
	//   jd.upperAngle = 85.0f / (180.0f / M_PI);
	//   jd.Initialize(torso1, upperArmR, b2Vec2(ptm(_ragDollPosition.x + 13.0f), ptm(_ragDollPosition.y + 14.0f)));
	//   _world->CreateJoint(&jd);
	//   
	//   // -------------------------
	//   
	//   //// Lower arm to shoulders --
	//   // Left
	//   jd.lowerAngle = -130.0f / (180.0f / M_PI);
	//   jd.upperAngle = 10.0f / (180.0f / M_PI);
	//   jd.Initialize(upperArmL, lowerArmL, b2Vec2(ptm(_ragDollPosition.x - 20.0f), ptm(_ragDollPosition.y + 18.0f)));
	//   _world->CreateJoint(&jd);
	//   
	//   // Right
	//   jd.lowerAngle = -10.0f / (180.0f / M_PI);
	//   jd.upperAngle = 130.0f / (180.0f / M_PI);
	//   jd.Initialize(upperArmR, lowerArmR, b2Vec2(ptm(_ragDollPosition.x + 20.0f), ptm(_ragDollPosition.y + 18.0f)));
	//   _world->CreateJoint(&jd);
	//   //
	//   // -------------------------
	//   
	//   //// Shoulders / stomach -----
	//   //jd.lowerAngle = -15.0f / (180.0f / M_PI);
	//   //jd.upperAngle = 15.0f / (180.0f / M_PI);
	//   //jd.Initialize(torso1, torso2, b2Vec2(ptm(_ragDollPosition.x), ptm(_ragDollPosition.y + 20.0f)));
	//   //_world->CreateJoint(&jd);
	//   //
	//   //// Stomach / hips
	//   //jd.Initialize(torso2, torso3, b2Vec2(ptm(_ragDollPosition.x), ptm(_ragDollPosition.y + 30.0f)));
	//   //_world->CreateJoint(&jd);
	//   
	//   // -------------------------
	//   
	//   // Torso to upper leg ------
	//   // Left
	//   jd.lowerAngle = -25.0f / (180.0f / M_PI);
	//   jd.upperAngle = 115.0f / (180.0f / M_PI);
	//   jd.Initialize(torso1, upperLegL, b2Vec2(ptm(_ragDollPosition.x - 0), ptm(_ragDollPosition.y + 55.0f)));
	//   _world->CreateJoint(&jd);
	//   
	//   // Right
	//   jd.lowerAngle = -115.0f / (180.0f / M_PI);
	//   jd.upperAngle = 25.0f / (180.0f / M_PI);
	//   jd.Initialize(torso1, upperLegR, b2Vec2(ptm(_ragDollPosition.x + 2), ptm(_ragDollPosition.y + 55.0f)));
	//   _world->CreateJoint(&jd);
	//   
	//   // -------------------------
	//   
	//   //// Upper leg to lower leg --
	//   //// Left
	//   //jd.lowerAngle = -25.0f / (180.0f / M_PI);
	//   //jd.upperAngle = 115.0f / (180.0f / M_PI);
	//   //jd.Initialize(upperLegL, lowerLegL, b2Vec2(ptm(_ragDollPosition.x - 8), ptm(_ragDollPosition.y + 74.0f)));
	//   //_world->CreateJoint(&jd);
	//   //
	//   //// Right
	//   //jd.lowerAngle = -115.0f / (180.0f / M_PI);
	//   //jd.upperAngle = 25.0f / (180.0f / M_PI);
	//   //jd.Initialize(upperLegR, lowerLegR, b2Vec2(ptm(_ragDollPosition.x + 8), ptm(_ragDollPosition.y + 74.0f)));
	//   //_world->CreateJoint(&jd);

	// Set these to dynamics bodies
	b2BodyDef bd;
	bd.type = b2_dynamicBody;
	b2PolygonShape box;
	//b2PolygonShape box;
	b2FixtureDef fixtureDef;

	// Head ------
	CCSprite *sp = CCSprite::create("PlayerBody2\\Head.png");
	b2CircleShape headShape;
	headShape.m_radius = sp->getContentSize().width/2.0f/PTM_RATIO;
	fixtureDef.shape = &headShape;
	fixtureDef.density = 1.0f;
	fixtureDef.friction = 0.4f;
	fixtureDef.restitution = 0.3f;

	addChild(sp, 1);
	bd.userData=sp;
	bd.position.Set(ptm(_ragDollPosition.x), ptm(_ragDollPosition.y));
	b2Body *head = _world->CreateBody(&bd);
	head->CreateFixture(&fixtureDef);
	//head->ApplyLinearImpulse(b2Vec2(rand() % 100 - 50.0f, rand() % 100 - 50.0f), head->GetWorldCenter());

	// -----------
	//b2PolygonShape box;
	// Torso1 ----
	sp = CCSprite::create("PlayerBody\\Torso.png");
	box.SetAsBox(sp->getContentSize().width/2.0f/PTM_RATIO, sp->getContentSize().height/2.0f/PTM_RATIO);
	fixtureDef.shape = &box;
	fixtureDef.density = 1.0f;
	fixtureDef.friction = 0.4f;
	fixtureDef.restitution = 0.1f;

	addChild(sp, +10);
	bd.userData=sp;
	bd.position.Set(ptm(_ragDollPosition.x), ptm(_ragDollPosition.y + 21.0f));
	b2Body *torso1 = _world->CreateBody(&bd);
	torso1->CreateFixture(&fixtureDef);
	torso1->ApplyLinearImpulse(b2Vec2(-5, 5), head->GetWorldCenter());

	// -----------

	//   // Torso2 ----
	//sp = CCSprite::spriteWithFile("Torso.png");
	//  box.SetAsBox(sp->getContentSize().width/2.0f/PTM_RATIO, sp->getContentSize().height/2.0f/PTM_RATIO);
	//   fixtureDef.shape = &box;
	//	
	//addChild(sp, 1);
	//bd.userData=sp;
	//   bd.position.Set(ptm(_ragDollPosition.x), ptm(_ragDollPosition.y + 20.0f));
	//   b2Body *torso2 = _world->CreateBody(&bd);
	//   torso2->CreateFixture(&fixtureDef);
	//   
	//   // -----------

	//   // Torso3 ----
	//sp = CCSprite::spriteWithFile("Torso.png");
	//    box.SetAsBox(sp->getContentSize().width/2.0f/PTM_RATIO, sp->getContentSize().height/2.0f/PTM_RATIO);
	//   fixtureDef.shape = &box;
	//	
	//addChild(sp, 1);
	//bd.userData=sp;
	//   bd.position.Set(ptm(_ragDollPosition.x), ptm(_ragDollPosition.y + 30.0f));
	//   b2Body *torso3 = _world->CreateBody(&bd);
	//   torso3->CreateFixture(&fixtureDef);
	//   
	// -----------

	// UpperArm --
	fixtureDef.density = 1.0f;
	fixtureDef.friction = 0.1f;
	fixtureDef.restitution = 0.1f;

	// Left
	sp = CCSprite::create("PlayerBody\\LeftArm.png");
	box.SetAsBox(sp->getContentSize().width/2.0f/PTM_RATIO, sp->getContentSize().height/2.0f/PTM_RATIO);
	fixtureDef.shape = &box;

	addChild(sp, 1);
	bd.userData=sp;
	bd.position.Set(ptm(_ragDollPosition.x - 14.0f), ptm(_ragDollPosition.y + 18.0f));
	b2Body *upperArmL = _world->CreateBody(&bd);
	upperArmL->CreateFixture(&fixtureDef);

	// Right
	sp = CCSprite::create("PlayerBody\\RightArm.png");
	box.SetAsBox(sp->getContentSize().width/2.0f/PTM_RATIO, sp->getContentSize().height/2.0f/PTM_RATIO);


	/*b2Vec2 RightArm_vertices[4];
	RightArm_vertices[0].Set(+0.06667f, +0.14019f);
	RightArm_vertices[1].Set(+0.32000f, +0.40187f);
	RightArm_vertices[2].Set(+0.09333f, +0.09346f);
	RightArm_vertices[3].Set(+0.42667f, +0.36449f);*/
	//box.Set(RightArm_vertices, 4);
	fixtureDef.shape = &box;
	addChild(sp, 1);
	bd.userData=sp;
	bd.position.Set(ptm(_ragDollPosition.x + 10.0f), ptm(_ragDollPosition.y + 18.0f));
	b2Body *upperArmR = _world->CreateBody(&bd);
	upperArmR->CreateFixture(&fixtureDef);

	// -----------

	//   // Lower Arm
	//   fixtureDef.density = 1.0f;
	//   fixtureDef.friction = 0.4f;
	//   fixtureDef.restitution = 0.1f;
	//   
	//   // Left
	//sp = CCSprite::spriteWithFile("PlayerBody2\\LeftLowerArm.png");
	//   box.SetAsBox(sp->getContentSize().width/2.0f/PTM_RATIO, sp->getContentSize().height/2.0f/PTM_RATIO);
	//   fixtureDef.shape = &box;
	//
	//addChild(sp, 1);
	//bd.userData=sp;
	//   bd.position.Set(ptm(_ragDollPosition.x - 20.0f), ptm(_ragDollPosition.y + 18.0f));
	//   b2Body *lowerArmL = _world->CreateBody(&bd);
	//   lowerArmL->CreateFixture(&fixtureDef);
	//   
	//   // Right
	//sp = CCSprite::spriteWithFile("PlayerBody2\\RightLowerArm.png");
	//   box.SetAsBox(sp->getContentSize().width/2.0f/PTM_RATIO, sp->getContentSize().height/2.0f/PTM_RATIO);
	//   fixtureDef.shape = &box;
	//
	//addChild(sp, 1);
	//bd.userData=sp;
	//   bd.position.Set(ptm(_ragDollPosition.x + 20.0f), ptm(_ragDollPosition.y + 18.0f));
	//   b2Body *lowerArmR = _world->CreateBody(&bd);
	//   lowerArmR->CreateFixture(&fixtureDef);
	//   
	// -----------

	// UpperLeg --
	fixtureDef.density = 1.0f;
	fixtureDef.friction = 0.1f;
	fixtureDef.restitution = 0.1f;

	// Left
	sp = CCSprite::create("PlayerBody\\RightLeg.png");
	/*b2Vec2 LeftLeg_vertices[6];


	LeftLeg_vertices[0].Set(-0.12000f, -0.40187f);
	LeftLeg_vertices[1].Set(-0.26667f, -0.44860f);
	LeftLeg_vertices[2].Set(-0.20000f, -0.32710f);
	LeftLeg_vertices[3].Set(-0.06667f, -0.10280f);
	LeftLeg_vertices[4].Set(-0.06667f, -0.10280f);
	LeftLeg_vertices[5].Set(-0.13333f, -0.01869f);
	box.Set(LeftLeg_vertices, 6);*/

	box.SetAsBox(sp->getContentSize().width/2.0f/PTM_RATIO, sp->getContentSize().height/2.0f/PTM_RATIO);
	fixtureDef.shape = &box;

	addChild(sp, 1);
	bd.userData=sp;
	bd.position.Set(ptm(_ragDollPosition.x - 0.0f), ptm(_ragDollPosition.y + 44.0f));
	b2Body *upperLegL = _world->CreateBody(&bd);
	upperLegL->CreateFixture(&fixtureDef);

	// Right
	sp = CCSprite::create("PlayerBody\\LeftLeg.png");
	box.SetAsBox(sp->getContentSize().width/2.0f/PTM_RATIO, sp->getContentSize().height/2.0f/PTM_RATIO);

	/*b2Vec2 RightLeg_vertices[6];
	RightLeg_vertices[0].Set(+0.05333f, -0.04673f);
	RightLeg_vertices[1].Set(+0.12000f, -0.20561f);
	RightLeg_vertices[2].Set(+0.17333f, -0.47664f);
	RightLeg_vertices[3].Set(+0.05333f, -0.42991f);
	RightLeg_vertices[4].Set(-0.02667f, -0.19626f);
	RightLeg_vertices[5].Set(-0.01333f, -0.06542f);
	box.Set(RightLeg_vertices, 6);*/


	fixtureDef.shape = &box;

	addChild(sp, 1);
	bd.userData=sp;
	bd.position.Set(ptm(_ragDollPosition.x + 0.0f), ptm(_ragDollPosition.y + 42.0f));
	b2Body *upperLegR = _world->CreateBody(&bd);
	upperLegR->CreateFixture(&fixtureDef);

	// -----------

	//   // LowerLeg --
	//   fixtureDef.density = 1.0f;
	//   fixtureDef.friction = 0.4f;
	//   fixtureDef.restitution = 0.1f;
	//   
	//   // Left
	//   box.SetAsBox(ptm(6.0f), ptm(20.0f));
	//   fixtureDef.shape = &box;
	//sp = CCSprite::spriteWithFile("UpperLeg.png");
	//addChild(sp, 1);
	//bd.userData=sp;
	//   bd.position.Set(ptm(_ragDollPosition.x - 8.0f), ptm(_ragDollPosition.y + 74.0f));
	//   b2Body *lowerLegL = _world->CreateBody(&bd);
	//   lowerLegL->CreateFixture(&fixtureDef);
	//   
	//   // Right
	//   box.SetAsBox(ptm(6.0f), ptm(20.0f));
	//   fixtureDef.shape = &box;
	//sp = CCSprite::spriteWithFile("UpperLeg.png");
	//addChild(sp, 1);
	//bd.userData=sp;
	//   bd.position.Set(ptm(_ragDollPosition.x + 8.0f), ptm(_ragDollPosition.y + 74.0f));
	//   b2Body *lowerLegR = _world->CreateBody(&bd);
	//   lowerLegR->CreateFixture(&fixtureDef);
	//   // -----------

	// -------------------------
	// Joints ------------------
	// -------------------------

	b2RevoluteJointDef jd;
	jd.enableLimit = true;

	// Head to shoulders
	jd.lowerAngle = -20.0f / (180.0f / M_PI);
	jd.upperAngle = 20.0f / (180.0f / M_PI);
	jd.Initialize(torso1, head, b2Vec2(ptm(_ragDollPosition.x), ptm(_ragDollPosition.y + 20.0f)));
	_world->CreateJoint(&jd);

	// Upper arm to shoulders --
	// Left
	jd.lowerAngle = -85.0f / (180.0f / M_PI);
	jd.upperAngle = 130.0f / (180.0f / M_PI);
	jd.Initialize(torso1, upperArmL, b2Vec2(ptm(_ragDollPosition.x - 14.0f), ptm(_ragDollPosition.y + 14.0f)));
	_world->CreateJoint(&jd);

	// Right
	jd.lowerAngle = -15.0f / (180.0f / M_PI);
	jd.upperAngle = 15.0f / (180.0f / M_PI);
	jd.Initialize(torso1, upperArmR, b2Vec2(ptm(_ragDollPosition.x + 10.0f), ptm(_ragDollPosition.y + 14.0f)));
	_world->CreateJoint(&jd);

	// -------------------------

	//// Lower arm to shoulders --
	// Left
	//jd.lowerAngle = -130.0f / (180.0f / M_PI);
	//jd.upperAngle = 10.0f / (180.0f / M_PI);
	//jd.Initialize(upperArmL, lowerArmL, b2Vec2(ptm(_ragDollPosition.x - 20.0f), ptm(_ragDollPosition.y + 18.0f)));
	//_world->CreateJoint(&jd);
	//
	//// Right
	//jd.lowerAngle = -10.0f / (180.0f / M_PI);
	//jd.upperAngle = 130.0f / (180.0f / M_PI);
	//jd.Initialize(upperArmR, lowerArmR, b2Vec2(ptm(_ragDollPosition.x + 20.0f), ptm(_ragDollPosition.y + 18.0f)));
	//_world->CreateJoint(&jd);
	////
	// -------------------------

	//// Shoulders / stomach -----
	//jd.lowerAngle = -15.0f / (180.0f / M_PI);
	//jd.upperAngle = 15.0f / (180.0f / M_PI);
	//jd.Initialize(torso1, torso2, b2Vec2(ptm(_ragDollPosition.x), ptm(_ragDollPosition.y + 20.0f)));
	//_world->CreateJoint(&jd);
	//
	//// Stomach / hips
	//jd.Initialize(torso2, torso3, b2Vec2(ptm(_ragDollPosition.x), ptm(_ragDollPosition.y + 30.0f)));
	//_world->CreateJoint(&jd);

	// -------------------------

	// Torso to upper leg ------
	// Left
	jd.lowerAngle = -125.0f / (180.0f / M_PI);
	jd.upperAngle = 185.0f / (180.0f / M_PI);
	jd.Initialize(torso1, upperLegL, b2Vec2(ptm(_ragDollPosition.x - 0), ptm(_ragDollPosition.y + 30.0f)));
	_world->CreateJoint(&jd);

	// Right
	jd.lowerAngle = -0.0f / (180.0f / M_PI);
	jd.upperAngle = 185.0f / (180.0f / M_PI);
	jd.Initialize(torso1, upperLegR, b2Vec2(ptm(_ragDollPosition.x + 2), ptm(_ragDollPosition.y + 30.0f)));
	_world->CreateJoint(&jd);

	// -------------------------

	//// Upper leg to lower leg --
	//// Left
	//jd.lowerAngle = -25.0f / (180.0f / M_PI);
	//jd.upperAngle = 115.0f / (180.0f / M_PI);
	//jd.Initialize(upperLegL, lowerLegL, b2Vec2(ptm(_ragDollPosition.x - 8), ptm(_ragDollPosition.y + 74.0f)));
	//_world->CreateJoint(&jd);
	//
	//// Right
	//jd.lowerAngle = -115.0f / (180.0f / M_PI);
	//jd.upperAngle = 25.0f / (180.0f / M_PI);
	//jd.Initialize(upperLegR, lowerLegR, b2Vec2(ptm(_ragDollPosition.x + 8), ptm(_ragDollPosition.y + 74.0f)));
	//_world->CreateJoint(&jd);

}
void LevelGenerator::AddPlayers(CCPoint position)
{

	state_Player=STATE_STAND_RIGHT;		
	_Player = CCSprite::createWithSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("Standing/Stand.png"));
	//_Player = CCSprite::spriteWithFile("P002.png");
	_Player->setScale(0.6);
	addChild(_Player, 1);
	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set((position.x+_Player->getContentSize().width/2.0f)/PTM_RATIO,
		(position.y+_Player->getContentSize().height/2.0f)/PTM_RATIO);
	bodyDef.userData = _Player;

	_playerBody = _world->CreateBody(&bodyDef);
	_playerBody->SetFixedRotation(true);

	b2FixtureDef boxDef;

	boxDef.userData = (void*)1;
	_players->push_back(_playerBody);
	b2PolygonShape boxp;
	boxp.SetAsBox(_Player->getContentSize().width/2.0f/PTM_RATIO*0.6, _Player->getContentSize().height/2.0f/PTM_RATIO*0.6);
	boxDef.shape = &boxp;	
	boxDef.density = 1.5f;
	boxDef.friction = 1.0f;	
	boxDef.restitution = 0.0f;
	boxDef.filter.categoryBits = 0x0001;

	_playerBody->CreateFixture(&boxDef);

}
void LevelGenerator::AddGoldenPlayers(CCPoint position)
{

	_Player = CCSprite::createWithSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("GoldenPlayer_Walk/Right (3).png"));
	//_Player = CCSprite::spriteWithFile("P002.png");
	_Player->setScale(1.5);
	addChild(_Player, 1);
	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set((position.x+_Player->getContentSize().width/2.0f)/PTM_RATIO,
		(position.y+_Player->getContentSize().height/2.0f)/PTM_RATIO);
	bodyDef.userData = _Player;

	_playerBody = _world->CreateBody(&bodyDef);
	_playerBody->SetFixedRotation(true);

	b2FixtureDef boxDef;

	boxDef.userData = (void*)1;
	_players->push_back(_playerBody);
	b2PolygonShape boxp;
	boxp.SetAsBox(_Player->getContentSize().width/2.0f/PTM_RATIO*1.5, _Player->getContentSize().height/2.0f/PTM_RATIO*1.5);
	boxDef.shape = &boxp;	
	boxDef.density = 1.5f;
	boxDef.friction = 1.0f;	
	boxDef.restitution = 0.0f;
	boxDef.filter.categoryBits = 0x0001;

	_playerBody->CreateFixture(&boxDef);

}
void LevelGenerator::AddBrownPlayers(CCPoint position)
{


	_Player = CCSprite::createWithSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("BrownPlayer_Walk/Right (1).png"));
	//_Player = CCSprite::spriteWithFile("P002.png");
	_Player->setScale(1.2);
	addChild(_Player, 1);
	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set((position.x+_Player->getContentSize().width/2.0f)/PTM_RATIO,
		(position.y+_Player->getContentSize().height/2.0f)/PTM_RATIO);
	bodyDef.userData = _Player;

	_playerBody = _world->CreateBody(&bodyDef);
	_playerBody->SetFixedRotation(true);

	b2FixtureDef boxDef;

	boxDef.userData = (void*)1;
	_players->push_back(_playerBody);
	b2PolygonShape boxp;
	boxp.SetAsBox(_Player->getContentSize().width/2.0f/PTM_RATIO*1.2, _Player->getContentSize().height/2.0f/PTM_RATIO*1.2);
	boxDef.shape = &boxp;	
	boxDef.density = 1.5f;
	boxDef.friction = 1.0f;	
	boxDef.restitution = 0.0f;
	boxDef.filter.categoryBits = 0x0001;

	_playerBody->CreateFixture(&boxDef);

}
void LevelGenerator::AddEnemyPlayers(CCPoint position)
{
	isEnemyFire=false;
	isEnemyKill=false;

	EnemyHealth=100;
	_EnemyPlayer = CCSprite::createWithSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("GasPlayer_Walk/Left (1).png"));
	//_Player = CCSprite::spriteWithFile("P002.png");
	_EnemyPlayer->setScale(1.5);
	addChild(_EnemyPlayer, 1);
	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set((position.x+_EnemyPlayer->getContentSize().width/2.0f)/PTM_RATIO,
		(position.y+_EnemyPlayer->getContentSize().height/2.0f)/PTM_RATIO);
	bodyDef.userData = _EnemyPlayer;

	_enemyPlayerBody = _world->CreateBody(&bodyDef);
	_enemyPlayerBody->SetFixedRotation(true);

	b2FixtureDef boxDef;

	boxDef.userData = (void*)1;
	_enemyplayers->push_back(_enemyPlayerBody);
	b2PolygonShape boxp;
	boxp.SetAsBox(_EnemyPlayer->getContentSize().width/2.0f/PTM_RATIO*1.5, _EnemyPlayer->getContentSize().height/2.0f/PTM_RATIO*1.5);
	boxDef.shape = &boxp;	
	boxDef.density = 0.5f;
	boxDef.friction = 1.0f;	
	boxDef.restitution = 0.0f;
	boxDef.filter.categoryBits = 0x0001;

	_enemyPlayerBody->CreateFixture(&boxDef);

}
void LevelGenerator::AddEnemyZombiePlayers(CCPoint position)
{
	isEnemyZombieFire=false;
	isEnemyZombieKill=false;

	EnemyZombieHealth=100;
	_EnemyZombiePlayer = CCSprite::createWithSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("ZombiePlayer_Walk/Left (1).png"));
	//_Player = CCSprite::spriteWithFile("P002.png");
	_EnemyZombiePlayer->setScale(1.2);
	addChild(_EnemyZombiePlayer, 1);
	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set((position.x+_EnemyZombiePlayer->getContentSize().width/2.0f)/PTM_RATIO,
		(position.y+_EnemyZombiePlayer->getContentSize().height/2.0f)/PTM_RATIO);
	bodyDef.userData = _EnemyZombiePlayer;

	_enemyZombiePlayerBody = _world->CreateBody(&bodyDef);
	_enemyZombiePlayerBody->SetFixedRotation(true);

	b2FixtureDef boxDef;

	boxDef.userData = (void*)1;
	_enemyzombieplayers->push_back(_enemyZombiePlayerBody);
	b2PolygonShape boxp;
	boxp.SetAsBox(_EnemyZombiePlayer->getContentSize().width/2.0f/PTM_RATIO*1.2, _EnemyZombiePlayer->getContentSize().height/2.0f/PTM_RATIO*1.2);
	boxDef.shape = &boxp;	
	boxDef.density = 0.5f;
	boxDef.friction = 1.0f;	
	boxDef.restitution = 0.0f;
	boxDef.filter.categoryBits = 0x0001;

	_enemyZombiePlayerBody->CreateFixture(&boxDef);

}
void LevelGenerator::AddJeeps(CCPoint position)
{

	Id_JeepSound=  SimpleAudioEngine::sharedEngine()->playEffect("Sound/Jeep.mp3",true);
	healthJeep=100;
	b2CircleShape circle;

	float	m_hz = 4.0f;
	float m_zeta = 0.7f;
	float m_speed = 50.0f;

	CCSprite *sp =CCSprite::createWithSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("JeepHead.png"));
	addChild(sp, 1);

	b2BodyDef bd;
	bd.type = b2_dynamicBody;
	bd.userData=sp;
	bd.position.Set(position.x/PTM_RATIO,25.0f/PTM_RATIO);
	m_car = _world->CreateBody(&bd);
	b2FixtureDef boxDef;
	boxDef.userData = (void*)1;
	_jeeps->push_back(m_car );
	b2PolygonShape box2;
	box2.SetAsBox(sp->getContentSize().width/2.0f/PTM_RATIO, sp->getContentSize().height/2.0f/PTM_RATIO);
	boxDef.shape = &box2;	
	boxDef.density = 1.5f;

	m_car->CreateFixture(&boxDef);

	// GB2ShapeCache *sc = GB2ShapeCache::sharedGB2ShapeCache();
	// sc->addFixturesToBody(m_car, "JeepHead",false); 


	sp = CCSprite::createWithSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("wheeljeep.png"));
	circle.m_radius =sp->getContentSize().width/2.0f/PTM_RATIO;
	b2FixtureDef fd;
	fd.shape = &circle;
	fd.density = 1.0f;
	fd.friction = 0.9f;
	//fd.userData = (void*)1;
	addChild(sp, 1,20);
	bd.userData=sp;
	bd.position.Set((position.x-45.0f)/PTM_RATIO,0.0f/PTM_RATIO);
	m_wheel1 = _world->CreateBody(&bd);
	//_jeep_wheel1->push_back(m_wheel1 );
	m_wheel1->CreateFixture(&fd);

	sp = CCSprite::createWithSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("wheeljeep.png"));
	addChild(sp, 1,21);
	bd.userData=sp;
	bd.position.Set((position.x+40.0f)/PTM_RATIO,0.0f/PTM_RATIO);
	m_wheel2 = _world->CreateBody(&bd);
	//_jeep_wheel2->push_back(m_wheel2 );
	m_wheel2->CreateFixture(&fd);

	b2WheelJointDef jd;
	b2Vec2 axis(0.0f/PTM_RATIO,35.0f/PTM_RATIO);

	jd.Initialize(m_car, m_wheel1, m_wheel1->GetPosition(), axis);
	jd.motorSpeed = 0.0f;
	jd.maxMotorTorque = 20.0f;
	jd.enableMotor = true;
	jd.frequencyHz = m_hz;
	jd.dampingRatio = m_zeta;
	m_spring1 = (b2WheelJoint*)_world->CreateJoint(&jd);


	b2Vec2 axis2(0.0f/PTM_RATIO,35.0f/PTM_RATIO);
	jd.Initialize(m_car, m_wheel2, m_wheel2->GetPosition(), axis2);
	jd.motorSpeed = 0.0f;
	jd.maxMotorTorque = 10.0f;
	jd.enableMotor = false;
	jd.frequencyHz = m_hz;
	jd.dampingRatio = m_zeta;
	m_spring2 = (b2WheelJoint*)_world->CreateJoint(&jd);

}
void LevelGenerator::AddBaseCamp(CCPoint position)
{
	CCSprite *sprite = CCSprite::create("SpriteImages/basecamp.png");
	addChild(sprite, 1);
	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set((position.x+sprite->getContentSize().width/2.0f)/PTM_RATIO,

		(position.y+sprite->getContentSize().height/2.0f)/PTM_RATIO);
	bodyDef.userData = sprite;
	_baseCampBody = _world->CreateBody(&bodyDef);
	b2FixtureDef boxDef;
	//boxDef.userData = (void*)1;
	//_baseCamp->push_back(_baseCampBody);
	b2PolygonShape box;
	box.SetAsBox(sprite->getContentSize().width/2.0f/PTM_RATIO, sprite->getContentSize().height/2.0f/PTM_RATIO);
	boxDef.shape = &box;	
	boxDef.density = 10.5f;
	boxDef.friction = 1.0f;	
	boxDef.restitution = 0.0f;
	_baseCampBody->CreateFixture(&boxDef);



}
void LevelGenerator::AddTanks(CCPoint position)
{

	healthTank=100;

	b2CircleShape circle;

	float m_hz = 4.0f;
	float m_zeta = 0.7f;
	float m_speed = 50.0f;
	CCSprite *sp = CCSprite::create("SpriteImages/TankHead1.png");
	if(levelT==1)
		sp = CCSprite::create("SpriteImages/TankHead1.png");
	if(levelT==2)
		sp = CCSprite::create("SpriteImages/TankHead2.png");
	if(levelT==3)
		sp = CCSprite::create("SpriteImages/TankHead3.png");
	//CCSprite *sp = CCSprite::spriteWithSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("truckGun.png"));

	this->addChild(sp, 1);
	b2BodyDef bd;
	bd.type = b2_dynamicBody;
	bd.userData=sp;
	bd.position.Set(position.x/PTM_RATIO,40.0f/PTM_RATIO);
	b2FixtureDef boxDef;
	boxDef.userData = (void*)1;

	b2PolygonShape box2;
	box2.SetAsBox(sp->getContentSize().width/2.0f/PTM_RATIO, ((sp->getContentSize().height/2.0f)-30)/PTM_RATIO);
	/*	new Vector2(-0.48187f*width, +0.08696f*height),
	new Vector2(+0.18135f*width, +0.54348f*height),
	new Vector2(+0.13990f*width, +0.22826f*height),
	new Vector2(+0.14508f*width, +0.19565f*height),
	new Vector2(+0.45078f*width, -0.07609f*height),
	new Vector2(+0.48187f*width, -0.46739f*height),
	new Vector2(-0.50777f*width, -0.44565f*height),,*/
	/*   float width=193.0,height=92.0;
	b2Vec2 Tank_vertices[6];
	Tank_vertices[0].Set(-0.48187f*width, +0.08696f*height);
	Tank_vertices[1].Set(+0.18135f*width, +0.54348f*height);
	Tank_vertices[2].Set(+0.14508f*width, +0.19565f*height);
	Tank_vertices[3].Set(+0.45078f*width, -0.07609f*height);
	Tank_vertices[4].Set(+0.48187f*width, -0.46739f*height);
	Tank_vertices[5].Set(-0.50777f*width, -0.44565f*height);
	box2.Set(Tank_vertices, 6);*/


	boxDef.shape = &box2;	
	boxDef.density = 1.0f;
	m_tank = _world->CreateBody(&bd);


	_tanks->push_back(m_tank);
	m_tank ->CreateFixture(&boxDef);
	// GB2ShapeCache *sc = GB2ShapeCache::sharedGB2ShapeCache();
	//  sc->addFixturesToBody(m_truck, "truckGun",false); 
	//////////////////////////////////////////////////////////////////////


	//// Head ------
	//sp = CCSprite::spriteWithFile("PlayerBody\\Torso.png");
	//box2.SetAsBox(sp->getContentSize().width/2.0f/PTM_RATIO, sp->getContentSize().height/2.0f/PTM_RATIO);
	//boxDef.shape = &box2;
	//boxDef.density = 0.1f;
	//boxDef.friction = 0.4f;
	//boxDef.restitution = 0.1f;
	//
	//addChild(sp, 0);
	//bd.userData=sp;
	//bd.position.Set((position.x)/PTM_RATIO,60.0f/PTM_RATIO);
	//torso1 = _world->CreateBody(&bd);
	//torso1->CreateFixture(&boxDef);


	//b2Transform axisTransform;
	//axisTransform.Set(b2Vec2(0, 0), m_tank->GetAngle());
	//b2Vec2 axis = b2Mul(axisTransform.q, b2Vec2(0,1));
	//b2Vec2 axist(0.0f/PTM_RATIO,60.0f/PTM_RATIO);
	//b2PrismaticJointDef prisJointDef;
	//prisJointDef.Initialize(m_tank, torso1,
	//	torso1->GetWorldCenter(),axist);
	//prisJointDef.enableLimit = true;
	//prisJointDef.lowerTranslation = 0.0;
	//prisJointDef.upperTranslation = 43.0/PTM_RATIO ;
	//_world->CreateJoint(&prisJointDef);


	//	b2RevoluteJointDef rj;
	//rj.enableLimit = true;
	//   
	//// Head to shoulders
	//rj.lowerAngle = -20.0f / (180.0f / M_PI);
	//rj.upperAngle = 20.0f / (180.0f / M_PI);
	//rj.Initialize(m_tank, torso1,torso1->GetWorldCenter());
	//_world->CreateJoint(&rj);

	/////////////////////
	//sp = CCSprite::spriteWithSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("TuckWheel.png"));
	sp =CCSprite::create("SpriteImages/TankWheel.png");
	circle.m_radius =sp->getContentSize().width/2.0f/PTM_RATIO;
	b2FixtureDef fd;
	fd.shape = &circle;
	fd.density = 1.0f;
	fd.friction = 0.9f;

	addChild(sp, 1,31);
	bd.userData=sp;
	bd.position.Set((position.x-70.0f)/PTM_RATIO,0.0f/PTM_RATIO);
	tank_wheel1 = _world->CreateBody(&bd);
	tank_wheel1->CreateFixture(&fd);
	///////////////////////

	sp =CCSprite::create("SpriteImages/TankWheel.png");
	//sp = CCSprite::spriteWithSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("TuckWheel.png"));
	addChild(sp, 1,32);
	bd.userData=sp;
	bd.position.Set((position.x-30.0f)/PTM_RATIO,0.0f/PTM_RATIO);
	tank_wheel2 = _world->CreateBody(&bd);
	tank_wheel2->CreateFixture(&fd);

	////////////////////////////////////
	sp =CCSprite::create("SpriteImages/TankWheel.png");
	//sp = CCSprite::spriteWithSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("TuckWheel.png"));
	addChild(sp, 1,33);
	bd.userData=sp;
	bd.position.Set((position.x+10.0f)/PTM_RATIO,0.0f/PTM_RATIO);
	tank_wheel3 = _world->CreateBody(&bd);
	tank_wheel3->CreateFixture(&fd);

	////////////////////////////////////
	sp =CCSprite::create("SpriteImages/TankWheel.png");
	//sp = CCSprite::spriteWithSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("TuckWheel.png"));
	addChild(sp, 1,34);
	bd.userData=sp;
	bd.position.Set((position.x+50.0f)/PTM_RATIO,0.0f/PTM_RATIO);
	tank_wheel4 = _world->CreateBody(&bd);
	tank_wheel4->CreateFixture(&fd);
	////////////////////////////////////
	sp =CCSprite::create("SpriteImages/TankWheel.png");
	//sp = CCSprite::spriteWithSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("TuckWheel.png"));
	addChild(sp, 1,35);
	bd.userData=sp;
	bd.position.Set((position.x+90.0f)/PTM_RATIO,0.0f/PTM_RATIO);
	tank_wheel5 = _world->CreateBody(&bd);
	tank_wheel5->CreateFixture(&fd);
	////////////////////




	bd.type = b2_dynamicBody;

	//b2PolygonShape box;
	b2FixtureDef fixtureDef;






	b2WheelJointDef jd;
	b2Vec2 axis(0.0f/PTM_RATIO,35.0f/PTM_RATIO);
	jd.collideConnected=false;
	jd.Initialize(m_tank, tank_wheel1, tank_wheel1->GetPosition(), axis);
	jd.motorSpeed = 0.0f;
	jd.maxMotorTorque = 20.0f;
	jd.enableMotor = true;
	jd.frequencyHz = m_hz;
	jd.dampingRatio = m_zeta;
	tank_spring1 = (b2WheelJoint*)_world->CreateJoint(&jd);
	///////////////////////

	b2Vec2 axis2(0.0f/PTM_RATIO,35.0f/PTM_RATIO);
	jd.Initialize(m_tank, tank_wheel2, tank_wheel2->GetPosition(), axis2);
	jd.motorSpeed = 0.0f;
	jd.maxMotorTorque = 20.0f;
	jd.enableMotor = false;
	jd.frequencyHz = m_hz;
	jd.dampingRatio = m_zeta;
	jd.collideConnected=false;
	tank_spring2 = (b2WheelJoint*)_world->CreateJoint(&jd);

	///////////////////////////////////////////////////////

	///////////////////////

	b2Vec2 axis3(0.0f/PTM_RATIO,35.0f/PTM_RATIO);
	jd.Initialize(m_tank, tank_wheel3, tank_wheel3->GetPosition(), axis3);
	jd.motorSpeed = 0.0f;
	jd.maxMotorTorque = 10.0f;
	jd.enableMotor = false;
	jd.frequencyHz = m_hz;
	jd.dampingRatio = m_zeta;
	jd.collideConnected=false;
	tank_spring3 = (b2WheelJoint*)_world->CreateJoint(&jd);

	///////////////////////////////////////////////////////


	///////////////////////

	b2Vec2 axis4(0.0f/PTM_RATIO,35.0f/PTM_RATIO);
	jd.Initialize(m_tank,tank_wheel4, tank_wheel4->GetPosition(), axis4);
	jd.motorSpeed = 0.0f;
	jd.maxMotorTorque = 10.0f;
	jd.enableMotor = false;
	jd.frequencyHz = m_hz;
	jd.dampingRatio = m_zeta;
	jd.collideConnected=false;
	tank_spring4 = (b2WheelJoint*)_world->CreateJoint(&jd);

	///////////////////////

	b2Vec2 axis5(0.0f/PTM_RATIO,35.0f/PTM_RATIO);
	jd.Initialize(m_tank,tank_wheel5, tank_wheel5->GetPosition(), axis5);
	jd.motorSpeed = 0.0f;
	jd.maxMotorTorque = 10.0f;
	jd.enableMotor = false;
	jd.frequencyHz = m_hz;
	jd.dampingRatio = m_zeta;
	jd.collideConnected=false;
	tank_spring5 = (b2WheelJoint*)_world->CreateJoint(&jd);

	///////////////////////////////////////////////////////

}
void LevelGenerator::AddTrucks(CCPoint position)
{

	healthTruck=100;

	b2CircleShape circle;

	float m_hz = 4.0f;
	float m_zeta = 0.7f;
	float m_speed = 50.0f;

	//CCSprite *sp = CCSprite::spriteWithFile("truckGun.png");
	//CCSprite *sp = CCSprite::spriteWithSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("truckGun.png"));

	CCSprite *sp = CCSprite::create("SpriteImages/truckGun1.png");
	if(levelT==1)
		sp = CCSprite::create("SpriteImages/truckGun1.png");
	if(levelT==2)
		sp = CCSprite::create("SpriteImages/truckGun1.png");
	if(levelT==3)
		sp = CCSprite::create("SpriteImages/truckGun3.png");


	this->addChild(sp, 1);
	b2BodyDef bd;
	bd.type = b2_dynamicBody;
	bd.userData=sp;
	bd.position.Set(position.x/PTM_RATIO,37.0f/PTM_RATIO);
	b2FixtureDef boxDef;
	boxDef.userData = (void*)1;

	b2PolygonShape box2;
	box2.SetAsBox(sp->getContentSize().width/2.0f/PTM_RATIO, sp->getContentSize().height/2.0f/PTM_RATIO);
	boxDef.shape = &box2;	
	boxDef.density = 2.0f;
	m_truck = _world->CreateBody(&bd);


	_trucks->push_back(m_truck);
	m_truck ->CreateFixture(&boxDef);
	// GB2ShapeCache *sc = GB2ShapeCache::sharedGB2ShapeCache();
	//  sc->addFixturesToBody(m_truck, "truckGun",false); 



	/////////////////////
	sp = CCSprite::createWithSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("TuckWheel.png"));
	circle.m_radius =sp->getContentSize().width/2.0f/PTM_RATIO;
	b2FixtureDef fd;
	fd.shape = &circle;
	fd.density = 2.0f;
	fd.friction = 0.9f;

	addChild(sp, 1,41);
	bd.userData=sp;
	bd.position.Set((position.x-80.0f)/PTM_RATIO,0.0f/PTM_RATIO);
	truck_wheel1 = _world->CreateBody(&bd);
	truck_wheel1->CreateFixture(&fd);
	///////////////////////


	sp = CCSprite::createWithSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("TuckWheel.png"));
	addChild(sp, 1,42);
	bd.userData=sp;
	bd.position.Set((position.x-40.0f)/PTM_RATIO,0.0f/PTM_RATIO);
	truck_wheel2 = _world->CreateBody(&bd);
	truck_wheel2->CreateFixture(&fd);

	////////////////////////////////////

	sp = CCSprite::createWithSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("TuckWheel.png"));
	addChild(sp, 1,43);
	bd.userData=sp;
	bd.position.Set((position.x+55.0f)/PTM_RATIO,0.0f/PTM_RATIO);
	truck_wheel3 = _world->CreateBody(&bd);
	truck_wheel3->CreateFixture(&fd);

	////////////////////////////////////

	sp = CCSprite::createWithSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("TuckWheel.png"));
	addChild(sp, 1,44);
	bd.userData=sp;
	bd.position.Set((position.x+93.0f)/PTM_RATIO,0.0f/PTM_RATIO);
	truck_wheel4 = _world->CreateBody(&bd);
	truck_wheel4->CreateFixture(&fd);
	////////////////////


	b2WheelJointDef jd;
	b2Vec2 axis(0.0f/PTM_RATIO,30.0f/PTM_RATIO);
	jd.collideConnected=false;
	jd.Initialize(m_truck, truck_wheel1, truck_wheel1->GetPosition(), axis);
	jd.motorSpeed = 0.0f;
	jd.maxMotorTorque = 20.0f;
	jd.enableMotor = true;
	jd.frequencyHz = m_hz;
	jd.dampingRatio = m_zeta;
	truck_spring1 = (b2WheelJoint*)_world->CreateJoint(&jd);
	///////////////////////

	b2Vec2 axis2(0.0f/PTM_RATIO,30.0f/PTM_RATIO);
	jd.Initialize(m_truck, truck_wheel2, truck_wheel2->GetPosition(), axis2);
	jd.motorSpeed = 0.0f;
	jd.maxMotorTorque = 20.0f;
	jd.enableMotor = false;
	jd.frequencyHz = m_hz;
	jd.dampingRatio = m_zeta;
	jd.collideConnected=false;
	truck_spring2 = (b2WheelJoint*)_world->CreateJoint(&jd);

	///////////////////////////////////////////////////////

	///////////////////////

	b2Vec2 axis3(0.0f/PTM_RATIO,30.0f/PTM_RATIO);
	jd.Initialize(m_truck, truck_wheel3, truck_wheel3->GetPosition(), axis3);
	jd.motorSpeed = 0.0f;
	jd.maxMotorTorque = 10.0f;
	jd.enableMotor = false;
	jd.frequencyHz = m_hz;
	jd.dampingRatio = m_zeta;
	jd.collideConnected=false;
	truck_spring3 = (b2WheelJoint*)_world->CreateJoint(&jd);

	///////////////////////////////////////////////////////


	///////////////////////

	b2Vec2 axis4(0.0f/PTM_RATIO,30.0f/PTM_RATIO);
	jd.Initialize(m_truck, truck_wheel4, truck_wheel4->GetPosition(), axis4);
	jd.motorSpeed = 0.0f;
	jd.maxMotorTorque = 10.0f;
	jd.enableMotor = false;
	jd.frequencyHz = m_hz;
	jd.dampingRatio = m_zeta;
	jd.collideConnected=false;
	truck_spring4 = (b2WheelJoint*)_world->CreateJoint(&jd);

	///////////////////////////////////////////////////////

}
void LevelGenerator::AddHelicopters(CCPoint position)
{
	CCSprite *sprite = CCSprite::createWithSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("helicopter3.png"));
	sprite->setScale(0.9);
	addChild(sprite, 1);
	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set((position.x+sprite->getContentSize().width/2.0f)/PTM_RATIO,
		(position.y+sprite->getContentSize().height/2.0f)/PTM_RATIO);
	bodyDef.userData = sprite;
	b2Body *body = _world->CreateBody(&bodyDef);
	body->SetGravityScale(0);
	body->SetFixedRotation(true);



	_helicopterBody=body;


	b2FixtureDef boxDef;
	boxDef.userData = (void*)1;


	_helicopters->push_back(body);
	b2PolygonShape box;
	box.SetAsBox(sprite->getContentSize().width/2.0f/PTM_RATIO, sprite->getContentSize().height/2.0f/PTM_RATIO);
	boxDef.shape = &box;	
	boxDef.density = 1.5f;
	boxDef.friction = 1.0f;	
	boxDef.restitution = 0.0f;
	boxDef.isSensor=true;
	boxDef.filter.categoryBits = 0x0002;
	body->CreateFixture(&boxDef);

	// add the fixture definitions to the body
	//GB2ShapeCache *sc = GB2ShapeCache::sharedGB2ShapeCache();
	//sc->addFixturesToBody(body, "helicopter3",true); 

	// sprite->setAnchorPoint(sc->anchorPointForShape("helicopter3"));

	//Play heli sound
	this->runAction( CCSequence::create(
		CCDelayTime::create(7.0f),
		CCCallFunc::create(this, 
		callfunc_selector(LevelGenerator::playHeliSound)), 
		NULL) );


	//end of heli sound





}
void LevelGenerator::playHeliSound()
{
	/*float playerpos=_playerBody->GetPosition().x;
	float helipos=_helicopterBody->GetPosition().x;
	if(playerpos<_helicopterBody->GetPosition().x-10 && _playerBody->GetPosition().x>_helicopterBody->GetPosition().x-20)
	{*/
	//SimpleAudioEngine::sharedEngine()->playEffect("Sound/Helicopter.mp3");
	//	}
}
void LevelGenerator::AddLongRocks(CCPoint position)
{
	CCSprite *sprite = CCSprite::create("SpriteImages/Longrock.png");
	addChild(sprite, 1);
	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set((position.x+sprite->getContentSize().width/2.0f)/PTM_RATIO,
		(position.y+sprite->getContentSize().height/2.0f)/PTM_RATIO);
	bodyDef.userData = sprite;
	b2Body *body = _world->CreateBody(&bodyDef);
	b2FixtureDef boxDef;
	//boxDef.userData = (void*)1;
	//_rocks->push_back(body);
	b2PolygonShape box;
	box.SetAsBox(sprite->getContentSize().width/2.0f/PTM_RATIO, sprite->getContentSize().height/2.0f/PTM_RATIO);
	boxDef.shape = &box;	
	boxDef.density = 0.5f;	
	//boxDef.friction = 1.0f;	
	//boxDef.restitution = 0.0f;
	body->CreateFixture(&boxDef);



}
void LevelGenerator::AddRocks(CCPoint position)
{
	CCSprite *sprite = CCSprite::create("SpriteImages/rock.png");

	addChild(sprite, 1);
	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set((position.x+sprite->getContentSize().width/2.0f)/PTM_RATIO,
		(position.y+sprite->getContentSize().height/2.0f)/PTM_RATIO);
	bodyDef.userData = sprite;
	_rockBody = _world->CreateBody(&bodyDef);
	b2FixtureDef boxDef;
	boxDef.userData = (void*)1;
	_rocks->push_back(_rockBody);
	b2PolygonShape box;
	box.SetAsBox(sprite->getContentSize().width/2.0f/PTM_RATIO, sprite->getContentSize().height/2.0f/PTM_RATIO);
	boxDef.shape = &box;	
	boxDef.density = 0.5f;
	boxDef.friction = 1.0f;	
	boxDef.restitution = 0.0f;
	_rockBody->CreateFixture(&boxDef);



}
void LevelGenerator::AddWoods(CCPoint position)
{
	//CCSprite *sprite = CCSprite::spriteWithFile("SpriteImages/rock.png");
	CCSprite *sprite = CCSprite::create("SpriteImages/wood2.png");
	addChild(sprite, 1);
	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set((position.x+sprite->getContentSize().width/2.0f)/PTM_RATIO,
		(position.y+sprite->getContentSize().height/2.0f)/PTM_RATIO);
	bodyDef.userData = sprite;
	_woodBody = _world->CreateBody(&bodyDef);
	b2FixtureDef boxDef;
	boxDef.userData = (void*)1;
	_woods->push_back(_woodBody);
	b2PolygonShape box;
	box.SetAsBox(sprite->getContentSize().width/2.0f/PTM_RATIO, sprite->getContentSize().height/2.0f/PTM_RATIO);
	boxDef.shape = &box;	
	boxDef.density = 0.5f;
	boxDef.friction = 1.0f;	
	boxDef.restitution = 0.0f;
	_woodBody->CreateFixture(&boxDef);



}
void LevelGenerator::AddPowers(CCPoint position)
{
	CCSprite *sprite = CCSprite::create("SpriteImages/power.png");
	addChild(sprite, 1);
	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set((position.x+sprite->getContentSize().width/2.0f)/PTM_RATIO,
		(position.y+sprite->getContentSize().height/2.0f)/PTM_RATIO);
	bodyDef.userData = sprite;
	_powerBody = _world->CreateBody(&bodyDef);
	b2FixtureDef boxDef;
	boxDef.userData = (void*)1;
	_powers->push_back(_powerBody);
	b2PolygonShape box;
	box.SetAsBox(sprite->getContentSize().width/2.0f/PTM_RATIO, sprite->getContentSize().height/2.0f/PTM_RATIO);
	boxDef.shape = &box;	
	boxDef.density = 0.5f;
	boxDef.friction = 0.2f;	
	boxDef.restitution = 0.0f;
	_powerBody->CreateFixture(&boxDef);



}
void LevelGenerator::AddLongFixRocks(CCPoint position)
{
	CCSprite *sprite = CCSprite::create("SpriteImages/Longrock.png");
	addChild(sprite, 1);
	b2BodyDef bodyDef;
	bodyDef.type = b2_staticBody;
	bodyDef.position.Set((position.x+sprite->getContentSize().width/2.0f)/PTM_RATIO,
		(position.y+sprite->getContentSize().height/2.0f)/PTM_RATIO);
	bodyDef.userData = sprite;
	b2Body *body = _world->CreateBody(&bodyDef);
	b2FixtureDef boxDef;
	//boxDef.userData = (void*)1;
	//_rocks->push_back(body);
	b2PolygonShape box;
	box.SetAsBox(sprite->getContentSize().width/2.0f/PTM_RATIO, sprite->getContentSize().height/2.0f/PTM_RATIO);
	boxDef.shape = &box;	
	boxDef.density = 1.0f;	
	boxDef.friction = 1.0f;	
	boxDef.restitution = 0.5f;
	body->CreateFixture(&boxDef);

	///////////////////////////////////////////////////////////////////



	b2Vec2 startPos ((position.x+sprite->getContentSize().width/2.0f)/PTM_RATIO,
		(position.y-100)/PTM_RATIO);
	float linkHeight = 0.24;
	float linkWidth = 0.1;


	//body and fixture defs are common to all chain links
	// b2BodyDef bodyDef;
	CCSprite *segmentSprite = CCSprite::create("SpriteImages/rope_seg_new2.png");
	addChild(segmentSprite, 1);

	bodyDef.userData=segmentSprite;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position = startPos;
	b2FixtureDef fixtureDef;
	fixtureDef.density = 0.5;
	b2PolygonShape polygonShape;
	polygonShape.SetAsBox(linkWidth,linkHeight);
	fixtureDef.shape = &polygonShape;


	//create first link
	b2Body* link = _world->CreateBody( &bodyDef );
	link->CreateFixture( &fixtureDef );




	//set up the common properties of the joint before entering the loop
	b2RevoluteJointDef revoluteJointDef;
	revoluteJointDef.localAnchorA.Set( 0,  linkHeight);
	revoluteJointDef.localAnchorB.Set( 0, -linkHeight);
	b2Body* prevLink=link;
	//use same definitions to create multiple bodies
	for (int i = 0; i < 5; i++) {
		CCSprite *segmentSprite = CCSprite::create("SpriteImages/rope_seg_new2.png");
		addChild(segmentSprite, 1);
		bodyDef.userData=segmentSprite;
		b2Body*  newLink = _world->CreateBody( &bodyDef );
		newLink->CreateFixture( &fixtureDef );



		//  link.

		revoluteJointDef.bodyA = link;
		revoluteJointDef.bodyB = newLink;
		_world->CreateJoint( &revoluteJointDef );

		link = newLink;//prepare for next iteration
	}


	//CCSprite *circleBodySprite = CCSprite::spriteWithFile("circle2x_bounce.png");
	// addChild(circleBodySprite, 1);
	//body with circle fixture
	//  b2CircleShape circleShape;
	// circleShape.m_radius = circleBodySprite->getContentSize().width/2 / PTM_RATIO;
	CCSprite *spriteTNT = CCSprite::create("SpriteImages/TNT2.png");
	addChild(spriteTNT, 1);

	box.SetAsBox(spriteTNT->getContentSize().width/2.0f/PTM_RATIO, spriteTNT->getContentSize().height/2.0f/PTM_RATIO);
	//boxDef.shape = &box;
	fixtureDef.shape = &box;
	bodyDef.position .Set((position.x+spriteTNT->getContentSize().width/2.0f)/PTM_RATIO,
		(position.y-50+spriteTNT->getContentSize().height/2.0f)/PTM_RATIO);
	fixtureDef.density = 0.5;
	bodyDef.type = b2_dynamicBody;
	bodyDef.userData=spriteTNT;
	_TNTBody=_world->CreateBody( &bodyDef );
	//b2Body* chainBase =_world->CreateBody( &bodyDef );
	fixtureDef.userData = (void*)1;
	_tnt->push_back(_TNTBody);
	_TNTBody->CreateFixture( &fixtureDef );
	/////////////////////////////////////////////

	//a revolute joint to connect the circle to the ground
	revoluteJointDef.collideConnected=false;
	revoluteJointDef.bodyA = _body;//provided by testbed
	revoluteJointDef.bodyB = body;
	revoluteJointDef.localAnchorA = startPos;//world coords, because m_groundBody is at (0,0)
	revoluteJointDef.localAnchorB.Set(0,0);//center of circle
	_world->CreateJoint( &revoluteJointDef );


	/////////////////////////////////////////////////

	//another revolute joint to connect the chain to the circle
	revoluteJointDef.collideConnected=false;
	revoluteJointDef.bodyA = _TNTBody;//the last added link of the chain

	revoluteJointDef.bodyB = prevLink;

	//the regular position for chain link joints, as above
	revoluteJointDef.localAnchorA.Set(0,linkWidth);

	//a little in from the edge of the circle
	revoluteJointDef.localAnchorB.Set(0,linkWidth);
	_world->CreateJoint( &revoluteJointDef );

	////////////////////////////////////////////////
	//another revolute joint to connect the chain to the circle
	revoluteJointDef.collideConnected=false;
	revoluteJointDef.bodyA = body;//the last added link of the chain
	revoluteJointDef.bodyB = link;

	//the regular position for chain link joints, as above
	revoluteJointDef.localAnchorA.Set(0,linkWidth-0.5);

	//a little in from the edge of the circle
	revoluteJointDef.localAnchorB.Set(0,linkWidth);
	_world->CreateJoint( &revoluteJointDef );


	////////////////////////////////////////////////




}
void LevelGenerator::AddBridge(CCPoint position)
{
	CCSprite *sprite = CCSprite::create("SpriteImages/rock.png");
	addChild(sprite, 1);
	b2BodyDef bodyDef;
	bodyDef.type = b2_staticBody;
	bodyDef.position.Set((position.x+sprite->getContentSize().width/2.0f)/PTM_RATIO,
		(position.y+sprite->getContentSize().height/2.0f)/PTM_RATIO);
	bodyDef.userData = sprite;
	b2Body *body = _world->CreateBody(&bodyDef);
	b2FixtureDef boxDef;
	//boxDef.userData = (void*)1;
	//_rocks->push_back(body);
	b2PolygonShape box;
	box.SetAsBox(sprite->getContentSize().width/2.0f/PTM_RATIO, sprite->getContentSize().height/2.0f/PTM_RATIO);
	boxDef.shape = &box;	
	boxDef.density = 1.0f;	
	boxDef.friction = 1.0f;	
	boxDef.restitution = 0.5f;
	body->CreateFixture(&boxDef);

	///////////////////////////////////////////////////////////////////



	b2Vec2 startPos ((position.x+sprite->getContentSize().width/2.0f)/PTM_RATIO,
		(position.y-100)/PTM_RATIO);
	float linkHeight = 0.24;
	float linkWidth = 0.1;


	//body and fixture defs are common to all chain links
	// b2BodyDef bodyDef;
	CCSprite *segmentSprite = CCSprite::create("SpriteImages/rope_seg_new2.png");
	addChild(segmentSprite, 1);

	bodyDef.userData=segmentSprite;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position = startPos;
	b2FixtureDef fixtureDef;
	fixtureDef.density = 0.5;
	b2PolygonShape polygonShape;
	polygonShape.SetAsBox(linkWidth,linkHeight);
	fixtureDef.shape = &polygonShape;


	//create first link
	b2Body* link = _world->CreateBody( &bodyDef );
	link->CreateFixture( &fixtureDef );




	//set up the common properties of the joint before entering the loop
	b2RevoluteJointDef revoluteJointDef;
	revoluteJointDef.localAnchorA.Set( 0,  linkHeight);
	revoluteJointDef.localAnchorB.Set( 0, -linkHeight);
	b2Body* prevLink=link;

	//use same definitions to create multiple bodies
	for (int i = 0; i < 5; i++) {
		CCSprite *segmentSprite = CCSprite::create("SpriteImages/rope_seg_new2.png");
		addChild(segmentSprite, 1);
		bodyDef.userData=segmentSprite;
		b2Body*  newLink = _world->CreateBody( &bodyDef );
		newLink->CreateFixture( &fixtureDef );



		//  link.

		revoluteJointDef.bodyA = link;
		revoluteJointDef.bodyB = newLink;
		_world->CreateJoint( &revoluteJointDef );

		link = newLink;//prepare for next iteration
	}


	//CCSprite *circleBodySprite = CCSprite::spriteWithFile("circle2x_bounce.png");
	// addChild(circleBodySprite, 1);
	//body with circle fixture
	//  b2CircleShape circleShape;
	// circleShape.m_radius = circleBodySprite->getContentSize().width/2 / PTM_RATIO;
	CCSprite *spriteTNT = CCSprite::create("SpriteImages/rock.png");
	addChild(spriteTNT, 1);

	box.SetAsBox(spriteTNT->getContentSize().width/2.0f/PTM_RATIO, spriteTNT->getContentSize().height/2.0f/PTM_RATIO);
	//boxDef.shape = &box;
	fixtureDef.shape = &box;
	bodyDef.position .Set((position.x-115+spriteTNT->getContentSize().width/2.0f)/PTM_RATIO,
		(position.y+spriteTNT->getContentSize().height/2.0f)/PTM_RATIO);
	fixtureDef.density = 0.5;
	bodyDef.type = b2_staticBody;
	bodyDef.userData=spriteTNT;
	b2Body *_TNTBody =_world->CreateBody( &bodyDef );
	//b2Body* chainBase =_world->CreateBody( &bodyDef );
	//fixtureDef.userData = (void*)1;
	//_tnt->push_back(_TNTBody);
	_TNTBody->CreateFixture( &fixtureDef );
	/////////////////////////////////////////////

	//a revolute joint to connect the circle to the ground
	revoluteJointDef.collideConnected=false;
	revoluteJointDef.bodyA = _body;//provided by testbed
	revoluteJointDef.bodyB = body;
	revoluteJointDef.localAnchorA = startPos;//world coords, because m_groundBody is at (0,0)
	revoluteJointDef.localAnchorB.Set(0,0);//center of circle
	_world->CreateJoint( &revoluteJointDef );


	/////////////////////////////////////////////////

	//another revolute joint to connect the chain to the circle
	revoluteJointDef.collideConnected=false;
	revoluteJointDef.bodyA = _TNTBody;//the last added link of the chain

	revoluteJointDef.bodyB = prevLink;

	//the regular position for chain link joints, as above
	revoluteJointDef.localAnchorA.Set(0.6,linkWidth-0.1);

	//a little in from the edge of the circle
	revoluteJointDef.localAnchorB.Set(0,linkWidth);
	_world->CreateJoint( &revoluteJointDef );

	////////////////////////////////////////////////
	//another revolute joint to connect the chain to the circle
	revoluteJointDef.collideConnected=false;
	revoluteJointDef.bodyA = body;//the last added link of the chain
	revoluteJointDef.bodyB = link;

	//the regular position for chain link joints, as above
	revoluteJointDef.localAnchorA.Set(-0.4,linkWidth-0.1);

	//a little in from the edge of the circle
	revoluteJointDef.localAnchorB.Set(0,linkWidth);
	_world->CreateJoint( &revoluteJointDef );


	////////////////////////////////////////////////




}
/////////////////////////////////////////////////////////////////////////////////////////////////
//   UPDATE TICK 
////////////////////////////////////////////////////////////////////////////////////////////////
void LevelGenerator::update(float dt)
{


	//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ INFINITE SCROLLING @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

	CCPoint backgroundScrollVert = ccp(-500,0) ;
	_backgroundNode->setPosition(ccpAdd(_backgroundNode->getPosition(),ccpMult(backgroundScrollVert,dt))) ; 

	CCArray *clouds = CCArray::createWithCapacity(8) ;
	clouds->addObject(_cloud1) ;
	clouds->addObject(_cloud2) ;
	clouds->addObject(_cloud3) ;
	clouds->addObject(_cloud4) ;
	clouds->addObject(_bird1) ;
	clouds->addObject(_bird2) ;
	clouds->addObject(_bird3) ;
	clouds->addObject(_bird4) ;
	for ( int ii = 0  ; ii <clouds->count() ; ii++ ) {
		CCSprite * spclouds = (CCSprite *)(clouds->objectAtIndex(ii)) ;
		float xPosition = _backgroundNode->convertToWorldSpace(spclouds->getPosition()).x  ;
		float size = spclouds->getContentSize().width ;
		if ( xPosition < -size ) {
			_backgroundNode->incrementOffset(ccp(980,0),spclouds) ; 

		}

	}
	////////////////////////////////////////////////////////////////////

	if(m_car->GetPosition().x<=0 && healthJeep>0)
		healthJeep=0;


	if(m_tank->GetPosition().x<=200/ PTM_RATIO && healthTank>0)
		healthTank=0;

	if(m_truck->GetPosition().x<=200/ PTM_RATIO   && healthTruck>0 )
		healthTruck=0;



	////////////////////////////////////////////////////////////////////
	if(isPlayerLive)
	{
		if(isPowerGive==false && _Player->getPosition().x >920)
		{
			AddPowers(CCPointMake(750.0f, 150.0f));

			isPowerGive=true;
			SimpleAudioEngine::sharedEngine()->playEffect("Sound/powerup.mp3");
			_hud->showHealthPackAlert();

		}

		//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ HELICOPTER IMPLUSE @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
		_helicopterBody->ApplyLinearImpulse( b2Vec2(-0.3,0), _helicopterBody->GetWorldCenter() );
		//CCLog("Helicopter x %f",_helicopterBody->GetPosition().x*PTM_RATIO);

		//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ JEEP OR TRUCK IMPLUSE @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
		float vechicle_speed =3.0;

		if(levelT==1)
			vechicle_speed=ENEMY_VEHICLE_SPEED_LEVEL1;
		if(levelT==2)
			vechicle_speed=ENEMY_VEHICLE_SPEED_LEVEL2;
		if(levelT==3)
			vechicle_speed=ENEMY_VEHICLE_SPEED_LEVEL3;

		if(healthTank>0 && healthJeep<=0 )
		{

			tank_spring1->SetMotorSpeed(vechicle_speed);
			tank_spring2->SetMotorSpeed(vechicle_speed);
			// torso1->ApplyLinearImpulse(b2Vec2(rand() % 5 - 2.0f, rand() % 5 - 2.0f), torso1->GetWorldCenter());
		}

		if(healthTruck>0 && healthTank<=0)
		{


			truck_spring1->SetMotorSpeed(vechicle_speed);
			truck_spring2->SetMotorSpeed(vechicle_speed);
		}


		if(healthJeep>0 )
		{
			m_spring1->SetMotorSpeed(vechicle_speed);


		}

		//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ CAMERA @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

		b2Vec2 position = _playerBody->GetPosition();
		CCPoint myPosition = getPosition();
		CCSize screenSize = CCDirector::sharedDirector()->getWinSize();

		// Move the camera.
		if (position.x > screenSize.width / 2.0f / PTM_RATIO)
		{
			myPosition.x = -MIN(screenSize.width * 2.0f - screenSize.width, position.x * PTM_RATIO - screenSize.width / 2.0f);
			//getPosition().x = myPosition.x;
			setPosition(myPosition);

		}
		//////////////////////////////////////////////////


	}

}
void LevelGenerator::updateTimer(float dt)
{
	bool timeUp=_hud->updateTimer();
	if(timeUp && levelT==2)
	{


		this->runAction( CCSequence::create(
			CCDelayTime::create(1.5),
			CCCallFunc::create(this, 
			callfunc_selector(LevelGenerator::gameOver)),NULL));
	}
}
void LevelGenerator::updateFire(float dt)
{
	if(isPlayerLive)
	{
		float playerpos=_playerBody->GetPosition().x;
		float helipos=_helicopterBody->GetPosition().x;
		if(playerpos<_helicopterBody->GetPosition().x-10 && _playerBody->GetPosition().x>_helicopterBody->GetPosition().x-20 && !heliSoundOn)
		{
			SimpleAudioEngine::sharedEngine()->playEffect("Sound/Helicopter.mp3");
			heliSoundOn=true;
		}
		if( _playerBody->GetPosition().x<_helicopterBody->GetPosition().x-20 && heliSoundOn)
		{

			heliSoundOn=false;
		}

		CCSize screenSize = CCDirector::sharedDirector()->getWinSize();

		//float a=this->getPosition().x;
		//float b=_helicopterBody->GetPosition().x*PTM_RATIO;
		//if(_helicopterBody->GetPosition().x*PTM_RATIO<=this->getPosition().x+screenSize.width && !heliSoundOn)
		//{
		//	SimpleAudioEngine::sharedEngine()->playEffect("Sound/Helicopter.mp3");
		//	heliSoundOn=true;
		//}
		//if(_helicopterBody->GetPosition().x*PTM_RATIO<=this->getPosition().x && heliSoundOn)
		//{
		//	//SimpleAudioEngine::sharedEngine()->playEffect("Sound/Helicopter.mp3");
		//	heliSoundOn=false;
		//}
		// CCLog("Player helicopter Diff  x %f", (_playerBody->GetPosition().x -_helicopterBody->GetPosition().x)*PTM_RATIO);
		float Player_Diff=-(_playerBody->GetPosition().x -_helicopterBody->GetPosition().x)*PTM_RATIO;
		if(Player_Diff<300 && Player_Diff>0)	
			FireHelicopter();

		float vechicle_threshold=350;
		if(levelT==1)
			vechicle_threshold=ENEMY_VEHICLE_THRESHOLD_LEVEL1;
		if(levelT==2)
			vechicle_threshold=ENEMY_VEHICLE_THRESHOLD_LEVEL2;
		if(levelT==3)
			vechicle_threshold=ENEMY_VEHICLE_THRESHOLD_LEVEL3;
		/////////////////////////TRUCK //////////////////////////////////////
		if(healthTruck>0)
		{	
			Player_Diff=-(_playerBody->GetPosition().x -m_truck->GetPosition().x)*PTM_RATIO;
			if(Player_Diff<vechicle_threshold && Player_Diff>0)
				FireTruck();
		}

		/////////////////////////TANK //////////////////////////////////////
		if(healthTank>0)
		{
			Player_Diff=-(_playerBody->GetPosition().x -m_tank->GetPosition().x)*PTM_RATIO;
			if(Player_Diff<vechicle_threshold && Player_Diff>0)
				FireTank();

		}
		/////////////////////////Jeep //////////////////////////////////////
		if(healthJeep>0 )
		{
			Player_Diff=-(_playerBody->GetPosition().x -m_car->GetPosition().x)*PTM_RATIO;
			if(Player_Diff<vechicle_threshold && Player_Diff>0)	
				FireJeep();
		}
		int enemy_threshold=200;
		if(levelT==1)
			enemy_threshold=ENEMY_PLAYER_THRESHOLD_LEVEL1;
		else if(levelT==2)
			enemy_threshold=ENEMY_PLAYER_THRESHOLD_LEVEL2;
		else if(levelT==3)
			enemy_threshold=ENEMY_PLAYER_THRESHOLD_LEVEL3;

		/////////////////////////// Enemy Player///////////////////////////////////////////////////
		if(EnemyHealth==100)
		{
			isEnemyFire=false;
			float	Player_Enemy_Diff=abs(_playerBody->GetPosition().x -_enemyPlayerBody->GetPosition().x)*PTM_RATIO;
			if(Player_Enemy_Diff<enemy_threshold && Player_Enemy_Diff>0)
				FireEnemyPlayer();
		}
		///////////////////////////// Zombie Enmey Player/////////////////////////////////////////////////
		if(EnemyZombieHealth==100)
		{
			isEnemyZombieFire=false;
			float	Player_Enemy_Diff=abs(_playerBody->GetPosition().x -_enemyZombiePlayerBody->GetPosition().x)*PTM_RATIO;
			if(Player_Enemy_Diff<enemy_threshold && Player_Enemy_Diff>0)
				FireEnemyZombiePlayer();

		}

	}

}
void LevelGenerator::FireHelicopter()
{

	CCSprite *sprite = CCSprite::create("SpriteImages/bullet24.png");


	addChild(sprite, 1);
	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;

	bodyDef.position.Set(_helicopterBody->GetPosition().x,_helicopterBody->GetPosition().y-(50/PTM_RATIO));
	bodyDef.userData = sprite;
	_EnemyBulletBody = _world->CreateBody(&bodyDef); 

	b2FixtureDef boxDef;
	boxDef.userData = (void*)1;
	_bullets->push_back(_EnemyBulletBody);
	b2PolygonShape box;
	box.SetAsBox(sprite->getContentSize().width/2.0f/PTM_RATIO, sprite->getContentSize().height/2.0f/PTM_RATIO);
	boxDef.shape = &box;
	boxDef.density = 10.5f;

	boxDef.restitution =0.5f;
	boxDef.friction = 0.5f;
	boxDef.filter.categoryBits = 0x0003;

	_EnemyBulletBody->CreateFixture(&boxDef);


}
void LevelGenerator::FireJeep(){

	CCSprite *sprite = CCSprite::create("SpriteImages/JeepBullet.png");
	SimpleAudioEngine::sharedEngine()->playEffect("Sound/GunFire.mp3");

	addChild(sprite, 1);
	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;

	bodyDef.position.Set(m_car->GetPosition().x-(50/PTM_RATIO),m_car->GetPosition().y+(40/PTM_RATIO));
	bodyDef.userData = sprite;
	_EnemyBulletBody = _world->CreateBody(&bodyDef); 

	b2FixtureDef boxDef;
	boxDef.userData = (void*)1;
	_bullets->push_back(_EnemyBulletBody);
	b2PolygonShape box;
	box.SetAsBox(sprite->getContentSize().width/PTM_RATIO, sprite->getContentSize().height/PTM_RATIO);
	boxDef.shape = &box;
	boxDef.density = 3.5f;

	boxDef.restitution =0.5f;
	boxDef.friction = 0.5f;
	boxDef.filter.categoryBits = 0x0003;

	_EnemyBulletBody->CreateFixture(&boxDef);
	_EnemyBulletBody->ApplyLinearImpulse( b2Vec2(-2,0), _EnemyBulletBody->GetWorldCenter() );


}
void LevelGenerator::FireTruck()
{

	CCSprite *sprite = CCSprite::create("SpriteImages/Tbullet.png");
	SimpleAudioEngine::sharedEngine()->playEffect("Sound/GunFire.mp3");

	addChild(sprite, 1);
	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;

	bodyDef.position.Set(m_truck->GetPosition().x-(50/PTM_RATIO),m_truck->GetPosition().y+(50/PTM_RATIO));
	bodyDef.userData = sprite;
	_EnemyBulletBody = _world->CreateBody(&bodyDef); 

	b2FixtureDef boxDef;
	boxDef.userData = (void*)1;
	_bullets->push_back(_EnemyBulletBody);
	b2PolygonShape box;
	box.SetAsBox(sprite->getContentSize().width/2.0f/PTM_RATIO, sprite->getContentSize().height/2.0f/PTM_RATIO);
	boxDef.shape = &box;
	boxDef.density = 3.5f;

	boxDef.restitution =0.5f;
	boxDef.friction = 0.5f;
	boxDef.filter.categoryBits = 0x0003;

	_EnemyBulletBody->CreateFixture(&boxDef);
	_EnemyBulletBody->ApplyLinearImpulse( b2Vec2(-3,0.3), _EnemyBulletBody->GetWorldCenter() );



}
void LevelGenerator::FireTank()
{

	CCSprite *sprite = CCSprite::create("SpriteImages/Tbullet.png");


	addChild(sprite, 1);
	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;

	bodyDef.position.Set(m_tank->GetPosition().x-(100/PTM_RATIO),m_tank->GetPosition().y+(30/PTM_RATIO));
	bodyDef.userData = sprite;
	_EnemyBulletBody = _world->CreateBody(&bodyDef); 

	b2FixtureDef boxDef;
	boxDef.userData = (void*)1;
	_bullets->push_back(_EnemyBulletBody);
	b2PolygonShape box;
	box.SetAsBox(sprite->getContentSize().width/2.0f/PTM_RATIO, sprite->getContentSize().height/2.0f/PTM_RATIO);
	boxDef.shape = &box;
	boxDef.density = 3.5f;

	boxDef.restitution =0.5f;
	boxDef.friction = 0.5f;
	boxDef.filter.categoryBits = 0x0003;

	_EnemyBulletBody->CreateFixture(&boxDef);
	_EnemyBulletBody->ApplyLinearImpulse( b2Vec2(-3,0.3), _EnemyBulletBody->GetWorldCenter() );



}
void LevelGenerator::FireEnemyZombiePlayer()
{
	CCSprite *sprite = CCSprite::create("SpriteImages/JeepBullet.png");


	addChild(sprite, 1);
	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	if(_Player->getPosition().x< _EnemyZombiePlayer->getPosition().x)
		bodyDef.position.Set(_enemyZombiePlayerBody->GetPosition().x-(55/PTM_RATIO),_enemyZombiePlayerBody->GetPosition().y+(10/PTM_RATIO));
	else
		bodyDef.position.Set(_enemyZombiePlayerBody->GetPosition().x+(55/PTM_RATIO),_enemyZombiePlayerBody->GetPosition().y+(10/PTM_RATIO));

	bodyDef.userData = sprite;
	_EnemyBulletBody = _world->CreateBody(&bodyDef); 

	b2FixtureDef boxDef;
	boxDef.userData = (void*)1;
	_bullets->push_back(_EnemyBulletBody);
	b2PolygonShape box;
	box.SetAsBox(sprite->getContentSize().width/PTM_RATIO, sprite->getContentSize().height/PTM_RATIO);
	boxDef.shape = &box;
	boxDef.density = 2.5f;

	boxDef.restitution =0.5f;
	boxDef.friction = 0.5f;
	boxDef.filter.categoryBits = 0x0003;

	_EnemyBulletBody->CreateFixture(&boxDef);
	if(isEnemyZombieKill==false)
	{
		if(_Player->getPosition().x< _EnemyZombiePlayer->getPosition().x)
		{
			GeneratePlayerAnimation("ZombiePlayer_FireLeft/Fire",8,_EnemyZombiePlayer);

			SimpleAudioEngine::sharedEngine()->playEffect("Sound/shotgun2.mp3") ;
			_EnemyBulletBody->ApplyLinearImpulse( b2Vec2(-2,0), _EnemyBulletBody->GetWorldCenter() );
		}
		else
		{
			GeneratePlayerAnimation("ZombiePlayer_FireRight/Fire",8,_EnemyZombiePlayer);
			SimpleAudioEngine::sharedEngine()->playEffect("Sound/shotgun2.mp3") ;
			_EnemyBulletBody->ApplyLinearImpulse( b2Vec2(2,0), _EnemyBulletBody->GetWorldCenter() );

		}
	}
	isEnemyZombieFire=true;



}
void LevelGenerator::FireEnemyPlayer(){
	CCSprite *sprite = CCSprite::create("SpriteImages/JeepBullet.png");
	SimpleAudioEngine::sharedEngine()->playEffect("Sound/GunFire.mp3");


	addChild(sprite, 1);
	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	if(_Player->getPosition().x< _EnemyPlayer->getPosition().x)
		bodyDef.position.Set(_enemyPlayerBody->GetPosition().x-(55/PTM_RATIO),_enemyPlayerBody->GetPosition().y+(10/PTM_RATIO));
	else
		bodyDef.position.Set(_enemyPlayerBody->GetPosition().x+(55/PTM_RATIO),_enemyPlayerBody->GetPosition().y+(10/PTM_RATIO));

	bodyDef.userData = sprite;
	_EnemyBulletBody = _world->CreateBody(&bodyDef); 

	b2FixtureDef boxDef;
	boxDef.userData = (void*)1;
	_bullets->push_back(_EnemyBulletBody);
	b2PolygonShape box;
	box.SetAsBox(sprite->getContentSize().width/PTM_RATIO, sprite->getContentSize().height/PTM_RATIO);
	boxDef.shape = &box;
	boxDef.density = 2.5f;

	boxDef.restitution =0.5f;
	boxDef.friction = 0.5f;
	boxDef.filter.categoryBits = 0x0003;

	_EnemyBulletBody->CreateFixture(&boxDef);
	if(isEnemyKill==false)
	{
		if(_Player->getPosition().x< _EnemyPlayer->getPosition().x)
		{
			GeneratePlayerAnimation("GasPlayer_FireLeft/Fire",5,_EnemyPlayer);
			_EnemyBulletBody->ApplyLinearImpulse( b2Vec2(-2,0), _EnemyBulletBody->GetWorldCenter() );
			SimpleAudioEngine::sharedEngine()->playEffect("Sound/shotgun.mp3");
		}
		else
		{
			GeneratePlayerAnimation("GasPlayer_FireRight/Fire",5,_EnemyPlayer);
			_EnemyBulletBody->ApplyLinearImpulse( b2Vec2(2,0), _EnemyBulletBody->GetWorldCenter() );
			SimpleAudioEngine::sharedEngine()->playEffect("Sound/shotgun.mp3");	
		}
	}
	isEnemyFire=true;

}
void LevelGenerator::updateEnemyPostion(float dt)

{	


	bool isEnemyPlayerWalk=true;
	if( EnemyZombieHealth==100 )
		isEnemyPlayerWalk=false;


	if( EnemyHealth==100 && EnemyZombieHealth==100 &&isPlayerLive  )
	{
		float EnmeyPlayer_Diff= abs(_EnemyPlayer->getPosition().x -_Player->getPosition().x);
		float EnmeyZombiePlayer_Diff= abs(_EnemyZombiePlayer->getPosition().x -_Player->getPosition().x);

		if(EnmeyPlayer_Diff<EnmeyZombiePlayer_Diff)
			isEnemyPlayerWalk=true;
		else
			isEnemyPlayerWalk=false;

	}

	if(EnemyHealth==100  &&isPlayerLive  )
	{
		b2Vec2 vel = _enemyPlayerBody->GetLinearVelocity();




		if(isEnemyFire==false && isEnemyPlayerWalk  )
		{
			float enemy_impluse=1.5;
			if(levelT==1)
				enemy_impluse=ENEMY_IMPULSE_LEVEL1;
			else if(levelT==2)
				enemy_impluse=ENEMY_IMPULSE_LEVEL2;
			else if(levelT==3)
				enemy_impluse=ENEMY_IMPULSE_LEVEL3;
			if(_Player->getPosition().x< _EnemyPlayer->getPosition().x )
			{

				GeneratePlayerAnimation("GasPlayer_Walk/Left",4,_EnemyPlayer);
				SimpleAudioEngine::sharedEngine()->playEffect("Sound/walking.wav") ;
				_enemyPlayerBody->ApplyLinearImpulse( b2Vec2(-enemy_impluse,0),_enemyPlayerBody->GetWorldCenter() );

			}
			else
			{
				GeneratePlayerAnimation("GasPlayer_Walk/Right",4,_EnemyPlayer);
				SimpleAudioEngine::sharedEngine()->playEffect("Sound/walking.wav") ;
				_enemyPlayerBody->ApplyLinearImpulse( b2Vec2(enemy_impluse,0),_enemyPlayerBody->GetWorldCenter() );

			}


		}

	}

	////@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ Zombie @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
	if(EnemyZombieHealth==100  && isPlayerLive  )
	{
		if(isEnemyZombieFire==false&& isEnemyPlayerWalk==false  )
		{
			float enemy_impluse=2.3;
			if(levelT==1)
				enemy_impluse=ENEMY_ZOMBIE_IMPULSE_LEVEL1;
			else if(levelT==2)
				enemy_impluse=ENEMY_ZOMBIE_IMPULSE_LEVEL2;
			else if(levelT==3)
				enemy_impluse=ENEMY_ZOMBIE_IMPULSE_LEVEL3;
			if(_Player->getPosition().x< _EnemyZombiePlayer->getPosition().x)
			{
				GeneratePlayerAnimation("ZombiePlayer_Walk/Left",4,_EnemyZombiePlayer);
				SimpleAudioEngine::sharedEngine()->playEffect("Sound/walking.wav") ;
				_enemyZombiePlayerBody->ApplyLinearImpulse( b2Vec2(-enemy_impluse,0),_enemyZombiePlayerBody->GetWorldCenter() );

			}
			else
			{
				GeneratePlayerAnimation("ZombiePlayer_Walk/Right",4,_EnemyZombiePlayer);
				SimpleAudioEngine::sharedEngine()->playEffect("Sound/walking.wav") ;
				_enemyZombiePlayerBody->ApplyLinearImpulse( b2Vec2(enemy_impluse,0),_enemyZombiePlayerBody->GetWorldCenter() );

			}

		}

	}


}
void LevelGenerator::updateHelicopter(float dt)

{	

	if(isPlayerLive)

		AddHelicopters(CCPointMake(1800.0f, 200.0f));


}
void LevelGenerator::spawnEnemies(float dt)
{
	if(EnemyHealth==0  &&isPlayerLive )

	{
		if(_Player->getPosition().x<960)
			AddEnemyPlayers(CCPointMake(1320.0f, 0.0f));
		else
			AddEnemyPlayers(CCPointMake(400.0f, 0.0f));


	}

	if(EnemyZombieHealth==0  &&isPlayerLive )

	{
		if(_Player->getPosition().x<760)
			AddEnemyZombiePlayers(CCPointMake(1250.0f, 0.0f));
		else
			AddEnemyZombiePlayers(CCPointMake(200.0f, 0.0f));


	}
	//if(m_car->GetPosition().x<0 && m_truck->GetPosition().x<400 && 	isPlayerLive)
	//AddJeeps(CCPointMake(850.0f, 0.0f));

}
void LevelGenerator::updateTruck(float dt)

{
	//if(m_truck->GetPosition().x<0	&& isPlayerLive)
	//AddTrucks(CCPointMake(800.0f, 0.0f));

}
void LevelGenerator::tick(float dt)
{
	int velocityIterations = 10;
	int positionIterations = 9;

	_world->Step(dt, velocityIterations, positionIterations);

	for (b2Body* b = _world->GetBodyList(); b; b = b->GetNext())
	{

		if (b->GetUserData() != NULL) {
			b2Fixture  *fdef=b->GetFixtureList();
			b2Filter bfilter= fdef->GetFilterData();

			CCSprite* sprite = (CCSprite*)b->GetUserData();

			sprite->setPosition( CCPointMake( b->GetPosition().x * PTM_RATIO, b->GetPosition().y * PTM_RATIO) );

			sprite->setRotation( -1 * CC_RADIANS_TO_DEGREES(b->GetAngle()) );
		}	

	}


	std::set<b2Body*>::iterator pos;
	for(pos = this->contacts.begin();  pos != this->contacts.end(); pos++)
	{
		b2Body *body = *pos;

		CCNode *contactNode = (CCNode*)body->GetUserData();
		CCPoint position = contactNode->getPosition();


		std::vector<b2Body*>::iterator it,iter,itrWoods,itrRocks,itheads,itbull,ith,itPly,itJeep;
		//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ Player@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@		
		for(itPly = _players->begin() ; itPly != _players->end() ; itPly++)
		{
			if(*itPly == body )
			{
				//_players->erase(iter);
				health-=10;
				SimpleAudioEngine::sharedEngine()->playEffect("Sound/hit.wav") ;
				_hud->numCollectedChanged(health);
				_hud->updateHealthBar(health);
				if(health<=0)
				{
					isPlayerLive=false;
					if(playerType==1)
					{
						AddRagDollPlayers(CCPointMake(_Player->getPosition().x, _Player->getPosition().y));
						SimpleAudioEngine::sharedEngine()->playEffect("Sound/dying.wav");
						removeChild(contactNode, true);
					}
					else if (playerType==2) 
					{
						GeneratePlayerAnimation("GoldenPlayer_KillRight/Kill",14,_Player);
						SimpleAudioEngine::sharedEngine()->playEffect("Sound/dying.wav");
						_Player->runAction( CCSequence::create(
							CCDelayTime::create(2.3),
							CCCallFuncN::create(this, callfuncN_selector(LevelGenerator::spriteMoveFinished)), 
							NULL) );
					}
					else if (playerType==3) 
					{
						GeneratePlayerAnimation("BrownPlayer_KillLeft/Kill",8,_Player);
						SimpleAudioEngine::sharedEngine()->playEffect("Sound/dying.wav");
						_Player->runAction( CCSequence::create(
							CCDelayTime::create(1.5),
							CCCallFuncN::create(this, callfuncN_selector(LevelGenerator::spriteMoveFinished)), 
							NULL) );
					}
					_world->DestroyBody(body);

					this->runAction( CCSequence::create(
						CCDelayTime::create(3.5),
						CCCallFunc::create(this, 
						callfunc_selector(LevelGenerator::gameOver)),NULL));
					health=0;
				}
				break;
			}
		}



		//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ Jeep @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
		if(_jeeps->size()!=0)
		{
			for(itJeep = _jeeps->begin() ; itJeep != _jeeps->end() ; itJeep++)
			{
				if(*itJeep == body )
				{
					SimpleAudioEngine::sharedEngine()->playEffect("Sound/vehiclehit.wav");

					healthJeep-=33;
					if(healthJeep<=0)
					{
						if(Id_JeepSound>-1)
							SimpleAudioEngine::sharedEngine()->stopEffect(Id_JeepSound);
						SimpleAudioEngine::sharedEngine()->playEffect("Sound/Jeepblast.mp3");
						healthJeep=0;
						scoreCount+=500;
						//ExplosionAnimation(position,"SpriteImages/Blast3.png" ,5);
						VehicleExplosionAnimation( position);

						ScoreAnimation(position,"Score\\500.png","Score\\500g.png" );
						_jeeps->erase(itJeep);


						/*	for (b2JointEdge* jointEdge =body->GetJointList(); jointEdge != NULL; jointEdge = jointEdge->next)
						{
						b2Joint* targetJoint = jointEdge->joint;
						_world->DestroyJoint(targetJoint);

						}*/
						removeChild(contactNode, true);
						_world->DestroyBody(body);
						VehicleDestroyedCount+=1;

						_hud->updateCarScore(VehicleDestroyedCount);
						//m_wheel2->SetTransform(m_wheel1->GetPosition(),0);
						_world->DestroyBody(m_wheel1);
						CCNode *contactNode2 = (CCNode*)m_wheel1->GetUserData();
						removeChildByTag(20,true);
						removeChild(contactNode2, true);
						_world->DestroyBody(m_wheel2);
						CCNode *contactNode3 = (CCNode*)m_wheel2->GetUserData();
						removeChildByTag(21,true);
						removeChild(contactNode3, true);

						//AddTrucks(CCPointMake(1200.0f, 0.0f));
						updateScore(scoreCount);
					}

					break;
				}
			}
		}
		//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ Enemy Players @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
		if(_enemyplayers->size()!=0)
		{
			std::vector<b2Body*>::iterator it;
			for(it =_enemyplayers->begin() ; it != _enemyplayers->end() ; it++)
			{
				if(*it == body )
				{

					isEnemyKill=true;

					scoreCount+=100;
					SimpleAudioEngine::sharedEngine()->playEffect("Sound/enemydying.wav") ;

					if(_Player->getPosition().x< _EnemyPlayer->getPosition().x )
						GeneratePlayerAnimation("GasPlayer_KillLeft/Kill",11,_EnemyPlayer);
					else
						GeneratePlayerAnimation("GasPlayer_KillRight/Kill",11,_EnemyPlayer);
					//ScoreAnimation(position,"100.png","100g.png" );
					_enemyplayers->erase(it);
					_EnemyPlayer->runAction( CCSequence::create(
						CCDelayTime::create(2),
						CCCallFuncN::create(this, callfuncN_selector(LevelGenerator::spriteMoveFinished)), 
						NULL) );
					//removeChild(contactNode, true);
					_world->DestroyBody(body);
					EnemyDestroyedCount+=1;

					_hud->updateEnemyScore(EnemyDestroyedCount);

					EnemyHealth=0;
					updateScore(scoreCount);

					break;
				}
			}
		}
		//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ Zombie Enemy Players @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
		if(_enemyzombieplayers->size()!=0)
		{
			std::vector<b2Body*>::iterator it;
			for(it =_enemyzombieplayers->begin() ; it != _enemyzombieplayers->end() ; it++)
			{
				if(*it == body )
				{

					isEnemyZombieKill=true;
					SimpleAudioEngine::sharedEngine()->playEffect("Sound/enemydying.wav") ;

					scoreCount+=100;
					if(_Player->getPosition().x< _EnemyZombiePlayer->getPosition().x )
						GeneratePlayerAnimation("ZombiePlayer_KillLeft/Kill",8,_EnemyZombiePlayer);
					else
						GeneratePlayerAnimation("ZombiePlayer_KillRight/Kill",8,_EnemyZombiePlayer);
					//ScoreAnimation(position,"100.png","100g.png" );
					_enemyzombieplayers->erase(it);
					_EnemyZombiePlayer->runAction( CCSequence::create(
						CCDelayTime::create(1.5),
						CCCallFuncN::create(this, callfuncN_selector(LevelGenerator::spriteMoveFinished)), 
						NULL) );
					//removeChild(contactNode, true);
					_world->DestroyBody(body);
					ZombieDestroyedCount+=1;

					_hud->updateZombieScore(ZombieDestroyedCount);

					EnemyZombieHealth=0;
					updateScore(scoreCount);

					break;
				}
			}
		}
		//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ BaseCamp @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
		if(_baseCamp->size()!=0)
		{
			std::vector<b2Body*>::iterator itw1;
			for(itw1 =_baseCamp->begin() ; itw1 != _baseCamp->end() ; itw1++)
			{
				if(*itw1 == body )
				{


					scoreCount+=5000;
					ExplosionAnimation(position,"SpriteImages/Blast3.png" ,10);
					ExplosionAnimation(position,"SpriteImages/Blast4.png" ,10);
					SimpleAudioEngine::sharedEngine()->playEffect("Sound/explosion.mp3") ;
					ScoreAnimation(position,"Score\\5000.png","Score\\5000g.png" );
					_baseCamp->erase(itw1);

					removeChild(contactNode, true);
					_world->DestroyBody(body);

					updateScore(scoreCount);
					this->runAction( CCSequence::create(
						CCDelayTime::create(5),
						CCCallFunc::create(this, 
						callfunc_selector(LevelGenerator::gameWin)),NULL));

					break;
				}
			}
		}
		//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ TNT @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
		if(_tnt->size()!=0)
		{
			std::vector<b2Body*>::iterator it;
			for(it =_tnt->begin() ; it != _tnt->end() ; it++)
			{
				if(*it == body )
				{

					SimpleAudioEngine::sharedEngine()->playEffect("Sound/Campblast1.mp3");

					SimpleAudioEngine::sharedEngine()->playEffect("Sound/Campblast2.mp3");
					scoreCount+=5000;
					ExplosionAnimation(position,"SpriteImages/Blast3.png" ,10);
					ExplosionAnimation(position,"SpriteImages/Blast4.png" ,10);
					ScoreAnimation(position,"Score\\5000.png","Score\\5000g.png" );
					_tnt->erase(it);
					removeChild(contactNode, true);
					_world->DestroyBody(body);

					CCNode *contactNode2 = (CCNode*)_baseCampBody->GetUserData();
					ExplosionAnimation(contactNode2->getPosition(),"SpriteImages/Blast3.png" ,30);
					ExplosionAnimation(contactNode2->getPosition(),"SpriteImages/Blast4.png" ,30);
					FireExplosionAnimation( CCPointMake(1850.0f, 0.0f));
					FireExplosionAnimation( CCPointMake(1750.0f, 0.0f));
					SimpleAudioEngine::sharedEngine()->playEffect("Sound/FireBurning.mp3");
					removeChild(contactNode2, true);
					_world->DestroyBody(_baseCampBody);

					updateScore(scoreCount);
					this->runAction( CCSequence::create(
						CCDelayTime::create(5),
						CCCallFunc::create(this, 
						callfunc_selector(LevelGenerator::gameWin)),NULL));


					break;
				}
			}
		}
		//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ TRUCK @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
		if(_trucks->size()!=0  )
		{
			std::vector<b2Body*>::iterator it;
			for(it =_trucks->begin() ; it != _trucks->end() ; it++)
			{
				if(*it == body )
				{
					SimpleAudioEngine::sharedEngine()->playEffect("Sound/vehiclehit.wav");

					healthTruck-=20;
					if(healthTruck<=0)
					{
						if(Id_TruckSound>-1)
							SimpleAudioEngine::sharedEngine()->stopEffect(Id_TruckSound);
						SimpleAudioEngine::sharedEngine()->playEffect("Sound/Jeepblast.mp3");
						scoreCount+=1000;
						//ExplosionAnimation(position,"SpriteImages/Blast3.png" ,10);
						VehicleExplosionAnimation( position);
						ScoreAnimation(position,"Score\\1000.png","Score\\1000g.png" );
						_trucks->erase(it);

						VehicleDestroyedCount+=1;

						_hud->updateCarScore(VehicleDestroyedCount);

						removeChild(contactNode, true);
						_world->DestroyBody(body);

						healthTruck=0;

						_world->DestroyBody(truck_wheel1);

						CCNode *contactNode1 = (CCNode*)truck_wheel1->GetUserData();
						removeChildByTag(41,true);
						removeChild(contactNode1, true);
						_world->DestroyBody(truck_wheel2);
						CCNode *contactNode2 = (CCNode*)truck_wheel2->GetUserData();
						removeChildByTag(42,true);
						removeChild(contactNode2, true);
						_world->DestroyBody(truck_wheel3);
						CCNode *contactNode3 = (CCNode*)truck_wheel3->GetUserData();
						removeChildByTag(43,true);
						removeChild(contactNode3, true);
						_world->DestroyBody(truck_wheel4);
						CCNode *contactNode4 = (CCNode*)truck_wheel4->GetUserData();
						removeChildByTag(44,true);
						removeChild(contactNode4, true);
						updateScore(scoreCount);

					}
					break;
				}
			}
		}

		//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ TANK @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
		if(_tanks->size()!=0)
		{
			std::vector<b2Body*>::iterator it;
			for(it =_tanks->begin() ; it != _tanks->end() ; it++)
			{
				if(*it == body )
				{
					SimpleAudioEngine::sharedEngine()->playEffect("Sound/vehiclehit.wav");
					healthTank-=20;
					if(healthTank<=0)
					{


						scoreCount+=1000;
						///ExplosionAnimation(position,"SpriteImages/Blast3.png" ,10);
						VehicleExplosionAnimation( position);
						ScoreAnimation(position,"Score\\1000.png","Score\\1000g.png" );
						_tanks->erase(it);

						VehicleDestroyedCount+=1;

						_hud->updateCarScore(VehicleDestroyedCount);

						removeChild(contactNode, true);
						_world->DestroyBody(body);
						SimpleAudioEngine::sharedEngine()->playEffect("Sound/explosion.mp3") ;
						healthTank=0;

						_world->DestroyBody(tank_wheel1);

						CCNode *contactNode1 = (CCNode*)tank_wheel1->GetUserData();
						removeChildByTag(31,true);
						removeChild(contactNode1, true);

						_world->DestroyBody(tank_wheel2);
						CCNode *contactNode2 = (CCNode*)tank_wheel2->GetUserData();
						removeChildByTag(32,true);
						removeChild(contactNode2, true);
						_world->DestroyBody(tank_wheel3);
						CCNode *contactNode3 = (CCNode*)tank_wheel3->GetUserData();
						removeChildByTag(33,true);
						removeChild(contactNode3, true);
						_world->DestroyBody(tank_wheel4);
						CCNode *contactNode4 = (CCNode*)tank_wheel4->GetUserData();
						removeChildByTag(34,true);
						removeChild(contactNode4, true);
						_world->DestroyBody(tank_wheel5);
						CCNode *contactNode5 = (CCNode*)tank_wheel5->GetUserData();
						removeChildByTag(35,true);
						removeChild(contactNode5, true);

						updateScore(scoreCount);

					}
					break;
				}
			}
		}

		//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ Bullets @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
		if(_bullets->size()!=0)
		{
			for(itbull = _bullets->begin() ; itbull != _bullets->end() ; itbull++)
			{
				if(*itbull == body )
				{
					_bullets->erase(itbull);

					ExplosionAnimation(position,"SpriteImages/Blast3.png" ,1);
					//SimpleAudioEngine::sharedEngine()->playEffect("Sound/explosion.mp3") ;
					removeChild(contactNode, true);
					_world->DestroyBody(body);


					break;
				}
			}
		}
		//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ Helicopters@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
		if(_helicopters->size()!=0)
		{
			for(ith = _helicopters->begin() ; ith != _helicopters->end() ; ith++)
			{
				if(*ith == body )
				{
					scoreCount+=100;
					_helicopters->erase(ith);
					updateScore(scoreCount);
					ExplosionAnimation(position,"SpriteImages/Blast1.png",5 );
					ExplosionAnimation(position,"SpriteImages/Blast2.png" ,5);
					ExplosionAnimation(position,"SpriteImages/Blast4.png",5 );
					removeChild(contactNode, true);
					//					HeliDestroyedCount+=1;

					//_hud->updateHeliScore(HeliDestroyedCount);
					_world->DestroyBody(body);
					break;
				}
			}
		}
		//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ Rock @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
		if(_rocks->size()!=0)
		{
			std::vector<b2Body*>::iterator it;
			for(it= _rocks->begin() ; it != _rocks->end() ; it++)
			{


				if(*it == body  )
				{
					_rocks->erase(it);
					scoreCount+=50;
					CCLog("rock removed");

					SimpleAudioEngine::sharedEngine()->playEffect("Sound/rock.mp3") ;

					ExplosionAnimation(position,"SpriteImages/rock.png" ,2);
					//ScoreAnimation(position,"Score\\500.png","Score\\500g.png" );
					updateScore(scoreCount);

					SimpleAudioEngine::sharedEngine()->playEffect("sound\rock.mp3") ;
					removeChild(contactNode, true);
					_world->DestroyBody(body);
					break;
				}
			}
		}
		//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ wood @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
		if(_woods->size()!=0)
		{
			std::vector<b2Body*>::iterator it;
			for(it= _woods->begin() ; it != _woods->end() ; it++)
			{


				if(*it == body  )
				{
					_woods->erase(it);
					scoreCount+=50;

					SimpleAudioEngine::sharedEngine()->playEffect("Sound/woodexplosion.mp3") ;

					ExplosionAnimation(position,"SpriteImages/woodpiece.png" ,2);

					//ScoreAnimation(position,"Score\\500.png","Score\\500g.png" );
					updateScore(scoreCount);

					//SimpleAudioEngine::sharedEngine()->playEffect("Sound\wood.mp3") ;
					removeChild(contactNode, true);
					_world->DestroyBody(body);
					break;
				}
			}
		}

		//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ Power @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
		if(_powers->size()!=0)
		{
			std::vector<b2Body*>::iterator it;
			for(it = _powers->begin() ; it != _powers->end() ; it++)
			{


				if(*it == body  )
				{
					_powers->erase(it);

					health=100;
					_hud->numCollectedChanged(health);
					_hud->updateHealthBar(health);
					_hud->hideHealthPackAlert();
					SimpleAudioEngine::sharedEngine()->playEffect("Sound/powerup.mp3");


					removeChild(contactNode, true);
					_world->DestroyBody(body);
					break;
				}
			}
		}
		//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@	

	}
	this->contacts.clear();

}
void LevelGenerator::spriteMoveFinished(CCNode* sender)
{
	CCSprite *sprite = (CCSprite *)sender;
	this->removeChild(sprite, true);
}

/////////////////////////////////////////////////////////////////////////////////////////////////
//   MOUSE EVENT
////////////////////////////////////////////////////////////////////////////////////////////////
void LevelGenerator::ccTouchesMoved(cocos2d::CCSet* touches, cocos2d::CCEvent* events)
{


}
void LevelGenerator::ccTouchesBegan(cocos2d::CCSet* touches, cocos2d::CCEvent* events)
{

	CCTouch* touch = (CCTouch*)( touches->anyObject() );

	CCPoint location = touch->getLocationInView();
	location = CCDirector::sharedDirector()->convertToGL(location);
	b2Vec2 locationWorld = b2Vec2(location.x/PTM_RATIO,location.y/PTM_RATIO);
	if(isPlayerLive)
	{
		float playerX=_Player->getPosition().x;
		if( playerX>960)
			playerX=playerX/2;
		CCLog("Player Pos  x %f", playerX );
		CCLog("Mouse Pos  x %f", location.x );

		if(location.x >= (playerX))
		{
			if(playerType==1)
			{
				GeneratePlayerAnimation("FireRight/Fire",5,_Player);
				SimpleAudioEngine::sharedEngine()->playEffect("Sound/doublegun.mp3") ;
			}
			else if (playerType==2) 
			{
				GeneratePlayerAnimation("GoldenPlayer_FireRight/Fire",8,_Player);
				SimpleAudioEngine::sharedEngine()->playEffect("Sound/snipershot.mp3") ;
			}
			else if (playerType==3) 
			{
				GeneratePlayerAnimation("BrownPlayer_FireRight/Fire",4,_Player);
				SimpleAudioEngine::sharedEngine()->playEffect("Sound/barreta.mp3") ;
			}
		}

		else

		{
			if(playerType==1)
			{
				GeneratePlayerAnimation("FireLeft/Fire",5,_Player);
				SimpleAudioEngine::sharedEngine()->playEffect("Sound/doublegun.mp3") ;
			}
			else if (playerType==2) 
			{
				GeneratePlayerAnimation("GoldenPlayer_FireLeft/Fire",9,_Player);
				SimpleAudioEngine::sharedEngine()->playEffect("Sound/snipershot.mp3") ;
			}
			else if (playerType==3) 
			{
				GeneratePlayerAnimation("BrownPlayer_FireLeft/Fire",3,_Player);
				SimpleAudioEngine::sharedEngine()->playEffect("Sound/barreta.mp3") ;
			}

		}

		CCSprite *sprite = CCSprite::create("SpriteImages/Bullet.png");

		//  sprite->setPosition( CCPointMake(_playerBody->GetPosition().x +200/PTM_RATIO, _playerBody->GetPosition().y+100/PTM_RATIO ));
		addChild(sprite, 1);
		b2BodyDef bodyDef;
		bodyDef.type = b2_dynamicBody;
		if(location.x >= (playerX))
			bodyDef.position.Set(_playerBody->GetPosition().x,_playerBody->GetPosition().y);
		else
			bodyDef.position.Set(_playerBody->GetPosition().x-1,_playerBody->GetPosition().y);

		bodyDef.userData = sprite;
		_playerBulletBody = _world->CreateBody(&bodyDef); 


		b2FixtureDef boxDef;
		boxDef.userData = (void*)1;
		_bullets->push_back(_playerBulletBody);
		b2CircleShape circle;
		circle.m_radius = sprite->getContentSize().width/2.0f/PTM_RATIO;
		boxDef.shape = &circle;
		boxDef.density = 5.5f;

		boxDef.restitution =0.5f;
		boxDef.friction = 0.0f;
		_playerBulletBody->CreateFixture(&boxDef);
		if(location.x >= (playerX))

			_playerBulletBody->ApplyForce(b2Vec2(130,location.y-30),_playerBulletBody->GetWorldCenter()); 
		else
			_playerBulletBody->ApplyForce(b2Vec2(-130,location.y-30),_playerBulletBody->GetWorldCenter()); 
	}
	if(playerType==1)
		SimpleAudioEngine::sharedEngine()->playEffect("Sound/doublegun.mp3") ;
	else if (playerType==2) 
		SimpleAudioEngine::sharedEngine()->playEffect("Sound/snipershot.mp3") ;
	else if (playerType==3) 
		SimpleAudioEngine::sharedEngine()->playEffect("Sound/barreta.mp3") ;

}
void LevelGenerator::ccTouchesEnded(cocos2d::CCSet* touches, cocos2d::CCEvent* events)
{





}
/////////////////////////////////////////////////////////////////////////////////////////////////
//   ANIMATION
////////////////////////////////////////////////////////////////////////////////////////////////

void LevelGenerator::ScoreAnimation(CCPoint position,const char* imgName,const char* imgName2 )
{

	spExplosion = CCSprite::create(imgName);
	spExplosion->setPosition(position);
	this->addChild(spExplosion,0,22);
	CCAnimation * anim = CCAnimation::create();		
	anim->addSpriteFrameWithFileName(imgName);
	anim->addSpriteFrameWithFileName(imgName2);
	spExplosion->runAction( CCSequence::create(
		//20121101 ActionWithDuration
		CCAnimate::create(anim),
		CCCallFuncN::create(this, callfuncN_selector(LevelGenerator::spriteMoveFinished)), 
		NULL) );

}
void LevelGenerator::ExplosionAnimation(CCPoint position,const char* imgName,int Max )
{

	CCParticleExplosion * emitter = new CCParticleExplosion();
	emitter->setTexture(CCTextureCache::sharedTextureCache()->addImage(imgName));
	emitter->initWithTotalParticles(Max);      
	emitter->setStartSize(20.0f);
	emitter->setSpeed(50.0f);
	emitter->setAnchorPoint(ccp(0.5f,0.5f));
	emitter->setPosition(position);
	emitter->setDuration(0.5f);

	ccColor4F startColor = {219.5f, 147.5f, 29.5f, 1.0f};
	emitter->setStartColor(startColor);

	ccColor4F startColorVar = {219.5f, 147.5f, 29.5f, 1.0f};
	emitter->setStartColorVar(startColorVar);

	ccColor4F endColor = {167.1f, 91.1f, 39.1f, 1.0f};
	emitter->setEndColor(endColor);

	ccColor4F endColorVar = {167.1f, 91.1f, 39.1f, 1.0f};    
	emitter->setEndColorVar(endColorVar);
	//emitter->setIsBlendAdditive(true);
	emitter->setAutoRemoveOnFinish(true);
	emitter->retain();
	addChild(emitter, 1);
	emitter->release();

}
void LevelGenerator::RainAnimation(CCPoint position )
{
	CCParticleSystem*	m_emitter;
	CCSprite*			m_background;
	m_emitter = CCParticleRain::create();
	m_emitter->retain();
	this->addChild(m_emitter, 10);
	CCPoint p = m_emitter->getPosition();
	m_emitter->setPosition( CCPointMake( p.x, p.y-100) );

	m_emitter->setLife(2);
	m_emitter->setEmissionRate(120.0f);
	m_emitter->setTotalParticles(200);
	m_emitter->setTexture( CCTextureCache::sharedTextureCache()->addImage("Background1/fire.png") );



	m_emitter->setPosition(position );
}
void LevelGenerator::FireExplosionAnimation(CCPoint position)
{
	CCParticleSystem*	m_emitter;
	CCSprite*			m_background;
	m_emitter = CCParticleSmoke::create();
	m_emitter->retain();
	this->addChild(m_emitter, 10);
	m_emitter->setStartSize(20.0f);

	//m_emitter->setTotalParticles(300);
	m_emitter->setSpeed(30.0f);
	m_emitter->setTexture( CCTextureCache::sharedTextureCache()->addImage("Background1/fire.png") );//.pvr"];

	m_emitter->setPosition(position);
	m_emitter->release();
	CCParticleSystem*	m_emitter2;
	CCSprite*			m_background2;
	m_emitter2 = CCParticleFire::create();
	m_emitter2->retain();
	this->addChild(m_emitter2, 10);
	m_emitter2->setStartSize(20.0f);
	//m_emitter->setTotalParticles(300);
	m_emitter2->setSpeed(30.0f);

	m_emitter2->setTexture( CCTextureCache::sharedTextureCache()->addImage("Background1/fire.png") );//.pvr"];

	m_emitter2->setPosition(position);
	m_emitter2->release();
}
void LevelGenerator::VehicleExplosionAnimation(CCPoint position)
{
	CCParticleSystem*	m_emitter;
	CCSprite*			m_background;
	m_emitter = CCParticleSmoke::create();
	m_emitter->retain();
	this->addChild(m_emitter, 10);
	m_emitter->setStartSize(20.0f);
	//m_emitter->setTotalParticles(300);
	m_emitter->setSpeed(50.0f);
	m_emitter->setTexture( CCTextureCache::sharedTextureCache()->addImage("Background1/fire.png") );//.pvr"];
	m_emitter->setDuration(0.8f);
	m_emitter->setPosition(position);
	m_emitter->release();
	CCParticleSystem*	m_emitter2;
	CCSprite*			m_background2;
	m_emitter2 = CCParticleFire::create();
	m_emitter2->retain();
	this->addChild(m_emitter2, 10);
	m_emitter2->setStartSize(30.0f);
	//m_emitter->setTotalParticles(300);
	m_emitter2->setSpeed(30.0f);
	m_emitter2->setTexture( CCTextureCache::sharedTextureCache()->addImage("Background1/fire.png") );//.pvr"];
	m_emitter2->setDuration(0.8f);
	m_emitter2->setPosition(position);
	m_emitter2->release();
}
void LevelGenerator::GeneratePlayerAnimation(char *Name,int Max,cocos2d::CCSprite* sp)
{
	cocos2d::CCAnimate *_curAnimate;
	CCAnimation* animaiton = CCAnimation::create();

	for(int i = 1; i <= Max; ++i){
		char *frameName = new char[1024];
		sprintf(frameName, "%s (%d).png", Name, i);
		animaiton->addSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(frameName)); 



	}
	animaiton->setDelayPerUnit(0.2);
	_curAnimate = CCAnimate::create(animaiton);
	sp->runAction(_curAnimate);
}
void LevelGenerator::updateScore(int score)
{
	_hud->updateScore(score);
}
void LevelGenerator::gameOver()
{
	SimpleAudioEngine::sharedEngine()->playBackgroundMusic("Sound/defeat.mp3") ;	

	SimpleAudioEngine::sharedEngine()->playEffect("alarm.wav") ;				
	GameOverScene *objModeScene = GameOverScene::node();
	CCDirector::sharedDirector()->replaceScene(objModeScene);
}
void LevelGenerator::gameWin()
{
	SimpleAudioEngine::sharedEngine()->playBackgroundMusic("Sound/won.mp3") ;	


	SimpleAudioEngine::sharedEngine()->playEffect("Winner.wav") ;				
	WinnerScene *objModeScene = WinnerScene::node();
	CCDirector::sharedDirector()->replaceScene(objModeScene);
}
void LevelGenerator::updateKeyBoard(float dt)
{


	if(isPlayerLive)
	{

		Input::instance()->update();
		int JumpSteps = 0;


		if(Input::instance()->keyDown(VK_UP)) 
		{
			state_Player=STATE_JUMP;
			SimpleAudioEngine::sharedEngine()->playEffect("Sound/jumping.wav") ;
		}
		//if(Input::instance()->keyDown(VK_DOWN)) 
		if(Input::instance()->keyRight(VK_RIGHT)) 
		{
			state_Player=STATE_WALK_RIGHT;
			SimpleAudioEngine::sharedEngine()->playEffect("Sound/walking.wav") ;
			if(playerType==1)
				GeneratePlayerAnimation("WalkRight/Walk",2,_Player);
			else if (playerType==2) 
				GeneratePlayerAnimation("GoldenPlayer_Walk/Right",7,_Player);
			else if (playerType==3) 
				GeneratePlayerAnimation("BrownPlayer_Walk/Right",4,_Player);

		}
		if(Input::instance()->keyLeft(VK_LEFT)) 
		{
			state_Player=STATE_WALK_LEFT;
			SimpleAudioEngine::sharedEngine()->playEffect("Sound/walking.wav") ;
			if(playerType==1)
				GeneratePlayerAnimation("WalkLeft/Walk",2,_Player);
			else if (playerType==2) 
				GeneratePlayerAnimation("GoldenPlayer_Walk/Left",7,_Player);
			else if (playerType==3) 
				GeneratePlayerAnimation("BrownPlayer_Walk/Left",4,_Player);
		}


		///@@@@@@@@@@@@@@@@@@@@@@@@@@ JUMP @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

		if (  state_Player==STATE_JUMP  )
		{
			if(playerType==1)
				_playerBody->SetLinearVelocity(b2Vec2(0,_playerBody->GetMass()*PLAYER1_JUMP));
			else if (playerType==2) 
				_playerBody->SetLinearVelocity(b2Vec2(0,_playerBody->GetMass()*PLAYER2_JUMP));
			else if (playerType==3) 

				_playerBody->SetLinearVelocity(b2Vec2(0,_playerBody->GetMass()*PLAYER3_JUMP));
			SimpleAudioEngine::sharedEngine()->playEffect("Sound/jumping.wav") ;
		}


		///@@@@@@@@@@@@@@@@@@@@@@@@@@ JOYSTICK @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
		if (_hud->getJumpButton()->getIsActive())
		{
			if(playerType==1)
				_playerBody->SetLinearVelocity(b2Vec2(0,_playerBody->GetMass()*PLAYER1_JUMP));
			else if (playerType==2) 
				_playerBody->SetLinearVelocity(b2Vec2(0,_playerBody->GetMass()*PLAYER2_JUMP));
			else if (playerType==3) 

				_playerBody->SetLinearVelocity(b2Vec2(0,_playerBody->GetMass()*PLAYER3_JUMP));
			SimpleAudioEngine::sharedEngine()->playEffect("Sound/jumping.wav") ;
		}


		//ATTACK BUTTON CLICKED
		if (_hud->getAttackButton()->getIsActive())
		{

			if(state_Player==STATE_STAND_RIGHT)
			{
				if(playerType==1)
				{
					GeneratePlayerAnimation("FireRight/Fire",5,_Player);
					SimpleAudioEngine::sharedEngine()->playEffect("Sound/doublegun.mp3") ;
				}
				else if (playerType==2) 
				{
					GeneratePlayerAnimation("GoldenPlayer_FireRight/Fire",8,_Player);
					SimpleAudioEngine::sharedEngine()->playEffect("Sound/snipershot.mp3") ;
				}
				else if (playerType==3) 
				{
					GeneratePlayerAnimation("BrownPlayer_FireRight/Fire",4,_Player);
					SimpleAudioEngine::sharedEngine()->playEffect("Sound/barreta.mp3") ;
				}
			}

			else if(state_Player==STATE_STAND_LEFT )

			{
				if(playerType==1)
				{
					GeneratePlayerAnimation("FireLeft/Fire",5,_Player);
					SimpleAudioEngine::sharedEngine()->playEffect("Sound/doublegun.mp3") ;
				}
				else if (playerType==2) 
				{
					GeneratePlayerAnimation("GoldenPlayer_FireLeft/Fire",9,_Player);
					SimpleAudioEngine::sharedEngine()->playEffect("Sound/snipershot.mp3") ;
				}
				else if (playerType==3) 
				{
					GeneratePlayerAnimation("BrownPlayer_FireLeft/Fire",3,_Player);
					SimpleAudioEngine::sharedEngine()->playEffect("Sound/barreta.mp3") ;
				}

			}

			CCSprite *sprite = CCSprite::create("SpriteImages/Bullet.png");

			//  sprite->setPosition( CCPointMake(_playerBody->GetPosition().x +200/PTM_RATIO, _playerBody->GetPosition().y+100/PTM_RATIO ));
			addChild(sprite, 1);
			b2BodyDef bodyDef;
			bodyDef.type = b2_dynamicBody;
			if(state_Player>=0)
				bodyDef.position.Set(_playerBody->GetPosition().x,_playerBody->GetPosition().y);
			else
				bodyDef.position.Set(_playerBody->GetPosition().x-1,_playerBody->GetPosition().y);

			bodyDef.userData = sprite;
			_playerBulletBody = _world->CreateBody(&bodyDef); 


			b2FixtureDef boxDef;
			boxDef.userData = (void*)1;
			_bullets->push_back(_playerBulletBody);
			b2CircleShape circle;
			circle.m_radius = sprite->getContentSize().width/2.0f/PTM_RATIO;
			boxDef.shape = &circle;
			boxDef.density = 5.5f;

			boxDef.restitution =0.5f;
			boxDef.friction = 0.0f;
			_playerBulletBody->CreateFixture(&boxDef);
			float FireImpluse=300;
			if(playerType==1)
				FireImpluse = PLAYER1_FIRE;
			else if (playerType==2) 
				FireImpluse = PLAYER2_FIRE;
			else if (playerType==3) 
				FireImpluse = PLAYER3_FIRE;


			if(state_Player==1)

				_playerBulletBody->ApplyForce(b2Vec2(FireImpluse,20),_playerBulletBody->GetWorldCenter()); 
			else
				_playerBulletBody->ApplyForce(b2Vec2(-FireImpluse,20),_playerBulletBody->GetWorldCenter()); 

			if(playerType==1)
				SimpleAudioEngine::sharedEngine()->playEffect("Sound/doublegun.mp3") ;
			else if (playerType==2) 
				SimpleAudioEngine::sharedEngine()->playEffect("Sound/snipershot.mp3") ;
			else if (playerType==3) 
				SimpleAudioEngine::sharedEngine()->playEffect("Sound/barreta.mp3") ;
		}


		int a=_hud->getxval();
		if (_hud->getxval()> 0.0f)
		{

			state_Player=STATE_WALK_RIGHT;
			SimpleAudioEngine::sharedEngine()->playEffect("Sound/walking.wav") ;

			if(playerType==1)
				GeneratePlayerAnimation("WalkRight/Walk",2,_Player);
			else if (playerType==2) 
				GeneratePlayerAnimation("GoldenPlayer_Walk/Right",7,_Player);
			else if (playerType==3) 
				GeneratePlayerAnimation("BrownPlayer_Walk/Right",4,_Player);		

		}
		if (_hud->getxval() < 0.0f)
		{

			state_Player=STATE_WALK_LEFT;			
			SimpleAudioEngine::sharedEngine()->playEffect("Sound/walking.wav") ;

			if(playerType==1)
				GeneratePlayerAnimation("WalkLeft/Walk",2,_Player);
			else if (playerType==2) 
				GeneratePlayerAnimation("GoldenPlayer_Walk/Left",7,_Player);
			else if (playerType==3) 
				GeneratePlayerAnimation("BrownPlayer_Walk/Left",4,_Player);

		}

		b2Vec2 vel = _playerBody->GetLinearVelocity();

		float desiredVel = 0;
		switch ( state_Player )
		{
		case STATE_WALK_LEFT:
			if(playerType==1)
				desiredVel = -PLAYER1_WALK;
			else if (playerType==2) 
				desiredVel = -PLAYER2_WALK;
			else if (playerType==3) 
				desiredVel = -PLAYER3_WALK;

			state_Player=STATE_STAND_LEFT;
			break;
		case STATE_STAND_RIGHT:  
			desiredVel =  0; 
			break;
		case STATE_STAND_LEFT:  
			desiredVel =  0; 
			break;
		case STATE_WALK_RIGHT:
			if(playerType==1)
				desiredVel = PLAYER1_WALK;
			else if (playerType==2) 
				desiredVel = PLAYER2_WALK;
			else if (playerType==3) 
				desiredVel = PLAYER3_WALK;
			state_Player=STATE_STAND_RIGHT;
			break;

		}
		float velChange = desiredVel - vel.x;
		float impulse = _playerBody->GetMass() * velChange; //disregard time factor
		_playerBody->ApplyLinearImpulse( b2Vec2(impulse,0), _playerBody->GetWorldCenter() );
		//_playerBody->SetLinearVelocity(b2Vec2(2.0,0));

	}

}
/////////////////////////////////////////////////////////////////////////////////////////////////
//   CONTACT FUNCTION
////////////////////////////////////////////////////////////////////////////////////////////////
void LevelGenerator::BeginContact(b2Contact* contact)
{ /* handle begin event */ }

void  LevelGenerator::EndContact(b2Contact* contact)
{ /* handle begin event */ }

void  LevelGenerator::PreSolve(b2Contact* contact, const b2Manifold* oldManifold)
{ /* handle begin event */ }

void  LevelGenerator::PostSolve(b2Contact* contact,  const b2ContactImpulse* impulse)
{

	if (contact->GetFixtureA()->GetUserData() !=NULL || contact->GetFixtureB()->GetUserData() !=NULL  )
	{

		int32 count = contact->GetManifold()->pointCount;

		b2Fixture  *fdef=contact->GetFixtureA();
		b2Filter bfilterA= fdef->GetFilterData();
		fdef=contact->GetFixtureB();
		b2Filter bfilterB= fdef->GetFilterData();

		float32 maxImpulse = 0.0f;
		for (int32 i = 0; i < count; ++i)
		{
			maxImpulse = b2Max(maxImpulse, impulse->normalImpulses[i]);
		}
		float32 breakThreshold=1.0 ;
		bool isCollision=true;
		//if(bfilterA.categoryBits==0x0004 ||bfilterB.categoryBits==0x0004 )
		//	breakThreshold=3.5;
		//if(bfilterA.categoryBits==0x0008 ||bfilterB.categoryBits==0x0008 )
		//	breakThreshold=2.0;
		//if(( bfilterA.categoryBits==0x0001 && bfilterB.categoryBits==0x0002) || ( bfilterA.categoryBits==0x0002 && bfilterB.categoryBits==0x0001) )
		//	isCollision=false;
		b2Body* ABody=contact->GetFixtureA()->GetBody();
		b2Body* BBody=contact->GetFixtureB()->GetBody();
		//if(( ABody==_playerBody && BBody==_body) || ( BBody==_playerBody && ABody==_body) )
		//isCollision=false;
		// if(( ABody==_playerBody && BBody==m_car) || ( BBody==_playerBody && ABody==m_car) )
		//isCollision=false;
		//if(( ABody==_playerBody && BBody==_rockBody) || ( BBody==_playerBody && ABody==_rockBody) )
		//isCollision=false;
		//if(( ABody==m_truck && BBody==m_car) || ( BBody==m_truck && ABody==m_car) )
		//isCollision=false;
		if(( ABody==m_wheel1 && BBody==_body) || ( BBody==m_wheel1 && ABody==_body) )
			isCollision=false;
		if(( ABody==m_wheel2 && BBody==_body) || ( BBody==m_wheel2 && ABody==_body) )
			isCollision=false;
		if(( ABody==_baseCampBody && BBody==_body) || ( BBody==_baseCampBody && ABody==_body) )
			isCollision=false;
		if(( ABody==_baseCampBody && BBody==_playerBody) || ( BBody==_baseCampBody && ABody==_playerBody) )
			isCollision=false;
		if(( ABody==_baseCampBody && BBody==m_car) || ( BBody==_baseCampBody && ABody==m_car) )
			isCollision=false;
		if(( ABody==_baseCampBody && BBody==m_truck) || ( BBody==_baseCampBody && ABody==m_truck) )
			isCollision=false;
		if(( ABody==_baseCampBody && BBody==_EnemyBulletBody) || ( BBody==_baseCampBody && ABody==_EnemyBulletBody) )
			isCollision=false;



		if(( ABody== _powerBody && BBody==_playerBody) || ( BBody==_powerBody && ABody==_playerBody) )
		{

			if (contact->GetFixtureA()->GetUserData() !=NULL)
				contacts.insert(contact->GetFixtureA()->GetBody());
			if (contact->GetFixtureB()->GetUserData() !=NULL)
				contacts.insert(contact->GetFixtureB()->GetBody());
			isCollision=false;

		}




		if(( ABody==m_truck && BBody==_playerBulletBody) || ( BBody==m_truck && ABody==_playerBulletBody) )
			isCollision=true;
		else if(( ABody==m_truck ) || ( BBody==m_truck) )
			isCollision=false;

		/*	if(( ABody==m_tank && BBody==_playerBulletBody) || ( BBody==m_tank && ABody==_playerBulletBody) )
		isCollision=true;
		else if(( ABody==m_tank ) || ( BBody==m_tank) )
		isCollision=false;*/


		if(( ABody==_playerBody && BBody==_EnemyBulletBody) || ( BBody==_playerBody && ABody==_EnemyBulletBody) )
			isCollision=true;
		else if(( ABody==_playerBody ) || ( BBody==_playerBody) )
			isCollision=false;


		if(( ABody==_TNTBody && BBody==_playerBulletBody) || ( BBody==_TNTBody && ABody==_playerBulletBody) )
			isCollision=true;
		else if(( ABody==_TNTBody ) || ( BBody==_TNTBody) )
			isCollision=false;

		if(( ABody==_enemyPlayerBody && BBody==_playerBulletBody) || ( BBody==_enemyPlayerBody && ABody==_playerBulletBody) )
			isCollision=true;
		else if(( ABody==_enemyPlayerBody ) || ( BBody==_enemyPlayerBody) )
			isCollision=false;

		if(( ABody==_enemyZombiePlayerBody && BBody==_playerBulletBody) || ( BBody==_enemyZombiePlayerBody && ABody==_playerBulletBody) )
			isCollision=true;
		else if(( ABody==_enemyZombiePlayerBody ) || ( BBody==_enemyZombiePlayerBody) )
			isCollision=false;

		if(( ABody==_rockBody && BBody==_playerBulletBody) || ( BBody==_rockBody && ABody==_playerBulletBody) )
			isCollision=true;
		else if(( ABody==_rockBody ) || ( BBody==_rockBody) )
			isCollision=false;

		if(( ABody==_woodBody && BBody==_playerBulletBody) || ( BBody==_woodBody && ABody==_playerBulletBody) )
			isCollision=true;
		else if(( ABody==_woodBody ) || ( BBody==_woodBody) )
			isCollision=false;

		uint16 catA = bfilterA.categoryBits;

		uint16 maskA = bfilterA.maskBits;

		uint16 catB = bfilterB.categoryBits;

		uint16 maskB = bfilterB.maskBits;





		if (maxImpulse > breakThreshold && isCollision )
		{

			if (contact->GetFixtureA()->GetUserData() !=NULL)
				contacts.insert(contact->GetFixtureA()->GetBody());
			if (contact->GetFixtureB()->GetUserData() !=NULL)
				contacts.insert(contact->GetFixtureB()->GetBody());
		}
	}
}