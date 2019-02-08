#pragma once

#include "cocos2d.h"
#include"Player.h"

class GameplayScene : public cocos2d::Scene {
private:
	const float PLAYER_SPEED = 100;
	Player * player;

public:
	struct {
		bool key_up = false;
		bool key_right = false;
		bool key_down = false;
		bool key_left = false;
		bool key_space = false;
		bool key_space_p = false;
	} GAMEPLAY_INPUT;

	static cocos2d::Scene * createScene();

	virtual bool init();

	//void menuCloseCallback(cocos2d::Ref* pSender);

	void update(float dt) override;

	CREATE_FUNC(GameplayScene);
};