#pragma once
#include "Entities/CoreEntities.h"

class Torch : public Enemy {
private:
	LightEffect * _light = nullptr;
	int _light_id = -1;
	bool lit = false;

	cocos2d::Vector<cocos2d::Animation * > animations;
public:
	static Torch * create(cocos2d::Vec2 pos, LightEffect * l);

	// Inherited via Entity
	virtual void Move() override;
	virtual bool HitDetect(Entity * other) override;
	virtual void Update(float dt) override;

	virtual void AI(Player * p) override;
	virtual void Hit(Player * p) override;
	virtual void Hurt(int d) override;

	void Light();

	void setLight(LightEffect * l) {
		_light = l;
		setEffect(l, "layerNorm.png");
	}
};