#ifndef __CONSTANTS_H__
#define __CONSTANTS_H__

// Constants.h


// Screen size
#define SCREEN_SIZE cocos2d::CCDirector::sharedDirector()->getWinSize()
#define SCREEN_WIDTH cocos2d::CCDirector::sharedDirector()->getWinSize().width
#define SCREEN_HEIGHT cocos2d::CCDirector::sharedDirector()->getWinSize().height

#define SCREEN_SIZE_PX cocos2d::CCDirector::sharedDirector()->getWinSizeInPixels()
#define SCREEN_WIDTH_PX cocos2d::CCDirector::sharedDirector()->getWinSizeInPixels().width
#define SCREEN_HEIGHT_PX cocos2d::CCDirector::sharedDirector()->getWinSizeInPixels().height

#define SOUND_MENU "Sound/Menu.mp3"

// Menu Scenes
#define PATH_SCORE "Score\\"
//////////////STATES //////////////////////////////
#define STATE_STAND_LEFT -1
#define STATE_STAND_RIGHT 0
#define STATE_WALK_LEFT 1
#define STATE_WALK_RIGHT 2
#define STATE_FIRE_LEFT 3
#define STATE_FIRE_RIGHT 4
#define STATE_KIll_RIGHT 5
#define STATE_KIll_LEFT 6

#define STATE_STOP 7
#define STATE_MOVE 8
#define STATE_WALK 9
#define STATE_FIRE 10
#define STATE_KILL 11
#define STATE_JUMP 11
////////////////PLAYER CONFIGURATION////
#define PLAYER1_JUMP 1.5
#define PLAYER2_JUMP 1.4
#define PLAYER3_JUMP 1.0

#define PLAYER1_WALK 2.5
#define PLAYER2_WALK 2.4
#define PLAYER3_WALK 2.0

#define PLAYER1_FIRE 150
#define PLAYER2_FIRE 300
#define PLAYER3_FIRE 300

///////LEVEL1 CONFIGURATION 
#define ENEMY_PLAYER_THRESHOLD_LEVEL1 200
#define ENEMY_VEHICLE_THRESHOLD_LEVEL1 350

#define ENEMY_IMPULSE_LEVEL1 1.5
#define ENEMY_ZOMBIE_IMPULSE_LEVEL1 2.0
#define ENEMY_VEHICLE_SPEED_LEVEL1 3.0

///////LEVEL2 CONFIGURATION 
#define ENEMY_PLAYER_THRESHOLD_LEVEL2 200
#define ENEMY_VEHICLE_THRESHOLD_LEVEL2 400

#define ENEMY_IMPULSE_LEVEL2 1.8
#define ENEMY_ZOMBIE_IMPULSE_LEVEL2 2.2
#define ENEMY_VEHICLE_SPEED_LEVEL2 4.0

///////LEVEL3 CONFIGURATION 
#define ENEMY_PLAYER_THRESHOLD_LEVEL3 200
#define ENEMY_VEHICLE_THRESHOLD_LEVEL3 400

#define ENEMY_IMPULSE_LEVEL3 1.6
#define ENEMY_ZOMBIE_IMPULSE_LEVEL3 2.2
#define ENEMY_VEHICLE_SPEED_LEVEL3 3.5

/////////////////////////////////////////

#define PTM_RATIO 32.0

#endif