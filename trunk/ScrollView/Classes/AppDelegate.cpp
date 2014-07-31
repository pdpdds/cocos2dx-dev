#include "cocos2d.h"
#include "CCEGLView.h"
#include "AppDelegate.h"
#include "GalleryLayer.h"
#include "SimpleAudioEngine.h"

using namespace CocosDenshion;

USING_NS_CC;

AppDelegate::AppDelegate()
{
}

AppDelegate::~AppDelegate()
{
    SimpleAudioEngine::end();
}

bool AppDelegate::applicationDidFinishLaunching()
{
 // initialize director
    CCDirector *pDirector = CCDirector::sharedDirector();
    pDirector->setOpenGLView(CCEGLView::sharedOpenGLView());

    CCSize screenSize = CCEGLView::sharedOpenGLView()->getFrameSize();

    CCSize designSize = CCSizeMake(960, 640);
    
    if (screenSize.height != 960)
    {
        CCSize resourceSize = CCSizeMake(960, 640);
        pDirector->setContentScaleFactor(resourceSize.height / designSize.height);
    }

    CCEGLView::sharedOpenGLView()->setDesignResolutionSize(designSize.width, designSize.height, kResolutionNoBorder);
    // set FPS. the default value is 1.0/60 if you don't call this
    pDirector->setAnimationInterval(1.0 / 60);

    CCScene *pScene = CCScene::create();
	CCLayer *pLayer = GalleryLayer::create();
	pScene->addChild(pLayer);

    pDirector->runWithScene(pScene);
    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground()
{
    CCDirector::sharedDirector()->stopAnimation();

    SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground()
{
    CCDirector::sharedDirector()->startAnimation();

    SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
}
