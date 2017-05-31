#ifndef __MAIN_MENU_SCENE_H__
#define __MAIN_MENU_SCENE_H__

#include "cocos2d.h"

class MainMenuScene : public cocos2d::Layer
{
public:

	
    static cocos2d::Scene* createScene();

    virtual bool init();
    
    // a selector callback
	bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);


	void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event * event);
	cocos2d::Size winSize;
	bool wantsToExit;
	cocos2d::Sprite* exitConfirmSprite;

	cocos2d::Vec2 origin;
	cocos2d::Size visibleSize;
	float scaleX;
	float scaleY;
    
	cocos2d::Label* startGameLabel;
	cocos2d::Label* creditsLabel;
	cocos2d::Label* quitLabel;
	
	// implement the "static create()" method manually
    CREATE_FUNC(MainMenuScene);
};

#endif // __HELLOWORLD_SCENE_H__
