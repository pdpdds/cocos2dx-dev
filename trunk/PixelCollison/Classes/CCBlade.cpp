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

#include "CCBlade.h"

float fangle(CCPoint vect)
{
	if (vect.x == 0.0 && vect.y == 0.0) {
		return 0;
	}
	
	if (vect.x == 0.0) {
		return vect.y > 0 ? M_PI/2 : -M_PI/2;
	}
	
	if (vect.y == 0.0 && vect.x < 0) {
		return -M_PI;
	}
	
	float angle = atan(vect.y / vect.x);
    
	return vect.x < 0 ? angle + M_PI : angle;
}

void f1(CCPoint p1, CCPoint p2, float d, CCPoint *o1, CCPoint *o2)
{
	float l = ccpDistance(p1, p2);
	float angle = fangle(ccpSub(p2, p1));
	*o1 = ccpRotateByAngle(ccp(p1.x + l,p1.y + d), p1, angle);
	*o2 = ccpRotateByAngle(ccp(p1.x + l,p1.y - d), p1, angle);
}

inline float lagrange1(CCPoint p1, CCPoint p2, float x)
{
	return (x-p1.x)/(p2.x - p1.x)*p2.y + (x-p2.x)/(p1.x - p2.x)*p1.y ;
}

inline void CCPointSet(CCPoint *v, float x, float y)
{
	v->x = x;
	v->y = y;
}

CCBlade* CCBlade::bladeWithMaximumPoint(int limit)
{
	CCBlade *pRet = new CCBlade();
	if(pRet && pRet->initWithMaximumPoint(limit))
	{
		pRet->autorelease();
		return pRet;
	}
	CC_SAFE_DELETE(pRet);
	return NULL;    
}

bool CCBlade::initWithMaximumPoint(int limit)
{       
    pointLimit = limit;
	width = 10;
	
    vertices = (CCPoint *)calloc(2*limit+5, sizeof(vertices[0]));
    coordinates = (CCPoint *)calloc(2*limit+5, sizeof(coordinates[0]));
    
    CCPointSet(coordinates+0, 0.00, 0.5);
    isReset = false;
    
    return true;
}

void CCBlade::cleanup()
{
	if (_texture != NULL) 
	{
		_texture->release();
	}

    free(vertices);
    free(coordinates);
	CCNode::cleanup();
}

void CCBlade::populateVertices()
{	
	vertices[0] = path[0]; 
    CCPoint pre = vertices[0];
    
    unsigned int i = 0;
    CCPoint it = path[1];
	float dd = width / path.size();
	while (i < path.size() - 2){
		f1(pre, it, width - i * dd , vertices+2*i+1, vertices+2*i+2);
		CCPointSet(coordinates+2*i+1, .5, 1.0);
		CCPointSet(coordinates+2*i+2, .5, 0.0);
		
		i++;
		pre = it;
		
		it = path[i+1];
	}
   
    CCPointSet(coordinates+1, 0.25, 1.0); 
	CCPointSet(coordinates+2, 0.25, 0.0);
	
	vertices[2*path.size()-3] = it;
	CCPointSet(coordinates+2*path.size()-3, 0.75, 0.5);
}

void CCBlade::shift()
{
	int index = 2 * pointLimit - 1;
	for (int i = index; i > 3; i -= 2) {
		vertices[i] = vertices[i-2];
		vertices[i-1] = vertices[i-3];
	}
}

void CCBlade::setWidth(float width_)
{
    width = width_ * CC_CONTENT_SCALE_FACTOR();
}

#define DISTANCE_TO_INTERPOLATE 10

void CCBlade::push(CCPoint v)
{
	if (isReset) 
	{
		return;
	}
    if (CC_CONTENT_SCALE_FACTOR() != 1.0f) {
        v = ccpMult(v, CC_CONTENT_SCALE_FACTOR());
    }

	#if USE_LAGRANGE

    if (path.size() == 0) 
	{
		path.insert(path.begin(),v);        
        return;
    }
    
    CCPoint first = path[0];
    if (ccpDistance(v, first) < DISTANCE_TO_INTERPOLATE) {
        path.insert(path.begin(),v);
        if (path.size() > pointLimit) {
            path.pop_back();
        }
    }else{
        int num = ccpDistance(v, first) / DISTANCE_TO_INTERPOLATE;
        CCPoint iv = ccpMult(ccpSub(v, first), (float)1./(num + 1));
		for (int i = 1; i <= num + 1; i++) {
            path.insert(path.begin(),ccpAdd(first, ccpMult(iv, i)));
		}
		while (path.size() > pointLimit) {
			path.pop_back();
		}
    }
#else // !USE_LAGRANGE
	path.push_front(v);
	if (path.size() > pointLimit) {
		path.pop_back();
	}
#endif // !USE_LAGRANGE
	this->populateVertices();
}

void CCBlade::pop(int n)
{
    while (path.size() > 0 && n > 0) 
	{
        path.pop_back();
        n--;
    }
    
    if (path.size() > 2) 
	{
        this->populateVertices();
    }
}

void CCBlade::clear()
{
	path.clear();
	isReset = false;
} 

void CCBlade::reset()
{
	isReset = true;	
}

void CCBlade::dim(bool dim)
{
	isReset = dim;
}

void CCBlade::draw()
{
	this->pop(3);
    if (isReset && path.size() > 0)
	{
        this->pop(1);
        if (path.size() < 3) {
            schedule(schedule_selector(CCBlade::destroyMyself),0);		
        }
    }
    
    if (path.size() < 3) {
        return;
    }
	
    glDisableClientState(GL_COLOR_ARRAY);
    CCAssert(_texture, @"NO TEXTURE SET");
    
    glBindTexture(GL_TEXTURE_2D, _texture->getName());
    glVertexPointer(2, GL_FLOAT, 0, vertices);
    glTexCoordPointer(2, GL_FLOAT, 0, coordinates);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 2*path.size()-2);
	glEnableClientState(GL_COLOR_ARRAY);
}

void CCBlade::destroyMyself(float t)
{
	unschedule(schedule_selector(CCBlade::destroyMyself));		
	this->getParent()->removeChild(this,true);
}

void CCBlade::setTexture(CCTexture2D* texture)
{
	this->_texture = texture;
	_texture->retain();
}

CCTexture2D* CCBlade::getTexture()
{
	return _texture;
}
