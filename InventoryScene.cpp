#include "InventoryScene.h"
#include "cocos2d.h"

USING_NS_CC;

void InventoryScene::pickUpItem(int id, std::string name, std::string pic){
	inventory.push_back({int(id), name, pic});
	currInvNum += 1;
}

void InventoryScene::dropItem(int id, std::string name, std::string pic){
	inventory.erase(inventory.begin()+id);
	currInvNum -= 1;
}

Scene * InventoryScene::createScene(){
	return InventoryScene::create();
}

bool InventoryScene::init()
{
	if (!Scene::init()) {
		return false;
	}
	
	pickUpItem(0, "Key", "Key.png");
	pickUpItem(1, "Bandages", "Bandages.png");
	pickUpItem(2, "Rose", "Rose.png");

	title = Label::createWithTTF("INVENTORY", "fonts/horrendo.ttf", 36);
	invLabel = Label::createWithTTF("There is nothing in your inventory.", "fonts/horrendo.ttf", 24);
	title->setColor(Color3B(200, 180, 150));
	title->enableGlow(Color4B::RED);
	title->enableBold();
	invLabel->setColor(Color3B(200, 180, 150));
	pic = Sprite::create();
	prevPic = Sprite::create();
	nextPic = Sprite::create();

	title->setPosition(cocos2d::Vec2(400.0f, 500.0f));
	addChild(title, 1);

	if (currInvNum >= 1)
	{
		invLabel->setString(inventory[pointer].itemName);
		pic->setTexture(inventory[pointer].itemPic);
		pic->setPosition(cocos2d::Vec2(400.0f, 350.0f));
	}

	if (currInvNum >= 2)
	{
		int prev_id = (pointer + 1 >= currInvNum) ? 0 : pointer + 1;
		prevPic->setTexture(inventory[prev_id].itemPic);
		prevPic->setPosition(cocos2d::Vec2(200.0f, 350.0f));
	}

	if (currInvNum >= 3)
	{
		int next_id = (pointer - 1 < 0) ? currInvNum - 1 : pointer - 1;
		nextPic->setTexture(inventory[next_id].itemPic);
		nextPic->setPosition(cocos2d::Vec2(600.0f, 350.0f));
	}


	invLabel->setPosition(cocos2d::Vec2(400.0f, 250.0f));
	invLabel->enableGlow(Color4B::RED);
	if (currInvNum == 0)
	{
		addChild(invLabel, 1);
	}
	else {
		invLabel->setScale(2);
		pic->setScale(2);
		addChild(invLabel, 1);
		addChild(pic, 1);
		addChild(prevPic, 1);
		addChild(nextPic, 1);
	}

	this->scheduleUpdate();
	return true;
}

void InventoryScene::update(float dt)
{
	if (currInvNum != 0) {
		timer -= dt;
		exitTimer -= dt;
		if (timer == 0)
		{
			timer = 10;
			if (GetAsyncKeyState(VK_LEFT) != 0)
			{
				pointer += 1;
				if (pointer == currInvNum)
				{
					pointer = 0;
				}
			}
			else if (GetAsyncKeyState(VK_RIGHT) != 0)
			{
				pointer -= 1;
				if (pointer == -1)
				{
					pointer = currInvNum - 1;
				}
			}
			else if (GetAsyncKeyState(VK_RETURN) != 0)
			{
				if (exitTimer == 0)
				{
					Director::getInstance()->popScene();
				}
			}

			invLabel->setString(inventory[pointer].itemName);
			if (currInvNum >= 1)
			{
				pic->setTexture(inventory[pointer].itemPic);
			}
			if (pointer - 1 == -1 && currInvNum >= 2)
			{
				nextPic->setTexture(inventory[currInvNum - 1].itemPic);
			}
			else if (pointer - 1 != -1 && currInvNum >= 2)
			{
				nextPic->setTexture(inventory[pointer - 1].itemPic);
			}

			if (pointer + 1 == currInvNum && currInvNum >= 3)
			{
				prevPic->setTexture(inventory[0].itemPic);
			}
			else if (pointer + 1 != currInvNum && currInvNum >= 3)
			{
				prevPic->setTexture(inventory[pointer + 1].itemPic);
			}
		}
	}
}

