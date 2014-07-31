#include "HelloWorldScene.h"
#include "tilemap_parallax_nodes/CCTMXTiledMap.h"

CCScene* HelloWorld::scene()
{
    CCScene *scene = CCScene::create();
    
    HelloWorld *layer = HelloWorld::create();

    scene->addChild(layer);

    return scene;
}

bool HelloWorld::init()
{
    if ( !CCLayerColor::initWithColor(ccc4(255, 255, 255, 255)) )
    {
        return false;
    }

    /////////////////////////////

    // 화면 사이즈 구하기
    winSize = CCDirector::sharedDirector()->getWinSize();
    
    // 타일맵 읽어 오기
    tmap = CCTMXTiledMap::create("TileMaps/TestDesert.tmx");
    background = tmap->layerNamed("Background");
    items      = tmap->layerNamed("Items");
    metainfo   = tmap->layerNamed("MetaInfo");
    metainfo->setVisible(false);
    this->addChild(tmap, 0, 11);   // z-order를 -1로 하면 화면이 나오지 않는다.
    
   
    CCTMXObjectGroup *objects = tmap->objectGroupNamed("Objects");
  
	CCDictionary* spawnPoint = objects->objectNamed("SpawnPoint");
    
    int x = ((CCString*)spawnPoint->objectForKey("x"))->intValue();
    int y = ((CCString*)spawnPoint->objectForKey("y"))->intValue();
    
    
    // SpawnPoint의 위치로 생성할 드래곤의 위치 지정하기
    dragonPosition = ccp(x, y);
    
    
    // 드래곤을 만든다.
    this->createDragon();
    

    
    return true;
}

void HelloWorld::onEnter()
{
	CCLayer::onEnter();
    
    CCDirector* pDirector = CCDirector::sharedDirector();
    pDirector->getTouchDispatcher()->addTargetedDelegate(this, 0, true);
}

void HelloWorld::onExit()
{
    CCDirector* pDirector = CCDirector::sharedDirector();
    pDirector->getTouchDispatcher()->removeDelegate(this);
    
    CCLayer::onExit();
}

void HelloWorld::createDragon()
{
    // 움직이는 드래곤 넣기 시작
    CCTexture2D *texture = CCTextureCache::sharedTextureCache()->addImage("Images/dragon_animation.png");
    
    CCAnimation *animation = CCAnimation::create();
    animation->setDelayPerUnit(0.1);
    
    for(int i=0;i<6;i++){
        // 첫째 줄에 4개의 프레임이 있고, 5번째 부터는 두 번째 줄에 있으므로,
        // 5번째(idx==4)부터는 y 좌표의 값을 증가시켜야 한다.
        int index    = i % 4;
        int rowIndex = i / 4;
        
        animation->addSpriteFrameWithTexture(texture, CCRectMake(index*130, rowIndex*140+70, 120, 70));
    }

    dragon = CCSprite::createWithTexture(texture, CCRectMake(0, 70, 120, 70));
    dragon->setFlipX(true);
    
    dragon->setScale(0.4);
    
    dragon->setPosition( dragonPosition );
    this->addChild(dragon);

    CCAnimate *animate = CCAnimate::create(animation);
    CCAction* rep = CCRepeatForever::create(animate);
    dragon->runAction(rep);

}


bool HelloWorld::ccTouchBegan(CCTouch *pTouch, CCEvent* event)
{
    return true;
}

void HelloWorld::ccTouchEnded(CCTouch *pTouch, CCEvent* event)
{
    CCPoint touchLocation = pTouch->getLocation();
    touchLocation = this->convertToNodeSpace(touchLocation);
    
    CCPoint playerPos = dragon->getPosition();

    CCPoint diff = ccpSub(touchLocation, playerPos);
    
    if (abs(diff.x) > abs(diff.y)) {
        if (diff.x > 0) {
            playerPos.x += tmap->getTileSize().width;
            
            // 드래곤의 방향을 바꾸어준다.
            dragon->setFlipX(true);
        } else {
            playerPos.x -= tmap->getTileSize().width;
            
            // 드래곤의 방향을 바꾸어준다.
            dragon->setFlipX(false);
        }
    } else {
        if (diff.y > 0) {
            playerPos.y += tmap->getTileSize().height;
        } else {
            playerPos.y -= tmap->getTileSize().height;
        }
    }
    
    if (playerPos.x <= (tmap->getMapSize().width * tmap->getTileSize().width) &&
        playerPos.y <= (tmap->getMapSize().height * tmap->getTileSize().height) &&
        playerPos.y >= 0 &&
        playerPos.x >= 0 )
    {
        // 드래곤의 새로운 위치 지정
        this->setPlayerPosition(playerPos);
    }
    
    // 드래곤의 위치에 맞춰 화면 위치 조정
    this->setViewpointCenter(dragon->getPosition());
    
}

// 현재 탭으로 선택된 타일의 위치를 가져온다..
CCPoint HelloWorld::tileCoordForPosition(CCPoint position)
{
    int x = position.x / tmap->getTileSize().width;
    int y = ((tmap->getMapSize().height * tmap->getTileSize().height) - position.y) / tmap->getTileSize().height;
    return ccp(x, y);
}

void HelloWorld::setPlayerPosition(CCPoint position)
{
  
    CCPoint tileCoord = this->tileCoordForPosition(position);
  
    int tileGid = metainfo->tileGIDAt(tileCoord);
    
	if (tileGid != 0) {
        
  
		CCDictionary* properties = tmap->propertiesForGID(tileGid);
        
        if (properties) {
            
            CCString *wall = (CCString*)properties->objectForKey("Wall");

            if ( wall && (wall->compare("YES") == 0) ) {
                return;
            }
            
        
            CCString *item1 = (CCString*)properties->objectForKey("Items");
            if ( item1 && (item1->compare("YES") == 0) ) {
                
                this->metainfo->removeTileAt(tileCoord);
                items->removeTileAt(tileCoord);
                
                // 먹은 수만큼 점수를 올려주는 코딩이 추가적으로 필요하다.
                CCLog("아이템 획득!!! 이 메시지가 여러번 출력되는지 확인");
            }
  
        }
    }
    
    dragon->setPosition( position );
}

void HelloWorld::setViewpointCenter(CCPoint position)
{
    // 파라미터로 들어오는 위치에 맞춰 화면을 움직인다.
    
    int x = MAX(position.x, winSize.width / 2);
    int y = MAX(position.y, winSize.height / 2);
    
    x = MIN(x, (tmap->getMapSize().width * tmap->getTileSize().width) - winSize.width / 2);
    
    y = MIN(y, (tmap->getMapSize().height * tmap->getTileSize().height) - winSize.height/2);
    
    CCPoint actualPosition = ccp(x, y);
    CCPoint centerOfView = ccp(winSize.width/2, winSize.height/2);
    CCPoint viewPoint = ccpSub(centerOfView, actualPosition);
    
    this->setPosition(viewPoint);
}






