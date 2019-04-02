#include "InventoryScene.h"
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "Entities/CoreEntities.h"
#include <iostream>


USING_NS_CC;
using namespace std;

void InventoryScene::pickUpItem(int id, std::string name, std::string pic, std::string des, itemEnum V){
	inventory->push_back({int(id), name, pic, des, V});
	currInvNum += 1;
}

void InventoryScene::dropItem(int id){
	inventory->erase(inventory->begin()+id);
	currInvNum -= 1;
	pointer--;
	if (pointer < 0) {
		pointer = currInvNum - 1;
	}
}

Scene * InventoryScene::createScene(GameplayScene * playScene){
	auto ret = InventoryScene::create(playScene);

	return ret;
}

InventoryScene * InventoryScene::create(GameplayScene * playScene)
{
	auto ret = new (std::nothrow) InventoryScene;
	if (ret != nullptr) {
		ret->play = playScene;
		ret->player = playScene->player;
		ret->puzzles = playScene->interactables;
		ret->inventory = playScene->getInvRef();
		ret->currInvNum = ret->inventory->size();

		if (ret->init()) {
			return ret;
		}

		CC_SAFE_RELEASE(ret);
		return nullptr;
	}
	CC_SAFE_RELEASE(ret);
	return nullptr;
}

void InventoryScene::useItem(std::string usename)
{
	if (usename == "Bandages") {

		if ((player->getHP()) <= 4) {
			player->setHP(player->getHP() + 2);
		}
		else {
			player->setHP(6);
		}
		dropItem((*inventory)[pointer].itemId);
	}

	for (int i = 0; i < puzzles.size(); i++) {
		if (usename == "Rose" && ((PuzzleInteract*)puzzles.at(i))->checkPuzzle(Princess1) && ((Interactable*)puzzles.at(i))->inRange(player) ) {
			Knight * newKnight = Knight::create("knightwalkyboi0000.png");
			newKnight->setScale(1);
			newKnight->setPosition(cocos2d::Vec2(850, 200 + (newKnight->getBoundingBox().size.height / 2)));
			play->setKnight(newKnight);
			play->addChild(play->knight);
			dropItem((*inventory)[pointer].itemId);
			Director::getInstance()->popScene();
		}
		InteractType type =(puzzles.at(i))->getType();
		if (usename == "Key" && type == DOOR && ((Interactable*)puzzles.at(i))->inRange(player)) {
			if (d->locked) {	 // Checks to see A. Door is locked ... B. Player has enough of Key ... C. removes a key and unlocks door

				if (d->requiredKey == GEN_KEY) {
					
					dropItem((*inventory)[pointer].itemId);
					d->locked = false;
	
					}
				}
				//Include Each Keytype below

			if (!(d->CoolDownState) && !(d->locked)) {

				d->CoolDownState = true;

				if (d->Active == false) {

					d->Active = true;

				}
				else {
					d->Active = false;
				}
			}
		}
		if (usename == "Key" && type == S_DOOR && ((Interactable*)puzzles.at(i))->inRange(player)) {
			if (sd->locked) {	 // Checks to see A. Door is locked ... B. Player has enough of Key ... C. removes a key and unlocks door

				if (sd->requiredKey == GEN_KEY) {
					dropItem((*inventory)[pointer].itemId);
					sd->locked = false;
				}
			}
				//Include Each Keytype below

			if (!(sd->CoolDownState) && !(sd->locked)) {

				sd->CoolDownState = true;

				if (sd->Active == false) {

					sd->Active = true;

					switch (sd->goTo) {
					case A1_R1:
						cocos2d::Director::getInstance()->replaceScene(LevelManager::GetLevel(A1_R1));
						//GameplayScene::movePlayer();
						player = nullptr;
						break;
					case A1_R2:
						cocos2d::Director::getInstance()->replaceScene(LevelManager::GetLevel(A1_R2));
						player->setPosition(sd->movePlayer);
						player = nullptr;
						break;
					case A1_R3:
						cocos2d::Director::getInstance()->replaceScene(LevelManager::GetLevel(A1_R3));
						player->setPosition(sd->movePlayer);
						break;
					case A1_R4:
						cocos2d::Director::getInstance()->replaceScene(LevelManager::GetLevel(A1_R4));
						player->setPosition(sd->movePlayer);
						break;
					case A1_R5:
						cocos2d::Director::getInstance()->replaceScene(LevelManager::GetLevel(A1_R5));
						player->setPosition(sd->movePlayer);
						break;
					case A1_R6:
						cocos2d::Director::getInstance()->replaceScene(LevelManager::GetLevel(A1_R6));
						player->setPosition(sd->movePlayer);
						break;
					

					}


				}
				else {
					sd->Active = false;
				}
			}

		}

	}

}



