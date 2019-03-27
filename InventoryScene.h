#pragma once
#include <iostream>
#include <vector>
#include <string>
#include "2d/CCScene.h"
#include "2d/CCLabel.h"
#include "2d/CCSprite.h"
#include "Textbox.h"
#include "Entities/Player.h"

enum Combining {
	e1 = 1,
	e2 = 2,
	e3 = 3,
	e4 = 4,
	e5 = 5
};


struct inventoryItem
{
	int itemId = 0;
	std::string itemName = "NA";
	std::string itemPic = "NA";
	std::string itemDescription = "NA";
	Combining Val;
};

class InventoryScene : public cocos2d::Scene {
protected:
	Player * player;
private:
	int currInvNum = 0;
	cocos2d::Label* invLabel;
	cocos2d::Label* title;
	cocos2d::Label* description;
	cocos2d::Label* combinewith;
	cocos2d::Sprite* pic;
	cocos2d::Sprite* nextPic;
	cocos2d::Sprite* prevPic;
	cocos2d::Label* lastLabel;

public:
	std::vector<inventoryItem> inventory;
	void pickUpItem(int id, std::string name, std::string pic, std::string des, Combining V);
	void dropItem(int id, std::string name, std::string pic, std::string des,Combining V);
	static Scene* InventoryScene::createScene();
	virtual bool init();
	virtual void update(float dt);
	int pointer = 0;
	int timer = 15;
	int exitTimer = 35;
	bool canCombine = false;
	int combine1 = 0;
	int combine2 = 0;
	//void combine(std::string cOne, std::string cTwo);
	void invReturnCallback(cocos2d::Ref* pSender);
	void invCloseCallback(cocos2d::Ref* pSender);
	CREATE_FUNC(InventoryScene);
};