/*
Soundtrack:
Merry Go Slower
by Kevin MacLeod (incompetech.com)

*/
#include "GameLayer.h"
#include "Platform.h"
#include "GSwitch.h"
#include "MenuLayer.h"

static GameLayer* _instance = NULL;

GameLayer::~GameLayer()
{
    
    CC_SAFE_RELEASE(_platformPool);
    CC_SAFE_RELEASE(_gSwitchPool);
    CC_SAFE_RELEASE(_levels);
    CC_SAFE_RELEASE(_player);
    CC_SAFE_RELEASE(_buttons);
    
    delete _world;
    _world = NULL;
    
    //delete m_debugDraw;
}

GameLayer::GameLayer() {
    
    
    _screenSize = CCDirector::sharedDirector()->getWinSize();
    _running = false;
    
    createScreen();
    
    std::string levelsFile = CCFileUtils::sharedFileUtils()->fullPathForFilename("levels.plist");
    _levels = CCArray::createWithContentsOfFileThreadSafe(levelsFile.c_str());
    _levels->retain();
    
    initPhysics();
    
    createPools();
    
    setTouchEnabled( true );
    setAccelerometerEnabled( true );
    
    
}

GameLayer * GameLayer::create (int level, int levelsCompleted) {
    if (!_instance) {
        _instance = new GameLayer();
    } else {
        _instance->clearLayer();
    }
    _instance->setLevelsCompleted(levelsCompleted);
    _instance->loadLevel(level);
    _instance->scheduleUpdate();
    SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();
    SimpleAudioEngine::sharedEngine()->playBackgroundMusic("background.mp3", true);
    return _instance;
}

//create Box2D world with default gravity
void GameLayer::initPhysics() {
    
    b2Vec2 gravity;
    gravity.Set(0.0f, -GRAVITY);
    _world = new b2World(gravity);
    
    _world->SetAllowSleeping(true);
    _world->SetContinuousPhysics(true);
    
    //m_debugDraw = new GLESDebugDraw( PTM_RATIO );
    //_world->SetDebugDraw(m_debugDraw);
    
    //uint32 flags = 0;
    //flags += b2Draw::e_shapeBit;
    //m_debugDraw->SetFlags(flags);
    
    _player = Eskimo::create(this);
    _gameBatchNode->addChild(_player, kMiddleground);
}

//
void GameLayer::loadLevel (int level) {
    
    clearLayer();
    
    _currentLevel = level;
    
    resetLevel();
    
    CCDictionary * levelData = (CCDictionary *) _levels->objectAtIndex(_currentLevel);
    
    int count;
    CCDictionary * data;
    
    //create platforms
    CCArray * platforms = (CCArray *) levelData->objectForKey("platforms");
    Platform * platform;
    count = platforms->count();
    
    for (int i = 0; i < count; i++) {
        platform = (Platform *) _platformPool->objectAtIndex(_platformPoolIndex);
        _platformPoolIndex++;
        if (_platformPoolIndex == _platformPool->count()) _platformPoolIndex = 0;
        
        data = (CCDictionary *) platforms->objectAtIndex(i);
        platform->initPlatform (
                                data->valueForKey("width")->intValue() * TILE,
                                data->valueForKey("angle")->floatValue(),
                                ccp(data->valueForKey("x")->floatValue() * TILE,
                                    data->valueForKey("y")->floatValue() * TILE)
                                );
    }
}

