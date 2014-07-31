#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"

using namespace cocos2d;

class HelloWorld : public CCLayerColor
{
public:
    virtual bool init();

    static CCScene* scene();
    
    CREATE_FUNC(HelloWorld);

    CCSize          winSize;
    CCPoint         dragonPosition;
    CCSprite        *dragon;
    CCTMXTiledMap   *tmap;
    CCTMXLayer      *background;
    CCTMXLayer      *items;
    CCTMXLayer      *metainfo;

    virtual void onEnter();
    virtual void onExit();
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent* event);
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent* event);
    
    void createDragon();
    CCPoint tileCoordForPosition(CCPoint position);
    void setPlayerPosition(CCPoint position);
    void setViewpointCenter(CCPoint position);
    
};

#endif // __HELLOWORLD_SCENE_H__
