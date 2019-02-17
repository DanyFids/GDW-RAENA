#include "Torch.h"
#include "Effects/LightEffect.h"

Torch * Torch::create(cocos2d::Vec2 pos, LightEffect * l)
{
	auto ret = new (std::nothrow) Torch;
	if (ret && ret->initWithFile("temp_torch.png")) {
		ret->setPosition(pos);
		ret->setLight(l);

		cocos2d::Vector<cocos2d::SpriteFrame *> unlit_frames = {cocos2d::SpriteFrame::create("temp_torch.png", cocos2d::Rect(0,0,16,32))};
		cocos2d::Vector<cocos2d::SpriteFrame *> lit_frames = { cocos2d::SpriteFrame::create("temp_torch_lit.png", cocos2d::Rect(0,0,16,32)) };

		ret->animations.pushBack(cocos2d::Animation::createWithSpriteFrames(unlit_frames));
		ret->animations.pushBack(cocos2d::Animation::createWithSpriteFrames(lit_frames));

		ret->autorelease();
		return ret;
	}
	CC_SAFE_RELEASE(ret);
	return nullptr;
}

void Torch::Move()
{
}

bool Torch::HitDetect(Entity * other)
{
	return false;
}

void Torch::Update(float dt)
{
}

void Torch::AI(Player * p)
{
}

void Torch::Hit(Player * p)
{
}

void Torch::Hurt(int d)
{
	Light();
}

void Torch::Light()
{
	if (_light != nullptr && !lit) {
		this->runAction(cocos2d::RepeatForever::create(cocos2d::Animate::create(animations.at(0))));
		_light_id = _light->addLight(cocos2d::Vec3(getPositionX(), getPositionY() + (getBoundingBox().size.height / 3), 50));
		lit = true;
	}
}
