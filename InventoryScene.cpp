#include "InventoryScene.h"
#include "cocos2d.h"
#include "ui/CocosGUI.h"
USING_NS_CC;

void InventoryScene::pickUpItem(int id, std::string name, std::string pic){
	inventory.push_back({int(id), name, pic});
	currInvNum += 1;
}

void InventoryScene::dropItem(int id, std::string name, std::string pic){
	inventory.erase(inventory.begin()+id);
	currInvNum -= 1;
	pointer--;
	if (pointer < 0) {
		pointer = currInvNum - 1;
	}
}

Scene * InventoryScene::createScene(){
	return InventoryScene::create();
}

bool InventoryScene::init()
{
	if (!Scene::init()) {
		return false;
	}
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	
	pickUpItem(0, "Key", "Key.png");
	pickUpItem(1, "Bandages", "Bandages.png");
	pickUpItem(2, "Rose", "Rose.png");
	pickUpItem(0, "Key", "Key.png");
	pickUpItem(1, "Bandages", "Bandages.png");

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
		int next_id = (pointer - 1 < 0) ? currInvNum - 1 : pointer - 1;
		nextPic->setTexture(inventory[next_id].itemPic);
		nextPic->setPosition(cocos2d::Vec2(600.0f, 350.0f));
	}

	if (currInvNum >= 3)
	{
		int prev_id = (pointer + 1 >= currInvNum) ? 0 : pointer + 1;
		prevPic->setTexture(inventory[prev_id].itemPic);
		prevPic->setPosition(cocos2d::Vec2(200.0f, 350.0f));
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



	auto use = cocos2d::ui::Button::create("CloseNormal.png", "CloseSelected.png");

	use->setTitleText("");

	use->addTouchEventListener([&](Ref* sender, cocos2d::ui::Widget::TouchEventType type) {
		switch (type)
		{
		case ui::Widget::TouchEventType::BEGAN:
			break;
		case ui::Widget::TouchEventType::ENDED:
			//Player::use = pointer;
			break;
		default:
			break;
		}
	});



	//auto drop = cocos2d::ui::Button::create("CloseNormal.png", "Drop.png");
	//
	//drop->setTitleText("");
	//
	//drop->addTouchEventListener([&](Ref* sender, cocos2d::ui::Widget::TouchEventType type) {
	//	switch (type)
	//	{
	//	case ui::Widget::TouchEventType::BEGAN:
	//		break;
	//	case ui::Widget::TouchEventType::ENDED:
	//		dropItem(pointer, inventory[pointer].itemName, inventory[pointer].itemPic);
	//
	//		break;
	//	default:
	//		break;
	//	}
	//});




	auto examine = cocos2d::ui::Button::create("CloseNormal.png", "CloseSelected.png");

	examine->setTitleText("");

	examine->addTouchEventListener([&](Ref* sender, cocos2d::ui::Widget::TouchEventType type) {
		auto description = Textbox::create(1, { 1 }, { "This is used to unlock a door." }, InventoryScene::create());
		switch (type)
		{
		case ui::Widget::TouchEventType::BEGAN:
			break;
		case ui::Widget::TouchEventType::ENDED:
			description->setPosition(400.0f, 300.0f);
			description->Load();
			addChild(description, 2);
			break;
		default:
			break;
		}
	});

	//this->addChild(examine);

	std::string combine1;
	std::string combine2;

	auto combine = cocos2d::ui::Button::create("CloseNormal.png", "CloseSelected.png");

	combine->setTitleText("");

	combine->addTouchEventListener([&](Ref* sender, cocos2d::ui::Widget::TouchEventType type) {
		switch (type)
		{
		case ui::Widget::TouchEventType::BEGAN:
			break;
		case ui::Widget::TouchEventType::ENDED:
			if (combine1 == "") {
				combine1 = inventory[pointer].itemName;
			}
			else {
				combine2 = inventory[pointer].itemName;
				if (canCombine)
				{
					InventoryScene::combine(combine1, combine2);
				}
			}
			break;
		default:
			break;
		}
	});





	auto left = cocos2d::ui::Button::create("CloseNormal.png", "CloseSelected.png");

	left->setTitleText("");

	left->addTouchEventListener([&](Ref* sender, cocos2d::ui::Widget::TouchEventType type) {
		switch (type)
		{
		case ui::Widget::TouchEventType::BEGAN:
			break;
		case ui::Widget::TouchEventType::ENDED:
			pointer += 1;
			if (pointer == currInvNum)
			{
				pointer = 0;
			}
			break;
		default:
			break;
		}
	});




	auto right = cocos2d::ui::Button::create("CloseNormal.png", "CloseSelected.png");

	right->setTitleText("");

	right->addTouchEventListener([&](Ref* sender, cocos2d::ui::Widget::TouchEventType type) {
		switch (type)
		{
		case ui::Widget::TouchEventType::BEGAN:
			break;
		case ui::Widget::TouchEventType::ENDED:
			pointer -= 1;
			if (pointer == -1)
			{
				pointer = currInvNum - 1;
			}
			break;
		default:
			break;
		}
	});


	auto returnToTheGame = MenuItemImage::create(
		"RETURN TO THE GAME.png",
		"CloseSelected.png",
		CC_CALLBACK_1(InventoryScene::invReturnCallback, this));

	auto quitTheGame = MenuItemImage::create(
		"QUITTHEGAME.png",
		"CloseSelected.png",
		CC_CALLBACK_1(InventoryScene::invCloseCallback, this));


	if (use == nullptr ||
		use->getContentSize().width <= 0 ||
		use->getContentSize().height <= 0)
	{
		//problemLoading(" 'Use Unselected.png' and the other one");
	}
	else
	{
		//button pos
		float x = visibleSize.width / 2 - visibleSize.width / 4;//origin.x + visibleSize.width - closeItem->getContentSize().width / 2;
		float y = visibleSize.height / 2 - use->getContentSize().height / 2 - visibleSize.height / 3;//origin.y + closeItem->getContentSize().height / 2;
		use->setPosition(Vec2(x, y));
	}

	if (combine == nullptr ||
		combine->getContentSize().width <= 0 ||
		combine->getContentSize().height <= 0)
	{
		//problemLoading(" 'Combine Unselected.png' and the other one");
	}
	else
	{
		//button pos

		float x = visibleSize.width / 2 - visibleSize.width / 12;//origin.x + visibleSize.width - closeItem->getContentSize().width / 2;
		float y = visibleSize.height / 2 - combine->getContentSize().height / 2 - visibleSize.height / 3;//origin.y + closeItem->getContentSize().height / 2;
		combine->setPosition(Vec2(x, y));
	}

	//if (drop == nullptr ||
	//	drop->getContentSize().width <= 0 ||
	//	drop->getContentSize().height <= 0)
	//{
	//	//problemLoading(" 'Drop Unselected.png' and the other one");
	//}
	//else
	//{
	//	//button pos
	//
	//	float x = visibleSize.width / 2 + visibleSize.width / 12;//origin.x + visibleSize.width - closeItem->getContentSize().width / 2;
	//	float y = visibleSize.height / 2 - drop->getContentSize().height / 2 - visibleSize.height / 3;//origin.y + closeItem->getContentSize().height / 2;
	//	drop->setPosition(Vec2(x, y));
	//}

	if (examine == nullptr ||
		examine->getContentSize().width <= 0 ||
		examine->getContentSize().height <= 0)
	{
		//problemLoading(" 'Examine Unselected.png' and the other one");
	}
	else
	{
		//button pos

		float x = visibleSize.width / 2 + visibleSize.width / 4;//origin.x + visibleSize.width - closeItem->getContentSize().width / 2;
		float y = visibleSize.height / 2 - examine->getContentSize().height / 2 - visibleSize.height / 3;//origin.y + closeItem->getContentSize().height / 2;
		examine->setPosition(Vec2(x, y));
	}

	if (left == nullptr ||
		left->getContentSize().width <= 0 ||
		left->getContentSize().height <= 0)
	{
		//problemLoading(" 'Left Unselected.png' and the other one");
	}
	else
	{
		//button pos

		float x = left->getContentSize().width;//origin.x + visibleSize.width - closeItem->getContentSize().width / 2;
		float y = visibleSize.height / 2;//origin.y + closeItem->getContentSize().height / 2;
		left->setPosition(Vec2(x, y));
	}

	if (right == nullptr ||
		right->getContentSize().width <= 0 ||
		right->getContentSize().height <= 0)
	{
		//problemLoading(" 'Right Unselected.png' and the other one");
	}
	else
	{
		//button pos

		float x = visibleSize.width - right->getContentSize().width;//origin.x + visibleSize.width - closeItem->getContentSize().width / 2;
		float y = visibleSize.height / 2;//origin.y + closeItem->getContentSize().height / 2;
		right->setPosition(Vec2(x, y));
	}

	if (returnToTheGame == nullptr ||
		returnToTheGame->getContentSize().width <= 0 ||
		returnToTheGame->getContentSize().height <= 0)
	{
		//problemLoading(" 'Exit Unselected.png' and the other one");
	}
	else
	{
		//button pos

		float x = visibleSize.width - returnToTheGame->getContentSize().width;//origin.x + visibleSize.width - closeItem->getContentSize().width / 2;
		float y = visibleSize.height - returnToTheGame->getContentSize().height;//origin.y + closeItem->getContentSize().height / 2;
		returnToTheGame->setPosition(Vec2(x, y));
	}

	if (quitTheGame == nullptr ||
		quitTheGame->getContentSize().width <= 0 ||
		quitTheGame->getContentSize().height <= 0)
	{
		//problemLoading(" 'Exit Unselected.png' and the other one");
	}
	else
	{
		//button pos

		float x = visibleSize.width - quitTheGame->getContentSize().width;//origin.x + visibleSize.width - closeItem->getContentSize().width / 2;
		float y = visibleSize.height - visibleSize.height + quitTheGame->getContentSize().height;//origin.y + closeItem->getContentSize().height / 2;
		quitTheGame->setPosition(Vec2(x, y));
	}














	//currinv.at(i);




























	//auto inv7 = Menu::create(returnToTheGame, NULL);
	//auto inv8 = Menu::create(quitTheGame, NULL);
	//inv7->setPosition(Vec2::ZERO);
	//inv8->setPosition(Vec2::ZERO);
	use->setPosition(Vec2(400.0f, 100.0f));
	examine->setPosition(Vec2(600.0f, 100.0f));
	combine->setPosition(Vec2(200.0f, 100.0f));
	this->addChild(use, 1);
	//this->addChild(drop, 1);
	this->addChild(examine, 1);
	this->addChild(combine, 1);
	this->addChild(left, 1);
	this->addChild(right, 1);
	//this->addChild(inv7, 1);
	//this->addChild(inv8, 1);



	this->scheduleUpdate();
	return true;
}

void InventoryScene::update(float dt)
{
	timer -= dt;
	exitTimer -= dt;
	if (currInvNum != 0) {
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
					exitTimer = 20;
					Director::getInstance()->popScene();
				}
			}

			if (inventory.size() != 0) {
				invLabel->setString(inventory[pointer].itemName);
			}
			else {
				invLabel->setString("There is nothing in your inventory");

			}

			if (currInvNum == 2)
			{
				removeChild(prevPic);
			}
			if (currInvNum == 1)
			{
				removeChild(nextPic);
			}
			if (currInvNum == 0)
			{
				removeChild(invLabel);
				removeChild(pic);
			}
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
	else
	{
		if (GetAsyncKeyState(VK_RETURN) != 0)
		{
			if (exitTimer == 0)
			{ 
				exitTimer = 20;
				Director::getInstance()->popScene();
			}
		}
	}
}



void InventoryScene::combine(std::string cOne, std::string cTwo)
{
}

void InventoryScene::invReturnCallback(Ref* pSender) {
	Director::getInstance()->popScene();
}

void InventoryScene::invCloseCallback(Ref* pSender)
{
	//Close the cocos2d-x game scene and quit the application
	Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
}
