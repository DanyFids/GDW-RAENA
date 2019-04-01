#pragma once
#include"Entities/CoreEntities.h"

class Ladder : public Entity {
protected:
	bool player_Top = false;

public:
	static Ladder* create(int x, int y, int w=32, int h=128);

	virtual void Move() override;
	virtual bool HitDetect(Entity * other) override;
	virtual void Update(float dt) override;
	virtual void Land() override {}

	bool PlayerOnTop() {
		return player_Top;
	}
};