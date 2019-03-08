#pragma once
#pragma once
#include <iostream>
#include <vector>
#include <string>
#include "2d/CCScene.h"

struct inventoryItem
{
	int itemId = 0;
	std::string itemName = "NA";
	std::string itemPic = "NA";
};

class InventoryScene : public cocos2d::Scene {
private:
	int currInvNum = 0;
public:
	std::vector<inventoryItem> inventory;
	void displayInventory();
	void pickUpItem(int id, std::string name, std::string pic);
	void dropItem(int id, std::string name, std::string pic);
	static Scene* InventoryScene::createScene();
	virtual bool init();
	virtual void update(float dt);
	int pointer = 0;
	CREATE_FUNC(InventoryScene);
};