void GameLayer::resetLevel() {
    /*
     level initialization is broken into loadLevel and resetLevel because there are
     actions that need to be taken only when loading the new level, and actions that
     need to be taken when loading a new level AND resetting the level (when user presses
     play again or the reset level button)
     */
    
    _btnReset->setVisible(false);
    _btnPause->setVisible(false);
    
    CCDictionary * levelData = (CCDictionary *) _levels->objectAtIndex(_currentLevel);
    
    //set up world gravity based on level data value
    _gravity = levelData->valueForKey("gravity")->intValue();
    CCNotificationCenter::sharedNotificationCenter()->postNotification(NOTIFICATION_GRAVITY_SWITCH, (CCObject*)_gravity);
    
    switch (_gravity) {
        case kDirectionUp:
            _world->SetGravity(b2Vec2(0,GRAVITY));
            break;
        case kDirectionDown:
            _world->SetGravity(b2Vec2(0,-GRAVITY));
            break;
        case kDirectionLeft:
            _world->SetGravity(b2Vec2(-GRAVITY, 0));
            break;
        case kDirectionRight:
            _world->SetGravity(b2Vec2(GRAVITY, 0));
            break;
    }
    
    int count;
    CCDictionary * data;
    
    //create switches
    CCArray * switches = (CCArray *) levelData->objectForKey("switches");
    GSwitch * gswitch;
    count = switches->count();
    for (int i = 0; i < _gSwitchPool->count(); i++) {
        gswitch = (GSwitch *) _gSwitchPool->objectAtIndex(i);
        if (i < count) {
            data = (CCDictionary *) switches->objectAtIndex(i);
            gswitch->initGSwitch(
                                 data->valueForKey("gravity")->intValue(),
                                 ccp(data->valueForKey("x")->floatValue() * TILE,
                                     data->valueForKey("y")->floatValue() * TILE
                                     ));
        } else {
            gswitch->setVisible(false);
        }
        
    }
    //reset player to level start position
    _player->setSpritePosition(ccp(levelData->valueForKey("startx")->floatValue() * TILE,
                                   levelData->valueForKey("starty")->floatValue() * TILE));
    _player->setVisible(true);
    _player->reset();
    
    //reset igloo to level end position
    _igloo->initIgloo(_gravity, ccp(levelData->valueForKey("endx")->floatValue() * TILE,
                                    levelData->valueForKey("endy")->floatValue() * TILE));
    //reset smoke particle to match igloo position
    _smoke->setPosition(ccp(_igloo->getPositionX() + TILE,
                            _igloo->getPositionY() + TILE * 0.5f));
    //hide smoke. only shown when level is completed
    _smoke->setVisible(false);
    _smoke->stopSystem();
    
    //if _currentLevel == 0 this is tutorial level
    if (_currentLevel != 0) {
        char szValue[10] = {0};
        sprintf(szValue, "level %i", (int) _currentLevel);
        _messages->setString(szValue);
        
    } else {
        _messages->setString("tutorial");
        _tutorialStep = 0;
        _tutorialCounter = 0.0;
    }
    _tutorialLabel->setVisible(false);
    
    _messages->setVisible(true);
    _btnStart->setVisible(true);
    _btnMenu->setVisible(false);
    _btnAgain->setVisible(false);
    
    _acceleration = ccp(0,0);
}
/*
void GameLayer::draw() {
    
    
    CCLayer::draw();
    
    ccGLEnableVertexAttribs( kCCVertexAttribFlag_Position );
    
    kmGLPushMatrix();
    
    _world->DrawDebugData();
    
    kmGLPopMatrix();
}
*/

