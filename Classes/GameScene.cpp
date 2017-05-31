#include "GameScene.h"
#include "GameOverScene.h"

USING_NS_CC;

static float value;

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
extern "C" {
	void Java_org_cocos2dx_cpp_AppActivity_UpdateLightSensorValue(JNIEnv* env, jobject thiz, jfloat v) {
		/*EventCustom event("UpdateSensor");
		event.setUserData(v);*/
		//_eventDispatcher->dispatchEvent(&event);
		value = v;
	}
}
#endif

Scene* MazeGame::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = MazeGame::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool MazeGame::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Layer::init())
	{
		return false;
	}

	//Enable Keypad
	this->setKeypadEnabled(true);

	//Generate random seed
	srand(time(NULL));

	//Enable accelerometer
	Device::setAccelerometerEnabled(true);

	//Create Accelerometer listener
	auto acceletometerListener = EventListenerAcceleration::create(CC_CALLBACK_2(MazeGame::OnAcceleration, this));
	_eventDispatcher->addEventListenerWithSceneGraphPriority(acceletometerListener, this);

	//Create Touch listener
	auto touchListener = EventListenerTouchOneByOne::create();
	touchListener->onTouchBegan = CC_CALLBACK_2(MazeGame::onTouchBegan, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);

	//Create Timer to update sensor
	this->schedule(schedule_selector(MazeGame::Update), kUpdateTick);
	elapsedTime = 0;

	/////////////////////////////
	// 3. add your codes below...

	visibleSize = Director::getInstance()->getVisibleSize();
	origin = Director::getInstance()->getVisibleOrigin();
	winSize = Director::getInstance()->getWinSize();

	scaleX = visibleSize.width / 480.0f;
	scaleY = visibleSize.height / 800.0f;

	//Create background
	auto background = Sprite::create("Background.png");
	background->setScaleX(winSize.width / background->getContentSize().width);
	background->setScaleY(winSize.height / background->getContentSize().height);
	background->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
	this->addChild(background, 0);

	//add labels with Boo
	boo_distance = visibleSize.width / 3.0f;
	boo_center = boo_distance / 2.0f;
	lane1X = origin.x + boo_distance - boo_center;
	lane2X = origin.x + boo_distance * 2.0f - boo_center;
	lane3X = origin.x + boo_distance * 3.0f - boo_center;

	boo_1 = Label::createWithTTF("Boo", "fonts/Marker Felt.ttf", kBooFontSize);
	boo_1->setPosition(Vec2(lane1X, origin.y + winSize.height*0.05f));
	this->addChild(boo_1, 1);

	boo_2 = Label::createWithTTF("Boo", "fonts/Marker Felt.ttf", kBooFontSize);
	boo_2->setPosition(Vec2(lane2X, origin.y + winSize.height*0.05f));
	this->addChild(boo_2, 1);

	boo_3 = Label::createWithTTF("Boo", "fonts/Marker Felt.ttf", kBooFontSize);
	boo_3->setPosition(Vec2(lane3X, origin.y + winSize.height*0.05f));
	this->addChild(boo_3, 1);

	boo_1->setColor(Color3B(0, 0, 0));
	boo_2->setColor(Color3B(0, 0, 0));
	boo_3->setColor(Color3B(0, 0, 0));

	//Create Timer Label
	timerLabel = Label::createWithTTF("Time: 0s", "fonts/Marker Felt.ttf", 20);
	timerLabel->setPosition(Vec2(lane3X, origin.y + winSize.height*0.97f));
	this->addChild(timerLabel, 1);
	timerLabel->setColor(Color3B(0, 0, 0));

	//Create Victims Label
	victimsLabel = Label::createWithTTF("Score: 0", "fonts/Marker Felt.ttf", 20);
	victimsLabel->setPosition(Vec2(lane3X, origin.y + winSize.height*0.93f));
	this->addChild(victimsLabel, 1);
	victimsLabel->setColor(Color3B(0, 0, 0));

	//Create Lives Icon
	auto livesIcon = Sprite::create("Heart.png");
	livesIcon->setScale(3.0f);
	livesIcon->setPosition(Vec2(lane1X - livesIcon->getContentSize().width,
		origin.y + winSize.height - 40.0f * scaleY));
	this->addChild(livesIcon, 1);

	//Create Lives Label
	livesLabel = Label::createWithTTF("x ", "fonts/Marker Felt.ttf", 20);
	livesLabel->setPosition(Vec2(lane1X + livesIcon->getContentSize().width * 2.0f,
		origin.y + winSize.height*0.95f));
	this->addChild(livesLabel, 1);
	livesLabel->setColor(Color3B(0, 0, 0));
	

	//Create Ghost Meter Label
	ghostMeterLabel = Label::createWithTTF("Ghost Meter: 0", "fonts/Marker Felt.ttf", 20);
	ghostMeterLabel->setPosition(Vec2(lane2X,
		origin.y + winSize.height*0.95f));
	this->addChild(ghostMeterLabel, 1);
	ghostMeterLabel->setColor(Color3B(0, 0, 0));

	//Create Player Icon
	playerSprite = Sprite::create("ghost.png");
	playerSprite->setScale(2.0f);
	playerSprite->setPosition(Vec2(lane2X,
		origin.y + winSize.height*0.15f));
	this->addChild(playerSprite, 1);

	//Initialize variables
	ghostModeOn = false;
	currentLane = kInitialLane;
	value = kInitialLightSensorValue;
	spawnY = origin.y + winSize.height*0.87f;
	limitY = origin.y + winSize.height*0.14f;
	peopleKilledSinceLastAdaptation = 0;
	crowdsKilledSinceLastAdaptation = 0;
	elapsedTimeSinceLastAdaptation = 0.0f;
	wantsToExit = false;

	ghostMeter = kInitialGhostMeter;
	lives = kInitialLives;

	//Dynamic difficulty
	//Easy mode
	currentSpawnTime = kInitialSpawnTime;
	minimumCrowd = kInitialMinCrowdSize;
	obstaclesSpeed = kInitialObstacleSpeed;

	////Hardest mode
	//currentSpawnTime = kMinSpawnTime;
	//minimumCrowd = kMaxCrowdSize;
	//obstaclesSpeed = kMinObstacleSpeed;

	//Update labels
	UpdateLivesLabel();

	hasCompletedTutorial = UserDefault::getInstance()->getBoolForKey("Tutorial");

	if (!hasCompletedTutorial) {
		Director::getInstance()->pause();
		tutorialSprite1 = Sprite::create("Tutorial1.png");
		tutorialSprite1->setScaleX(winSize.width / tutorialSprite1->getContentSize().width);
		tutorialSprite1->setScaleY(winSize.height / tutorialSprite1->getContentSize().height);
		tutorialSprite1->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
		this->addChild(tutorialSprite1, 0);
		tutorialStep = kTutorial1;
	}
	else {
		tutorialStep = kTutorialFinished;
	}

	return true;
}

