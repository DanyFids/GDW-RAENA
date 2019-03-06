#include "Door.h"

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

			if (other->spd.x >= 0) {
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

void Door::Effect(InteractType t, Entity * player, player_inventory * p_inv)
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
