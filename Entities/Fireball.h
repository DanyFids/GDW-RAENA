#pragma once
#include "2d/CCSprite.h"
#include "Entities/CoreEntities.h"
#include "Effects/LightEffect.h"

class Fireball : public cocos2d::Sprite {
protected:
	LightEffect * _light;
	int _fire_light = -1;
public:
	Fireball(cocos2d::Vec2 pos, LightEffect * l);
	static Fireball * create(cocos2d::Vec2 pos, LightEffect * l);

	bool HitDetect(Entity * other);
	void Die();
	void SetLight(LightEffect * l) {_light = l;}
	LightEffect* GetLight() {return _light;}
};