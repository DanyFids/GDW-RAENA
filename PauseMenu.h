#pragma once
#include "2d/CCScene.h"
#include "2d/CCSprite.h"
#include "2d/CCLabel.h"
#include "cocos2d.h"

class PauseMenu : public cocos2d::Scene {
private:
	cocos2d::Label* pauseLabel;
public:
	static Scene* PauseMenu::createScene();
	virtual bool init();
	virtual void update(float dt);
	void menuReturnCallback(cocos2d::Ref* pSender);
	void menuCloseCallback(cocos2d::Ref* pSender);
	int exitTimer = 20;
	CREATE_FUNC(PauseMenu);
};