#include "HelloWorldScene.h"
#include "GameOver.h"

CCScene* HelloWorld::createScene()
{
    auto scene = CCScene::create();
    
    auto layer = HelloWorld::create();

    scene->addChild(layer);

    return scene;
}

bool HelloWorld::init()
{
    if ( !CCLayer::init() )
    {
        return false;
    }
    
    /////////////////////////////
	CCDirector* pDirector = CCDirector::sharedDirector();
	pDirector->getTouchDispatcher()->addStandardDelegate(this, 0);

	//this->setTouchEnabled(true);

    srand((int)time(NULL));

    BRICKS_HEIGHT = 4;
    BRICKS_WIDTH = 5;

	texture = CCTextureCache::sharedTextureCache()->addImage("Images/white-512x512.png");

    // 벽돌 초기화
    this->initializeBricks();

    // 공 초기화
    this->initializeBall();

    // 패들 초기화
    this->initializePaddle();

    // 2초후 게임 시작
    this->scheduleOnce(schedule_selector(HelloWorld::startGame), 2.0f);

	

    return true;
}

HelloWorld::~HelloWorld()
{
//    targets->release();
}

void HelloWorld::initializeBricks()
{
    int count = 0;

    for (int y = 0; y < BRICKS_HEIGHT; y++) 
    {
        for (int x = 0; x < BRICKS_WIDTH; x++) 
        {
            CCSprite* bricks = CCSprite::createWithTexture(texture, CCRect(0, 0, 64, 40));

            // 색 지정
			switch (count++ % 4) {
			case 0:
			{
					  ccColor3B color = { 255, 255, 255 };
					  bricks->setColor(color);
			}
				break;
			case 1:
			{
					  ccColor3B color = { 255, 0, 0 };
					  bricks->setColor(color);
					  break;
			}
			case 2:
				bricks->setColor(ccYELLOW);
				break;
			case 3:
			{
				ccColor3B color = { 75, 255, 0 };
				bricks->setColor(color);
			}
                break;

            default:
                break;
            }

            // 좌표 지정
            bricks->setPosition(CCPoint(x * 64 + 32, (y * 40) + 280));

            // 화면에 추가
            this->addChild(bricks);

            // 벡터에 추가
            targets.push_back(bricks);
        }
    }
}

void HelloWorld::initializeBall()
{
    ball = CCSprite::createWithTexture(texture, CCRect(0, 0, 16, 16));

	ccColor3B color = { 0, 255, 255 };
	ball->setColor(color);
    ball->setPosition(CCPoint(160, 240));
    this->addChild(ball);
}

void HelloWorld::initializePaddle()
{
    paddle = CCSprite::createWithTexture(texture, CCRect(0, 0, 80, 10));
	ccColor3B color = { 255, 255, 255 };
	paddle->setColor(color);
    paddle->setPosition(CCPoint(160, 50));
    this->addChild(paddle);
}

void HelloWorld::startGame(float dt)
{
    ball->setPosition(CCPoint(160, 240));

    ballMovement = CCPoint(4, 4);
    if (rand() % 100 < 50) {
        ballMovement.x = -ballMovement.x;
    }
    ballMovement.y = -ballMovement.y;

    isPlaying = true;

    this->schedule(schedule_selector(HelloWorld::gameLogic), 2.0f / 60.0f);

}

