#define TOTAL_PUFFS 3


#include "Block.h"

Block::~Block () {
    
    CC_SAFE_RELEASE(_chimneys);
    CC_SAFE_RELEASE(_wallTiles);
    CC_SAFE_RELEASE(_roofTiles);
    
    CC_SAFE_RELEASE(_puffAnimation);
    CC_SAFE_RELEASE(_puffSpawn);
    CC_SAFE_RELEASE(_puffMove);
    CC_SAFE_RELEASE(_puffFade);
    CC_SAFE_RELEASE(_puffScale);
    
    
}

Block::Block () {
    
    //get screen size
	_screenSize = CCDirector::sharedDirector()->getWinSize();
    _tileWidth = _screenSize.width / TILE_W_SIZE;
    _tileHeight = _screenSize.height / TILE_H_SIZE;
    _puffing = false;
    this->setVisible(false);
}

Block * Block::create () {
    
    Block * block = new Block();
	if (block && block->initWithSpriteFrameName("blank.png")) {
		block->autorelease();
        block->initBlock();
		return block;
	}
	CC_SAFE_DELETE(block);
	return NULL;
}

void Block::setPuffing (bool value) {
    
    _puffing = value;
    
    if (value) {
        this->runAction((CCAction *) _puffSpawn->copy()->autorelease());
        CCAction * hide = CCSequence::create(CCDelayTime::create(2.5f),
                                             CCCallFuncN::create( this, callfuncN_selector(Block::hidePuffs) ),
                                             NULL);
        this->runAction(hide);
    } else {
        //reset all puffs
        _puffIndex = 0;
        int count = _chimneys->count();
        CCSprite * chimney;
        CCSprite * puff;
        
        for (int i = 0; i < count; i++) {
            chimney = (CCSprite * ) _chimneys->objectAtIndex(i);
            for (int j = 0; j < TOTAL_PUFFS; j++) {
                puff = (CCSprite *) chimney->getChildByTag(j);
                puff->setVisible(false);
                puff->stopAllActions();
                puff->setScale(1.0);
                puff->setOpacity(255);
                puff->setPosition(ccp(0,0));
            }
        }

    }
    
}

void Block::hidePuffs(CCNode * node) {
    setPuffing(false);
}


void Block::setupBlock (int width, int height, int type) {
    
    this->setPuffing(false);
    
    _type = type;
    
    _width = width * _tileWidth;
    _height = height * _tileHeight + _tileHeight * 0.49f;
    this->setPositionY(_height);
    
    CCSpriteFrame * wallFrame;
    CCSpriteFrame * roofFrame = rand() % 10 > 6 ? _roof1 : _roof2;
    
    
    int num_chimneys;
    float chimneyX[] = {0,0,0,0,0};
    
    switch (type) {
        
        case kBlockGap:
            this->setVisible(false);
            return;
            
        case kBlock1:
            wallFrame = _tile1;
            chimneyX[0] = 0.2f;
            chimneyX[1] = 0.8f;
            num_chimneys = 2;
            break;
        case kBlock2:
            wallFrame = _tile2;
            chimneyX[0] = 0.2f;
            chimneyX[1] = 0.8f;
            chimneyX[2] = 0.5f;
            num_chimneys = 3;
            break;
        case kBlock3:
            wallFrame = _tile3;
            chimneyX[0] = 0.2f;
            chimneyX[1] = 0.8f;
            chimneyX[2] = 0.5f;
            num_chimneys = 3;
            
            break;
        case kBlock4:
            wallFrame = _tile4;
            chimneyX[0] = 0.2f;
            chimneyX[1] = 0.5f;
            num_chimneys = 2;
            break;
    }
    
    int i;
    GameSprite * chimney;
    int count;
    count = _chimneys->count();
    for (i = 0; i < count; i++) {
        chimney = (GameSprite *) _chimneys->objectAtIndex(i);
        if (i < num_chimneys) {
            chimney->setPosition( ccp (chimneyX[i] * _width, 0) );
            chimney->setVisible(true);
            
        } else {
            chimney->setVisible(false);
        }
    }
    
    this->setVisible(true);
    
    CCSprite * tile;
   
    count = _roofTiles->count();
    for (i  = 0; i < count; i++) {
        tile = (CCSprite *) _roofTiles->objectAtIndex(i);
        if (tile->getPositionX() < _width) {
            tile->setVisible(true);
            tile->setDisplayFrame(roofFrame);
        } else {
            tile->setVisible(false);
        }
    }
    
    count = _wallTiles->count();
    for (i  = 0; i < count; i++) {
        tile = (CCSprite *) _wallTiles->objectAtIndex(i);
        if (tile->getPositionX() < _width && tile->getPositionY() > -_height) {
            tile->setVisible(true);
            tile->setDisplayFrame(wallFrame);
        } else {
            tile->setVisible(false);
        }
    }
}

