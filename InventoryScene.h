#pragma once
#include <iostream>
#include <vector>
#include <string>
#include "2d/CCScene.h"
#include "2d/CCLabel.h"
#include "2d/CCSprite.h"
#include "GameplayScene.h"
#include "Entities/Player.h"
#include "Entities/PuzzleInteractable.h"
#include "Entities/Door.h"
#include "classes/LevelManager/LevelManager.h"
class Interactables;
class PuzzleInteract;

class InventoryScene : public cocos2d::Scene {
protected:
	player_inventory * p_inv;
	SceneDoor * sd;
	Door * d;
	cocos2d::Vector< Interactable*> puzzles;
	GameplayScene * playScene;
	Player * player;

private:
	struct {
		bool left = false;
		bool right = false;
	} INPUT;

	int currInvNum = 0;
	cocos2d::Label* invLabel;
	cocos2d::Label* title;
	cocos2d::Label* description;
	cocos2d::Label* combinewith;
	cocos2d::Sprite* pic;
	cocos2d::Sprite* nextPic;
	cocos2d::Sprite* prevPic;
	cocos2d::Label* lastLabel;
	GameplayScene * play;

	bool left = true;
	bool right = true;
	bool A = true;
	bool B = true;
	bool X = true;

	Gamepad* TheGamepad;
public:
	std::vector<inventoryItem>* inventory;
	void pickUpItem(int id, std::string name, std::string pic, std::string des, itemEnum V);
	void dropItem(int id);
	static Scene* InventoryScene::createScene(GameplayScene * playScene);
	static InventoryScene* InventoryScene::create(GameplayScene * playScene);
	void useItem(std::string usename);
	virtual bool init();
	virtual void update(float dt);
	int pointer = 0;
	int timer = 15;
	bool enter_released = false;
	bool canCombine = false;
	int combine1 = 0;
	int combine2 = 0;
	//void combine(std::string cOne, std::string cTwo);
	void invReturnCallback(cocos2d::Ref* pSender);
	void invCloseCallback(cocos2d::Ref* pSender);
	CREATE_FUNC(InventoryScene);
};