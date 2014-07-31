#ifndef __BLOCK_H__
#define __BLOCK_H__

#define TILE_H_SIZE 6
#define TILE_W_SIZE 8

#include "cocos2d.h"
#include "GameSprite.h"

USING_NS_CC;


enum {
    kWallTile,
    kRoofTile,
    kChimney
};

enum  {
    kBlockGap,
    kBlock1,
    kBlock2,
    kBlock3,
    kBlock4
};

class Block : public GameSprite {
    
    CCSize _screenSize;
    
    CCArray * _wallTiles;
    CCArray * _roofTiles;
    
    CCSpriteFrame * _tile1;
    CCSpriteFrame * _tile2;
    CCSpriteFrame * _tile3;
    CCSpriteFrame * _tile4;
    
    CCSpriteFrame * _roof1;
    CCSpriteFrame * _roof2;
    
    CCAction * _puffAnimation;
    CCAction * _puffSpawn;
    CCAction * _puffMove;
    CCAction * _puffFade;
    CCAction * _puffScale;
    
    
    int _tileWidth;
    int _tileHeight;
    int _puffIndex;
    
    
    void initBlock(void);
	void createPuff(CCNode * node);
    
public:
    
    CC_SYNTHESIZE(int, _type, Type);
    CC_SYNTHESIZE_READONLY(bool, _puffing, Puffing);
	CC_SYNTHESIZE(CCArray *, _chimneys, Chimneys);
    
    
    Block();
    ~Block();
    
    static Block * create();
    void setupBlock (int width, int height, int type);
    void setPuffing (bool value);
	void hidePuffs(CCNode * node);
    
    
    inline virtual int left() {
    	return this->getPositionX();
	}
    
	inline virtual int right() {
    	return this->getPositionX() + _width;
	}
    
    inline virtual int top() {
        return this->getHeight();
    }
    
    inline virtual int bottom() {
		return 0;
    }
    
    
};

#endif // __BLOCK_H__
