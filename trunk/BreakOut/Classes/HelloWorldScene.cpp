//
//  HelloWorldScene.cpp
//  BreakoutCocos2D-x
//
//  Created by Clawoo on 9/15/11.
//  Copyright __MyCompanyName__ 2011. All rights reserved.
//
#include "HelloWorldScene.h"

using namespace cocos2d;
using namespace CocosDenshion;

#define PTM_RATIO 32

HelloWorld::~HelloWorld()
{
    CC_SAFE_DELETE(_world);
    _groundBody = NULL;
    delete _contactListener;
}

HelloWorld::HelloWorld()
{
    setTouchEnabled( true );
    
	CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    
	// Define the gravity vector.
	b2Vec2 gravity;
	gravity.Set(0.0f, 0.0f);
	
	// Do we want to let bodies sleep?
	bool doSleep = true;
    
	// Construct a world object, which will hold and simulate the rigid bodies.
	_world = new b2World(gravity);
    
    // Create edges around the entire screen
	// Define the ground body.
	b2BodyDef groundBodyDef;
	groundBodyDef.position.Set(0, 0); // bottom-left corner
	
	// Call the body factory which allocates memory for the ground body
	// from a pool and creates the ground box shape (also from a pool).
	// The body is also added to the world.
	_groundBody = _world->CreateBody(&groundBodyDef);

    b2EdgeShape groundBox;

    // Create fixtures for the four borders (the border shape is re-used)
    groundBox.Set(b2Vec2(0,0), b2Vec2(winSize.width/PTM_RATIO,0));
    _bottomFixture = _groundBody->CreateFixture(&groundBox, 0);
    groundBox.Set(b2Vec2(0,winSize.height/PTM_RATIO), b2Vec2(winSize.width/PTM_RATIO,winSize.height/PTM_RATIO));
    _groundBody->CreateFixture(&groundBox, 0);
    groundBox.Set(b2Vec2(0,winSize.height/PTM_RATIO), b2Vec2(0,0));
    _groundBody->CreateFixture(&groundBox, 0);
    groundBox.Set(b2Vec2(winSize.width/PTM_RATIO,winSize.height/PTM_RATIO), b2Vec2(winSize.width/PTM_RATIO,0));
    _groundBody->CreateFixture(&groundBox, 0);
	
	//groundBox.SetAsBox(winSize.width/PTM_RATIO);
	// bottom
	/*groundBox.SetAsEdge(b2Vec2(0,0), b2Vec2(winSize.width/PTM_RATIO,0));
	_bottomFixture = _groundBody->CreateFixture(&groundBox, 0);
	
	// top
	groundBox.SetAsEdge(b2Vec2(0,winSize.height/PTM_RATIO), b2Vec2(winSize.width/PTM_RATIO,winSize.height/PTM_RATIO));
	_groundBody->CreateFixture(&groundBox, 0);
	
	// left
	groundBox.SetAsEdge(b2Vec2(0,winSize.height/PTM_RATIO), b2Vec2(0,0));
	_groundBody->CreateFixture(&groundBox, 0);
	
	// right
	groundBox.SetAsEdge(b2Vec2(winSize.width/PTM_RATIO,winSize.height/PTM_RATIO), b2Vec2(winSize.width/PTM_RATIO,0));
	_groundBody->CreateFixture(&groundBox, 0);*/
    
    // Create sprite and add it to the layer
	CCSprite *ball = CCSprite::create("Ball.jpg");
    ball->setPosition(ccp(100, 100));
    ball->setTag(1);
    this->addChild(ball);
    
    // Create ball body
    b2BodyDef ballBodyDef;
    ballBodyDef.type = b2_dynamicBody;
    ballBodyDef.position.Set(100/PTM_RATIO, 100/PTM_RATIO);
    ballBodyDef.userData = ball;
    
    b2Body *ballBody = _world->CreateBody(&ballBodyDef);
    
    // Create circle shape
    b2CircleShape circle;
    circle.m_radius = 26.0/PTM_RATIO;
    
    // Create shape definition and add body
    b2FixtureDef ballShapeDef;
    ballShapeDef.shape = &circle;
    ballShapeDef.density = 1.0f;
    ballShapeDef.friction = 0.f;
    ballShapeDef.restitution = 1.0f;
    _ballFixture = ballBody->CreateFixture(&ballShapeDef);
    
    b2Vec2 force = b2Vec2(10, 10);
    ballBody->ApplyLinearImpulse(force, ballBodyDef.position);
    
    CCSprite *paddle = CCSprite::create("Paddle.jpg");
    paddle->setPosition(ccp(winSize.width/2, 50));
    this->addChild(paddle);
    
    // Create paddle body
    b2BodyDef paddleBodyDef;
    paddleBodyDef.type = b2_dynamicBody;
    paddleBodyDef.position.Set(winSize.width/2/PTM_RATIO, 50/PTM_RATIO);
    paddleBodyDef.userData = paddle;
    _paddleBody = _world->CreateBody(&paddleBodyDef);
    
    // Create paddle shape
    b2PolygonShape paddleShape;
    paddleShape.SetAsBox(paddle->getContentSize().width/PTM_RATIO/2, 
                         paddle->getContentSize().height/PTM_RATIO/2);
    
    // Create shape definition and add to body
    b2FixtureDef paddleShapeDef;
    paddleShapeDef.shape = &paddleShape;
    paddleShapeDef.density = 10.0f;
    paddleShapeDef.friction = 0.4f;
    paddleShapeDef.restitution = 0.1f;
    _paddleFixture = _paddleBody->CreateFixture(&paddleShapeDef);
    
    // Restrict paddle along the x axis
    b2PrismaticJointDef jointDef;
    b2Vec2 worldAxis(1.0f, 0.0f);
    jointDef.collideConnected = true;
    jointDef.Initialize(_paddleBody, _groundBody, 
                        _paddleBody->GetWorldCenter(), worldAxis);
    _world->CreateJoint(&jointDef);

    // in C++ you need to initialize objects to NULL
    _mouseJoint = NULL;
    
    // Create contact listener
    _contactListener = new MyContactListener();
    _world->SetContactListener(_contactListener);
    
    for(int i = 0; i < 4; i++) {
        
        static int padding=20;
        
        // Create block and add it to the layer
        CCSprite *block = CCSprite::create("Block.jpg");
        int xOffset = padding+block->getContentSize().width/2+
        ((block->getContentSize().width+padding)*i);
        block->setPosition(ccp(xOffset, 250));
        block->setTag(2);
        this->addChild(block);
        
        // Create block body
        b2BodyDef blockBodyDef;
        blockBodyDef.type = b2_dynamicBody;
        blockBodyDef.position.Set(xOffset/PTM_RATIO, 250/PTM_RATIO);
        blockBodyDef.userData = block;
        b2Body *blockBody = _world->CreateBody(&blockBodyDef);
        
        // Create block shape
        b2PolygonShape blockShape;
        blockShape.SetAsBox(block->getContentSize().width/PTM_RATIO/2,
                            block->getContentSize().height/PTM_RATIO/2);
        
        // Create shape definition and add to body
        b2FixtureDef blockShapeDef;
        blockShapeDef.shape = &blockShape;
        blockShapeDef.density = 10.0;
        blockShapeDef.friction = 0.0;
        blockShapeDef.restitution = 0.1f;
        blockBody->CreateFixture(&blockShapeDef);
        
    }
    
    SimpleAudioEngine::sharedEngine()->playBackgroundMusic("background-music-aac.caf");
    
    this->schedule(schedule_selector(HelloWorld::tick));
}