void HelloWorld::gameLogic(float dt)
{
    // ballMovement.y가 음수이면 볼이 내려오고 있는 것.
    // ballMovement.y가 양수이면 볼이 올라가고 있는 것.
    //log("tick..%f",ballMovement.y);


    // 볼의 현재위치
    ball->setPosition(CCPoint(ball->getPosition().x + ballMovement.x,
                            ball->getPosition().y + ballMovement.y));

    // 볼과 패들 충돌여부
    bool paddleCollision =
        ball->getPosition().y <= paddle->getPosition().y + 13 &&
        ball->getPosition().x >= paddle->getPosition().x - 48 &&
        ball->getPosition().x <= paddle->getPosition().x + 48;

    // 패들과 충돌시 처리
    if (paddleCollision) 
    {
        if (ball->getPosition().y <= paddle->getPosition().y + 13 && ballMovement.y < 0) 
        {
            ball->setPosition(CCPoint(ball->getPosition().x, paddle->getPosition().y + 13));
        }

        // 내려오던거 위로 올라가게 공의 상하 진행방향 바꾸기
        ballMovement.y = -ballMovement.y;
    }

    // 블록과 충돌 파악
    bool there_are_solid_bricks = false;

    for (auto &item : targets)
    {
        CCSprite* brick = item;

        if (255 == brick->getOpacity())
        {
            there_are_solid_bricks = true;

            CCRect rectA = ball->boundingBox();
            CCRect rectB = brick->boundingBox();
            if (rectA.intersectsRect(rectB)) {
                // 블록과 충돌 처리
                this->processCollision(brick);
            }
        }
    }

    // 블록이 없을 때 - 게임 종료 상태
    if (!there_are_solid_bricks) {
        isPlaying = false;
        ball->setOpacity(0);

        // 모든 스케쥴 제거
        this->unscheduleAllSelectors();

        CCLog("You win !!!");

        // 게임에 이겼다. 새로운 게임 대기 화면...
        CCScene* pScene = GameOver::createScene();
        CCDirector::sharedDirector()->replaceScene(CCTransitionProgressRadialCCW::create(1, pScene));

    }

    // 벽면 충돌 체크
    if (ball->getPosition().x > 312 || ball->getPosition().x < 8)
        ballMovement.x = -ballMovement.x;

    if (ball->getPosition().y > 450)
        ballMovement.y = -ballMovement.y;

    //if (ball.position.y < 10) {
    //    ballMovement.y = -ballMovement.y;
    //}

    // 페달을 빠져 나갈 때
    if (ball->getPosition().y < (50 + 5 + 8)) {
        isPlaying = false;
        ball->setOpacity(0);

        // 모든 스케쥴 제거
        this->unscheduleAllSelectors();

        CCLog("You lose..");

        // 게임에 졌다. 새로운 게임 대기 화면...
        CCScene* pScene = GameOver::createScene();
        CCDirector::sharedDirector()->replaceScene(CCTransitionProgressRadialCCW::create(1, pScene));

    }

}

void HelloWorld::processCollision(CCSprite *brick)
{
    CCPoint brickPos = brick->getPosition();
    CCPoint ballPos = ball->getPosition();

    if (ballMovement.x > 0 && brick->getPosition().x < ball->getPosition().x) 
    {
        ballMovement.x = -ballMovement.x;
    }
    else if (ballMovement.x < 0 && brick->getPosition().x < ball->getPosition().x) 
    {
        ballMovement.x = -ballMovement.x;
    }

    if (ballMovement.y > 0 && brick->getPosition().y > ball->getPosition().y) 
    {
        ballMovement.y = -ballMovement.y;
    }
    else if (ballMovement.y < 0 && brick->getPosition().y < ball->getPosition().y) 
    {
        ballMovement.y = -ballMovement.y;
    }

    brick->setOpacity(0);
}

void HelloWorld::onEnter()
{
    CCLayer::onEnter();

    // 싱글터치모드로 터치리스너 등록
   /* auto listener = EventListenerTouchOneByOne::create();
    // Swallow touches only available in OneByOne mode.
    // 핸들링된 터치 이벤트를 터치 이벤트 array에서 지우겠다는 의미이다.
    listener->setSwallowTouches(true);

    listener->onTouchBegan = CC_CALLBACK_2(HelloWorld::onTouchBegan, this);
    listener->onTouchMoved = CC_CALLBACK_2(HelloWorld::onTouchMoved, this);

    // The priority of the touch listener is based on the draw order of sprite
    // 터치리스너의 우선순위를 (노드가) 화면에 그려진 순서대로 한다.
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);*/
}

