#pragma once
#include "Effects/EffectSprite.h"
#include "cocos2d.h"

class Entity : public EffectSprite {
protected:
	bool on_ground;
public:
	cocos2d::Vec2 spd;

	virtual void Move() = 0;
	virtual bool HitDetect(Entity * other) = 0;
	virtual void Update(float dt) = 0;

	void SetOnGround(bool b) {on_ground = b;}
};

class Player;

class Enemy: public Entity {
protected:
	int hp;
public:
	virtual void AI(Player * p, float dt) = 0;
	virtual void Hit(Player * p) = 0;

	virtual void Hurt(int d) = 0;
};

