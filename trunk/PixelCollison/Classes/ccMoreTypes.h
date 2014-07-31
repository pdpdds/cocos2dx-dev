#ifndef	__CCMORETYPES_H__
#define	__CCMORETYPES_H__

/*
 * Kobold2D™ --- http://www.kobold2d.org
 *
 * Copyright (c) 2010-2011 Steffen Itterheim. 
 * Released under MIT License in Germany (LICENSE-Kobold2D.txt).
 */

#include "cocos2d.h"
//#import "CCDirectorExtensions.h"


/** @file ccMoreTypes.h - Adding some missing type declarations and helpers that are not in ccTypes.h */


/** Quick and dirty rectangle drawing. Should be improved to render a triangle strip instead. */

static inline void ccDrawRect(cocos2d::CCRect rect)
{
	cocos2d::CCPoint pt1 = rect.origin;
	cocos2d::CCPoint pt2 = cocos2d::CCPointMake(rect.origin.x + rect.size.width, rect.origin.y);
	cocos2d::CCPoint pt3 = cocos2d::CCPointMake(pt2.x, rect.origin.y + rect.size.height);
	cocos2d::CCPoint pt4 = cocos2d::CCPointMake(rect.origin.x, pt3.y);
	ccDrawLine(pt1, pt2);
	ccDrawLine(pt2, pt3);
	ccDrawLine(pt3, pt4);
	ccDrawLine(pt4, pt1);
}

/** creates and returns a ccColor4F struct */
static inline cocos2d::ccColor4F ccc4f(const GLfloat r, const GLfloat g, const GLfloat b, const GLfloat o)
{
	cocos2d::ccColor4F c = {r, g, b, o};
	return c;
}

//! Cyan Color (0,255,255)
static const cocos2d::ccColor3B ccCYAN = {0,255,255};

/** creates and returns a ccBlendFunc struct */
static inline cocos2d::ccBlendFunc ccBlendFuncMake(GLenum src, GLenum dst)
{
	cocos2d::ccBlendFunc blendFunc = {src, dst};
	return blendFunc;
}

#endif	//__CCMORETYPES_H__
