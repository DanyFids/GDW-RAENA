#include "InventoryScene.h"
#include "cocos2d.h"

USING_NS_CC;

void InventoryScene::displayInventory()
{  

	auto invLabel = Label::createWithTTF("There is nothing in your inventory.", "fonts/Marker Felt.ttf", 24);
	auto pic = Sprite::create();
	if (currInvNum == 0)
	{
		invLabel->enableGlow(Color4B::BLUE);
		invLabel->setPosition(cocos2d::Vec2(400.0f, 300.0f));
		addChild(invLabel, 1);
	}
	else
	{
		invLabel->setString(inventory[pointer].itemName);
		invLabel->setPosition(cocos2d::Vec2(400.0f, 300.0f));
		addChild(invLabel, 1);
		pic->initWithFile(inventory[pointer].itemPic);
		pic->setPosition(cocos2d::Vec2(400.0f, 350.0f));
		addChild(pic);
		if (GetAsyncKeyState(VK_LEFT) != 0)
		{
			pointer -= 1;
			if (pointer == -1)
			{
				pointer = currInvNum;
			}
		}
		else if (GetAsyncKeyState(VK_RIGHT) != 0)
		{
			pointer += 1;
			if (pointer == currInvNum + 1)
			{
				pointer = 0;
			}
		}
	}
}

void InventoryScene::pickUpItem(int id, std::string name, std::string pic){
	inventory.push_back({int(id), name, pic});
	currInvNum += 1;
}

void InventoryScene::dropItem(int id, std::string name, std::string pic)
{
	
}

Scene * InventoryScene::createScene(){
	return InventoryScene::create();
}

bool InventoryScene::init()
{
	if (!Scene::init()) {
		return false;
	}
	displayInventory();
	return true;
}

void InventoryScene::update(float dt)
{
	
}

