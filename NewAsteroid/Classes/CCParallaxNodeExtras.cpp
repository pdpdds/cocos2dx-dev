//
//  CCParallaxNodeExtras.cpp
//  cocos2dx_space_game
//
//  Created by jean=yves mengant on 2/6/12 week 6.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include "CCParallaxNodeExtras.h" 

using namespace jymc ;

// Caveat : CCPointObject is not a public class 
// ==> we will mimic it here in order to be able to dynamically cast it 
// and usit ; I know it's an ugly ack ... but there's a tiny issue
class CCPointObject  : CCObject {
  CC_SYNTHESIZE(CCPoint, m_tRatio, Ratio)
  CC_SYNTHESIZE(CCPoint, m_tOffset, Offset)
  CC_SYNTHESIZE(CCNode *,m_pChild, Child)	// weak ref
} ;


// Need to provide our own constructor 
CCParallaxNodeExtras::CCParallaxNodeExtras() {
  CCParallaxNode::CCParallaxNode() ; // call parent constructor
}

CCParallaxNodeExtras * CCParallaxNodeExtras::node() {
  return new CCParallaxNodeExtras() ;       
}


void CCParallaxNodeExtras::incrementOffset(CCPoint offset,CCNode* node){
  for( unsigned int i=0;i < m_pParallaxArray->num;i++) {
    CCPointObject *point = (CCPointObject *)m_pParallaxArray->arr[i];
    CCNode * curNode = point->getChild() ;
    if( curNode->isEqual(node) ) {
      point->setOffset( ccpAdd(point->getOffset(), offset) );
      break;
    }
  }
}