void MazeGame::OnAcceleration(Acceleration* acc, Event* event) {
	//Left
	if (acc->x <= -(kAccelerometerSensitivity) && !switchingLane && currentLane != kLeftLane) {
		switchingLane = true;
		MoveLeft();
	}
	//Right
	if (acc->x >= kAccelerometerSensitivity && !switchingLane && currentLane != kRightLane) {
		switchingLane = true;
		MoveRight();
	}
}

void MazeGame::Update(float dt) {

	//Update Light Sensor
	lightSensorValue = value;
	//near
	if (lightSensorValue == 0 && !ghostModeOn && ghostMeter > 1) {
		playerSprite->setOpacity(120.0f);
		ghostModeOn = true;
	}
	//far
	if ((lightSensorValue != 0 && ghostModeOn) || ghostMeter < dt) {
		playerSprite->setOpacity(255.0f);
		ghostModeOn = false;
	}
	//Ghost Mode
	if (ghostModeOn && ghostMeter > dt) {
		ghostMeter -= dt;
	}
	//No Ghost Mode
	if (!ghostModeOn && ghostMeter<5) {
		ghostMeter += dt;
	}
	//Update Ghost Meter
	UpdateGhostMeterLabel();
	//Check Spawn
	elapsedSpawnTime += dt;
	if (elapsedSpawnTime >= currentSpawnTime) {
		Spawn();
		elapsedSpawnTime = 0.0f;
	}
	//Check player collisions
	CheckPlayerCollisions();
	//Update timer
	elapsedTime += dt;
	char temp[40];
	sprintf(temp, "Time: %.0fs", elapsedTime);
	//sprintf(temp, "OS: %.2fs", obstaclesSpeed);
	timerLabel->setString(temp);

	//Adapt Difficulty
	AdaptDifficulty(kElapsedTime, dt);
}

