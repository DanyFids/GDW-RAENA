#include"cocos2d.h"
#include "Ladder.h"

Ladder * Ladder::create(int x, int y, int w, int h)
{
	auto ret = new (std::nothrow) Ladder;
	if (ret) {
		ret->setAnchorPoint(cocos2d::Vec2(0,0));
		ret->setPosition(cocos2d::Vec2(x, y));
		ret->setContentSize(cocos2d::Size(cocos2d::Vec2(w, h)));
		
		auto prim = cocos2d::DrawNode::create();
		prim->drawSolidRect(cocos2d::Vec2(0, 0), cocos2d::Vec2(w, h), cocos2d::Color4F::GREEN);
		ret->addChild(prim);

		return ret;
	}

	CC_SAFE_RELEASE(ret);
}

void Ladder::Move()
{
}

bool Ladder::HitDetect(Entity * other)
{
	player_Top = false;

	float t_head = this->getPositionY() + (this->getBoundingBox().size.height);
	float t_foot = this->getPositionY();
	float t_left = this->getPositionX();
	float t_right = this->getPositionX() + (this->getBoundingBox().size.width);

	float o_head = other->getPositionY() + (other->getBoundingBox().size.height / 2);
	float o_foot = other->getPositionY() - (other->getBoundingBox().size.height / 2);
	float o_left = other->getPositionX() - (other->getBoundingBox().size.width / 2);
	float o_right = other->getPositionX() + (other->getBoundingBox().size.width / 2);

	if (t_head <= o_foot && o_foot + other->spd.y < t_head && o_right > t_left && o_left < t_right) {
		other->spd.y = t_head - o_foot;
		player_Top = true;
		other->Land();
		if (o_right < t_right + (other->getBoundingBox().size.width / 4) && o_left > t_left - (other->getBoundingBox().size.width / 4)) {
			return true;
		}
	}
	else {
		if (o_head > t_foot + (other->getBoundingBox().size.height / 2) && o_foot < t_head
			&& o_right < t_right + (other->getBoundingBox().size.width / 4) && o_left > t_left - (other->getBoundingBox().size.width / 4)) {
			return true;
		}
	}

	return false;
}

void Ladder::Update(float dt)
{
}
