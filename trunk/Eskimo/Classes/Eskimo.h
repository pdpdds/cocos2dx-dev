//
//  Eskimo.h
//  MiniPool
//
//  Created by Roger Engelbert on 1/13/13.
//
//

#ifndef __Eskimo__
#define __Eskimo__


#include "b2Sprite.h"


class Eskimo : public b2Sprite {

public:
    
    CC_SYNTHESIZE(int, _state, State);
    CC_SYNTHESIZE(bool, _switchShape, SwitchShape);
    
    ~Eskimo();
    Eskimo(GameLayer * game);
    static Eskimo* create(GameLayer * game);
    virtual void reset(void);
    virtual void update (void);

private:
    void makeCircleShape();
    void makeBoxShape();
    
    

};

#endif /* defined(__Eskimo__) */