void MazeGame::AdaptDifficulty(MazeGame::kReason reason, float dt) {
	switch (reason) {
	case kCrashedWithWall:
		obstaclesSpeed = clampf(obstaclesSpeed + 0.2f, kMinObstacleSpeed, kMaxObstacleSpeed);
		currentSpawnTime = clampf(currentSpawnTime + 0.1f, kMinSpawnTime, kMaxSpawnTime);
		break;
	case kCrashedWithCrowd:
		obstaclesSpeed = clampf(obstaclesSpeed + 0.2f, kMinObstacleSpeed, kMaxObstacleSpeed);
		currentSpawnTime = clampf(currentSpawnTime + 0.1f, kMinSpawnTime, kMaxSpawnTime);
		break;
	case kKilledPerson:
		peopleKilledSinceLastAdaptation++;
		break;
	case kKilledCrowd:
		crowdsKilledSinceLastAdaptation++;
		break;
	case kElapsedTime: elapsedTimeSinceLastAdaptation += dt;
		break;
	}
	if (peopleKilledSinceLastAdaptation >= kPeopleKillsToAdapt) {
		obstaclesSpeed = clampf(obstaclesSpeed - 0.1f, kMinObstacleSpeed, kMaxObstacleSpeed);
		currentSpawnTime = clampf(currentSpawnTime - 0.1f, kMinSpawnTime, kMaxSpawnTime);
		peopleKilledSinceLastAdaptation = 0;
		elapsedTimeSinceLastAdaptation = 0.0f;
	}
	if (crowdsKilledSinceLastAdaptation >= kCrowdKillsToAdapt) {
		minimumCrowd = clampf(minimumCrowd + 1, kInitialMinCrowdSize, kMaxCrowdSize);
		crowdsKilledSinceLastAdaptation = 0;
	}
	if (elapsedTimeSinceLastAdaptation >= kElapsedTimeToAdapt) {
		obstaclesSpeed = clampf(obstaclesSpeed - 0.1f, kMinObstacleSpeed, kMaxObstacleSpeed);
		elapsedTimeSinceLastAdaptation = 0.0f;
	}
}

void MazeGame::UpdateVictimsLabel() {
	char temp[40];
	sprintf(temp, "Score: %d", victims);
	victimsLabel->setString(temp);
}

void MazeGame::UpdateGhostMeterLabel() {
	char temp[40];
	sprintf(temp, "Ghost Meter: %.0f", ghostMeter);
	ghostMeterLabel->setString(temp);
}

void MazeGame::UpdateLivesLabel() {
	char temp[40];
	sprintf(temp, "x %d", lives);
	livesLabel->setString(temp);
}

void MazeGame::UpdateCrowdSprite(Sprite* sprite, int crowdSize) {
	if (crowdSize >= 0 && crowdSize < 3) {
		sprite->setTexture(CCTextureCache::sharedTextureCache()->addImage("SmallCrowd.png"));
	}
	if (crowdSize >= 3 && crowdSize < 5) {
		sprite->setTexture(CCTextureCache::sharedTextureCache()->addImage("MediumCrowd.png"));
	}
	if (crowdSize >= 5) {
		sprite->setTexture(CCTextureCache::sharedTextureCache()->addImage("BigCrowd.png"));
	}
}

