
#ifndef __LEVEL_GENERATOR_H__
#define __LEVEL_GENERATOR_H__

#include "cocos2d.h"
#include "Box2D/Box2D.h"

#include "Scenes/LevelComplete/GameOverScene.h"
#include "Scenes/LevelComplete/WinnerScene.h"
#include "SimpleAudioEngine.h"
#include "Scenes/MainMenu/MainScene.h"
#include "Singletons/GLES-Render.h"
#include "Singletons/CCParallaxNodeExtras.h"
#include "KeyboardClasses/Input.h"
#include "Singletons/GB2ShapeCache-x.h"

#include "Scenes/Levels/LevelGeneratorHud.h"
#include "Scenes/Levels/BackgroundLayer/BGScene.h"

#include "JoystickClasses\SneakyButton.h"
#include "JoystickClasses\SneakyButtonSkinnedBase.h"
#include "JoystickClasses\SneakyJoystick.h"
#include "JoystickClasses\SneakyJoystickSkinnedBase.h"
#include "Constants\Constants.h"
#include "BackgroundLayer/TextureBackground.h"

USING_NS_CC ;

class LevelGenerator : public cocos2d::CCLayer ,  public b2ContactListener  {
public:
    ~LevelGenerator();
    virtual bool init(); 
    static cocos2d::CCScene* scene();
    void tick(float dt);
	void update(float dt);
	void spriteMoveFinished(CCNode* sender);
	void GameSetup();
	void gameOver();
	void gameWin();
	void FireTruck();
	void FireHelicopter();
	void FireJeep();
	void FireTank();
	void FireEnemyPlayer();
    void FireEnemyZombiePlayer();
	void AddPlayers(CCPoint position);
	void AddGoldenPlayers(CCPoint position);
	void AddBrownPlayers(CCPoint position);
	void AddRocks(CCPoint position);
	void AddPowers(CCPoint position);
	void AddPower(CCPoint position);
	void AddHelicopters(CCPoint position);
	void AddJeeps(CCPoint position);
	void AddTanks(CCPoint position);
	void AddTrucks(CCPoint position);
    void AddEnemyPlayers(CCPoint position);
	void AddEnemyZombiePlayers(CCPoint position);
	void AddBaseCamp(CCPoint position);
	void AddLongRocks(CCPoint position);
	void AddWoods(CCPoint position);
	void AddBridge(CCPoint position);
	void AddLongFixRocks(CCPoint position);
	void updateTimer(float dt);
	void generateMap();
	void createBackground();
	void updateScore(int score);
	void updateKeyBoard(float dt);
	void updateEnemyPostion(float dt);
	void updateHelicopter(float dt);
	void spawnEnemies(float dt);
	void updateTruck(float dt);
	void updateFire(float dt);
	float ptm(float value);
    void AddRagDollPlayers(CCPoint position);
	void FireExplosionAnimation(CCPoint position);
	void VehicleExplosionAnimation(CCPoint position);
	void RainAnimation(CCPoint position );
	void GeneratePlayerAnimation(char *Name,int Max,cocos2d::CCSprite* sp);
	void ScoreAnimation(CCPoint position,const char* imgName,const char* imgName2 );
	void ExplosionAnimation(CCPoint position,const char* imgName,int Max  );
	/*virtual void draw();*/

	virtual void ccTouchesBegan(cocos2d::CCSet* touches, cocos2d::CCEvent* events);
	virtual void ccTouchesMoved(cocos2d::CCSet* touches, cocos2d::CCEvent* events);
	virtual void ccTouchesEnded(cocos2d::CCSet* touches, cocos2d::CCEvent* events);
    NODE_FUNC(LevelGenerator);
	b2Body* _playerBody;
	b2Body* _enemyPlayerBody;
    b2Body* _enemyZombiePlayerBody;
	b2Body* _helicopterBody;
	b2Body* _fanBody;


	CC_SYNTHESIZE(LevelGeneratorHud *, _hud, Hud);
	CC_SYNTHESIZE(BG*, Blayer, BGLayer);
private:
	
	int Id_JeepSound;
	int Id_TruckSound;
	int scoreCount;
	int playerType;
	
	int health;
	int healthJeep;
	int healthTruck;
	int healthTank;
	int EnemyHealth;
	int EnemyZombieHealth;
	int state_Player;

	


	//int JeepDestroyedCount;
	int EnemyDestroyedCount;
	int ZombieDestroyedCount;
	//int TruckDestroyedCount;
	//int HeliDestroyedCount;
	int VehicleDestroyedCount;
	bool isPlayerLive;
	bool isEnemyFire;
	bool isEnemyZombieFire;
	bool isEnemyKill;
	bool isEnemyZombieKill;
	bool isPowerGive;
	bool heliSoundOn;
	TextureBackground *tex;

