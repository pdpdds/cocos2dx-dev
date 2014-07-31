//
//  Ball.h
//  MiniPool
//
//  Created by Roger Engelbert on 1/13/13.
//
//

#ifndef __MiniPool__Ball__
#define __MiniPool__Ball__


#include "b2Sprite.h"

enum {
    kColorYellow,
    kColorRed,
    kColorBlack,
    kColorWhite
    
};

class Ball : public b2Sprite {

public:
    
    CC_SYNTHESIZE(bool, _inPlay, InPlay);
    
    ~Ball();
    Ball(GameLayer * game, int type, CCPoint position, int color);
    static Ball* create(GameLayer * game, int type, CCPoint position, int color);
    virtual void reset(void);
    virtual void update (float dt);
private:
    void initBall();
    CCPoint _startPosition;
    
    int _color;

};

#endif /* defined(__MiniPool__Ball__) */
