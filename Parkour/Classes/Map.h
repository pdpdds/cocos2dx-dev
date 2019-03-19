//
//  Map.h
//  PaoKu
//
//  Created by YTB on 13-11-26.
//
//

#ifndef __PaoKu__Map__
#define __PaoKu__Map__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "chipmunk.h"

USING_NS_CC;

class Map : public CCSprite
{
public:
    static Map *create(int index);
    Map(int index);
    ~Map();
    
    void reload(int index);
private:
    const char *getMapName(int index);
    const char *getGroundName(int index);
};

#endif /* defined(__PaoKu__Map__) */