	CCSpriteBatchNode *sceneSpriteBatchNode;

	//LevelGeneratorContactListener* _contact;
	GLESDebugDraw* m_debugDraw;
	cocos2d::CCSprite* spExplosion; 
	//cocos2d::CCLabelTTF* _lblScore;

	CCParallaxNodeExtras *_backgroundNode;  
    cocos2d::CCSprite *_cloud1;
    cocos2d::CCSprite *_cloud2;
	cocos2d::CCSprite *_cloud3;
    cocos2d::CCSprite *_cloud4;

	cocos2d::CCSprite *_bird1;
    cocos2d::CCSprite *_bird2;
	cocos2d::CCSprite *_bird3;
    cocos2d::CCSprite *_bird4;

	 cocos2d::CCSprite *_Player;
	 cocos2d::CCSprite *_EnemyPlayer;
	 cocos2d::CCSprite *_EnemyZombiePlayer;
	 cocos2d::CCSprite *_Helicopter;
	 

	int birdindex;


    b2World* _world;
	

	b2Body* _body;

	std::vector<b2Body*>* _powers;
	std::vector<b2Body*>* _rocks;
	std::vector<b2Body*>* _woods;
	std::vector<b2Body*>* _enemyzombieplayers;
	std::vector<b2Body*>* _enemyplayers;
	std::vector<b2Body*>* _players;
	std::vector<b2Body*>* _bullets;
	std::vector<b2Body*>* _helicopters;
	std::vector<b2Body*>* _jeeps;
	std::vector<b2Body*>* _tanks;
	std::vector<b2Body*>* _baseCamp;
	std::vector<b2Body*>* _tnt;
	std::vector<b2Body*>* _trucks;

	b2Body* _baseCampBody;
	b2Body* _playerBulletBody;
    b2Body* _EnemyBulletBody;
	b2Body* _TNTBody;
	b2Body* _rockBody;
	b2Body* _woodBody;
	b2Body* _powerBody;

	b2Body *torso1;

	b2Body* m_car;
	b2Body* m_wheel1;
	b2Body* m_wheel2;

	//float32 m_hz;
	//float32 m_zeta;
	//float32 m_speed;
	b2WheelJoint* m_spring1;
	b2WheelJoint* m_spring2;


	b2Body* m_truck;
	b2Body* truck_wheel1;
	b2Body* truck_wheel2;
	b2Body* truck_wheel3;
	b2Body* truck_wheel4;


	b2WheelJoint* truck_spring1;
	b2WheelJoint* truck_spring2;
	b2WheelJoint* truck_spring3;
	b2WheelJoint* truck_spring4;

	b2Body* m_tank;
	b2Body* tank_wheel1;
	b2Body* tank_wheel2;
	b2Body* tank_wheel3;
	b2Body* tank_wheel4;
	b2Body* tank_wheel5;

	b2WheelJoint* tank_spring1;
	b2WheelJoint* tank_spring2;
	b2WheelJoint* tank_spring3;
	b2WheelJoint* tank_spring4;
	b2WheelJoint* tank_spring5;


	SneakyJoystick *leftJoystick;
	SneakyButton *jumpButton;
	SneakyButton *attackButton;

	CC_SYNTHESIZE(SneakyJoystick *, joystick, Joystick);
	void applyJoystick(SneakyJoystick *aJoystick, float deltaTime);
	void initJoystickAndButtons();
	void preloadSounds();
	void playHeliSound();

 ////////////////////////////////////////////////////////////////////////

	  std::set<b2Body*>contacts;
    

  
    virtual void BeginContact(b2Contact* contact);
    virtual void EndContact(b2Contact* contact);
    virtual void PreSolve(b2Contact* contact, const b2Manifold* oldManifold);    
    virtual void PostSolve(b2Contact* contact, const b2ContactImpulse* impulse);

	
};
class LevelGeneratorScene : public cocos2d::CCScene
{
public:
   LevelGeneratorScene():_layer(NULL) {};
	 //static cocos2d::CCScene* scene();
	//LevelGeneratorScene(int playertype);
    ~LevelGeneratorScene();
    bool init();
	void setPlayerType(int type,int level);
    NODE_FUNC( LevelGeneratorScene);
	//  SCENE_NODE_FUNC( LevelGeneratorScene(int playertype));
    CC_SYNTHESIZE_READONLY(LevelGenerator*, _layer, Layer);
	CC_SYNTHESIZE(int, pt, PT);
	CC_SYNTHESIZE(int, lt, LT);
};

#endif // __HELLO_WORLD_H__
