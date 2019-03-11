#pragma once
#include <iostream>
#include <vector>
#include <string>
#include "2d/CCScene.h"
#include "2d/CCLabel.h"
#include "2d/CCSprite.h"
#include "Textbox.h"

struct inventoryItem
{
	int itemId = 0;
	std::string itemName = "NA";
	std::string itemPic = "NA";
};

class InventoryScene : public cocos2d::Scene {
private:
	int currInvNum = 0;
	cocos2d::Label* invLabel;
	cocos2d::Label* title;
	cocos2d::Sprite* pic;
	cocos2d::Sprite* nextPic;
	cocos2d::Sprite* prevPic;
public:
	std::vector<inventoryItem> inventory;
	void pickUpItem(int id, std::string name, std::string pic);
	void dropItem(int id, std::string name, std::string pic);
	static Scene* InventoryScene::createScene();
	virtual bool init();
	virtual void update(float dt);
	int pointer = 0;
	int timer = 10;
	int exitTimer = 20;
	bool canCombine = false;
	void combine(std::string cOne, std::string cTwo);
	void invReturnCallback(cocos2d::Ref* pSender);
	void invCloseCallback(cocos2d::Ref* pSender);
	CREATE_FUNC(InventoryScene);
};