#include "PuzzleInteractable.h"
#include "GameplayScene.h"

PuzzleInteract * PuzzleInteract::create(std::string filename, cocos2d::Vec2 p, PuzzleType puz, itemEnum item)
{
	auto ret = new (std::nothrow) PuzzleInteract;

	if (ret && ret->initWithFile(filename)) {
		ret->autorelease();

		ret->objectType = PUZZLE;
		ret->puzzle = puz;
		ret->requiredItem = item;

		//if (key != NONE) { ret->locked = true; } // If this door requires a key it starts as locked.

		ret->CD = 0.5f;
		ret->temp_CD = 0.5f;

		ret->setAnchorPoint(cocos2d::Vec2(0, 0));
		ret->setPosition(p);

		return ret;
	}
	CC_SAFE_RELEASE(ret);
	return nullptr;
}

bool PuzzleInteract::inRange(Entity * other)
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
			o_RIGHT + other->spd.x >= MIN_X - 20.0f && o_LEFT + other->spd.x <= MAX_X + 20.0f) {

			return true;
		}

		return false;
	
	
}

void PuzzleInteract::Effect(Entity * player, player_inventory * p_inv,GameplayScene* scn)
{
	if (puzzle == Princess1 && this->getCooldown() == false) {
		
		Knight * knight = Knight::create("knightwalkyboi0000.png");
		knight->setPosition(cocos2d::Vec2(850, 200 + (knight->getBoundingBox().size.height / 2)));
		scn->addChild(knight);
		scn->setKnight(knight);
	}
}

void PuzzleInteract::Update(float dt)
{
	//Do something?
	return;
}

bool PuzzleInteract::checkItem(itemEnum i)
{
	if (i == requiredItem) {
		return true;
	}
	return false;
}

bool PuzzleInteract::checkPuzzle(PuzzleType p)
{
	if (p == puzzle) {
		return true;
	}
	return false;
}
