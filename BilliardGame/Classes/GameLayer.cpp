//
//  GameLayer.cpp
//  MiniPool
//
//  Created by Roger Engelbert on 1/13/13.
//  Copyright __MyCompanyName__ 2013. All rights reserved.
//
#include "GameLayer.h"
#include "CollisionListener.h"

#define SHOT_POWER 6.0

using namespace cocos2d;
using namespace CocosDenshion;


GameLayer::GameLayer() {
    
    _screenSize = CCDirector::sharedDirector()->getWinSize();
    
    CCSprite * bg = CCSprite::create("bg.png");
    bg->setPosition(ccp(_screenSize.width * 0.5f, _screenSize.height * 0.5f));
    this->addChild(bg);
    
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("sprite_sheet.plist");
    _gameBatchNode = CCSpriteBatchNode::create("sprite_sheet.png", 50);
    this->addChild(_gameBatchNode, kMiddleground);
    
    // init physics
    this->initPhysics();
    
    setTouchEnabled( true );
    scheduleUpdate();
    
    _usingCue = false;
    _running = true;
    _canShoot = true;
    _gameState = kGameIntro;
    _time = 0;
    _ballsInPlay = 15;
    
    _lineContainer = LineContainer::create();
    this->addChild(_lineContainer);
    
    _intro = CCSprite::createWithSpriteFrameName("logo.png");
    _intro->setPosition(ccp(_screenSize.width * 0.5f, _screenSize.height * 0.55f));
    _gameBatchNode->addChild(_intro, kForeground);
    
    _gameOver = CCSprite::createWithSpriteFrameName("gameover.png");
    _gameOver->setPosition(ccp(_screenSize.width * 0.5f, _screenSize.height * 0.55f));
    _gameOver->setVisible(false);
    _gameBatchNode->addChild(_gameOver, kForeground);
    
    _timer = CCLabelBMFont::create("0", "font.fnt", _screenSize.width * 0.5f);
    _timer->setPosition(ccp(_screenSize.width * 0.5f, _screenSize.height * 0.94f));
    _timer->setVisible(false);
    this->addChild(_timer, kForeground);
    
    this->schedule(schedule_selector(GameLayer::ticktock), 1.5f); 
}



GameLayer::~GameLayer() {
    delete _world;
    _world = NULL;
    
    delete m_debugDraw;
    delete _collisionListener;
    
    CC_SAFE_RELEASE(_balls);
    CC_SAFE_RELEASE(_pockets);
}