void GameLayer::update(float dt) {
    
    
    if (!_running) return;
    
    _world->Step(dt, 10, 10);
    _world->ClearForces();
    
    _player->update();
    
    //if player is touching something, update it with accelerometer data
    if (_player->getBody()->GetContactList()) {
        b2Vec2 vec;
        
        switch (_gravity) {
        	//update X axis
            case kDirectionUp:
            case kDirectionDown:
            //if player is a circle, slow it down.
                if (_player->getState() == kStateCircle) {
                    _player->getBody()->ApplyForce(b2Vec2(_acceleration.x * CIRCLE_MULTIPLIER, 0),
                                                   _player->getBody()->GetWorldCenter());
                    
                    vec = _player->getBody()->GetLinearVelocity();
                    vec.x *= BALL_FRICTION;
                    _player->getBody()->SetLinearVelocity(vec);
            //if player is a box don't apply any friction    
                } else {
                    _player->getBody()->ApplyLinearImpulse(b2Vec2(_acceleration.x * BOX_MULTIPLIER, 0),
                                                           _player->getBody()->GetWorldCenter());
                }
                break;
                
            //update Y axis
            case kDirectionLeft:
            case kDirectionRight:
            //if player is a circle, slow it down.
                if (_player->getState() == kStateCircle) {
                    _player->getBody()->ApplyForce(b2Vec2(0, _acceleration.y * CIRCLE_MULTIPLIER),
                                                   _player->getBody()->GetWorldCenter());
                    
                    vec = _player->getBody()->GetLinearVelocity();
                    vec.y *= BALL_FRICTION;
                    _player->getBody()->SetLinearVelocity(vec);
            //if player is a box don't apply any friction  
                } else {
                    _player->getBody()->ApplyLinearImpulse(b2Vec2(0, _acceleration.y * BOX_MULTIPLIER),
                                                           _player->getBody()->GetWorldCenter());
                }
                
                break;
        }
    }
    
    //track collision with GSwitches
    int count = _gSwitchPool->count();
    GSwitch * gSwitch;
    float diffx;
    float diffy;
    
    for (int i = 0; i < count; i++) {
        gSwitch = (GSwitch *) _gSwitchPool->objectAtIndex(i);
        if (!gSwitch->isVisible()) continue;
        diffx = gSwitch->getPositionX() - _player->getPositionX();
        diffy = gSwitch->getPositionY() - _player->getPositionY();
        if (pow(diffx, 2) + pow (diffy, 2) < PLAYER_SWITCH_RADII) {
            SimpleAudioEngine::sharedEngine()->playEffect("switch.wav");
            //collision!
            _gravity = gSwitch->getDirection();
            gSwitch->setVisible(false);
            //notify of collision
            CCNotificationCenter::sharedNotificationCenter()->postNotification(NOTIFICATION_GRAVITY_SWITCH, (CCObject*)_gravity);

			//change world gravity
            switch (_gravity) {
                case kDirectionUp:
                    _world->SetGravity(b2Vec2(0, GRAVITY));
                    break;
                case kDirectionDown:
                    _world->SetGravity(b2Vec2(0, -GRAVITY));
                    break;
                case kDirectionLeft:
                    _world->SetGravity(b2Vec2(-GRAVITY, 0 ));
                    break;
                case kDirectionRight:
                    _world->SetGravity(b2Vec2(GRAVITY, 0 ));
                    break;
            }
            break;
        }
    }
    
    
    //check for level complete (collision with Igloo)
    diffx = _player->getPositionX() - _igloo->getPositionX();
    diffy = _player->getPositionY() - _igloo->getPositionY();
    if (pow(diffx, 2) + pow(diffy, 2) < IGLOO_SQ_RADIUS) {
        SimpleAudioEngine::sharedEngine()->playEffect("igloo.wav");
        
        _player->setVisible(false);
        _btnPause->setVisible(false);
        _btnReset->setVisible(false);
        
        //notify of Level Completed. Igloo will listen to this and change its texture.
        CCNotificationCenter::sharedNotificationCenter()->postNotification(NOTIFICATION_LEVEL_COMPLETED, NULL);
        
        //run smoke particle
        _smoke->setVisible(true);
        _smoke->resetSystem();
        _messages->setString("well done!");
        _messages->setVisible(true);
        
        //create delay until new level is loaded
        CCFiniteTimeAction*  sequence = CCSequence::create(
                                                           CCDelayTime::create(2.5f),
                                                           CCCallFunc::create(this, callfunc_selector(GameLayer::newLevel)),
                                                           NULL);
        this->runAction(sequence);
        _tutorialLabel->setVisible(false);
        _running = false;
    }
    
    //check for game over. player is off screen
    if (_player->getPositionY() > _screenSize.height || _player->getPositionY() < 0
        || _player->getPositionX() > _screenSize.width || _player->getPositionX() < 0) {
        SimpleAudioEngine::sharedEngine()->playEffect("oops.wav");
        _running = false;
        _player->setVisible(false);
        _btnPause->setVisible(false);
        _btnReset->setVisible(false);
        
        _messages->setString("oops!");
        _messages->setVisible(true);
        _btnAgain->setVisible(true);
        _btnMenu->setVisible(true);
    }
    
    //update text in tutorial level (currentLevel == 0)
    if (_currentLevel == 0) {
        _tutorialCounter += dt;
        if (_tutorialCounter > 15 && _tutorialStep == 1) {
            _tutorialStep = 2;
            _tutorialLabel->setString(TUTORIAL_3);
        } else if (_tutorialCounter > 5 && _tutorialStep == 0) {
            _tutorialStep = 1;
            _tutorialLabel->setString(TUTORIAL_2);
        }
    }
    
}


void GameLayer::newLevel() {
    
    if (_currentLevel + 1 >= _levels->count()) return;
    
    loadLevel(_currentLevel + 1);
    
    //update user data
    if (_currentLevel > _levelsCompleted) {
        _levelsCompleted = _currentLevel;
        CCUserDefault::sharedUserDefault()->setIntegerForKey("levelsCompleted", _levelsCompleted);
        CCUserDefault::sharedUserDefault()->flush();
    }
}

void GameLayer::ccTouchesBegan(CCSet* touches, CCEvent* event) {
    
    
    
    CCTouch *touch = (CCTouch *)touches->anyObject();
    
    if (touch) {
        
	    CCPoint tap = touch->getLocation();
        CCRect boundary;
        //handle button touches
        CCSprite * button;
        CCSprite * buttonPress;
        
        for (int i = 0; i < 5; i++) {
            button = (CCSprite *) _buttons->objectAtIndex(i);
            if (!button->isVisible()) continue;
            boundary = button->boundingBox();
            
            if (boundary.containsPoint(tap)) {
                buttonPress = (CCSprite *) button->getChildByTag(kSpriteBtnOn);
                buttonPress->setVisible(true);
                return;
            }
        }
    }
    
}

