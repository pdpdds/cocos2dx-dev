//
//  Platform.h
//  MiniPool
//
//  Created by Roger Engelbert on 1/13/13.
//
//

#ifndef __Platform__
#define __Platform__


#include "b2Sprite.h"


class Platform : public b2Sprite {

public:
    
    ~Platform();
    Platform(GameLayer * game);
    static Platform* create(GameLayer * game);
    void initPlatform(int width, float angle, CCPoint position);
    
    
    void onGravityChanged(CCObject * note);
    
private:
    CCArray * _tiles;
    void createTiles(void);
    void switchTexture(void);
};

#endif /* defined(__Platform__) */