void GameLayer::initPhysics() {
    
    
    b2Vec2 gravity;
    gravity.Set(0.0f, 0.0f);
    _world = new b2World(gravity);
    
    _world->SetAllowSleeping(true);
    _world->SetContinuousPhysics(true);
    _collisionListener = new CollisionListener();
    _world->SetContactListener(_collisionListener);
    
    m_debugDraw = new GLESDebugDraw( PTM_RATIO );
    _world->SetDebugDraw(m_debugDraw);
    
    uint32 flags = 0;
    flags += b2Draw::e_shapeBit;
    m_debugDraw->SetFlags(flags);
    
    
    b2FixtureDef fixtureDef;
    b2BodyDef bodyDef;

    //Create table
    
    //add pockets
    _pockets = CCArray::createWithCapacity(6);
    _pockets->retain();
    
    b2Body * pocket;
    b2Sprite * pocketData;
    b2CircleShape circle;
    float startX = _screenSize.width * 0.07;
    float startY = _screenSize.height * 0.92f;
    for (int i = 0; i < 6; i++) {
        bodyDef.type = b2_staticBody;
        if (i < 3) {
            bodyDef.position.Set(
                 startX/PTM_RATIO,
                 (startY - i * (_screenSize.height * 0.84f * 0.5f))/PTM_RATIO);
            
        } else {
            bodyDef.position.Set(
                 (startX + _screenSize.width * 0.85f)/PTM_RATIO,
                 (startY - (i-3) * (_screenSize.height * 0.84f * 0.5f))/PTM_RATIO);
        }
        pocket = _world->CreateBody(&bodyDef);
        fixtureDef.isSensor = true;
        circle.m_radius = (float) (1.5 * BALL_RADIUS) / PTM_RATIO;
        fixtureDef.shape = &circle;
        
        pocket->CreateFixture(&fixtureDef);
        pocketData = new b2Sprite(this, kSpritePocket);
        pocket->SetUserData(pocketData);
        _pockets->addObject(pocketData);
    }
    
    //create table sides
    b2BodyDef tableBodyDef;
    tableBodyDef.position.Set(0, 0);
    b2Body* tableBody = _world->CreateBody(&tableBodyDef);
    
    // Define the table edges
    b2EdgeShape tableBox;
    
    // bottom
    tableBox.Set(b2Vec2(_screenSize.width * 0.14f/PTM_RATIO, _screenSize.height * 0.09f/PTM_RATIO),
                  b2Vec2(_screenSize.width * 0.86f/PTM_RATIO, _screenSize.height * 0.09f/PTM_RATIO));
    tableBody->CreateFixture(&tableBox,0);
    
    // top
    tableBox.Set(b2Vec2(_screenSize.width * 0.14f/PTM_RATIO, _screenSize.height * 0.91f/PTM_RATIO),
                  b2Vec2(_screenSize.width * 0.86f/PTM_RATIO, _screenSize.height * 0.91f/PTM_RATIO));
    tableBody->CreateFixture(&tableBox,0);
    
    // left
    tableBox.Set(b2Vec2(_screenSize.width * 0.09f/PTM_RATIO, _screenSize.height * 0.12f/PTM_RATIO),
                 b2Vec2(_screenSize.width * 0.09f/PTM_RATIO, _screenSize.height * 0.46f/PTM_RATIO));
    tableBody->CreateFixture(&tableBox,0);
    
    tableBox.Set(b2Vec2(_screenSize.width * 0.91f/PTM_RATIO, _screenSize.height * 0.12f/PTM_RATIO),
                 b2Vec2(_screenSize.width * 0.91f/PTM_RATIO, _screenSize.height * 0.46f/PTM_RATIO));
    tableBody->CreateFixture(&tableBox,0);
    
    
    // right
    tableBox.Set(b2Vec2(_screenSize.width * 0.09f/PTM_RATIO, _screenSize.height * 0.54f/PTM_RATIO),
                 b2Vec2(_screenSize.width * 0.09f/PTM_RATIO, _screenSize.height * 0.88f/PTM_RATIO));
    tableBody->CreateFixture(&tableBox,0);
    
    
    tableBox.Set(b2Vec2(_screenSize.width * 0.91f/PTM_RATIO, _screenSize.height * 0.54f/PTM_RATIO),
                 b2Vec2(_screenSize.width * 0.91f/PTM_RATIO, _screenSize.height * 0.88f/PTM_RATIO));
    tableBody->CreateFixture(&tableBox,0);
   
    //add balls
    _balls = CCArray::createWithCapacity(15);
    _balls->retain();
    
    Ball * ball;
    int col = 5;
    int colCnt = 0;
    startX = (_screenSize.width - 8 * BALL_RADIUS) * 0.5f;
    startY = _screenSize.height * 0.82f;
    float newX;
    float newY;
    int color;
    for (int i = 0; i < 15; i++) {
        if (i == 10) {
            color = kColorBlack;
        } else if (i % 2 == 0) {
            color = kColorYellow;
        } else {
            color = kColorRed;
        }
        if (colCnt < col) {
            newX = startX + colCnt * 2 * BALL_RADIUS;
            colCnt++;
        } else {
            startX += BALL_RADIUS;
            startY -= 2 * BALL_RADIUS;
            newX = startX;
            col--;
            colCnt = 1;
        }
        newY = startY;
        ball = Ball::create(this, kSpriteBall, ccp(newX, newY), color);
        _gameBatchNode->addChild(ball, kMiddleground);
        _balls->addObject(ball);
    }
    
    //add cue ball
    _player = Ball::create(this, kSpritePlayer, ccp(_screenSize.width * 0.5f, _screenSize.height * 0.3f), kColorWhite);
    _gameBatchNode->addChild(_player, kMiddleground);
    
    _cue = Cue::create(this);
    
    _cue->setSpritePosition(ccp(_screenSize.width * 0.2f, _screenSize.height * 0.3f));
    _cue->getBody()->SetTransform(_cue->getBody()->GetPosition(), M_PI*0.5);
    _gameBatchNode->addChild(_cue, kForeground);
    
    
}