void GameLayer::ccTouchesEnded(CCSet* touches, CCEvent* event) {
    
    CCTouch *touch = (CCTouch *)touches->anyObject();
    
    if (touch) {
        
	    CCPoint tap = touch->getLocation();
        CCRect boundary;
        //handle button touches
        CCSprite * button;
        CCSprite * buttonPress;
        for (int i = 0; i < 5; i++) {
            button = (CCSprite *) _buttons->objectAtIndex(i);
            if (!button->isVisible()) continue;
            boundary = button->boundingBox();
            if (boundary.containsPoint(tap)) {
                buttonPress = (CCSprite *) button->getChildByTag(kSpriteBtnOn);
                buttonPress->setVisible(false);
                switch (button->getTag()) {
                    case kSpriteBtnAgain:
                        _running = false;
                        _btnAgain->setVisible(false);
                        _btnMenu->setVisible(false);
                        resetLevel();
                        SimpleAudioEngine::sharedEngine()->playEffect("button.wav");
                        break;
                    case kSpriteBtnReset:
                        _running = false;
                        resetLevel();
                        break;
                    case kSpriteBtnPause:
                        if (_running) {
                            _messages->setString("paused");
                            _messages->setVisible(true);
                        } else {
                            _messages->setVisible(false);
                        }
                        _running = !_running;
                        break;
                    case kSpriteBtnStart:
                        SimpleAudioEngine::sharedEngine()->playEffect("button.wav");
                        _btnPause->setVisible(true);
                        _btnReset->setVisible(true);
                        _btnStart->setVisible(false);
                        _messages->setVisible(false);
                        _running = true;
                        if (_currentLevel == 0) {
                            _tutorialLabel->setVisible(true);
                        }
                        break;
                    case kSpriteBtnMenu:
                        SimpleAudioEngine::sharedEngine()->playEffect("button.wav");
                        SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();
                        unscheduleUpdate();
                        CCScene* newScene = CCTransitionMoveInL::create(0.2f, MenuLayer::scene());
                        CCDirector::sharedDirector()->replaceScene(newScene);
                }
                return;
            }
        }
        
        if (!_running) return;
        _player->setSwitchShape(true);
        
    }
}

void GameLayer::didAccelerate(CCAcceleration* pAccelerationValue) {
    _acceleration = ccp(pAccelerationValue->x * ACCELEROMETER_MULTIPLIER,
                        pAccelerationValue->y * ACCELEROMETER_MULTIPLIER);
}

CCScene* GameLayer::scene(int level, int levelsCompleted)
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // add layer as a child to scene
    scene->addChild(GameLayer::create(level, levelsCompleted));
    
    return scene;
}

void GameLayer::createPools() {
    _gSwitchPool = CCArray::createWithCapacity(30);
    _gSwitchPool->retain();
    GSwitch * sprite;
    for (int i = 0; i < 30; i++) {
        sprite = GSwitch::create();
        sprite->setVisible(false);
        _gSwitchPool->addObject(sprite);
        _gameBatchNode->addChild(sprite);
    }
    
    _platformPool = CCArray::createWithCapacity(50);
    _platformPool->retain();
    Platform * platform;
    for (int  i = 0; i < 50; i++) {
        platform = Platform::create(this);
        platform->setVisible(false);
        _gameBatchNode->addChild(platform);
        _platformPool->addObject(platform);
    }
    _platformPoolIndex = 0;
    
}