bool MazeGame::onTouchBegan(Touch* touch, Event* event) {
	//Spooky Sounds effect + Atttack that lane
	//Left lane
	if (boo_1->getBoundingBox().containsPoint(touch->getLocation())) {
		if (leftCrowds.size() > 0) {
			int crowdSize = atoi(((Label*)(leftCrowds.at(0)->getChildByTag(2)))->getString().c_str());
			crowdSize--;
			if (crowdSize > 0) {
				char temp[40];
				sprintf(temp, "%d", crowdSize);
				((Label*)(leftCrowds.at(0)->getChildByTag(2)))->setString(temp);
				AdaptDifficulty(kKilledPerson);
				UpdateCrowdSprite(leftCrowds.at(0), crowdSize);
			}
			else {
				leftCrowds.at(0)->removeAllChildrenWithCleanup(true);
				leftCrowds.at(0)->removeFromParentAndCleanup(true);
				leftCrowds.erase(0);
				AdaptDifficulty(kKilledCrowd);
			}
			victims++;
			UpdateVictimsLabel();
		}
	}
	//Middle lane
	if (boo_2->getBoundingBox().containsPoint(touch->getLocation())) {
		if (middleCrowds.size() > 0) {
			int crowdSize = atoi(((Label*)(middleCrowds.at(0)->getChildByTag(2)))->getString().c_str());
			crowdSize--;
			if (crowdSize > 0) {
				char temp[40];
				sprintf(temp, "%d", crowdSize);
				((Label*)(middleCrowds.at(0)->getChildByTag(2)))->setString(temp);
				AdaptDifficulty(kKilledPerson);
				UpdateCrowdSprite(middleCrowds.at(0), crowdSize);
			}
			else {
				middleCrowds.at(0)->removeAllChildrenWithCleanup(true);
				middleCrowds.at(0)->removeFromParentAndCleanup(true);
				middleCrowds.erase(0);
				AdaptDifficulty(kKilledCrowd);
			}
			victims++;
			UpdateVictimsLabel();
		}
	}
	//Right lane
	if (boo_3->getBoundingBox().containsPoint(touch->getLocation())) {
		if (rightCrowds.size() > 0) {
			int crowdSize = atoi(((Label*)(rightCrowds.at(0)->getChildByTag(2)))->getString().c_str());
			crowdSize--;
			if (crowdSize > 0) {
				char temp[40];
				sprintf(temp, "%d", crowdSize);
				((Label*)(rightCrowds.at(0)->getChildByTag(2)))->setString(temp);
				AdaptDifficulty(kKilledPerson);
				UpdateCrowdSprite(rightCrowds.at(0), crowdSize);
			}
			else {
				rightCrowds.at(0)->removeAllChildrenWithCleanup(true);
				rightCrowds.at(0)->removeFromParentAndCleanup(true);
				rightCrowds.erase(0);
				AdaptDifficulty(kKilledCrowd);
			}
			victims++;
			UpdateVictimsLabel();
		}
	}
	if (wantsToExit) {
		wantsToExit = false;
		exitConfirmSprite->removeFromParentAndCleanup(true);
		if (hasCompletedTutorial) {
			cocos2d::Director::getInstance()->resume();
		}
	}
	else {
		if (!hasCompletedTutorial) {
			switch (tutorialStep) {
			case kTutorial1:
				tutorialSprite2 = Sprite::create("Tutorial2.png");
				tutorialSprite2->setScaleX(winSize.width / tutorialSprite2->getContentSize().width);
				tutorialSprite2->setScaleY(winSize.height / tutorialSprite2->getContentSize().height);
				tutorialSprite2->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
				this->addChild(tutorialSprite2, 0);
				tutorialSprite1->removeFromParentAndCleanup(true);
				tutorialStep = kTutorial2;
				break;
			case kTutorial2:
				tutorialSprite2->removeFromParentAndCleanup(true);
				tutorialStep = kTutorialFinished;
				UserDefault::getInstance()->setBoolForKey("Tutorial", true);
				Director::getInstance()->resume();
				hasCompletedTutorial = true;
				break;
			}
		}
	}
	return true;
}