void HelloWorld::draw()
{
	glDisable(GL_TEXTURE_2D);
	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	
	//world->DrawDebugData();
	
	// restore default GL states
	glEnable(GL_TEXTURE_2D);
	glEnableClientState(GL_COLOR_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);	
}

void HelloWorld::tick(float dt)
{
	//It is recommended that a fixed time step is used with Box2D for stability
	//of the simulation, however, we are using a variable time step here.
	//You need to make an informed choice, the following URL is useful
	//http://gafferongames.com/game-physics/fix-your-timestep/
	
	int velocityIterations = 8;
	int positionIterations = 1;
    
	// Instruct the world to perform a single step of simulation. It is
	// generally best to keep the time step and iterations fixed.
	_world->Step(dt, velocityIterations, positionIterations);
	
    bool blockFound = false;
    
	//Iterate over the bodies in the physics world
	for (b2Body* b = _world->GetBodyList(); b; b = b->GetNext())
	{
		if (b->GetUserData() != NULL) {
			//Synchronize the AtlasSprites position and rotation with the corresponding body
			CCSprite* myActor = (CCSprite*)b->GetUserData();
			myActor->setPosition( CCPointMake( b->GetPosition().x * PTM_RATIO, b->GetPosition().y * PTM_RATIO) );
			myActor->setRotation( -1 * CC_RADIANS_TO_DEGREES(b->GetAngle()) );
            
            if (myActor->getTag() == 1) {
                static int maxSpeed = 10;
                
                b2Vec2 velocity = b->GetLinearVelocity();
                float32 speed = velocity.Length();
                
                if (speed > maxSpeed) {
                    b->SetLinearDamping(0.5);
                } else if (speed < maxSpeed) {
                    b->SetLinearDamping(0.0);
                }
                
            }
            
            if (myActor->getTag() == 2) {
                blockFound = true;
            }
		}	
	}
    
    std::vector<b2Body *>toDestroy;
    std::vector<MyContact>::iterator pos;
    for(pos = _contactListener->_contacts.begin(); 
        pos != _contactListener->_contacts.end(); ++pos) {
        MyContact contact = *pos;
        
        if ((contact.fixtureA == _bottomFixture && contact.fixtureB == _ballFixture) ||
            (contact.fixtureA == _ballFixture && contact.fixtureB == _bottomFixture)) {
            GameOverScene *gameOverScene = GameOverScene::node();
            gameOverScene->getLayer()->getLabel()->setString("You Lose! :[");
            CCDirector::sharedDirector()->replaceScene(gameOverScene);
        } 
        
        b2Body *bodyA = contact.fixtureA->GetBody();
        b2Body *bodyB = contact.fixtureB->GetBody();
        if (bodyA->GetUserData() != NULL && bodyB->GetUserData() != NULL) {
            CCSprite *spriteA = (CCSprite *) bodyA->GetUserData();
            CCSprite *spriteB = (CCSprite *) bodyB->GetUserData();
            
            // Sprite A = ball, Sprite B = Block
            if (spriteA->getTag() == 1 && spriteB->getTag() == 2) {
                if (std::find(toDestroy.begin(), toDestroy.end(), bodyB) 
                    == toDestroy.end()) {
                    toDestroy.push_back(bodyB);
                }
            }
            // Sprite B = block, Sprite A = ball
            else if (spriteA->getTag() == 2 && spriteB->getTag() == 1) {
                if (std::find(toDestroy.begin(), toDestroy.end(), bodyA) 
                    == toDestroy.end()) {
                    toDestroy.push_back(bodyA);
                }
            }        
        }                 
    }
    
    std::vector<b2Body *>::iterator pos2;
    for(pos2 = toDestroy.begin(); pos2 != toDestroy.end(); ++pos2) {
        b2Body *body = *pos2;     
        if (body->GetUserData() != NULL) {
            CCSprite *sprite = (CCSprite *) body->GetUserData();
            this->removeChild(sprite, true);
        }
        _world->DestroyBody(body);
    }
    
    if (!blockFound)
    {
        GameOverScene *gameOverScene = GameOverScene::node();
        gameOverScene->getLayer()->getLabel()->setString("You Win!");
        CCDirector::sharedDirector()->replaceScene(gameOverScene);
    }
    
    if (toDestroy.size() > 0)
    {
        SimpleAudioEngine::sharedEngine()->playEffect("blip.caf");
    }
}

