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
#include "CCBlade.h"
using namespace cocos2d;

class RTLayer : public CCLayer 
{
	cocos2d::CCRenderTexture* m_pTarget;
	cocos2d::CCSprite *m_pBrush;

public:
	CREATE_FUNC(RTLayer);
	virtual bool init();
	virtual bool ccTouchBegan(CCTouch* pTouch, CCEvent* pEvent);
	virtual void ccTouchesMoved(CCSet* touches, CCEvent* event);
	virtual void ccTouchEnded(CCTouch* pTouch, CCEvent* pEvent);
	virtual void clenup();
    void clearImage(CCObject *pSender);
    void saveImage(CCObject *pSender);

private:
	~RTLayer();
};