void MazeGame::MoveLeft() {
	//From middle to left
	if (currentLane == kMiddleLane) {
		auto move_left = MoveTo::create(kChangeLaneDuration, Vec2(lane1X, playerSprite->getPositionY()));
		auto move_callback = CallFunc::create(CC_CALLBACK_0(MazeGame::MovementFinished, this));
		auto seq = Sequence::create(move_left, move_callback, nullptr);
		currentLane = kLeftLane;
		playerSprite->runAction(seq);
	}
	//From right to middle
	if (currentLane == kRightLane) {
		auto move_left2 = MoveTo::create(kChangeLaneDuration, Vec2(lane2X, playerSprite->getPositionY()));
		auto move_callback2 = CallFunc::create(CC_CALLBACK_0(MazeGame::MovementFinished, this));
		auto seq2 = Sequence::create(move_left2, move_callback2, nullptr);
		currentLane = kMiddleLane;
		playerSprite->runAction(seq2);
	}
}

void MazeGame::MoveRight() {
	//From middle to right
	if (currentLane == kMiddleLane) {
		auto move_right = MoveTo::create(kChangeLaneDuration, Vec2(lane3X, playerSprite->getPositionY()));
		auto move_callback = CallFunc::create(CC_CALLBACK_0(MazeGame::MovementFinished, this));
		auto seq = Sequence::create(move_right, move_callback, nullptr);
		currentLane = kRightLane;
		playerSprite->runAction(seq);
	}
	//From left to middle
	if (currentLane == kLeftLane) {
		auto move_right2 = MoveTo::create(kChangeLaneDuration, Vec2(lane2X, playerSprite->getPositionY()));
		auto move_callback2 = CallFunc::create(CC_CALLBACK_0(MazeGame::MovementFinished, this));
		auto seq2 = Sequence::create(move_right2, move_callback2, nullptr);
		currentLane = kMiddleLane;
		playerSprite->runAction(seq2);
	}
}

void MazeGame::MovementFinished() {
	switchingLane = false;
}

void MazeGame::Spawn() {
	int chance = rand() % 4;
	if (chance == 0) {
		SpawnWall();
	}
	else {
		SpawnCrowd(chance);
	}
}

void MazeGame::SpawnWall() {
	Sprite* wall = Sprite::create("Wall3.png");
	wall->setScale(3.5f);
	int chance = rand() % 3;
	//Left lane
	if (chance == 0) {
		wall->setPosition(Vec2(lane1X, spawnY));
		auto move_bottom = MoveTo::create(obstaclesSpeed, Vec2(wall->getPositionX(), limitY));
		auto move_callback = CallFunc::create(CC_CALLBACK_0(MazeGame::WallReachedBottom, this, (void*)wall));
		auto seq = Sequence::create(move_bottom, move_callback, nullptr);
		wall->runAction(seq);
	}
	//Middle lane
	if (chance == 1) {
		wall->setPosition(Vec2(lane2X, spawnY));
		auto move_bottom = MoveTo::create(obstaclesSpeed, Vec2(wall->getPositionX(), limitY));
		auto move_callback2 = CallFunc::create(CC_CALLBACK_0(MazeGame::WallReachedBottom, this, (void*)wall));
		auto seq2 = Sequence::create(move_bottom, move_callback2, nullptr);
		wall->runAction(seq2);
	}
	//Right lane
	if (chance == 2) {
		wall->setPosition(Vec2(lane3X, spawnY));
		auto move_bottom = MoveTo::create(obstaclesSpeed, Vec2(wall->getPositionX(), limitY));
		auto move_callback3 = CallFunc::create(CC_CALLBACK_0(MazeGame::WallReachedBottom, this, (void*)wall));
		auto seq3 = Sequence::create(move_bottom, move_callback3, nullptr);
		wall->runAction(seq3);
	}
	this->addChild(wall, 1);
	walls.pushBack(wall);
}

