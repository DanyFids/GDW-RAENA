#pragma once
#include "2d/CCScene.h"


class MenuScene : public cocos2d::Scene {
public:
	static Scene* MenuScene::createScene();
	virtual bool init();
	// Closes function
	void menuReturnCallback(cocos2d::Ref* pSender);
	void menuCloseCallback(cocos2d::Ref* pSender);

	CREATE_FUNC(MenuScene);
};