void HelloWorld::ccTouchesBegan(cocos2d::CCSet* touches, cocos2d::CCEvent* event)
{
    if (_mouseJoint != NULL) return;
    
    CCTouch *myTouch = (CCTouch *)touches->anyObject();
    CCPoint location = myTouch->getLocationInView();
    location = CCDirector::sharedDirector()->convertToGL(location);
    b2Vec2 locationWorld = b2Vec2(location.x/PTM_RATIO, location.y/PTM_RATIO);
    
    if (_paddleFixture->TestPoint(locationWorld)) {
        b2MouseJointDef md;
        md.bodyA = _groundBody;
        md.bodyB = _paddleBody;
        md.target = locationWorld;
        md.collideConnected = true;
        md.maxForce = 1000.0f * _paddleBody->GetMass();
        
        _mouseJoint = (b2MouseJoint *)_world->CreateJoint(&md);
        _paddleBody->SetAwake(true);
    }
}

void HelloWorld::ccTouchesMoved(CCSet* touches, CCEvent* event)
{
    if (_mouseJoint == NULL) return;
    
    CCTouch *myTouch = (CCTouch *)touches->anyObject();
    CCPoint location = myTouch->getLocationInView();
    location = CCDirector::sharedDirector()->convertToGL(location);
    b2Vec2 locationWorld = b2Vec2(location.x/PTM_RATIO, location.y/PTM_RATIO);

    _mouseJoint->SetTarget(locationWorld);
}

void HelloWorld::ccTouchesCancelled(CCSet* touches, CCEvent* event)
{
    if (_mouseJoint) 
    {
        _world->DestroyJoint(_mouseJoint);
        _mouseJoint = NULL;
    }
}

void HelloWorld::ccTouchesEnded(CCSet* touches, CCEvent* event)
{
    if (_mouseJoint) 
    {
        _world->DestroyJoint(_mouseJoint);
        _mouseJoint = NULL;
    }  
}

CCScene* HelloWorld::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // add layer as a child to scene
    CCLayer* layer = new HelloWorld();
    scene->addChild(layer);
    layer->release();
    
    return scene;
}