void GameLayer::draw()
{

    CCLayer::draw();
    
    ccGLEnableVertexAttribs( kCCVertexAttribFlag_Position );
    
    kmGLPushMatrix();
    
    _world->DrawDebugData();
    
    kmGLPopMatrix();
}

void GameLayer::update(float dt) {
    
   if (!_running) return;
   if (_gameState == kGameOver) return;

    _world->Step(dt, 10, 10);
    
    //track invisible objects
    int count = _balls->count();
    Ball * ball;
    
    for (int i = 0; i < count; i++) {
        
        ball = (Ball *) _balls->objectAtIndex(i);
        
        if (!ball->isVisible() && ball->getInPlay()) {
            ball->setInPlay(false);
            ball->hide();
            //count down balls
            _ballsInPlay--;
            SimpleAudioEngine::sharedEngine()->playEffect("drop.wav");
            if (_ballsInPlay == 0) {
                _gameState = kGameOver;
                _gameOver->setVisible(true);
            }
        } else {
         
            ball->update(dt);
        }
    }
    
    
    if (!_cue->isVisible())  {
        _cue->hide();
    } else {
        _cue->update(dt);
    }
    if (!_player->isVisible()) {
        _player->reset();
        _player->setVisible(true);
        SimpleAudioEngine::sharedEngine()->playEffect("whitedrop.wav");
    }
    _player->update(dt);
    
    //check to see if player ball is slow enough for a new shot
    if (_player->mag() < 0.5f && !_canShoot) {
        _player->getBody()->SetLinearVelocity(b2Vec2_zero);
        _player->getBody()->SetAngularVelocity(0);
        _canShoot = true;
    }
    
    if (_player->getPositionX() < 0 || _player->getPositionX() > _screenSize.width
        || _player->getPositionY() < 0 || _player->getPositionY() > _screenSize.height) {
        _player->setVisible(false);
    }
    
}

void GameLayer::ticktock(float dt){
    if (_gameState == kGamePlay) {
        _time++;
        char score[100] = {0};
        sprintf(score, "%i", _time);
        _timer->setString(score);
    }
}

void GameLayer::ccTouchesBegan(CCSet* touches, CCEvent* event) {

    if (!_running) return;
    
    if (_gameState == kGameOver) {
        if (_gameOver->isVisible()) _gameOver->setVisible(false);
        resetGame();
        return;
    }

    
    if (!_canShoot) return;
    
    CCTouch *touch = (CCTouch *)touches->anyObject();
    
    if (touch) {
        
	    CCPoint tap = touch->getLocation();
        
        CCPoint playerPos = _player->getPosition();
        float diffx = tap.x - playerPos.x;
        float diffy = tap.y - playerPos.y;
        float diff = pow(diffx, 2) + pow(diffy, 2);
        if (diff < pow(BALL_RADIUS * 4, 2)) {
            
            
            if (_gameState != kGamePlay) {
                _gameState = kGamePlay;
                if (_intro->isVisible()) _intro->setVisible(false);
                _timer->setVisible(true);
            }
            
            //make point lie within ball
            if (diff > pow(BALL_RADIUS * 2, 2)) {
                float angle = atan2(diffy, diffx);
                _cueStartPoint = ccp(
                         playerPos.x + BALL_RADIUS * 0.8f * cos(angle),
                         playerPos.y + BALL_RADIUS * 0.8f * sin(angle)
                                    );
            } else {
                _cueStartPoint = playerPos;
            }
            _lineContainer->setBallPoint(_cueStartPoint);
            
            _cue->getBody()->SetLinearVelocity(b2Vec2(0,0));
            _cue->getBody()->SetAngularVelocity(0.0);
            _touch = touch;
        }
    }
     
}


