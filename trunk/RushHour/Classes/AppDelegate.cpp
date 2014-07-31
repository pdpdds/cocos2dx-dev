//
//  Victorian_Rush_HourAppDelegate.cpp
//  Victorian Rush Hour
//
//  Created by Roger Engelbert on 11/17/12.
//  Copyright __MyCompanyName__ 2012. All rights reserved.
//

#include "AppDelegate.h"

#include "cocos2d.h"
#include "GameLayer.h"

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
    CCDirector* pDirector = CCDirector::sharedDirector();
    CCEGLView* pEGLView = CCEGLView::sharedOpenGLView();
    
    pDirector->setOpenGLView(pEGLView);
    
    CCSize screenSize = pEGLView->getFrameSize();
    CCSize designSize = CCSize(2048, 1536);
    
    CCEGLView::sharedOpenGLView()->setDesignResolutionSize(designSize.width, designSize.height, kResolutionExactFit);
	std::vector<std::string> searchPaths;

    if (screenSize.height > 768) {
		searchPaths.push_back("ipadhd");
		CCFileUtils::sharedFileUtils()->setSearchPaths(searchPaths);
        pDirector->setContentScaleFactor(1536/designSize.height);
    } else if (screenSize.height > 320) {
		searchPaths.push_back("ipad");
		CCFileUtils::sharedFileUtils()->setSearchPaths(searchPaths);
        pDirector->setContentScaleFactor(768/designSize.height);
    } else {
		searchPaths.push_back("iphone");
		CCFileUtils::sharedFileUtils()->setSearchPaths(searchPaths);
        pDirector->setContentScaleFactor(380/designSize.height);
    }
    
	SimpleAudioEngine::sharedEngine()->preloadBackgroundMusic(CCFileUtils::sharedFileUtils()->fullPathForFilename("background.mp3").c_str());
	SimpleAudioEngine::sharedEngine()->preloadEffect(CCFileUtils::sharedFileUtils()->fullPathForFilename("falling.wav").c_str());
	SimpleAudioEngine::sharedEngine()->preloadEffect(CCFileUtils::sharedFileUtils()->fullPathForFilename("hitBuilding.wav").c_str());
	SimpleAudioEngine::sharedEngine()->preloadEffect(CCFileUtils::sharedFileUtils()->fullPathForFilename("jump.wav").c_str());
	SimpleAudioEngine::sharedEngine()->preloadEffect(CCFileUtils::sharedFileUtils()->fullPathForFilename("crashing.wav").c_str());
	SimpleAudioEngine::sharedEngine()->preloadEffect(CCFileUtils::sharedFileUtils()->fullPathForFilename("start.wav").c_str());
	SimpleAudioEngine::sharedEngine()->preloadEffect(CCFileUtils::sharedFileUtils()->fullPathForFilename("openUmbrella.wav").c_str());
    
    SimpleAudioEngine::sharedEngine()->setBackgroundMusicVolume(0.5f);
    SimpleAudioEngine::sharedEngine()->setEffectsVolume(0.5f);
    
    
    // turn on display FPS
    pDirector->setDisplayStats(false);

    // set FPS. the default value is 1.0/60 if you don't call this
    pDirector->setAnimationInterval(1.0 / 60);

    // create a scene. it's an autorelease object
    CCScene *pScene = GameLayer::scene();

    // run
    pDirector->runWithScene(pScene);

    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground()
{
    CCDirector::sharedDirector()->pause();

    // if you use SimpleAudioEngine, it must be paused
    // SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground()
{
    CCDirector::sharedDirector()->resume();
    
    // if you use SimpleAudioEngine, it must resume here
    // SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
}
