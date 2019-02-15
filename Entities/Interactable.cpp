#include "Interactable.h"

Interactable * Interactable::create(int x, int y, int w, int h, InteractType type)
{
	auto ret = new (std::nothrow) Interactable;

	if (ret && ret->init()) {
		ret->autorelease();

		ret->objectType = type;

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

bool Interactable::HitDetect(Entity * other)
{
	//	 Entities Bound.
	float o_TOP = other->getPositionY() + (other->getBoundingBox().size.height / 2);
	float o_BOT = other->getPositionY() - (other->getBoundingBox().size.height / 2);

	float o_LEFT = other->getPositionX() - (other->getBoundingBox().size.width / 2);
	float o_RIGHT = other->getPositionX() + (other->getBoundingBox().size.width / 2);


	//Interactable's Position Bounds
	float MAX_Y = this->getPositionY() + (this->getBoundingBox().size.height/2);
	float MIN_Y = this->getPositionY() - (this->getBoundingBox().size.height / 2);

	float MAX_X = this->getPositionX() + (this->getBoundingBox().size.width / 2);
	float MIN_X = this->getPositionX() + (this->getBoundingBox().size.width / 2);;


	if (o_TOP >= MIN_Y || o_BOT <= MAX_Y && o_RIGHT >= MIN_X || o_LEFT <= MAX_X) 
	{
		return true;
	}
	else
	return false;
}

void Interactable::Update(float dt)
{
}

void Interactable::Move()
{
}

void Interactable::Effect(InteractType t)
{
	
	if (t == SWITCH) {
		if (this->Active == false) {
			this->Active = true;
			this->setPosition(this->getPosition() + cocos2d::Vec2(0, 5));
		}
		else {
			this->Active = false;
			this->setPosition(this->getPosition() - cocos2d::Vec2(0, 5));
		}
		
	}


}

InteractType Interactable::getType()
{
	return this->objectType;
}
