#pragma once
#include "2d/CCScene.h"


class ENDScene : public cocos2d::Scene {
private:
public:
	static Scene* ENDScene::createScene();
	virtual bool init();
	// Closes function
	void menuReturnCallback(cocos2d::Ref* pSender);
	void menuCloseCallback(cocos2d::Ref* pSender);

	CREATE_FUNC(ENDScene);
};
