//
//  TextureBackgroundScene.h
//  CCRenderTextureCocos2D-x
//

//

#ifndef __TextureBackground_SCENE_H__
#define __TextureBackground_SCENE_H__

#include "cocos2d.h"

 



class TextureBackground : public cocos2d::CCLayer
{
protected:
	cocos2d::CCSprite *_background;
		cocos2d::CCSprite *_background2;
		cocos2d::CCSprite *_grass;


public:
	
	
	// initialize the _background variable
	TextureBackground() : _background(NULL) {};
	//CC_SYNTHESIZE_RETAIN(Terrain *, _terrain, Terrain);
	// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	virtual bool init();  
	//virtual void update(float dt);
	~TextureBackground();

	cocos2d::CCSprite* spriteWithColor(cocos2d::ccColor4F bgColor, float textureSize);
	cocos2d::CCSprite* spriteWithColor2(cocos2d::ccColor4F bgColor, float width, float height);
	//cocos2d::CCSprite* stripedSpriteWithColor1(cocos2d::ccColor4F c1, cocos2d::ccColor4F c2, float textureSize, int nStripes);
	cocos2d::ccColor4F randomBrightColor();
	void genBackground();
	//bool ccTouchBegan(cocos2d::CCTouch *touch, cocos2d::CCEvent *event);


	// there's no 'id' in cpp, so we recommand to return the exactly class pointer
	//static cocos2d::CCScene* scene();

	// a selector callback
	virtual void menuCloseCallback(CCObject* pSender);

	virtual void ccTouchesBegan(cocos2d::CCSet* touches, cocos2d::CCEvent* event);

	// implement the "static node()" method manually
	NODE_FUNC(TextureBackground);
};

#endif // __TextureBackground_SCENE_H__