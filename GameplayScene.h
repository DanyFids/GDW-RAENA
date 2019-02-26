#pragma once

#include "cocos2d.h"
#include "Entities/Player.h"
#include "Entities/Enemies.h"
#include"Entities/Block.h"
#include"Entities/Torch.h"
#include "Entities/Ladder.h"

class GameplayScene : public cocos2d::Scene {
private:
	Player * player;
	Knight * knight;

	cocos2d::Camera * view;

	cocos2d::Vector<Entity *> terrain;
	cocos2d::Vector<Ladder *> ladders;
	cocos2d::Vector<Torch *> torches;

	int STAGE_WIDTH = 1000;
	int STAGE_HEIGHT = 600;
public:
	struct {
		bool key_up = false;
		bool key_right = false;
		bool key_down = false;
		bool key_left = false;
		bool key_space = false;
		bool key_space_p = false;
		bool key_jump = false;
		bool key_jump_p = false;
		bool key_crouch = false;
		bool key_crouch_p = false;
	} GAMEPLAY_INPUT;

	static cocos2d::Scene * createScene();

	virtual bool init();

	//void menuCloseCallback(cocos2d::Ref* pSender);

	void update(float dt) override;

	CREATE_FUNC(GameplayScene);
};