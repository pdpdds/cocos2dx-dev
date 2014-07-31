//
//  Cue.h
//  MiniPool
//
//  Created by Roger Engelbert on 1/13/13.
//
//

#ifndef __MiniPool__Cue__
#define __MiniPool__Cue__

#include "b2Sprite.h"

class Cue : public b2Sprite {
public:
    ~Cue();
    Cue(GameLayer * game);
    
    static Cue* create(GameLayer * game);
    
private:
    void initCue();
};
 

#endif /* defined(__MiniPool__Cue__) */