void HelloWorld::onExit()
{
//    _eventDispatcher->removeEventListeners(EventListener::Type::TOUCH_ONE_BY_ONE);

    CCLayer::onExit();
}

bool HelloWorld::ccTouchBegan(CCTouch* touch, CCEvent* event)
{
    if (!isPlaying) {
        return true;
    }

    // 하나의 터치이벤트만 가져온다.
    auto touchPoint = touch->getLocation();

    // 패들을 터치했는지 체크한다.
    CCRect rect = paddle->boundingBox();
    if (rect.containsPoint(touchPoint)) {
        // 패들이 터치되었음을 체크.
        isPaddleTouched = true;
    }
    else {
        isPaddleTouched = false;
    }

//    // 패들 sprite의 사이즈의 반을 계산합니다.
//    float halfWidth = paddle->getContentSize().width / 2.0;
//    float halfHeight = paddle->getContentSize().height / 2.0;
//    
//    // 터치된 위치가 패들 안에 들어오는 지 계산합니다.
//    if (touchPoint.x > (paddle->getPosition().x + halfWidth) ||
//        touchPoint.x < (paddle->getPosition().x - halfWidth) ||
//        touchPoint.y < (paddle->getPosition().y - halfHeight) ||
//        touchPoint.y > (paddle->getPosition().y + halfHeight) )
//    {
//        isPaddleTouched = false;
//    } else {
//        // 패들이 터치되었음을 체크.
//        isPaddleTouched = true;
//    }

    return true;
}

void HelloWorld::ccTouchMoved(CCTouch* touch, CCEvent* event)
{
    if (isPaddleTouched) 
    {
        auto touchPoint = touch->getLocation();

        // 패들이 좌우로만 움직이게 y값은 바꾸지 않는다.
        // 또한 패들이 화면 바깥으로 나가지 않도록 한다.
        if (touchPoint.x < 40) {
            touchPoint.x = 40;
        }
        if (touchPoint.x > 280) {
            touchPoint.x = 280;
        }

        CCPoint newLocation = CCPoint(touchPoint.x, paddle->getPosition().y);
        paddle->setPosition(newLocation);
    }
}

void HelloWorld::ccTouchesBegan(cocos2d::CCSet* touches, cocos2d::CCEvent* event)
{
	if (!isPlaying) {
		return;
	}

	CCSetIterator i;
	for (i = touches->begin(); i != touches->end(); i++)
	{
		CCTouch *t = (CCTouch*)*i;
		// 하나의 터치이벤트만 가져온다.
		auto touchPoint = t->getLocation();

		// 패들을 터치했는지 체크한다.
		CCRect rect = paddle->boundingBox();
		if (rect.containsPoint(touchPoint)) {
			// 패들이 터치되었음을 체크.
			isPaddleTouched = true;
		}
		else {
			isPaddleTouched = false;
		}

		return;

	}

}
void HelloWorld::ccTouchesMoved(CCSet *touches, CCEvent *event)
{
	if (isPaddleTouched)
	{
		CCSetIterator i;
		for (i = touches->begin(); i != touches->end(); i++)
		{
			CCTouch *t = (CCTouch*)*i;
			auto touchPoint = t->getLocation();

			// 패들이 좌우로만 움직이게 y값은 바꾸지 않는다.
			// 또한 패들이 화면 바깥으로 나가지 않도록 한다.
			if (touchPoint.x < 40) {
				touchPoint.x = 40;
			}
			if (touchPoint.x > 280) {
				touchPoint.x = 280;
			}

			CCPoint newLocation = CCPoint(touchPoint.x, paddle->getPosition().y);
			paddle->setPosition(newLocation);

			return;
		}
	}
}
void HelloWorld::ccTouchesEnded(CCSet *touches, CCEvent *event)
{        
	               
}