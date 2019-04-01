#include "Interactable.h"
#include "HelloWorldScene.h"

USING_NS_CC;

//'External' FUNCTIONS

//int Interactable::getKeys(player_inventory* p_inv,KeyType k)
//{
//	if (k == GEN_KEY) {
//		return p_inv->general_keys;
//	}
//
//	return NONE;
//}
//
//void Interactable::editKeys(player_inventory* p_inv,KeyType k, int i)
//{
//	if (k == GEN_KEY)
//	{
//		if (i > 0) {
//			p_inv->general_keys += i;
//		}
//		else p_inv->general_keys -= i;
//	}
//}

//void Interactable::SceneReturnCallBack(Ref * pSender)
//{
//	Director::getInstance()->replaceScene(HelloWorld::createScene());
//}

void Interactable::Land()
{
}

//'External' FUNCTIONS

//Interactable * Interactable::create(int x, int y, int w, int h, InteractType type, KeyType key)
//{
//	auto ret = new (std::nothrow) Interactable;
//
//	if (ret && ret->init()) {
//		ret->autorelease();
//
//		ret->objectType = type;
//		ret->requiredKey = key;
//
//		if (key != NONE) { ret->locked = true; } // If this door requires a key it starts as locked.
//
//
//		switch (type)
//		{
//		case SWITCH:
//			ret->CD = 0.6f;
//			ret->temp_CD = 0.6f;
//			break;
//		case DOOR:
//			ret->CD = 0.5f;
//			ret->temp_CD = 0.5f;
//			break;
//		case S_DOOR:
//			break;
//		}
//
//		ret->setAnchorPoint(cocos2d::Vec2(0, 0));
//		ret->setPositionX(x);
//		ret->setPositionY(y);
//		ret->setContentSize(cocos2d::Size(w, h));
//
//		auto rect = cocos2d::DrawNode::create();
//		rect->drawSolidRect(cocos2d::Vec2(0, 0), cocos2d::Vec2(w, h), cocos2d::Color4F::BLUE);
//		ret->addChild(rect);
//
//		return ret;
//	}
//	CC_SAFE_RELEASE(ret);
//	return nullptr;
//}
//
//Interactable * Interactable::create(std::string filename, cocos2d::Vec2 p, InteractType type, KeyType key)
//{
//	auto ret = new (std::nothrow) Interactable;
//
//	if (ret && ret->initWithFile(filename)) {
//		ret->autorelease();
//
//		ret->objectType = type;
//		ret->requiredKey = key;
//
//		if (key != NONE) { ret->locked = true; } // If this door requires a key it starts as locked.
//
//		switch (type)
//		{
//		case SWITCH:
//			ret->CD = 0.6f;
//			ret->temp_CD = 0.6f;
//			break;
//		case DOOR:
//			ret->CD = 0.5f;
//			ret->temp_CD = 0.5f;
//			break;
//		case S_DOOR:
//			break;
//		}
//
//		ret->setAnchorPoint(cocos2d::Vec2(0, 0));
//		ret->setPosition(p);
//
//		return ret;
//	}
//	CC_SAFE_RELEASE(ret);
//	return nullptr;
//}

bool Interactable::HitDetect(Entity * other)
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
																									
	if (this->objectType == SWITCH) {																//////// SWITCHES ////////////////
		if (o_TOP >= MIN_Y && o_BOT <= MAX_Y && o_RIGHT >= MIN_X && o_LEFT <= MAX_X)	 // AABB Intersecting Collisions 
		{
			return true;
		}
		else return false;
	}
																									//////// SWITCHES ////////////////
	

																									//////// DOOR ////////////////
	if (this->objectType == DOOR) {						   //Approaching Vector Collision?

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
	}

	return false;
}
																										//////// DOORS ////////////////	

bool Interactable::inRange(Entity * other)
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

//void Interactable::Update(float dt)
//{
//
//	if (this->CoolDownState == true)
//	{
//		this->temp_CD -= dt;
//		if (this->temp_CD <= 0) {
//			this->temp_CD = CD;
//			this->CoolDownState = false;
//		}
//
//	}
//
//}

void Interactable::Move()
{
}

void Interactable::Effect(Entity * player, player_inventory * p_inv)
{
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////	EFFECTS
// What does this specific Interactable Do? Define each Effect in it's own IF statement.
//void Interactable::Effect(InteractType t, Entity * player,player_inventory * p_inv, Scene * scn)
//{	
//	if (t == SWITCH && !(this->CoolDownState) ) {
//
//		this->CoolDownState = true;
//
//		if (this->Active == false) {
//
//			this->Active = true;
//			this->setPosition(this->getPosition() + cocos2d::Vec2(0, 5));
//		}
//		else {
//
//			this->Active = false;
//			this->setPosition(this->getPosition() - cocos2d::Vec2(0, 5));
//		}
//	}
//
//	if (t == DOOR) {
//
//		if (this->locked) {	 // Checks to see A. Door is locked ... B. Player has enough of Key ... C. removes a key and unlocks door
//			 
//			if (this->requiredKey == GEN_KEY) {
//				if (getKeys(p_inv,this->requiredKey) > 0) {
//					{
//						editKeys(p_inv,this->requiredKey, -1);
//						this->locked = false;
//					}
//			   }
//			}
//			//Include Each Keytype below
//
//		}
//
//		if (!(this->CoolDownState) && !(this->locked)) {
//
//			this->CoolDownState = true;
//
//			if (this->Active == false) {
//
//				this->Active = true;
//
//			}
//			else {
//				this->Active = false;
//			}
//		}
//	}
//
//	if (t == S_DOOR) {
//
//		if (this->locked) {	 // Checks to see A. Door is locked ... B. Player has enough of Key ... C. removes a key and unlocks door
//
//			if (this->requiredKey == GEN_KEY) {
//				if (getKeys(p_inv, this->requiredKey) > 0) {
//					{
//						editKeys(p_inv, this->requiredKey, -1);
//						this->locked = false;
//					}
//				}
//			}
//			//Include Each Keytype below
//
//		}
//
//		if (!(this->CoolDownState) && !(this->locked)) {
//
//			this->CoolDownState = true;
//
//			if (this->Active == false) {
//
//				this->Active = true;
//				CC_CALLBACK_1(Interactable::SceneReturnCallBack, scn);
//			}
//			else {
//				this->Active = false;
//			}
//		}
//	}
//}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////	EFFECTS


//InteractType Interactable::getType()
//{
//	return this->objectType;
//}
