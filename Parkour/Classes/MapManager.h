//
//  MapManager.h
//  PaoKu
//
//  Created by YTB on 13-11-24.
//
//

#ifndef __PaoKu__MapManager__
#define __PaoKu__MapManager__

#include "cocos2d.h"
#include "chipmunk.h"

USING_NS_CC;

class Map;

class MapManager
{
private:
    cpSpace *space;//current space;

    float spriteWidth;
    int curMap;// being displayed
    
    Map *map0;
    Map *map1;
public:
    MapManager(CCLayer *parent, cpSpace *pSpace);
    ~MapManager();

    static float getGroundHeight() {return 50.0;};

    float getMapWidth() {return spriteWidth;};
    int getCurMap() {return curMap;};

    bool checkAndReload(float eyeX);
};

#endif /* defined(__PaoKu__MapManager__) */
