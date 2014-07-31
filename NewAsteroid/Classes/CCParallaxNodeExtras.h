//
//  CCParallaxNode-Extras.h
//  cocos2dx_space_game
//
//  Created by jean-yves mengant on 2/6/12 week 6.
//  Copyright (c) 2012 . All rights reserved.
//

#ifndef cocos2dx_space_game_CCParallaxNodeExtras_h
#define cocos2dx_space_game_CCParallaxNodeExtras_h

#include "cocos2d.h"

USING_NS_CC; 

namespace jymc {

  class CCParallaxNodeExtras : public CCParallaxNode {
  
    public :
  
    // Need to provide our own constructor 
    CCParallaxNodeExtras() ;
    
    // just to avoid ugly later cast and also for safety
    static CCParallaxNodeExtras * node() ;
    
    // Facility method (we expect to have it soon in COCOS2DX)    
    void incrementOffset(CCPoint offset,CCNode* node) ;  
  } ;  
}  


#endif
