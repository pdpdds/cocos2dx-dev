/*
 * cocos2d+ext for iPhone
 *
 * Copyright (c) 2011 - Ngo Duc Hiep
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 */

#pragma once
#include "cocos2d.h"

#define USE_LAGRANGE    1
#define USE_STL_LIST    0

using namespace cocos2d;

float fangle(CCPoint vect);
float lagrange1(CCPoint p1, CCPoint p2, float x);

inline void CCPointSet(CCPoint *v, float x, float y);
inline void f1(CCPoint p1, CCPoint p2, float d, CCPoint *o1, CCPoint *o2);

class CCBlade : public CCNode 
{
	std::vector<CCPoint> path;
	unsigned int pointLimit;
	int count;
	CCPoint *vertices;
	CCPoint *coordinates;
	bool isReset;
	CCTexture2D *_texture;	
	float width;

public:
	CCTexture2D *texture;
	static CCBlade* bladeWithMaximumPoint(int limit);
	bool initWithMaximumPoint(int limit);
	void push(CCPoint v);
	void pop(int n);
	void clear();
	void reset();
	void dim(bool dim);
	void populateVertices();
	void shift();
	void setWidth(float width_);
	void setTexture(CCTexture2D* texture);
	CCTexture2D* getTexture();
	virtual void cleanup();
	virtual void draw();
	virtual void destroyMyself(float t);
};

