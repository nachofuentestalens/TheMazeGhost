#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include "cocos2d.h"

class MazeGame : public cocos2d::Layer
{
public:

	struct LabeledCrowd {
		cocos2d::Sprite* sprite;
		cocos2d::Label* label;
		int crowdSize;
	};

	enum kLane {
		kLeftLane = 0,
		kMiddleLane,
		kRightLane
	};

	enum kReason {
		kCrashedWithWall = 0,
		kCrashedWithCrowd,
		kKilledPerson,
		kKilledCrowd,
		kElapsedTime,
	};

	enum kTutorial {
		kTutorial1 = 0,
		kTutorial2,
		kTutorialFinished
	};

	//CONSTS
	const float kAccelerometerSensitivity = 0.5f;
	const float kChangeLaneDuration = 0.4f;
	const int kInitialLives = 5;
	const float kInitialSpawnTime = 1.0f;
	const int kInitialMinCrowdSize = 1;
	const float kInitialObstacleSpeed = 3.0f;
	const float kInitialGhostMeter = 0.0f;
	const float kUpdateTick = 0.1f;
	const float kInitialLightSensorValue = 5.0f;
	const kLane kInitialLane = kMiddleLane;
	const int kBooFontSize = 64;
	//Dynamic difficulty
	const float kMinObstacleSpeed = 1.2f;
	const float kMaxObstacleSpeed = 5.0f;
	const float kMinSpawnTime = 0.4f;
	const float kMaxSpawnTime = 1.5f;
	const int kMaxCrowdSize = 5;
	const float kElapsedTimeToAdapt = 4.0f;
	const int kPeopleKillsToAdapt = 6;
	const int kCrowdKillsToAdapt = 3;
	const float kBlinkDuration = 0.25f;
	const int kBlinkNumber = 3;
	
    static cocos2d::Scene* createScene();

    virtual bool init();
    
	//Events
	void OnAcceleration(cocos2d::Acceleration* acc, cocos2d::Event* event);
	bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);


	//Scheduler
	void Update(float dt);
	//Adapt difficulty based on reason
	void AdaptDifficulty(kReason reason, float dt = 0.0f);

	//Called every update
	void CheckPlayerCollisions();

	//Label updaters
	void UpdateVictimsLabel();
	void UpdateLivesLabel();
	void UpdateGhostMeterLabel();
	void UpdateCrowdSprite(cocos2d::Sprite* sprite, int crowdSize);

	//Movement
	void MoveLeft();
	void MoveRight();
	void MovementFinished();

	//Spawn
	void Spawn();
	void SpawnWall();
	void SpawnCrowd(int number);

	//Callbacks
	void WallReachedBottom(void* data);
	void LeftReachedBottom(void* data);
	void MiddleReachedBottom(void* data);
	void RightReachedBottom(void* data);
	
	void GameLost();


	void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event * event);

	//Sprites and Labels
	cocos2d::Sprite* playerSprite;
	cocos2d::Sprite* tutorialSprite1;
	cocos2d::Sprite* tutorialSprite2;
	cocos2d::Sprite* exitConfirmSprite;
	cocos2d::Label* boo_1;
	cocos2d::Label* boo_2;
	cocos2d::Label* boo_3;
	cocos2d::Label* timerLabel;
	cocos2d::Label* victimsLabel;
	cocos2d::Label* livesLabel;
	cocos2d::Label* ghostMeterLabel;

	//Obstacles
	cocos2d::Vector<cocos2d::Sprite*> leftCrowds;
	cocos2d::Vector<cocos2d::Sprite*> middleCrowds;
	cocos2d::Vector<cocos2d::Sprite*> rightCrowds;
	cocos2d::Vector<cocos2d::Sprite*> walls;
	cocos2d::Vector<cocos2d::Sprite*> collisions;

	//Multiresolution purposes
	cocos2d::Vec2 origin;
	cocos2d::Size visibleSize;
	float scaleX;
	float scaleY;
	cocos2d::Size winSize;

	//Chached positions
	float boo_center;
	float boo_distance;
	float lane1X;
	float lane2X;
	float lane3X;
	float spawnY;
	float limitY;

	//Dynamic difficulty data
	//How often obstacles spawn
	float currentSpawnTime;
	//Increased size of the crowds
	int minimumCrowd;
	//How long the obstacle will take to reach its destiny
	float obstaclesSpeed;
	int peopleKilledSinceLastAdaptation;
	int crowdsKilledSinceLastAdaptation;
	float elapsedTimeSinceLastAdaptation;

	//Player Data
	int victims;
	int lives;
	bool ghostModeOn;
	float ghostMeter;
	kLane currentLane;
	bool switchingLane;
	float elapsedTime;
	float elapsedSpawnTime;
	float lightSensorValue;
	bool hasCompletedTutorial;
	bool wantsToExit;
	kTutorial tutorialStep;

    // implement the "static create()" method manually
    CREATE_FUNC(MazeGame);
};

#endif // __HELLOWORLD_SCENE_H__