void GameLayer::ccTouchesMoved(CCSet* touches, CCEvent* event) {
    
   
    if (!_running) return;
    
	CCTouch *touch = (CCTouch *)touches->anyObject();
    
    if (touch && touch == _touch) {

        CCPoint tap = touch->getLocation();
        float diffx = tap.x - _player->getPositionX();
        float diffy = tap.y - _player->getPositionY();
        
        if (pow(diffx, 2) + pow(diffy, 2) > pow(BALL_RADIUS * 2, 2)) {
            _usingCue = true;
            _cue->setVisible(true);
            _lineContainer->setDrawing(true);
            placeCue(tap);
            _cue->getBody()->SetAwake(true);
        } else {
            _usingCue = false;
            _lineContainer->setDrawing(false);
            _cue->setVisible(false);
            _cue->getBody()->SetAwake(false);
        }
    }
}

void GameLayer::ccTouchesEnded(CCSet* touches, CCEvent* event) {
    
    if (_usingCue && _touch) {
        
        b2Body * cueBody = _cue->getBody();
        float angle = cueBody->GetAngle();
        
        //release cue!
        cueBody->
        ApplyLinearImpulse(b2Vec2(_pullBack * cos(angle) * SHOT_POWER,
                                          _pullBack * sin(angle) * SHOT_POWER
                                          ), cueBody->GetWorldCenter());
    }
    
    _usingCue = false;
    _touch = NULL;
    _lineContainer->setDrawing(false);
}

void GameLayer::placeCue(CCPoint position) {
    
    float diffx = _cueStartPoint.x - position.x;
    float diffy = _cueStartPoint.y - position.y;
    
    float angle = atan2(diffy, diffx);
    float distance = sqrt(pow (diffx, 2) + pow(diffy, 2));
    
    _pullBack = distance * 0.5f;
    CCPoint cuePosition = ccp(
          _cueStartPoint.x - (BALL_RADIUS * 21 + _pullBack) * cos(angle),
          _cueStartPoint.y - (BALL_RADIUS * 21 + _pullBack) * sin(angle));
    
    _cue->getBody()->SetTransform(b2Vec2(cuePosition.x/PTM_RATIO, cuePosition.y/PTM_RATIO), angle);
    _lineContainer->setCuePoint(ccp(
                                    _cueStartPoint.x - ( _pullBack) * cos(angle),
                                    _cueStartPoint.y - ( _pullBack) * sin(angle)));
}


void GameLayer::resetGame() {

    _gameState = kGameIntro;
    
    _player->reset();
    _cue->setVisible(false);

    int count = _balls->count();
    Ball * ball;
    for (int i = 0; i < count; i++) {
        ball = (Ball *) _balls->objectAtIndex(i);
        ball->reset();
    }
    
    _time = 0;
    _ballsInPlay = 15;
    _usingCue = false;
    _running = true;
    _canShoot = true;
    _timer->setVisible(false);
    
    char score[100] = {0};
    sprintf(score, "%i", _time);
    _timer->setString(score);
}



CCScene* GameLayer::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // add layer as a child to scene
    CCLayer* layer = new GameLayer();
    scene->addChild(layer);
    layer->release();
    
    return scene;
}
