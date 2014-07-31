/*
 Soundtrack:
 Merry Go Slower
 by Kevin MacLeod (incompetech.com)
 
 */


#include "AppDelegate.h"

#include "cocos2d.h"
#include "GameLayer.h"
#include "MenuLayer.h"
#include "LevelSelectLayer.h"

USING_NS_CC;

AppDelegate::AppDelegate()
{

}

AppDelegate::~AppDelegate()
{
}

bool AppDelegate::applicationDidFinishLaunching()
{
    // initialize director
    // initialize director
    CCDirector* pDirector = CCDirector::sharedDirector();
    CCEGLView* pEGLView = CCEGLView::sharedOpenGLView();
    
    pDirector->setOpenGLView(pEGLView);
    
    CCSize screenSize = pEGLView->getFrameSize();
    CCSize designSize = CCSize(320, 480);
    
    CCEGLView::sharedOpenGLView()->setDesignResolutionSize(designSize.width, designSize.height, kResolutionShowAll);
	std::vector<std::string> searchPaths;

    if (screenSize.width > 640) {
		searchPaths.push_back("ipadhd");
		CCFileUtils::sharedFileUtils()->setSearchPaths(searchPaths);
        pDirector->setContentScaleFactor(1280/designSize.width);
    } else if (screenSize.width > 320) {
		searchPaths.push_back("ipad");
		CCFileUtils::sharedFileUtils()->setSearchPaths(searchPaths);
        pDirector->setContentScaleFactor(640/designSize.width);
    } else {
		searchPaths.push_back("iphone");
		CCFileUtils::sharedFileUtils()->setSearchPaths(searchPaths);
        pDirector->setContentScaleFactor(320/designSize.width);
    }
    
	SimpleAudioEngine::sharedEngine()->preloadBackgroundMusic(CCFileUtils::sharedFileUtils()->fullPathForFilename("background.mp3").c_str());
	SimpleAudioEngine::sharedEngine()->preloadEffect(CCFileUtils::sharedFileUtils()->fullPathForFilename("button.wav").c_str());
	SimpleAudioEngine::sharedEngine()->preloadEffect(CCFileUtils::sharedFileUtils()->fullPathForFilename("cap.wav").c_str());
	SimpleAudioEngine::sharedEngine()->preloadEffect(CCFileUtils::sharedFileUtils()->fullPathForFilename("igloo.wav").c_str());
	SimpleAudioEngine::sharedEngine()->preloadEffect(CCFileUtils::sharedFileUtils()->fullPathForFilename("oops.wav").c_str());
	SimpleAudioEngine::sharedEngine()->preloadEffect(CCFileUtils::sharedFileUtils()->fullPathForFilename("shape.wav").c_str());
	SimpleAudioEngine::sharedEngine()->preloadEffect(CCFileUtils::sharedFileUtils()->fullPathForFilename("switch.wav").c_str());
    
    SimpleAudioEngine::sharedEngine()->setBackgroundMusicVolume(0.5f);
    SimpleAudioEngine::sharedEngine()->setEffectsVolume(0.6f);
    
    // turn on display FPS
    pDirector->setDisplayStats(false);

    // set FPS. the default value is 1.0/60 if you don't call this
    pDirector->setAnimationInterval(1.0 / 60);

    // create a scene. it's an autorelease object
    
    CCScene *pScene = MenuLayer::scene();
    
    // run
    pDirector->runWithScene(pScene);

    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground()
{
    CCDirector::sharedDirector()->stopAnimation();

    // if you use SimpleAudioEngine, it must be paused
    // SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground()
{
    CCDirector::sharedDirector()->startAnimation();
    
    // if you use SimpleAudioEngine, it must resume here
    // SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
}
