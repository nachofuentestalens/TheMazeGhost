#include "CreditsScene.h"
#include "GameScene.h"
#include "MainMenuScene.h"

USING_NS_CC;

Scene* Credits::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = Credits::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool Credits::init()
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
	touchListener->onTouchBegan = CC_CALLBACK_2(Credits::onTouchBegan, this);
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

	//Create Title Label
	auto titleLabel = Label::createWithTTF("The Maze Ghost", "fonts/Marker Felt.ttf", 68);
	titleLabel->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height + origin.y - titleLabel->getContentSize().height));
	titleLabel->setColor(Color3B(0, 0, 0));
	this->addChild(titleLabel, 1);

	//Create credits label
	auto creditsLabel = Label::createWithTTF("Developed by", "fonts/Marker Felt.ttf", 36);
	creditsLabel->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y + creditsLabel->getContentSize().height));
	creditsLabel->setColor(Color3B(0, 0, 0));
	this->addChild(creditsLabel, 1);
	auto nameLabel = Label::createWithTTF("Nacho Fuentes Talens", "fonts/Marker Felt.ttf", 36);
	nameLabel->setPosition(Vec2(visibleSize.width / 2 + origin.x, creditsLabel->getPositionY() - creditsLabel->getContentSize().height * 1.2f));
	nameLabel->setColor(Color3B(0, 0, 0));
	this->addChild(nameLabel, 1);

	//Create Start Game Label
	startGameLabel = Label::createWithTTF("Start Game", "fonts/Marker Felt.ttf", 36);
	startGameLabel->setPosition(Vec2(visibleSize.width + origin.x - startGameLabel->getContentSize().width, origin.y + startGameLabel->getContentSize().height));
	startGameLabel->setColor(Color3B(0, 0, 0));
	this->addChild(startGameLabel, 1);

	//Create main menu
	mainMenuLabel = Label::createWithTTF("Back", "fonts/Marker Felt.ttf", 36);
	mainMenuLabel->setPosition(Vec2(origin.x + mainMenuLabel->getContentSize().width, startGameLabel->getPositionY()));
	mainMenuLabel->setColor(Color3B(0, 0, 0));
	this->addChild(mainMenuLabel, 1);

	wantsToExit = false;

	return true;
}

bool Credits::onTouchBegan(Touch* touch, Event* event) {
	if (startGameLabel->getBoundingBox().containsPoint(touch->getLocation())) {
		_eventDispatcher->removeAllEventListeners();
		auto newGameScene = MazeGame::createScene();
		Director::getInstance()->replaceScene(newGameScene);
	}
	if (mainMenuLabel->getBoundingBox().containsPoint(touch->getLocation())) {
		_eventDispatcher->removeAllEventListeners();
		auto newMainMenuScene = MainMenuScene::createScene();
		Director::getInstance()->replaceScene(newMainMenuScene);
	}

	if (wantsToExit) {
		wantsToExit = false;
		exitConfirmSprite->removeFromParentAndCleanup(true);
		cocos2d::Director::getInstance()->resume();
	}

	return true;
}

void Credits::onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event * event) {
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
