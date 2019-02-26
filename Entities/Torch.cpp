#include "Torch.h"
#include "Effects/LightEffect.h"

Torch * Torch::create(cocos2d::Vec2 pos, LightEffect * l)
{
	auto ret = new (std::nothrow) Torch;
	if (ret && ret->initWithFile("unlitTorchyBoi.png")) {
		ret->setPosition(pos);
		ret->setLight(l);

		cocos2d::Vector<cocos2d::SpriteFrame *> unlit_frames = { cocos2d::SpriteFrame::create("unlitTorchyBoi.png", cocos2d::Rect(0,0,24,28), false, {0,0}, {24,28}) };
		cocos2d::Vector<cocos2d::SpriteFrame *> lit_frames = { cocos2d::SpriteFrame::create("torch_lit-1.png", cocos2d::Rect(0,0,24,59), false, {0,0}, {24,59}),
																cocos2d::SpriteFrame::create("torch_lit-2.png", cocos2d::Rect(0,0,24,59), false, {0,0}, {24,59}),
																cocos2d::SpriteFrame::create("torch_lit-3.png", cocos2d::Rect(0,0,24,59), false, {0,0}, {24,59}),
																cocos2d::SpriteFrame::create("torch_lit-4.png", cocos2d::Rect(0,0,24,59), false, {0,0}, {24,59}) };

		ret->animations.pushBack(cocos2d::Animation::createWithSpriteFrames(unlit_frames, 0.1f));
		ret->animations.pushBack(cocos2d::Animation::createWithSpriteFrames(lit_frames, 0.15f));

		ret->runAction(cocos2d::RepeatForever::create(cocos2d::Animate::create(ret->animations.at(0))));

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
		this->stopAllActions();
		this->runAction(cocos2d::RepeatForever::create(cocos2d::Animate::create(animations.at(1))));
		_light_id = _light->addLight(cocos2d::Vec3(getPositionX(), getPositionY() + (getBoundingBox().size.height / 3), 50));
		lit = true;
	}
}
