//
//  PlayScene.h
//  Parkour
//
//  Created by YTB on 13-11-20.
//
//

#ifndef __Parkour__PlayScene__
#define __Parkour__PlayScene__

#include "cocos2d.h"
#include "chipmunk.h"

USING_NS_CC;

class Runner;
class MapManager;
class ObjectManager;
class SimpleRecognizer;

enum {
    TAG_STATUSLAYER = 10,
    TAG_GAMEOVER,
    TAG_PLAYER,
};

class PlayLayer : public CCLayer
{
public:
    CREATE_FUNC(PlayLayer);
    ~PlayLayer();
    virtual void onExit();
    virtual bool init();
    virtual void update(float dt);
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    
    static CCScene* scene();
private:
    cpSpace *space;
    cpShape *wallBottom;
    CCSpriteBatchNode *spriteSheet;
    Runner *runner;
    MapManager *mapManager;
    ObjectManager *objectManager;
    float lastEyeX;
    
    void notifiCoin(CCObject *unuse);
    void notifiRock(CCObject *unuse);
    
    CCPoint touchBeganPoint,touchEndedPoint;
    std::list<CCPoint> points;
    
    SimpleRecognizer *recognizer;
};
#endif /* defined(__Parkour__PlayScene__) */
