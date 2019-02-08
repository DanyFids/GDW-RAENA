#include "Block.h"
#include "Entities/CoreEntities.h"

Block * Block::create(int x, int y, int w, int h)
{
	auto ret = new (std::nothrow) Block;

	if (ret && ret->init()) {
		ret->autorelease();

		ret->setAnchorPoint(cocos2d::Vec2(0, 0));
		ret->setPositionX(x);
		ret->setPositionY(y);
		ret->setContentSize(cocos2d::Size(w, h));

		return ret;
	}
	CC_SAFE_RELEASE(ret);
	return nullptr;
}

bool Block::HitDetect(Entity * other)
{
	float o_head = other->getPositionY() + (other->getBoundingBox().size.height / 2);
	float o_foot = other->getPositionY() - (other->getBoundingBox().size.height / 2);
	float o_left = other->getPositionX() - (other->getBoundingBox().size.width / 2);
	float o_right = other->getPositionX() + (other->getBoundingBox().size.width / 2);

	if (o_head + other->spd.y > this->getPositionY() && o_foot + other->spd.y < this->getPositionY() + this->getBoundingBox().size.height &&
		o_left < this->getBoundingBox().size.width && o_right > this->getPositionX()) {
		if (other->spd.y > 0) {
			other->spd.y = this->getPositionY() - o_head;
		}
		else {
			other->spd.y = this->getPositionY() + this->getBoundingBox().size.height - o_foot;
		}
	}

	if (o_head > this->getPositionY() && o_foot < this->getPositionY() + this->getBoundingBox().size.height &&
		o_left + other->spd.x < this->getBoundingBox().size.width && o_right + other->spd.x > this->getPositionX()) {
		if (other->spd.x > 0) {
			other->spd.x = this->getPositionX() - o_right;
		}
		else {
			other->spd.x = this->getPositionX() + this->getBoundingBox().size.width - o_left;
		}
	}
	

	return false;
}

void Block::Update(float dt)
{
}

void Block::Move()
{
}
