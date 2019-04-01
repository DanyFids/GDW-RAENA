#pragma once
#include "Entities/CoreEntities.h"

class Torch : public Enemy {
private:
	LightEffect * _light = nullptr;
	int _light_id = -1;

public:
	static Torch * create(cocos2d::Vec2 pos, LightEffect * l);

	// Inherited via Entity
	virtual void Move() override;
	virtual bool HitDetect(Entity * other) override;
	virtual void Update(float dt) override;
	virtual void Land() override {}

	virtual void AI(Player * p, float dt) override;
	virtual void Hit(Player * p) override;
	virtual void Hurt(int d) override;

	void Light();

	bool lit = false;

	void setLight(LightEffect * l) {
		_light = l;
		setEffect(l, "layerNorm.png");
	}
};