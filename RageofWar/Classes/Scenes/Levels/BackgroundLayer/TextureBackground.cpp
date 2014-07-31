
#include "TextureBackground.h"

USING_NS_CC;
#define A_S 48

TextureBackground::~TextureBackground()
{
}



// on "init" you need to initialize your instance
bool TextureBackground::init()
{
	//////////////////////////////
	// 1. super init first
	if ( !CCLayer::init() )
	{
		return false;
	}

	
	cocos2d::CCLog("ininit");
	this->genBackground();
	this->setTouchEnabled(true);
	
	return true;
}



cocos2d::CCSprite* TextureBackground::spriteWithColor(ccColor4F bgColor, float textureSize)
{
	// 1: Create new CCRenderTexture
	CCRenderTexture *rt = CCRenderTexture::create(textureSize, textureSize);

	// 2: Call CCRenderTexture:begin
	rt->beginWithClear(bgColor.r, bgColor.g, bgColor.b, bgColor.a);

	// 3: Draw into the texture
	glDisable(GL_TEXTURE_2D);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);

	float gradientAlpha = 0.7;    
	CCPoint vertices[4];
	ccColor4F colors[4];
	int nVertices = 0;
	
	ccColor4F color={0, 0, 0, 0};
	ccColor4F color1={0, 0, 0, gradientAlpha};

	vertices[nVertices] = CCPointMake(0, 0);
	colors[nVertices++] = color;
	vertices[nVertices] = CCPointMake(textureSize, 0);
	colors[nVertices++] = color;
	vertices[nVertices] = CCPointMake(0, textureSize);
	colors[nVertices++] = color1;
	vertices[nVertices] = CCPointMake(textureSize, textureSize);
	colors[nVertices++] = color1;

	glVertexPointer(2, GL_FLOAT, 0, vertices);
	glColorPointer(4, GL_FLOAT, 0, colors);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, (GLsizei)nVertices);

	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnable(GL_TEXTURE_2D);

	CCSprite *noise = CCSprite::create("Background1/Noise.jpg");
	cocos2d::ccBlendFunc blendfunc={GL_DST_COLOR, GL_ZERO};
	//blendfunc.dst=GL_DST_COLOR;
	//blendfunc.src=GL_ZERO;
	noise->setBlendFunc(blendfunc);

	CCSize winSize = CCDirector::sharedDirector()->getWinSize();
	
	noise->setPosition(ccp(textureSize/2, textureSize/2));
	noise->visit();

	// 4: Call CCRenderTexture:end
	rt->end();

	// 5: Create a new Sprite from the texture
	return CCSprite::createWithTexture(rt->getSprite()->getTexture());
}


cocos2d::ccColor4F TextureBackground::randomBrightColor()
{
	while (true) {
		float requiredBrightness = 192;

		ccColor4B randomColor = 
			ccc4(rand() % 255,
				 rand() % 255,
				 rand() % 255,
				 255);
		if (randomColor.r > requiredBrightness || 
            randomColor.g > requiredBrightness ||
            randomColor.b > requiredBrightness) {
			
            return ccc4FFromccc4B(randomColor);
        }
	}
}

