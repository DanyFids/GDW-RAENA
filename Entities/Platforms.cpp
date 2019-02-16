#include "Platforms.h"

Platform * Platform::create(std::string filename, float x, float y)
{
	auto ret = new (std::nothrow) Platform;

	if (ret && ret->initWithFile(filename)) {
		ret->setPositionX(x);
		ret->setPositionY(y);

		ret->origin.x = x;
		ret->origin.y = y;

		ret->autorelease();
		return ret;
	}
	CC_SAFE_RELEASE(ret);
	return nullptr;
}

Platform * Platform::create(std::string filename, cocos2d::Vec2 p)
{
	auto ret = new (std::nothrow) Platform;

	if (ret && ret->initWithFile(filename)) {

		ret->setPosition(p);
		

		ret->origin.x = p.x;
		ret->origin.y = p.y;

		ret->autorelease();
		return ret;
	}
	CC_SAFE_RELEASE(ret);
	return nullptr;
}

Platform * Platform::create(std::string filename, float x, float y, float xSpeed, float ySpeed)
{
	auto ret = new (std::nothrow) Platform;

	if (ret && ret->initWithFile(filename)) {
		
		ret->setPositionX(x);
		ret->setPositionY(y);

		ret->origin.x = x;
		ret->origin.y = y;

		ret->spd.x = xSpeed;
		ret->spd.y = ySpeed;
		ret->autorelease();
		return ret;
	}
	CC_SAFE_RELEASE(ret);
	return nullptr;
}

Platform * Platform::create(std::string filename, cocos2d::Vec2 p, float xSpeed, float ySpeed)
{
	auto ret = new (std::nothrow) Platform;

	if (ret && ret->initWithFile(filename)) {

		ret->setPosition(p);

		ret->origin.x = p.x;
		ret->origin.y = p.y;

		ret->spd.x = xSpeed;
		ret->spd.y = ySpeed;
		ret->autorelease();
		return ret;
	}
	CC_SAFE_RELEASE(ret);
	return nullptr;
}

bool Platform::HitDetect(Entity * other)
{
	float o_TOP = other->getPositionY() + (other->getBoundingBox().size.height / 2);
	float o_BOT = other->getPositionY() - (other->getBoundingBox().size.height / 2);
	float o_LEFT = other->getPositionX() - (other->getBoundingBox().size.width / 2);
	float o_RIGHT = other->getPositionX() + (other->getBoundingBox().size.width / 2);

	//if Entity's TOP plus +- Yspeed > Y value of platform
	//& Entity's BOT +- Yspeed < Y value of platform + sizeOfBoundingBoxHeight
	//&

	float pSurface = this->getBoundingBox().size.height / 2 + (this->getPositionY());
	float pLeft = getPositionX() - (this->getBoundingBox().size.width / 2);
	float pRight = getPositionX() + (this->getBoundingBox().size.width / 2);

	if (o_BOT >= pSurface) {
		if (o_TOP + other->spd.y >= pSurface && o_BOT + other->spd.y < pSurface &&
			o_LEFT < pRight && o_RIGHT > pLeft) {
			if (other->spd.y <= 0) {
				other->spd.y = pSurface - o_BOT;
				other->SetOnGround(true);
			}

		}
	}
																								  //+ this->getBoundingBox().size.height
	//if (o_TOP + other->spd.y > this->getPositionY() && o_BOT + other->spd.y < this->getPositionY() + this->getBoundingBox().size.height &&
	//	o_LEFT < getPositionX() + this->getBoundingBox().size.width && o_RIGHT > this->getPositionX()) {
	//	if (other->spd.y > 0) {
	//		//other->spd.y = this->getPositionY() - o_TOP;
	//	}
	//	else {
	//		other->spd.y = this->getPositionY() + this->getBoundingBox().size.height - o_BOT;
	//		other->SetOnGround(true);
	//	}
	//}

	//if (o_TOP > this->getPositionY() && o_BOT < this->getPositionY() + this->getBoundingBox().size.height &&
	//	o_LEFT + other->spd.x < getPositionX() + this->getBoundingBox().size.width && o_RIGHT + other->spd.x > this->getPositionX()) {
	//	if (other->spd.x > 0) {
	//		other->spd.x = this->getPositionX() - o_RIGHT;																  
	//	}
	//	else {
	//		other->spd.x = this->getPositionX() + this->getBoundingBox().size.width - o_LEFT;
	//	}
	//}

	return false;
}

void Platform::Update(float dt)
{
}

void Platform::Move()
{
}
