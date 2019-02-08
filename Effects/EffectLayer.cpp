#include "EffectLayer.h"
#include "LightEffect.h"

USING_NS_CC;

EffectLayer * EffectLayer::create(Vec4 color) {
	auto ret = new (std::nothrow) EffectLayer;
	if (ret && ret->initWithColor(Color4B(color.x, color.y, color.z, color.w))) {
		ret->autorelease();
		return ret;
	}
	CC_SAFE_RELEASE(ret);
	return nullptr;
};

EffectLayer * EffectLayer::create(float r, float g, float b, float a) {
	auto ret = new (std::nothrow) EffectLayer;
	if (ret && ret->initWithColor(Color4B(r, g, b, a))) {
		ret->autorelease();
		return ret;
	}
	CC_SAFE_RELEASE(ret);
	return nullptr;
};

void EffectLayer::setEffect(LightEffect * e) {
	if (_effect != e) {

		CC_SAFE_RELEASE(_effect);
		_effect = e;
		CC_SAFE_RETAIN(_effect);

		setGLProgramState(_effect->getGLProgramState());
	}
}

void EffectLayer::draw(cocos2d::Renderer *renderer, const cocos2d::Mat4 &transform, uint32_t flags) {
	if (_effect != nullptr)
	{
		//_effect->prepareForRender(this);
	}
	LayerColor::draw(renderer, transform, flags);
	renderer->render();
}

EffectLayer::~EffectLayer()
{
	CC_SAFE_RELEASE(_effect);
}