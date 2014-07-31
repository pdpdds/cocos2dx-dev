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

#include "RTLayer.h"

RTLayer::~RTLayer()
{
    CCLOGINFO("cocos2d: deallocing.");
    m_pBrush->release();
    m_pTarget->release();
}

bool RTLayer::init()
{
	if (!CCLayer::init())
	{
		return false;
	}
	
    CCSize s = CCDirector::sharedDirector()->getWinSize();

    // create a render texture, this is what we are going to draw into
    m_pTarget = CCRenderTexture::create(s.width, s.height, kCCTexture2DPixelFormat_RGBA8888);
    m_pTarget->retain();
    m_pTarget->setPosition(ccp(s.width / 2, s.height / 2));

    // note that the render texture is a CCNode, and contains a sprite of its texture for convience,
    // so we can just parent it to the scene like any other CCNode
    this->addChild(m_pTarget, -1);

    // create a brush image to draw into the texture with
    m_pBrush = CCSprite::create("images/streak1.png");
    m_pBrush->retain();
    m_pBrush->setColor(ccRED);
    m_pBrush->setOpacity(20);
    this->setTouchEnabled(true);

    // Save Image menu
    CCMenuItemFont::setFontSize(16);
    CCMenuItem *item1 = CCMenuItemFont::create("Save Image", this, menu_selector(RTLayer::saveImage));
    CCMenuItem *item2 = CCMenuItemFont::create("Clear", this, menu_selector(RTLayer::clearImage));
    CCMenu *menu = CCMenu::create(item1, item2, NULL);
    this->addChild(menu);
    menu->alignItemsVertically();
    menu->setPosition(ccp(s.width - 80, s.height - 30));
	return true;
}

void RTLayer::clearImage(cocos2d::CCObject *pSender)
{
    m_pTarget->clear(CCRANDOM_0_1(), CCRANDOM_0_1(), CCRANDOM_0_1(), CCRANDOM_0_1());
}

void RTLayer::saveImage(cocos2d::CCObject *pSender)
{
    static int counter = 0;


    char png[20];
    sprintf(png, "image-%d.png", counter);
    char jpg[20];
    sprintf(jpg, "image-%d.jpg", counter);
#if 0
    m_pTarget->saveToFile(png, kCCImageFormatPNG);
    m_pTarget->saveToFile(jpg, kCCImageFormatJPEG);
#endif    

    CCImage *pImage = m_pTarget->newCCImage();

    CCTexture2D *tex = CCTextureCache::sharedTextureCache()->addUIImage(pImage, png);

    CC_SAFE_DELETE(pImage);

    CCSprite *sprite = CCSprite::spriteWithTexture(tex);

    sprite->setScale(0.3f);
    addChild(sprite);
    sprite->setPosition(ccp(40, 40));
    sprite->setRotation(counter * 3);

    CCLOG("Image saved %s and %s", png, jpg);

    counter++;
} 

bool RTLayer::ccTouchBegan(CCTouch* pTouch, CCEvent* pEvent)
{
	return true;
}

void RTLayer::ccTouchesMoved(CCSet* touches, CCEvent* event) 
{
    CCTouch *touch = (CCTouch *)touches->anyObject();
    CCPoint start = touch->locationInView();
    start = CCDirector::sharedDirector()->convertToGL(start);
    CCPoint end = touch->previousLocationInView();
    end = CCDirector::sharedDirector()->convertToGL(end);

    // begin drawing to the render texture
    m_pTarget->begin();

    // for extra points, we'll draw this smoothly from the last position and vary the sprite's
    // scale/rotation/offset
    float distance = ccpDistance(start, end);
    if (distance > 1)
    {
        int d = (int)distance;
        for (int i = 0; i < d; i++)
        {
            float difx = end.x - start.x;
            float dify = end.y - start.y;
            float delta = (float)i / distance;
            m_pBrush->setPosition(ccp(start.x + (difx * delta), start.y + (dify * delta)));
            m_pBrush->setRotation(rand() % 360);
            float r = (float)(rand() % 50 / 50.f) + 0.25f;
            m_pBrush->setScale(r);
            /*m_pBrush->setColor(ccc3(CCRANDOM_0_1() * 127 + 128, 255, 255));*/
            // Use CCRANDOM_0_1() will cause error when loading libtests.so on android, I don't know why.
            m_pBrush->setColor(ccc3(rand() % 127 + 128, 255, 255));
            // Call visit to draw the brush, don't call draw..
            m_pBrush->visit();
        }
    }

    // finish drawing and return context back to the screen
    m_pTarget->end();
}

void RTLayer::ccTouchEnded(CCTouch* pTouch, CCEvent* pEvent)
{

}

void RTLayer::clenup() 
{
//    map->release();
    CCNode::cleanup();
}
