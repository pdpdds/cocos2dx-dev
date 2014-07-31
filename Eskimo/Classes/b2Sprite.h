//
//  b2Sprite.h
//  MiniPool
//
//  Created by Roger Engelbert on 1/13/13.
//
//

#ifndef __MiniPool__b2Sprite__
#define __MiniPool__b2Sprite__


#include "cocos2d.h"
#include "Box2d/Box2D.h"
#include "GameConfig.h"

class GameLayer;

using namespace cocos2d;

class b2Sprite : public CCSprite {
    
public:
    
    b2Sprite (GameLayer * game);

    CC_SYNTHESIZE(b2Body *, _body, Body);
    CC_SYNTHESIZE(GameLayer *, _game, Game);
    
    virtual void setSpritePosition (CCPoint position);
    virtual void update(void);
    virtual void hide(void);
    virtual void reset(void);


};


#endif /* defined(__MiniPool__b2Sprite__) */