void MazeGame::SpawnCrowd(int number) {
	LabeledCrowd* labeledCrowd = new LabeledCrowd;
	labeledCrowd->crowdSize = number + minimumCrowd;
	labeledCrowd->sprite = Sprite::create("SmallCrowd.png");
	UpdateCrowdSprite(labeledCrowd->sprite, labeledCrowd->crowdSize);
	labeledCrowd->sprite->setScale(1.5f);
	char tmp[40];
	sprintf(tmp, "%d", labeledCrowd->crowdSize);
	labeledCrowd->label = Label::createWithTTF(tmp, "fonts/Marker Felt.ttf", 16);
	//->label->setColor(Color3B(255, 0, 0));

	int chance = rand() % 3;
	//Left
	if (chance == 0) {
		labeledCrowd->sprite->setPosition(Vec2(lane1X, spawnY));
		auto move_bottom = MoveTo::create(obstaclesSpeed, Vec2(labeledCrowd->sprite->getPositionX(), limitY));
		auto move_callback = CallFunc::create(CC_CALLBACK_0(MazeGame::LeftReachedBottom, this, (void*)labeledCrowd->sprite));
		auto seq = Sequence::create(move_bottom, move_callback, nullptr);
		labeledCrowd->sprite->runAction(seq);
		leftCrowds.pushBack(labeledCrowd->sprite);
	}
	//Midle
	if (chance == 1) {
		labeledCrowd->sprite->setPosition(Vec2(lane2X, spawnY));
		auto move_bottom = MoveTo::create(obstaclesSpeed, Vec2(labeledCrowd->sprite->getPositionX(), limitY));
		auto move_callback = CallFunc::create(CC_CALLBACK_0(MazeGame::MiddleReachedBottom, this, (void*)labeledCrowd->sprite));
		auto seq = Sequence::create(move_bottom, move_callback, nullptr);
		labeledCrowd->sprite->runAction(seq);
		middleCrowds.pushBack(labeledCrowd->sprite);
	}
	//Right
	if (chance == 2) {
		labeledCrowd->sprite->setPosition(Vec2(lane3X, spawnY));
		auto move_bottom = MoveTo::create(obstaclesSpeed, Vec2(labeledCrowd->sprite->getPositionX(), limitY));
		auto move_callback = CallFunc::create(CC_CALLBACK_0(MazeGame::RightReachedBottom, this, (void*)labeledCrowd->sprite));
		auto seq = Sequence::create(move_bottom, move_callback, nullptr);
		labeledCrowd->sprite->runAction(seq);
		rightCrowds.pushBack(labeledCrowd->sprite);
	}
	labeledCrowd->label->setPosition(Vec2(labeledCrowd->sprite->getContentSize().width / 2, labeledCrowd->label->getContentSize().height / 2));
	labeledCrowd->label->setTag(2);
	labeledCrowd->sprite->addChild(labeledCrowd->label);

	this->addChild(labeledCrowd->sprite, 1);
}

void MazeGame::LeftReachedBottom(void* data) {
	Sprite* sprite = (Sprite*)data;
	//Remove from collisions vector if necessary
	if (collisions.contains(sprite)) {
		collisions.eraseObject(sprite, true);
	}
	//Cleanup
	sprite->removeAllChildrenWithCleanup(true);
	sprite->removeFromParentAndCleanup(true);
	//Remove from vector
	leftCrowds.erase(0);
}

void MazeGame::MiddleReachedBottom(void* data) {
	Sprite* sprite = (Sprite*)data;
	//Remove from collisions vector if necessary
	if (collisions.contains(sprite)) {
		collisions.eraseObject(sprite, true);
	}
	//Cleanup
	sprite->removeAllChildrenWithCleanup(true);
	sprite->removeFromParentAndCleanup(true);
	//Remove from vector
	middleCrowds.erase(0);
}

void MazeGame::RightReachedBottom(void* data) {
	Sprite* sprite = (Sprite*)data;
	//Remove from collisions vector if necessary
	if (collisions.contains(sprite)) {
		collisions.eraseObject(sprite, true);
	}
	//Cleanup
	sprite->removeAllChildrenWithCleanup(true);
	sprite->removeFromParentAndCleanup(true);
	//Remove from vector
	rightCrowds.erase(0);
}

