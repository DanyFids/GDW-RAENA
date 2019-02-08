#pragma once
#include "cocos2d.h"

class LightEffect;

class EffectLayer : public cocos2d::LayerColor {
protected:
	virtual void draw(cocos2d::Renderer *renderer, const cocos2d::Mat4 &transform, uint32_t flags) override;
	~EffectLayer();

	LightEffect * _effect = nullptr;

public:
	static EffectLayer * create(cocos2d::Vec4 color);
	static EffectLayer * create(float r, float g, float b, float a);

	void setEffect(LightEffect * e);
};