bool InventoryScene::init()
{
	if (!Scene::init()) {
		return false;
	}
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	cocos2d::Texture2D::TexParams tp = { GL_NEAREST, GL_NEAREST, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE };

	auto BG = Sprite::create();
	BG->initWithFile("TutorialBackground.png");
	BG->getTexture()->setTexParameters(tp);
	BG->setPosition({ origin.x + (visibleSize.width / 2), origin.y + (visibleSize.height / 2) });
	addChild(BG);
	
	

	title = Label::createWithTTF("INVENTORY", "fonts/fofbb_reg.ttf", 36);
	invLabel = Label::createWithTTF("There is nothing in your inventory.", "fonts/fofbb_reg.ttf", 24);
	title->enableGlow(Color4B::RED);
	title->setColor(Color3B(200, 180, 150));
	title->enableBold();
	invLabel->setColor(Color3B(200, 180, 150));
	pic = Sprite::create();
	prevPic = Sprite::create();
	nextPic = Sprite::create();

	title->setPosition(cocos2d::Vec2(400.0f, 500.0f));
	addChild(title, 1);

	if (currInvNum >= 1)
	{
		invLabel->setString((*inventory)[pointer].itemName);
		pic->setTexture((*inventory)[pointer].itemPic);
	}
	pic->setPosition(cocos2d::Vec2(400.0f, 350.0f));

	if (currInvNum >= 2)

	{
		int next_id = (pointer - 1 < 0) ? currInvNum - 1 : pointer - 1;
		nextPic->setTexture((*inventory)[next_id].itemPic);
	}
	nextPic->setPosition(cocos2d::Vec2(600.0f, 350.0f));

	if (currInvNum >= 3)
	{
		int prev_id = (pointer + 1 >= currInvNum) ? 0 : pointer + 1;
		prevPic->setTexture((*inventory)[prev_id].itemPic);
	}
	prevPic->setPosition(cocos2d::Vec2(200.0f, 350.0f));


	invLabel->setPosition(cocos2d::Vec2(400.0f, 250.0f));
	invLabel->enableGlow(Color4B::RED);
	if (currInvNum == 0)
	{
		addChild(invLabel, 1);

		pic->setScale(2);
		addChild(pic, 1);
		addChild(prevPic, 1);
		addChild(nextPic, 1);
	}
	else {
		invLabel->setScale(2);
		pic->setScale(2);
		addChild(invLabel, 1);
		addChild(pic, 1);
		addChild(prevPic, 1);
		addChild(nextPic, 1);
	}



	auto use = cocos2d::ui::Button::create("UI/Use.png", "UI/UseClicked.png");

	use->setTitleText("");
	use->setScale(2);

	use->addTouchEventListener([this](Ref* sender, cocos2d::ui::Widget::TouchEventType type) {
		switch (type)
		{
		case ui::Widget::TouchEventType::BEGAN:
			break;
		case ui::Widget::TouchEventType::ENDED:
			if (this->inventory->size() > 0)
			{
				this->useItem((*inventory)[pointer].itemName);
			}
			
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

	auto KeyHandle = EventListenerKeyboard::create();

	KeyHandle->onKeyPressed = [this](EventKeyboard::KeyCode key, Event * event) {
		switch (key) {
		case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
			this->INPUT.left = true;
			break;
		case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
			this->INPUT.right = true;
			break;
		case EventKeyboard::KeyCode::KEY_ENTER:
			if (enter_released) {
				Director::getInstance()->popScene();
			}
			break;
		}
	};

	KeyHandle->onKeyReleased = [this](EventKeyboard::KeyCode key, Event * event) {
		switch (key) {
		case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
			this->INPUT.left = false;
			break;
		case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
			this->INPUT.right = false;
			break;
		case EventKeyboard::KeyCode::KEY_ENTER:
			enter_released = true;
			break;
		}
	};

	this->_eventDispatcher->addEventListenerWithSceneGraphPriority(KeyHandle, this);

	auto examine = cocos2d::ui::Button::create("UI/Examine.png", "UI/ExamineClicked.png");

	examine->setTitleText("");
	examine->setScale(2);
	examine->addTouchEventListener([this](Ref* sender, cocos2d::ui::Widget::TouchEventType type) {
		if (lastLabel != nullptr)
		{
			removeChild(lastLabel);
		}
		description = Label::createWithTTF((*inventory)[pointer].itemDescription, "fonts/fofbb_reg.ttf", 16);
		lastLabel = description;

		switch (type)
		{
		case ui::Widget::TouchEventType::BEGAN:
			break;
		case ui::Widget::TouchEventType::ENDED:
			if(p_inv->items.size()>0)
			{
			description->setPosition(400, 200);
			addChild(description, 2);
			}
			break;
		default:
			break;
		}
	});

	

	auto combine = cocos2d::ui::Button::create("UI/Combine.png", "UI/CombineClicked.png");

	combine->setTitleText("");
	combine->setScale(2);
	combine->addTouchEventListener([&](Ref* sender, cocos2d::ui::Widget::TouchEventType type) {
		switch (type)
		{
		case ui::Widget::TouchEventType::BEGAN:
			break;
		case ui::Widget::TouchEventType::ENDED:
			if (p_inv->items.size() > 0)
			{
				if (combine1 == 0) {
					combine1 = pointer;
					combinewith = Label::createWithTTF("What would you like to combine the " + (*inventory)[combine1].itemName + " with?", "fonts/fofbb_reg.ttf", 16);
					combinewith->setPosition(400, 200);
					addChild(combinewith, 2);
				}
				else {
					if (!(pointer == combine1)) {
						combine2 = pointer;
						removeChild(combinewith);

						int yolo = (*inventory)[combine1].Val + (*inventory)[combine2].Val;

						if (combine1 < combine2) {
							int temp = combine1;
							combine1 = combine2;
							combine2 = temp;

						}

						itemEnum newItem;
						try {
							newItem = (itemEnum)yolo;
						}
						catch (exception e) {
							newItem = I_NONE;
						}

						if (newItem != I_NONE)
						{
							dropItem(combine1);
							dropItem(combine2);
							pickUpItem(inventory->size() - 1, "newItem", "CloseSelected.png", "This is the new item", newItem);
							combine1 = 0;
							combine2 = 0;
							pointer = inventory->size() - 1;
						}
					}
				}
			}
			break;
		default:
			break;
		}
	});





	auto left = cocos2d::ui::Button::create("UI/Left_Arrow.png", "UI/Left_Arrow.png");

	left->setTitleText("");

	left->addTouchEventListener([&](Ref* sender, cocos2d::ui::Widget::TouchEventType type) {
		switch (type)
		{
		case ui::Widget::TouchEventType::BEGAN:
			break;
		case ui::Widget::TouchEventType::ENDED:
			pointer += 1;
			removeChild(description);
			if (pointer == currInvNum)
			{
				pointer = 0;
			}
			break;
		default:
			break;
		}
	});




	auto right = cocos2d::ui::Button::create("UI/Right_Arrow.png", "UI/Right_Arrow.png");

	right->setTitleText("");

	right->addTouchEventListener([&](Ref* sender, cocos2d::ui::Widget::TouchEventType type) {
		switch (type)
		{
		case ui::Widget::TouchEventType::BEGAN:
			break;
		case ui::Widget::TouchEventType::ENDED:
			pointer -= 1;
			removeChild(description);
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


	auto inv7 = Menu::create(returnToTheGame, NULL);
	auto inv8 = Menu::create(quitTheGame, NULL);
	inv7->setPosition(Vec2::ZERO);
	inv8->setPosition(Vec2::ZERO);
	use->setPosition(Vec2(400.0f, 100.0f));
	examine->setPosition(Vec2(600.0f, 100.0f));
	combine->setPosition(Vec2(200.0f, 100.0f));
	this->addChild(use, 1);
	//this->addChild(drop, 1);
	this->addChild(examine, 1);
	this->addChild(combine, 1);
	this->addChild(left, 1);
	this->addChild(right, 1);
	this->addChild(inv7, 1);
	this->addChild(inv8, 1);

	auto Updatepad = new (std::nothrow) Gamepad;
	Updatepad->CheckConnection();
	TheGamepad = Updatepad;

	this->scheduleUpdate();
	return true;
}

void InventoryScene::update(float dt)
{
	TheGamepad->Refresh();
	
	timer -= dt;
	if (TheGamepad->IsPressed(XINPUT_GAMEPAD_START) && enter_released)
	{
		Director::getInstance()->popScene();
		enter_released = false;
	}
	else if (!TheGamepad->IsPressed(XINPUT_GAMEPAD_START))
	{
		enter_released = true;
	}
	/*if (TheGamepad->IsPressed(XINPUT_GAMEPAD_DPAD_LEFT) && left)
	{
		pointer += 1;
		removeChild(description);
		if (pointer == currInvNum)
		{
			pointer = 0;
		}
		left = false;
	}
	else if (!TheGamepad->IsPressed(XINPUT_GAMEPAD_DPAD_LEFT))
	{
		left = true;
	}
	if (TheGamepad->IsPressed(XINPUT_GAMEPAD_DPAD_RIGHT) && right)
	{
		Director::getInstance()->popScene();
		right = false;
	}
	else if (!TheGamepad->IsPressed(XINPUT_GAMEPAD_DPAD_RIGHT))
	{
		right = true;
	}*/

	if (TheGamepad->IsPressed(XINPUT_GAMEPAD_B) && B)
	{
		if (p_inv->items.size() > 0)
		{
			if (combine1 == 0) {
				combine1 = pointer;
				combinewith = Label::createWithTTF("What would you like to combine the " + (*inventory)[combine1].itemName + " with?", "fonts/fofbb_reg.ttf", 16);
				combinewith->setPosition(400, 200);
				addChild(combinewith, 2);
			}
			else {
				if (!(pointer == combine1)) {
					combine2 = pointer;
					removeChild(combinewith);

					int yolo = (*inventory)[combine1].Val + (*inventory)[combine2].Val;

					if (combine1 < combine2) {
						int temp = combine1;
						combine1 = combine2;
						combine2 = temp;

					}

					itemEnum newItem;
					try {
						newItem = (itemEnum)yolo;
					}
					catch (exception e) {
						newItem = I_NONE;
					}

					if (newItem != I_NONE)
					{
						dropItem(combine1);
						dropItem(combine2);
						pickUpItem(inventory->size() - 1, "newItem", "CloseSelected.png", "This is the new item", newItem);
						combine1 = 0;
						combine2 = 0;
						pointer = inventory->size() - 1;
					}
				}
			}
		}
		B = false;
	}
	else if (!TheGamepad->IsPressed(XINPUT_GAMEPAD_A))
	{
		B = true;
	}

	if (TheGamepad->IsPressed(XINPUT_GAMEPAD_A) && A)
	{
		if (this->inventory->size() > 0)
		{
			this->useItem((*inventory)[pointer].itemName);
		}
		A = false;
	}
	else if (!TheGamepad->IsPressed(XINPUT_GAMEPAD_A))
	{
		A = true;
	}
	if (TheGamepad->IsPressed(XINPUT_GAMEPAD_X) && X)
	{
		if (lastLabel != nullptr)
		{
			removeChild(lastLabel);
		}
		description = Label::createWithTTF((*inventory)[pointer].itemDescription, "fonts/fofbb_reg.ttf", 16);
		lastLabel = description;

		if (p_inv->items.size()>0)
		{
			description->setPosition(400, 200);
			addChild(description, 2);
		}
		X = false;
	}
	else if (!TheGamepad->IsPressed(XINPUT_GAMEPAD_X))
	{
		X = true;
	}
	
	

	if (currInvNum != 0) {
		if (timer == 0)
		{
			timer = 10;
			if (INPUT.left || TheGamepad->IsPressed(XINPUT_GAMEPAD_DPAD_LEFT))
			{
				removeChild(description);
				pointer += 1;
				if (pointer == currInvNum)
				{
					pointer = 0;
				}
			}
			else if (INPUT.right || TheGamepad->IsPressed(XINPUT_GAMEPAD_DPAD_RIGHT))
			{	
				removeChild(description);
				pointer -= 1;
				if (pointer == -1)
				{
					pointer = currInvNum - 1;
				}
			}

			if (inventory->size() != 0) {
				invLabel->setString((*inventory)[pointer].itemName);
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
				pic->setTexture((*inventory)[pointer].itemPic);
			}
			if (pointer - 1 == -1 && currInvNum >= 2)
			{
				nextPic->setTexture((*inventory)[currInvNum - 1].itemPic);
			}
			else if (pointer - 1 != -1 && currInvNum >= 2)
			{
				nextPic->setTexture((*inventory)[pointer - 1].itemPic);
			}

			if (pointer + 1 == currInvNum && currInvNum >= 3)
			{
				prevPic->setTexture((*inventory)[0].itemPic);
			}
			else if (pointer + 1 != currInvNum && currInvNum >= 3)
			{
				prevPic->setTexture((*inventory)[pointer + 1].itemPic);
			}
		}
	}
}



//void InventoryScene::combine(std::string cOne, std::string cTwo)
//{
//}

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
