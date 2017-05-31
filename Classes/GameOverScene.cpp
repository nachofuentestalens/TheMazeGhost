#include "GameOverScene.h"
#include "GameScene.h"
#include "MainMenuScene.h"

USING_NS_CC;

Scene* GameOver::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = GameOver::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool GameOver::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Layer::init())
	{
		return false;
	}

	this->setKeypadEnabled(true);

	visibleSize = Director::getInstance()->getVisibleSize();
	origin = Director::getInstance()->getVisibleOrigin();
	
	//Create Touch listener
	auto touchListener = EventListenerTouchOneByOne::create();
	touchListener->onTouchBegan = CC_CALLBACK_2(GameOver::onTouchBegan, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);

	/////////////////////////////
	// 3. add your codes below...
	winSize = Director::getInstance()->getWinSize();
	scaleX = visibleSize.width / 480.0f;
	scaleY = visibleSize.height / 800.0f;

	//Create Background
	auto background = Sprite::create("WhiteBackground.png");
	background->setScaleX(winSize.width / background->getContentSize().width);
	background->setScaleY(winSize.height / background->getContentSize().height);
	background->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
	this->addChild(background, 0);

	//Create GameOverLabel
	auto gameOverLabel = Label::createWithTTF("Game Over", "fonts/Marker Felt.ttf", 72);
	gameOverLabel->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y + gameOverLabel->getContentSize().height));
	gameOverLabel->setColor(Color3B(0, 0, 0));
	this->addChild(gameOverLabel, 1);
	

	//Set victims label
	char temp[100];
	sprintf(temp, "Score: %d", UserDefault::getInstance()->getIntegerForKey("Victims"));
	auto victimsLabel = Label::createWithTTF(temp, "fonts/Marker Felt.ttf", 36);
	victimsLabel->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
	victimsLabel->setColor(Color3B(0, 0, 0));
	this->addChild(victimsLabel, 1);

	//Set timer label
	sprintf(temp, "Time: %ds", UserDefault::getInstance()->getIntegerForKey("Time"));
	auto timeLabel = Label::createWithTTF(temp, "fonts/Marker Felt.ttf", 36);
	timeLabel->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y - victimsLabel->getContentSize().height));
	timeLabel->setColor(Color3B(0, 0, 0));
	this->addChild(timeLabel, 1);

	continueLabel = Label::createWithTTF("Tap here to play again", "fonts/Marker Felt.ttf", 18);
	continueLabel->setPosition(Vec2(visibleSize.width / 2 + origin.x, timeLabel->getPositionY() - timeLabel->getContentSize().height * 2.0f));
	continueLabel->setColor(Color3B(0, 0, 0));
	this->addChild(continueLabel, 1);

	//Create main menu
	mainMenuLabel = Label::createWithTTF("Main Menu", "fonts/Marker Felt.ttf", 18);
	mainMenuLabel->setPosition(Vec2(origin.x + mainMenuLabel->getContentSize().width, origin.y + mainMenuLabel->getContentSize().height));
	mainMenuLabel->setColor(Color3B(0, 0, 0));
	this->addChild(mainMenuLabel, 1);

	wantsToExit = false;

	return true;
}

bool GameOver::onTouchBegan(Touch* touch, Event* event) {
	if (continueLabel->getBoundingBox().containsPoint(touch->getLocation())) {
		_eventDispatcher->removeAllEventListeners();
		auto newGameScene = MazeGame::createScene();
		Director::getInstance()->replaceScene(newGameScene);
	}
	if (mainMenuLabel->getBoundingBox().containsPoint(touch->getLocation())) {
		_eventDispatcher->removeAllEventListeners();
		auto newMainMenu = MainMenuScene::createScene();
		Director::getInstance()->replaceScene(newMainMenu);
	}

	if (wantsToExit) {
		wantsToExit = false;
		exitConfirmSprite->removeFromParentAndCleanup(true);
		cocos2d::Director::getInstance()->resume();
	}

	return true;
}

void GameOver::onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event * event) {
	if (keyCode == cocos2d::EventKeyboard::KeyCode::KEY_ESCAPE) {
		if (wantsToExit) {
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
