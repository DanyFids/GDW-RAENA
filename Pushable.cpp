#include "Pushable.h"
#include "Constants.h"
#include "2d/CCDrawNode.h"

Pushable * Pushable::create(int x, int y, int w, int h, cocos2d::Vec2 s, cocos2d::Vec2 e)
{
	auto ret = new (std::nothrow) Pushable;

	if (ret && ret->init()) {
		ret->autorelease();

		ret->start = s;
		ret->end = e;

		ret->setPositionX(x);
		ret->setPositionY(y);
		ret->setContentSize(cocos2d::Size(w, h));

		auto rect = cocos2d::DrawNode::create();
		rect->drawSolidRect(cocos2d::Vec2(0, 0), cocos2d::Vec2(w, h), cocos2d::Color4F::WHITE);
		ret->addChild(rect);

		return ret;
	}
	CC_SAFE_RELEASE(ret);
	return nullptr;
}

bool Pushable::HitDetect(Entity * other)
{
	float o_head = other->getPositionY() + (other->getBoundingBox().size.height / 2.0f);
	float o_foot = other->getPositionY() - (other->getBoundingBox().size.height / 2.0f);
	float o_left = other->getPositionX() - (other->getBoundingBox().size.width / 2.0f);
	float o_right = other->getPositionX() + (other->getBoundingBox().size.width / 2.0f);

	float t_head = this->getPositionY() + (this->getBoundingBox().size.height / 2.0f);
	float t_foot = this->getPositionY() - (this->getBoundingBox().size.height / 2.0f);
	float t_left = this->getPositionX() - (this->getBoundingBox().size.width / 2.0f);
	float t_right = this->getPositionX() + (this->getBoundingBox().size.width / 2.0f);

	if (o_head + other->spd.y > t_foot && o_foot + other->spd.y < t_head &&
		o_left < t_right && o_right > t_left) {
		if (other->spd.y > 0) {
			other->spd.y = t_foot - o_head;

		}
		else {
			other->spd.y = t_head - o_foot;
			other->Land();
		}
	}

	if (o_head > t_foot && o_foot < t_head &&
		o_left + other->spd.x < t_right && o_right + other->spd.x > t_left) {
		if (other->spd.x > 0) {
			/*
			pushed = true;
			_other = other;*/
			if (other->isOnGround())
			{
				float n = t_left - o_right;
				spd.x = (other->spd.x - n) * 3 / 4;
				other->spd.x = n;
				//other->spd.x * 3 / 4;
				//spd.x = (other->spd.x);
				if (this->getPositionX() + this->spd.x <= start.x)
				{
					cocos2d::Vec2 newSpd = start - getPosition();
					other->spd.x = (t_right + newSpd.x) - o_left;
					this->spd = newSpd;
				}
				float testpos = this->getPositionX();
				if (this->getPositionX()+ this->spd.x >=end.x)
				{
					cocos2d::Vec2 newSpd = end - getPosition();
					other->spd.x = (t_left + newSpd.x) - o_right;
					this->spd = newSpd;
					//this->setPosition(end);
				}
			}
			else {
				if (other->spd.x > 0) {
					other->spd.x = t_left - o_right;
				}
				else {
					other->spd.x = t_right - o_left;
				}

			}
		}
		else {
			/*float n = t_right - o_left;
			spd.x = (other->spd.x + n) * 3 / 4;
			other->spd.x = n;
			pushed = true;
			_other = other;*/
			if (other->isOnGround())
			{
				float n = t_right - o_left;
				spd.x = (other->spd.x - n) * 3 / 4;
				other->spd.x = n;
				//other->spd.x * 3 / 4;
				//spd.x = (other->spd.x);
				if (this->getPositionX() + this->spd.x <= start.x)
				{
					cocos2d::Vec2 newSpd = start - getPosition();
					other->spd.x = (t_right + newSpd.x) - o_left;
					this->spd = newSpd;
				}
				float testpos = this->getPositionX();
				if (this->getPositionX() + this->spd.x >= end.x)
				{
					cocos2d::Vec2 newSpd = end - getPosition();
					other->spd.x = (t_left + newSpd.x) - o_right;
					this->spd = newSpd;
					//this->setPosition(end);
				}
			}
			else {
				if (other->spd.x > 0) {
					other->spd.x = t_left - o_right;
				}
				else {
					other->spd.x = t_right - o_left;
				}
			}
		}
	}
	
	return false;
}

void Pushable::Update(float dt)
{
	/*spd.y += GRAVITY * dt;
	if (spd.y < T_VELOCITY) {
		spd.y = T_VELOCITY;
	}*/
	spd.x = 0;

	//pushed = false;
}

void Pushable::Move()
{
	setPosition(getPosition() + spd);
	/*if(pushed){
		_other->setPositionX(_other->getPositionX() + spd.x);
	}*/

	/*if ((on_ground))
	{
		spd.y = 0;
	}

	if (spd.y != 0 && on_ground) {
		on_ground = false;
	}*/
}

void Pushable::Land()
{
}
