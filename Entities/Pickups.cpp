#include "Pickups.h"  
#include "InventoryScene.h"

Pickup * Pickup::create(std::string filename, cocos2d::Vec2 p, itemEnum i)
{
	auto ret = new (std::nothrow) Pickup;

	if (ret && ret->initWithFile(filename)) {
		//ret->autorelease();

		ret->objectType = PICKUP;
		ret->ItemType = i;

		ret->CD = 0.5f;
		ret->temp_CD = 0.5f;

		ret->setAnchorPoint(cocos2d::Vec2(0, 0));
		ret->setPosition(p);

		ret->autorelease();
		return ret;
	}
	CC_SAFE_RELEASE(ret);
	return nullptr;
}

bool Pickup::inRange(Entity * other)
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

void Pickup::Effect(Entity * player, player_inventory * p_inv)
{
	//InventoryScene::pickUpItem(2, "Rose", "Rose.png");
}
