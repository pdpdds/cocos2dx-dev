#ifndef Cocos2DxFirstIosSample_CCParallaxNodeExtras_h
#define Cocos2DxFirstIosSample_CCParallaxNodeExtras_h
 
#include "cocos2d.h"
 
USING_NS_CC;
 
class CCParallaxNodeExtras : public CCParallaxNode {
 
    public :
 
    // Need to provide our own constructor 
    CCParallaxNodeExtras() ;
 
    // just to avoid ugly later cast and also for safety
    static CCParallaxNodeExtras * node() ;
 
    // Facility method (we expect to have it soon in COCOS2DX)    
    void incrementOffset(CCPoint offset,CCNode* node) ;  
} ; 
 
#endif