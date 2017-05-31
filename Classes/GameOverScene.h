#ifndef __GAME_OVER_SCENE_H__
#define __GAME_OVER_SCENE_H__

#include "cocos2d.h"

class GameOver : public cocos2d::Layer
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
    
	cocos2d::Label* continueLabel;
	cocos2d::Label* mainMenuLabel;
	
	// implement the "static create()" method manually
    CREATE_FUNC(GameOver);
};

#endif // __HELLOWORLD_SCENE_H__
