#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"  
#include "CCParallaxNodeExtras.h"

USING_NS_CC ;

using namespace jymc ;

typedef enum {
  KENDREASONWIN,
  KENDREASONLOSE
} EndReason;

class HelloWorld : public cocos2d::CCLayer  
{
  private :  
    cocos2d::CCSpriteBatchNode *_batchNode ;
    cocos2d::CCSprite *_ship ;
  
    CCParallaxNodeExtras *_backgroundNode;  
    CCSprite *_spacedust1;
    CCSprite *_spacedust2;
    CCSprite *_planetsunrise;
    CCSprite *_galaxy;
    CCSprite *_spacialanomaly;
    CCSprite *_spacialanomaly2;  


    //Accelerometer 
    float _shipPointsPerSecY;  
    static const float _KFILTERINGFACTOR_ ;
    static const float _KRESTACCELX_ ;
    static const float _KMAXDIFFX_  ;
  
    //Asteroids
    static const int _KNUMASTEROIDS_  ;
    CCArray*  _asteroids ; 
    int _nextAsteroid ;
    float _nextAsteroidSpawn ;     

    // internal usage for asteroids
    float randomValueBetween( float low , float high ) ;
    void setInvisible(CCNode * node) ;
    float getTimeTick() ;

    // shipLasers
    static const int _KNUMLASERS_  ;
    CCArray*  _shipLasers ; 
    int _nextShipLaser ;
    virtual void ccTouchesBegan(cocos2d::CCSet* touches, cocos2d::CCEvent* event);
  
    int _lives ;
  
    // GameOver
    double _gameOverTime ;
    bool _gameOver ; 
  
  public:
	// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	virtual bool init();  

   // scheduled Update 
   void update(float dt);
  
  
	// there's no 'id' in cpp, so we recommand to return the exactly class pointer
	static cocos2d::CCScene* scene();
	
	// a selector callback
	virtual void menuCloseCallback(CCObject* pSender);

	// implement the "static node()" method manually
	NODE_FUNC(HelloWorld);

   // Accelerometer Callback definition
   virtual void didAccelerate(CCAcceleration* pAccelerationValue);
  
   // Game Over
   void endScene( EndReason endReason ) ;
   void restartTapped(CCObject* pObject = NULL) ;   
  
};
#endif // __HELLOWORLD_SCENE_H__
