#include "Door.h"
#include "GameplayScene.h"
#include "classes/LevelManager/LevelManager.h"


Door * Door::create(int x, int y, int w, int h, KeyType key)
{
	auto ret = new (std::nothrow) Door;

	if (ret && ret->init()) {
		ret->autorelease();

		ret->objectType = DOOR;
		ret->requiredKey = key;

		if (key != NONE) { ret->locked = true; } // If this door requires a key it starts as locked.
	
		ret->CD = 0.5f;
		ret->temp_CD = 0.5f;

		ret->setAnchorPoint(cocos2d::Vec2(0, 0));
		ret->setPositionX(x);
		ret->setPositionY(y);
		ret->setContentSize(cocos2d::Size(w, h));

		auto rect = cocos2d::DrawNode::create();
		rect->drawSolidRect(cocos2d::Vec2(0, 0), cocos2d::Vec2(w, h), cocos2d::Color4F::BLUE);
		ret->addChild(rect);

		return ret;
	}
	CC_SAFE_RELEASE(ret);
	return nullptr;
}

Door * Door::create(std::string filename, cocos2d::Vec2 p, KeyType key)
{
	auto ret = new (std::nothrow) Door;

	if (ret && ret->initWithFile(filename)) {
		ret->autorelease();

		ret->objectType = DOOR;
		ret->requiredKey = key;

		if (key != NONE) { ret->locked = true; } // If this door requires a key it starts as locked.

		ret->CD = 0.5f;
		ret->temp_CD = 0.5f;

		ret->setAnchorPoint(cocos2d::Vec2(0, 0));
		ret->setPosition(p);

		return ret;
	}
	CC_SAFE_RELEASE(ret);
	return nullptr;
}

bool Door::HitDetect(Entity * other)
{
	//	 Entity's Bound.
	float o_TOP = other->getPositionY() + (other->getBoundingBox().size.height / 2); //This is for Objs with A Sprite (beccause it has a centered Origin)
	float o_BOT = other->getPositionY() - (other->getBoundingBox().size.height / 2);

	float o_LEFT = other->getPositionX() - (other->getBoundingBox().size.width / 2);
	float o_RIGHT = other->getPositionX() + (other->getBoundingBox().size.width / 2);

	//

	//Interactable's Position Bounds
																								  //Use this when we convert to sprites
	//float MAX_Y = this->getPositionY() + (this->getBoundingBox().size.height / 2);
	//float MIN_Y = this->getPositionY() - (this->getBoundingBox().size.height / 2);
	//
	//float MAX_X = this->getPositionX() + (this->getBoundingBox().size.width  / 2);
	//float MIN_X = this->getPositionX() - (this->getBoundingBox().size.width  / 2);	 

	float MAX_Y = this->getPositionY() + (this->getBoundingBox().size.height);				// For simple shapes
	float MIN_Y = this->getPositionY();

	float MAX_X = this->getPositionX() + (this->getBoundingBox().size.width);
	float MIN_X = this->getPositionX();

	if (!(this->Active)) { // CLOSED
				//o_TOP >= MIN_Y && o_BOT <= MAX_Y && o_RIGHT >= MIN_X && o_LEFT <= MAX_X
		if (o_TOP > MIN_Y && o_BOT < MAX_Y &&
			o_RIGHT + other->spd.x > MIN_X && o_LEFT + other->spd.x < MAX_X) {

			if (other->spd.x >= 0.0f) {
				other->spd.x = MIN_X - o_RIGHT;
				return true;
			}
			else
			{
				other->spd.x = MAX_X - o_LEFT;
				return true;
			}

			return false;


		}

	}

	return false;
}

bool Door::inRange(Entity * other)
{
	//	 Entity's Bound.
	float o_TOP = other->getPositionY() + (other->getBoundingBox().size.height / 2); //This is for Objs with A Sprite (beccause it has a centered Origin)
	float o_BOT = other->getPositionY() - (other->getBoundingBox().size.height / 2);

	float o_LEFT = other->getPositionX() - (other->getBoundingBox().size.width / 2);
	float o_RIGHT = other->getPositionX() + (other->getBoundingBox().size.width / 2);

	//

	//Interactable's Position Bounds
																								  //Use this when we convert to sprites
	//float MAX_Y = this->getPositionY() + (this->getBoundingBox().size.height / 2);
	//float MIN_Y = this->getPositionY() - (this->getBoundingBox().size.height / 2);
	//
	//float MAX_X = this->getPositionX() + (this->getBoundingBox().size.width  / 2);
	//float MIN_X = this->getPositionX() - (this->getBoundingBox().size.width  / 2);	 

	// For simple shapes

	float MAX_Y = this->getPositionY() + (this->getBoundingBox().size.height);
	float MIN_Y = this->getPositionY();

	float MAX_X = this->getPositionX() + (this->getBoundingBox().size.width);
	float MIN_X = this->getPositionX();

	if (o_TOP > MIN_Y && o_BOT < MAX_Y &&
		o_RIGHT + other->spd.x >= MIN_X - 15.0f && o_LEFT + other->spd.x <= MAX_X + 15.0f) {

		return true;
	}
	return false;
}

