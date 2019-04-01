#include "Fireball.h"
#include "CoreEntities.h"

Fireball::Fireball(cocos2d::Vec2 pos, LightEffect * l)
{
	_light = l;
	_fire_light = _light->addLight(cocos2d::Vec3(pos.x, pos.y, 50));
}

Fireball * Fireball::create(cocos2d::Vec2 pos, LightEffect * l)
{
	auto ret = new (std::nothrow) Fireball(pos, l);
	if (ret && ret->initWithFile("fireball0000.png")) {
		cocos2d::Vector<cocos2d::SpriteFrame *> fire_frames = { cocos2d::SpriteFrame::create("fireball0000.png", cocos2d::Rect(0,0,33,18), false, {0,0}, {33,18}),
																cocos2d::SpriteFrame::create("fireball0001.png", cocos2d::Rect(0,0,33,18), false, {0,0}, {33,18}),
																cocos2d::SpriteFrame::create("fireball0002.png", cocos2d::Rect(0,0,33,18), false, {0,0}, {33,18}),
																cocos2d::SpriteFrame::create("fireball0003.png", cocos2d::Rect(0,0,33,18), false, {0,0}, {33,18}),
																cocos2d::SpriteFrame::create("fireball0004.png", cocos2d::Rect(0,0,33,18), false, {0,0}, {33,18}),
																cocos2d::SpriteFrame::create("fireball0005.png", cocos2d::Rect(0,0,33,18), false, {0,0}, {33,18}),
																cocos2d::SpriteFrame::create("fireball0006.png", cocos2d::Rect(0,0,33,18), false, {0,0}, {33,18}) };
		ret->animations.pushBack(cocos2d::Animation::createWithSpriteFrames(fire_frames, 0.05f));
		ret->runAction(cocos2d::RepeatForever::create(cocos2d::Animate::create(ret->animations.at(0))));
		ret->autorelease();
		ret->setPosition(pos);
		return ret;
	}
	CC_SAFE_RELEASE(ret);
	return nullptr;
}

bool Fireball::HitDetect(Entity * other)
{
	float head = this->getPositionY() + (this->getBoundingBox().size.height / 2);
	float foot = this->getPositionY() - (this->getBoundingBox().size.height / 2);
	float left = this->getPositionX() - (this->getBoundingBox().size.width / 2);
	float right = this->getPositionX() + (this->getBoundingBox().size.width / 2);

	float o_head = other->getPositionY() + (other->getBoundingBox().size.height / 2);
	float o_foot = other->getPositionY() - (other->getBoundingBox().size.height / 2);
	float o_left = other->getPositionX() - (other->getBoundingBox().size.width / 2);
	float o_right = other->getPositionX() + (other->getBoundingBox().size.width / 2);

	if (head > o_foot && foot < o_head && left < o_right && o_left < right) {
		return true;
	}
	return false;
}

void Fireball::Die()
{
	_light->rmvLight(_fire_light);
}