void TextureBackground::genBackground()
{
  /* if (_background != NULL)
	{
		_background->removeFromParentAndCleanup(true);
	}*/

    //ccColor4F bgColor = this->randomBrightColor();
	 ccColor4F bgColor ;
	 //= 0.898039 0.768627 0.894118
	 bgColor.r=0.89;
	 bgColor.g=0.76;
	 bgColor.b=0.89;
	 bgColor.a=0.5;

	 cocos2d::CCLog("BGColor = %f %f %f",bgColor.r,bgColor.g,bgColor.b);
    _background = this->spriteWithColor(bgColor,512);
	

     CCSize winSize = CCDirector::sharedDirector()->getWinSize();
	 _background->setAnchorPoint(ccp(0,0));
	 _background->setPosition(ccp(0, 0));
	// _background->setScale(0.5);
     ccTexParams tp = {GL_LINEAR, GL_LINEAR, GL_REPEAT, GL_REPEAT};
	 
	_background->getTexture()->setTexParameters(&tp);
	 this->addChild(_background,1);

	/*  bgColor.r=0.9;
	 bgColor.g=0.7;
	 bgColor.b=0.9;
	 bgColor.a=0.5;*/
	 _background2 = this->spriteWithColor(bgColor,512);
	 _background2->setPosition(ccp(512, 0));//
   	 _background2->setAnchorPoint(ccp(0,0));
	_background2->getTexture()->setTexParameters(&tp);
	 this->addChild(_background2,1);

	 CCSprite* _background3 = this->spriteWithColor(bgColor,512);
	 _background3->setPosition(ccp(1024, 0));
  	 _background3->setAnchorPoint(ccp(0,0));
	_background3->getTexture()->setTexParameters(&tp);
	 this->addChild(_background3,1);

	 CCSprite* _background4 = this->spriteWithColor(bgColor,512);
	 _background4->setPosition(ccp(1536, 0));
  	 _background4->setAnchorPoint(ccp(0,0));
	_background4->getTexture()->setTexParameters(&tp);
	 this->addChild(_background4,1);
 


	 //GRASS

	  bgColor.r=0.9;
	 bgColor.g=0.9;
	 bgColor.b=0.2;
	 bgColor.a=0.5;

    _grass = this->spriteWithColor2(bgColor,256,160);
	_grass->setAnchorPoint(ccp(0,0));
	_grass->setPosition(ccp(0,0));
	_grass->getTexture()->setTexParameters(&tp);
	 this->addChild(_grass,1);

	CCSprite* _grass2 = this->spriteWithColor2(bgColor,256,160);
	_grass2->setAnchorPoint(ccp(0,0));
	_grass2->setPosition(ccp(256,0));
	_grass2->getTexture()->setTexParameters(&tp);
	 this->addChild(_grass2,1);

	 CCSprite* _grass3 = this->spriteWithColor2(bgColor,256,160);
	_grass3->setAnchorPoint(ccp(0,0));
	_grass3->setPosition(ccp(512,0));
	_grass3->getTexture()->setTexParameters(&tp);
	 this->addChild(_grass3,1);

	  CCSprite* _grass4 = this->spriteWithColor2(bgColor,256,160);
	_grass4->setAnchorPoint(ccp(0,0));
	_grass4->setPosition(ccp(768,0));
	_grass4->getTexture()->setTexParameters(&tp);
	 this->addChild(_grass4,1);

	  CCSprite* _grass5 = this->spriteWithColor2(bgColor,256,160);
	_grass5->setAnchorPoint(ccp(0,0));
	_grass5->setPosition(ccp(1024,0));
	_grass5->getTexture()->setTexParameters(&tp);
	 this->addChild(_grass5,1);

	  CCSprite* _grass6 = this->spriteWithColor2(bgColor,256,160);
	_grass6->setAnchorPoint(ccp(0,0));
	_grass6->setPosition(ccp(1280,0));
	_grass6->getTexture()->setTexParameters(&tp);
	 this->addChild(_grass6,1);

	  CCSprite* _grass7 = this->spriteWithColor2(bgColor,256,160);
	_grass7->setAnchorPoint(ccp(0,0));
	_grass7->setPosition(ccp(1536,0));
	_grass7->getTexture()->setTexParameters(&tp);
	 this->addChild(_grass7,1);

	  CCSprite* _grass8 = this->spriteWithColor2(bgColor,256,160);
	_grass8->setAnchorPoint(ccp(0,0));
	_grass8->setPosition(ccp(1792,0));
	_grass8->getTexture()->setTexParameters(&tp);
	 this->addChild(_grass8,1);

	  CCSprite* _grass9 = this->spriteWithColor2(bgColor,256,160);
	_grass9->setAnchorPoint(ccp(0,0));
	_grass9->setPosition(ccp(2048,0));
	_grass9->getTexture()->setTexParameters(&tp);
	 this->addChild(_grass9,1);

	/*  CCSprite* _grass10 = this->spriteWithColor2(bgColor,256,160);
	_grass10->setAnchorPoint(ccp(0,0));
	_grass10->setPosition(ccp(1280,0));
	_grass10->getTexture()->setTexParameters(&tp);
	 this->addChild(_grass10,1);
*/

    ccColor4F color3 = this->randomBrightColor();
    ccColor4F color4 = this->randomBrightColor();

	CCSprite *sprite = CCSprite::create("Background1/mountoverlay2.png");
	sprite->setAnchorPoint(CCPointZero);
	sprite->setPosition(CCPointMake(0,0));
	addChild(sprite,2);

	//CCSprite* stripes=this->stripedSpriteWithColor1(color3, color4 ,512 ,4);
    //ccTexParams tp2 = {GL_LINEAR, GL_LINEAR, GL_REPEAT, GL_CLAMP_TO_EDGE};
	//cocos2d::CCTexture2D *tex= stripes->getTexture();
	//stripes->getTexture()->setTexParameters(&tp2);
	//int f=_terrain->getTag();
	//_terrain->setStripes(_background);

	
	
	
}
cocos2d::CCSprite* TextureBackground::spriteWithColor2(ccColor4F bgColor, float width, float height)
{
	// 1: Create new CCRenderTexture
	CCRenderTexture *rt = CCRenderTexture::create(width, height);

	// 2: Call CCRenderTexture:begin
	rt->beginWithClear(bgColor.r, bgColor.g, bgColor.b, bgColor.a);

	// 3: Draw into the texture
	glDisable(GL_TEXTURE_2D);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);

	float gradientAlpha = 0.7;    
	CCPoint vertices[4];
	ccColor4F colors[4];
	int nVertices = 0;
	
	ccColor4F color={0, 0, 0, 0};
	ccColor4F color1={0, 0, 0, gradientAlpha};

	vertices[nVertices] = CCPointMake(0, 0);
	colors[nVertices++] = color;
	vertices[nVertices] = CCPointMake(width, 0);
	colors[nVertices++] = color;
	vertices[nVertices] = CCPointMake(0, height);
	colors[nVertices++] = color1;
	vertices[nVertices] = CCPointMake(width,height);
	colors[nVertices++] = color1;

	glVertexPointer(2, GL_FLOAT, 0, vertices);
	glColorPointer(4, GL_FLOAT, 0, colors);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, (GLsizei)nVertices);

	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnable(GL_TEXTURE_2D);

	CCSprite *noise = CCSprite::create("Background1/Noise3.jpg");
	cocos2d::ccBlendFunc blendfunc={GL_DST_COLOR, GL_ZERO};
	//blendfunc.dst=GL_DST_COLOR;
	//blendfunc.src=GL_ZERO;
	noise->setBlendFunc(blendfunc);

	CCSize winSize = CCDirector::sharedDirector()->getWinSize();
	noise->setAnchorPoint(ccp(0,0));
	noise->setPosition(ccp(0, 0));
	noise->visit();

	// 4: Call CCRenderTexture:end
	rt->end();

	// 5: Create a new Sprite from the texture
	return CCSprite::createWithTexture(rt->getSprite()->getTexture());
}

void TextureBackground::ccTouchesBegan(cocos2d::CCSet* touches, cocos2d::CCEvent* event)
{
	//this->genBackground();
	//return true;
}

void TextureBackground::menuCloseCallback(CCObject* pSender)
{
	CCDirector::sharedDirector()->end();
}