void GameLayer::createScreen() {
    //if we load this scene first, load sprite frames
    //CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("sprite_sheet.plist");
    
    CCSprite * bg = CCSprite::create("bg.jpg");
    bg->setPosition(ccp(_screenSize.width * 0.5f, _screenSize.height * 0.5f));
    this->addChild(bg);
    
    _gameBatchNode = CCSpriteBatchNode::create("sprite_sheet.png", 200);
    this->addChild(_gameBatchNode, kMiddleground);
    
    _igloo = Igloo::create();
    _igloo->setVisible(false);
    _gameBatchNode->addChild(_igloo, kForeground);
    
    //create buttons
    _buttons = CCArray::createWithCapacity(5);
    _buttons->retain();
    CCSprite * buttonOn;
    
    _btnStart = CCSprite::createWithSpriteFrameName("btn_start_off.png");
    _btnStart->setPosition(ccp(_screenSize.width * 0.5f, _screenSize.height * 0.45f));
    _btnStart->setVisible(false);
    _gameBatchNode->addChild(_btnStart, kForeground, kSpriteBtnStart);
    buttonOn = CCSprite::createWithSpriteFrameName("btn_start_on.png");
    buttonOn->setAnchorPoint(ccp(0,0));
    buttonOn->setVisible(false);
    _btnStart->addChild(buttonOn, kForeground, kSpriteBtnOn);
    _buttons->addObject(_btnStart);
    
    _btnReset = CCSprite::createWithSpriteFrameName("btn_reset_off.png");
    _btnReset->setAnchorPoint(ccp(0.5, 1));
    _btnReset->setPosition(ccp(_screenSize.width * 0.9f, _screenSize.height));
    _btnReset->setVisible(false);
    _gameBatchNode->addChild(_btnReset, kForeground, kSpriteBtnReset);
    buttonOn = CCSprite::createWithSpriteFrameName("btn_reset_on.png");
    buttonOn->setAnchorPoint(ccp(0,0));
    buttonOn->setVisible(false);
    _btnReset->addChild(buttonOn, kForeground, kSpriteBtnOn);
    _buttons->addObject(_btnReset);
    
    _btnPause = CCSprite::createWithSpriteFrameName("btn_pause_off.png");
    _btnPause->setAnchorPoint(ccp(0.5, 1));
    _btnPause->setPosition(ccp(_screenSize.width * 0.1f, _screenSize.height));
    _btnPause->setVisible(false);
    _gameBatchNode->addChild(_btnPause, kForeground, kSpriteBtnPause);
    buttonOn = CCSprite::createWithSpriteFrameName("btn_pause_on.png");
    buttonOn->setAnchorPoint(ccp(0,0));
    buttonOn->setVisible(false);
    _btnPause->addChild(buttonOn, kForeground, kSpriteBtnOn);
    _buttons->addObject(_btnPause);
    
    _btnAgain = CCSprite::createWithSpriteFrameName("btn_again_off.png");
    _btnAgain->setPosition(ccp(_screenSize.width * 0.5f, _screenSize.height * 0.45f));
    _btnAgain->setVisible(false);
    _gameBatchNode->addChild(_btnAgain, kForeground, kSpriteBtnAgain);
    buttonOn = CCSprite::createWithSpriteFrameName("btn_again_on.png");
    buttonOn->setAnchorPoint(ccp(0,0));
    buttonOn->setVisible(false);
    _btnAgain->addChild(buttonOn, kForeground, kSpriteBtnOn);
    _buttons->addObject(_btnAgain);
    
    _btnMenu = CCSprite::createWithSpriteFrameName("btn_menu_off.png");
    _btnMenu->setPosition(ccp(_screenSize.width * 0.5f, _screenSize.height * 0.3f));
    _btnMenu->setVisible(false);
    _gameBatchNode->addChild(_btnMenu, kForeground, kSpriteBtnMenu);
    buttonOn = CCSprite::createWithSpriteFrameName("btn_menu_on.png");
    buttonOn->setAnchorPoint(ccp(0,0));
    buttonOn->setVisible(false);
    _btnMenu->addChild(buttonOn, kForeground, kSpriteBtnOn);
    _buttons->addObject(_btnMenu);
    
    _messages = CCLabelBMFont::create("level", "font_messages.fnt", _screenSize.width);
    _messages->setVisible(false);
    _messages->setPosition(ccp(_screenSize.width * 0.5f, _screenSize.height * 0.6f));
    this->addChild(_messages, kForeground);
    
    _smoke = CCParticleSystemQuad::create("smoke.plist");
    _smoke->setVisible(false);
    _smoke->stopSystem();
    this->addChild(_smoke, kForeground);
    
    
    _tutorialLabel = CCLabelTTF::create(TUTORIAL_1, "Verdana", 25, CCSize(_screenSize.width * 0.7f, _screenSize.height * 0.4f), kCCTextAlignmentCenter);
    _tutorialLabel->setPosition(ccp (_screenSize.width * 0.5f, _screenSize.height * 0.2f) );
    this->addChild(_tutorialLabel, kForeground);
    _tutorialLabel->setVisible(false);
}

//hide elements from previous level
void GameLayer::clearLayer() {
    
    int count = _platformPool->count();
    b2Sprite * sprite;
    for (int i = 0; i < count; i++) {
        sprite = (b2Sprite *) _platformPool->objectAtIndex(i);
        sprite->getBody()->SetActive(false);
        sprite->setVisible(false);
    }
    
    count = _gSwitchPool->count();
    for (int i = 0; i < count; i++) {
        sprite = (b2Sprite *) _gSwitchPool->objectAtIndex(i);
        sprite->setVisible(false);
    }
}