void MazeGame::WallReachedBottom(void* data) {
	Sprite* sprite = (Sprite*)data;
	//Remove from collisions vector if necessary
	if (collisions.contains(sprite)) {
		collisions.eraseObject(sprite, true);
	}
	//Cleanup
	sprite->removeAllChildrenWithCleanup(true);
	sprite->removeFromParentAndCleanup(true);
	//Remove from vector
	walls.erase(0);
}

void MazeGame::CheckPlayerCollisions() {
	if (!ghostModeOn) {
		Rect player = playerSprite->getBoundingBox();
		Rect obstacle;
		bool collided = false;
		//Check collisions with left lane enemies
		for (unsigned int i = 0; i < leftCrowds.size(); ++i) {
			if (!collisions.contains(leftCrowds.at(i))) {
				obstacle = leftCrowds.at(i)->getBoundingBox();
				if (player.intersectsRect(obstacle)) {
					lives--;
					UpdateLivesLabel();
					collisions.pushBack(leftCrowds.at(i));
					AdaptDifficulty(kCrashedWithCrowd);
					collided = true;
				}
			}
		}
		//Check collisions with middle lane enemies
		for (int i = 0; i < middleCrowds.size(); ++i) {
			if (!collisions.contains(middleCrowds.at(i))) {
				obstacle = middleCrowds.at(i)->getBoundingBox();
				if (player.intersectsRect(obstacle)) {
					lives--;
					UpdateLivesLabel();
					collisions.pushBack(middleCrowds.at(i));
					AdaptDifficulty(kCrashedWithCrowd);
					collided = true;
				}
			}
		}
		//Check collisions with left lane enemies
		for (int i = 0; i < rightCrowds.size(); ++i) {
			if (!collisions.contains(rightCrowds.at(i))) {
				obstacle = rightCrowds.at(i)->getBoundingBox();
				if (player.intersectsRect(obstacle)) {
					lives--;
					UpdateLivesLabel();
					collisions.pushBack(rightCrowds.at(i));
					AdaptDifficulty(kCrashedWithCrowd);
					collided = true;
				}
			}
		}
		//Check collisions with walls
		for (int i = 0; i < walls.size(); ++i) {
			if (!collisions.contains(walls.at(i))) {
				obstacle = walls.at(i)->getBoundingBox();
				if (player.intersectsRect(obstacle)) {
					lives--;
					UpdateLivesLabel();
					collisions.pushBack(walls.at(i));
					AdaptDifficulty(kCrashedWithWall);
					collided = true;
				}
			}
		}
		if (collided) {
			//playerSprite->runAction(Blink::create(kBlinkDuration, kBlinkNumber));
		}
		//No more lives
		if (lives == 0) {
			GameLost();
		}
	}
}

void MazeGame::GameLost() {
	//Remove event listeners
	_eventDispatcher->removeAllEventListeners();
	//Store scores
	int tmpTime = elapsedTime;
	UserDefault::getInstance()->setIntegerForKey("Victims", victims);
	UserDefault::getInstance()->setIntegerForKey("Time", tmpTime);
	//Change to Game Over scene
	auto gameOverScene = GameOver::createScene();
	Director::getInstance()->replaceScene(gameOverScene);
}

void MazeGame::onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event * event) {
	if (keyCode == cocos2d::EventKeyboard::KeyCode::KEY_ESCAPE) {
		if(wantsToExit) {
			cocos2d::Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
			exit(0);
#endif
		}
		else {
			wantsToExit = true;
			//Create exit confirm sprite
			exitConfirmSprite = Sprite::create("ConfirmExit.png");
			exitConfirmSprite->setScaleX(winSize.width / exitConfirmSprite->getContentSize().width);
			exitConfirmSprite->setScaleY(winSize.height / exitConfirmSprite->getContentSize().height);
			exitConfirmSprite->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
			this->addChild(exitConfirmSprite, 99);
			cocos2d::Director::getInstance()->pause();
		}
	}
}

