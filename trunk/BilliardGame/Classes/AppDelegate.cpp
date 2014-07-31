//
//  MiniPoolAppDelegate.cpp
//  MiniPool
//
//  Created by Roger Engelbert on 1/13/13.
//  Copyright __MyCompanyName__ 2013. All rights reserved.
//

#include "AppDelegate.h"

#include "GameLayer.h"

USING_NS_CC;

using namespace CocosDenshion;

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
    
    //designed for the regular iphone
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
    
	SimpleAudioEngine::sharedEngine()->preloadEffect(CCFileUtils::sharedFileUtils()->fullPathForFilename("ball.wav").c_str());
	SimpleAudioEngine::sharedEngine()->preloadEffect(CCFileUtils::sharedFileUtils()->fullPathForFilename("drop.wav").c_str());
	SimpleAudioEngine::sharedEngine()->preloadEffect(CCFileUtils::sharedFileUtils()->fullPathForFilename("hit.wav").c_str());
	SimpleAudioEngine::sharedEngine()->preloadEffect(CCFileUtils::sharedFileUtils()->fullPathForFilename("whitedrop.wav").c_str());
    
    
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