void Block::initBlock() {

    
    _tile1 = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName ("building_1.png");
    _tile2 = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName ("building_2.png");
    _tile3 = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName ("building_3.png");
    _tile4 = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName ("building_4.png");
    
    _roof1 = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName ("roof_1.png");
    _roof2 = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName ("roof_2.png");
    

    //create tiles
    int i;
    _wallTiles = CCArray::createWithCapacity(20);
    _wallTiles->retain();

    _roofTiles = CCArray::createWithCapacity(5);
    _roofTiles->retain();

    
    CCSprite * tile;
    
    for (i = 0; i < 5; i++) {
        tile = CCSprite::createWithSpriteFrameName("roof_1.png");
        tile->setAnchorPoint(ccp(0, 1));
        tile->setPosition(ccp(i * _tileWidth, 0));
        tile->setVisible(false);
        this->addChild(tile, kMiddleground, kRoofTile);
        _roofTiles->addObject(tile);
        
        for (int j = 0; j < 4; j++) {
            tile = CCSprite::createWithSpriteFrameName("building_1.png");
            tile->setAnchorPoint(ccp(0, 1));
            tile->setPosition(ccp(i * _tileWidth, -1 * (_tileHeight * 0.47f + j * _tileHeight)));
            tile->setVisible(false);
            this->addChild(tile, kBackground, kWallTile);
            _wallTiles->addObject(tile);
        }
        
    }
       
    _chimneys = CCArray::createWithCapacity(5);
    _chimneys->retain();
    
    
    CCSprite * chimney;
    CCSprite * puff;
    
    for (i = 0; i < 5; i++) {
        chimney = CCSprite::createWithSpriteFrameName("chimney.png");
        chimney->setVisible(false);
        this->addChild(chimney, kForeground, kChimney);
        _chimneys->addObject(chimney);
        
        for (int j = 0; j < TOTAL_PUFFS; j++) {
            puff = CCSprite::createWithSpriteFrameName("puff_1.png");
            puff->setAnchorPoint(ccp(0,-0.5));
            puff->setVisible(false);
            chimney->addChild(puff, -1, j);
        }
        
    }
    
    
    CCAnimation* animation;
    animation = CCAnimation::create();
    CCSpriteFrame * frame;

    for(i = 1; i <= 4; i++) {
        char szName[100] = {0};
        sprintf(szName, "puff_%i.png", i);
        frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(szName);
        animation->addSpriteFrame(frame);
    }

    animation->setDelayPerUnit(0.75f / 4.0f);
    animation->setRestoreOriginalFrame(false);
    animation->setLoops(-1);
    _puffAnimation = CCAnimate::create(animation);
    _puffAnimation->retain();

    _puffSpawn = CCRepeat::create(CCSequence::create(CCDelayTime::create(0.5f),
                                  CCCallFuncN::create( this, callfuncN_selector(Block::createPuff) ),
                                  NULL), TOTAL_PUFFS);
    _puffSpawn->retain();

    _puffMove = CCMoveBy::create(1.0f, ccp(-100,80));
    _puffMove->retain();
    _puffFade = CCFadeOut::create(2.0f);
    _puffFade->retain();
    _puffScale = CCScaleBy::create(1.5f, 1.5);
    _puffScale->retain();
    
    
    _puffIndex = 0;
}


void Block::createPuff(CCNode * node) {
    
    int count = _chimneys->count();
    CCSprite * chimney;
    CCSprite * puff;
    
    for (int i = 0; i < count; i++) {
        chimney = (CCSprite * ) _chimneys->objectAtIndex(i);
        if (chimney->isVisible()) {
            
            puff = (CCSprite *) chimney->getChildByTag(_puffIndex);
            puff->setVisible(true);
            puff->stopAllActions();
            puff->setScale(1.0);
            puff->setOpacity(255);
            puff->setPosition(ccp(0,0));
            puff->runAction((CCAction *) _puffAnimation->copy()->autorelease());
            puff->runAction((CCAction *) _puffMove->copy()->autorelease());
            //puff->runAction((CCAction *) _puffFade->copy()->autorelease());
            puff->runAction((CCAction *) _puffScale->copy()->autorelease());
            
        }
    }
    
    _puffIndex++;
    if (_puffIndex == TOTAL_PUFFS) _puffIndex = 0;
}