void Door::Update(float dt)
{
	if (this->CoolDownState == true)
	{
		this->temp_CD -= dt;
		if (this->temp_CD <= 0) {
			this->temp_CD = CD;
			this->CoolDownState = false;
		}

	}
}



void Door::Effect(Entity * player, player_inventory * p_inv)
{
	if (this->locked) {	 // Checks to see A. Door is locked ... B. Player has enough of Key ... C. removes a key and unlocks door

		if (this->requiredKey == GEN_KEY) {
			if (getKeys(p_inv, this->requiredKey) > 0) {
				{
					editKeys(p_inv, this->requiredKey, -1);
					this->locked = false;
				}
			}
		}
		//Include Each Keytype below

	}

	if (!(this->CoolDownState) && !(this->locked)) {

		this->CoolDownState = true;

		if (this->Active == false) {

			this->Active = true;

		}
		else {
			this->Active = false;
		}
	}
}

int Door::getKeys(player_inventory * p_inv, KeyType k)
{	
		if (k == GEN_KEY) {
			return p_inv->general_keys;
		}

		return NONE;
}

void Door::editKeys(player_inventory * p_inv, KeyType k, int i)
{
	if (k == GEN_KEY)
	{
		if (i > 0) {
			p_inv->general_keys += i;
		}
		else p_inv->general_keys -= i;
	}
}

//// ----------------------- S C E N E		D O O R	----------------------------------

SceneDoor * SceneDoor::create(std::string filename, cocos2d::Vec2 p, cocos2d::Vec2 pPos, levelEnum lvl, KeyType key)
{
	auto ret = new (std::nothrow) SceneDoor;

	if (ret && ret->initWithFile(filename)) {
		ret->autorelease();
		
		cocos2d::Vector<cocos2d::SpriteFrame *> open_frames = { cocos2d::SpriteFrame::create("closed_door_side.png", cocos2d::Rect(0,0,42,64), false, {0,0}, {42,64}) };
		cocos2d::Vector<cocos2d::SpriteFrame *> closed_frames = { cocos2d::SpriteFrame::create("open_door.png", cocos2d::Rect(0,0,42,64), false, {0,0}, {42,64}) };
		ret->animations.pushBack(cocos2d::Animation::createWithSpriteFrames(open_frames, 0.1f));
		ret->animations.pushBack(cocos2d::Animation::createWithSpriteFrames(closed_frames, 0.1f));

		ret->objectType = S_DOOR;
		ret->requiredKey = key;
		ret->goTo = lvl;
		ret->movePlayer = pPos;
		//ret->goTo = scn;

		if (key != NONE) { ret->locked = true; } // If this door requires a key it starts as locked.

		ret->CD = 0.5f;
		ret->temp_CD = 0.5f;

		ret->setAnchorPoint(cocos2d::Vec2(0, 0));
		ret->setPosition(p);

		return ret;
	}
	CC_SAFE_RELEASE(ret);
	return nullptr;
}

void SceneDoor::Effect(Entity * player, player_inventory * p_inv)
{
	if (this->locked) {	 // Checks to see A. Door is locked ... B. Player has enough of Key ... C. removes a key and unlocks door

		if (this->requiredKey == GEN_KEY) {
			if (getKeys(p_inv, this->requiredKey) > 0) {
				{
					editKeys(p_inv, this->requiredKey, -1);
					this->locked = false;
				}
			}
		}
		//Include Each Keytype below

	}

	if (!(this->CoolDownState) && !(this->locked)) {

		this->CoolDownState = true;

		if (this->Active == false) {

			this->Active = true;

			switch (this->goTo) {
			case A1_R1:
				cocos2d::Director::getInstance()->replaceScene(LevelManager::GetLevel(A1_R1));
				//GameplayScene::movePlayer();
				player = nullptr;
				break;
			case A1_R2:
				cocos2d::Director::getInstance()->replaceScene(LevelManager::GetLevel(A1_R2));
				player->setPosition(this->movePlayer);
				player = nullptr;
				break;
			case A1_R3:
				cocos2d::Director::getInstance()->replaceScene(LevelManager::GetLevel(A1_R3));
				player->setPosition(this->movePlayer);
				break;
			case A1_R4:
				cocos2d::Director::getInstance()->replaceScene(LevelManager::GetLevel(A1_R4));
				player->setPosition(this->movePlayer);
				break;
			case A1_R5:
				cocos2d::Director::getInstance()->replaceScene(LevelManager::GetLevel(A1_R5));
				player->setPosition(this->movePlayer);
				break;
			case A1_R6:
				cocos2d::Director::getInstance()->replaceScene(LevelManager::GetLevel(A1_R6));
				player->setPosition(this->movePlayer);
				break;
			case TUT_LVL1:
				//cocos2d::Director::getInstance()->pushScene(currScene);
				cocos2d::Director::getInstance()->replaceScene(LevelManager::GetLevel(TUT_LVL1));
				player->setPosition(this->movePlayer);
				break;

			}
			

		}
		else {
			this->Active = false;
		}
	}

}


