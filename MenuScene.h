#pragma once
#include "2d/CCScene.h"
#include "GamePad.h"

class MenuScene : public cocos2d::Scene {
private:
	Gamepad* TheGamepad;
public:
	static Scene* MenuScene::createScene();
	virtual bool init();
	// Closes function
	void menuReturnCallback(cocos2d::Ref* pSender);
	void menuCloseCallback(cocos2d::Ref* pSender);
	void update(float dt);

	CREATE_FUNC(MenuScene);
};
