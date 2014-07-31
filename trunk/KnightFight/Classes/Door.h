//
//  Door.h
//  KnightFight
//
//  Created by Loz Archer on 09/05/2011.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#include <Foundation/Foundation.h>
#include "cocos2d.h"

using namespace cocos2d;

class Door : public CCNode 
{
public:
	CCPoint gridPos;
	CCString *contents;
}

@property (nonatomic) CGPoint tilePos;
@property (nonatomic, retain) NSString *contents;

+(id) door;

@end