// LOAD ZONE

LoadZone * LoadZone::create(int x, int y, int w, int h, levelEnum dest,cocos2d::Vec2 pPos)
{
	auto ret = new (std::nothrow) LoadZone;

	if (ret && ret->init()) {
		ret->autorelease();

		ret->objectType = LOAD_ZONE;
		ret->goTo = dest;
		ret->movePlayer = pPos;
		//ret->requiredKey = key;

		//if (key != NONE) { ret->locked = true; } // If this door requires a key it starts as locked.

		//ret->CD = 0.5f;
		//ret->temp_CD = 0.5f;

		ret->setAnchorPoint(cocos2d::Vec2(0, 0));
		ret->setPositionX(x);
		ret->setPositionY(y);
		ret->setContentSize(cocos2d::Size(w, h));
		

		auto rect = cocos2d::DrawNode::create();
		rect->drawSolidRect(cocos2d::Vec2(0, 0), cocos2d::Vec2(w, h), cocos2d::Color4F::BLUE);
		
		ret->addChild(rect);

		return ret;
	}
	CC_SAFE_RELEASE(ret);
	return nullptr;
}

void LoadZone::Effect(Entity * player)
{
	switch (this->goTo) {
	case A1_R1:
		cocos2d::Director::getInstance()->replaceScene(LevelManager::GetLevel(A1_R1));
		LevelManager::GetLevel(A1_R1)->movePlayer(player, movePlayer);
		break;

	case A1_R2:
		cocos2d::Director::getInstance()->replaceScene(LevelManager::GetLevel(A1_R2));
		LevelManager::GetLevel(A1_R2)->movePlayer(player, movePlayer);
		break;

	case A1_R3:
		cocos2d::Director::getInstance()->replaceScene(LevelManager::GetLevel(A1_R3));
		LevelManager::GetLevel(A1_R3)->movePlayer(player, movePlayer);
		break;

	case A1_R4:
		cocos2d::Director::getInstance()->replaceScene(LevelManager::GetLevel(A1_R4));
		LevelManager::GetLevel(A1_R4)->movePlayer(player, movePlayer);
		break;

	case A1_R5:
		cocos2d::Director::getInstance()->replaceScene(LevelManager::GetLevel(A1_R5));
		LevelManager::GetLevel(A1_R5)->movePlayer(player, movePlayer);
		break;

	case A1_R6:
		cocos2d::Director::getInstance()->replaceScene(LevelManager::GetLevel(A1_R6));
		LevelManager::GetLevel(A1_R6)->movePlayer(player, movePlayer);
		break;
	case TUT_LVL1:
		//cocos2d::Director::getInstance()->pushScene(currScene);
		cocos2d::Director::getInstance()->replaceScene(LevelManager::GetLevel(TUT_LVL1));
		player->setPosition(this->movePlayer);
		break;

	}
}

bool LoadZone::HitDetect(Entity * other)
{
	float o_TOP = other->getPositionY() + (other->getBoundingBox().size.height / 2); //This is for Objs with A Sprite (beccause it has a centered Origin)
	float o_BOT = other->getPositionY() - (other->getBoundingBox().size.height / 2);

	float o_LEFT = other->getPositionX() - (other->getBoundingBox().size.width / 2);
	float o_RIGHT = other->getPositionX() + (other->getBoundingBox().size.width / 2);

	float MAX_Y = this->getPositionY() + (this->getBoundingBox().size.height);				// For simple shapes
	float MIN_Y = this->getPositionY();

	float MAX_X = this->getPositionX() + (this->getBoundingBox().size.width);
	float MIN_X = this->getPositionX();

				//o_TOP >= MIN_Y && o_BOT <= MAX_Y && o_RIGHT >= MIN_X && o_LEFT <= MAX_X
		if (o_TOP > MIN_Y && o_BOT < MAX_Y &&
			o_RIGHT + other->spd.x > MIN_X && o_LEFT + other->spd.x < MAX_X) {

			if (other->spd.x >= 0.0f) {
				this->Effect(other);
				return true;
			}
			else
			{
				this->Effect(other);
				return true;
			}

			return false;


		}

	return false;
}


