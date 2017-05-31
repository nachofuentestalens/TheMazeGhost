#include "GameScene.h"
#include "CreditsScene.h"
#include "MainMenuScene.h"

USING_NS_CC;

Scene* MainMenuScene::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = MainMenuScene::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool MainMenuScene::init()
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

	//Create Start Game Label
	startGameLabel = Label::createWithTTF("Start Game", "fonts/Marker Felt.ttf", 48);
	startGameLabel->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y + startGameLabel->getContentSize().height));
	startGameLabel->setColor(Color3B(0, 0, 0));
	this->addChild(startGameLabel, 1);
	
	//Create Credits Label
	creditsLabel = Label::createWithTTF("Credits", "fonts/Marker Felt.ttf", 36);
	creditsLabel->setPosition(Vec2(visibleSize.width / 2 + origin.x, startGameLabel->getPositionY() - creditsLabel->getContentSize().height * 2.0f));
	creditsLabel->setColor(Color3B(0, 0, 0));
	this->addChild(creditsLabel, 1);

	quitLabel = Label::createWithTTF("Quit", "fonts/Marker Felt.ttf", 36);
	quitLabel->setPosition(Vec2(visibleSize.width / 2 + origin.x, creditsLabel->getPositionY() - creditsLabel->getContentSize().height * 2.0f));
	quitLabel->setColor(Color3B(0, 0, 0));
	this->addChild(quitLabel, 1);

	//Create Touch listener
	auto touchListener = EventListenerTouchOneByOne::create();
	touchListener->setSwallowTouches(true);
	touchListener->onTouchBegan = CC_CALLBACK_2(MainMenuScene::onTouchBegan, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);

	wantsToExit = false;

	return true;
}

bool MainMenuScene::onTouchBegan(Touch* touch, Event* event) {
	if (startGameLabel->getBoundingBox().containsPoint(touch->getLocation())) {
		_eventDispatcher->removeAllEventListeners();
		auto newGameScene = MazeGame::createScene();
		Director::getInstance()->replaceScene(newGameScene);
	}
	if (creditsLabel->getBoundingBox().containsPoint(touch->getLocation())) {
		_eventDispatcher->removeAllEventListeners();
		auto newCreditsScene = Credits::createScene();
		Director::getInstance()->replaceScene(newCreditsScene);
	}
	if (quitLabel->getBoundingBox().containsPoint(touch->getLocation())) {
		cocos2d::Director::getInstance()->end();
		#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
			exit(0);
		#endif
	}

	if (wantsToExit) {
		wantsToExit = false;
		exitConfirmSprite->removeFromParentAndCleanup(true);
		cocos2d::Director::getInstance()->resume();
	}

	return true;
}

void MainMenuScene::onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event * event) {
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