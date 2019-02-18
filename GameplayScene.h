#pragma once

#include "cocos2d.h"
#include"Entities/Player.h"
#include"Entities/Block.h"
#include"Entities/Torch.h"
#include "Entities/Interactable.h"
#include "Entities/Platforms.h"

class GameplayScene : public cocos2d::Scene {
private:
	const float PLAYER_SPEED = 100;
	Player * player;
	cocos2d::Vector<Block *> platforms;
	cocos2d::Vector<Torch *> torches;
	cocos2d::Vector<Interactable *> interactables;
	cocos2d::Vector<Platform *> ActualPlatforms;
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
		bool key_interact = false;

		bool key_one = false;
		bool key_oneP = false;
		bool key_F = false;
		bool key_FP = false;
		bool key_two = false;
		bool key_twoP = false;
		
		bool key_P1 = false;
		bool key_P1P = false;
		bool key_P2 = false;
		bool key_P2P = false;
	} GAMEPLAY_INPUT;

	static cocos2d::Scene * createScene();

	virtual bool init();

	//void menuCloseCallback(cocos2d::Ref* pSender);

	void update(float dt) override;

	CREATE_FUNC(GameplayScene);
};