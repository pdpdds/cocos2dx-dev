//
//  AWTextureEffects.h
//
//  Created by Manuel Martinez-Almeida Castañeda on 09/05/10.
//  Copyright 2010 Abstraction Works. All rights reserved.
//  http://www.abstractionworks.com
//
#ifndef	__AWTEXTUREEFFECT_H__
#define	__AWTEXTUREEFFECT_H__

#include "cocos2d.h"
#include "CCTexture2DMutable.h"

class AWTextureEffect : public cocos2d::CCObject 
{

///
//	@param input: Original texture data
//	@param output: Empty (or not) buffer
//  @param format: Pixel format of the data
//	@param width: Real width (is a power of two)
//	@param height: Real height (is a power of two)
//	@param position: Top left vertex of the blur effect
//	@param size: The size of the blur effect
//	@param contentSize: 
//	@param radios: It's the radius of the blur effect
///

private:
	static	void blurInput(void* input, void* output, cocos2d::CCTexture2DPixelFormat format, int width, int height, cocos2d::CCPoint position, cocos2d::CCPoint size, cocos2d::CCSize contentSize, int radius);
	static	CCTexture2DMutable* blur(CCTexture2DMutable* texture, cocos2d::CCPoint position, cocos2d::CCPoint size, int radius);
	static	CCTexture2DMutable* blur(CCTexture2DMutable* texture, int radius);
};

#endif	// __AWTEXTUREEFFECT_H__