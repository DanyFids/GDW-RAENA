#pragma once
#include "Entities/CoreEntities.h"

class Pushable : public Entity {
	bool pushed = false;
	Entity * _other;

	cocos2d::Vec2 end;
	cocos2d::Vec2 start;

public:
	static Pushable* create(int x, int y, int w, int h, cocos2d::Vec2 s, cocos2d::Vec2 e);

	// Inherited via Entity
	virtual bool HitDetect(Entity * other) override;
	virtual void Update(float dt) override;
	virtual void Move() override;

	CREATE_FUNC(Pushable);

	// Inherited via Entity
	virtual void Land() override;
};