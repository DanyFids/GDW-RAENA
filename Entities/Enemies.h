#pragma once
#include"Entities/CoreEntities.h"
#include "Entities/Player.h"
#include "Torch.h"

class Knight : public Enemy {
private:
	bool death = false;
	int knight_light;
	bool light_on = false;
	bool face_right = true;
	bool attacking = false;
	bool paused = false;
	bool swipe = false;
	bool delay = false;
	bool dieDone = false;
	int turn;
	const float TURNT_TIME = 3;
	const float CHOP_TIME = 1;
	const float HIT_TIME = 0.5;
	const float INVINCE_TIME = 1;
	const float DEATH_TIME = 8.7;
	const float REVIVE_TIME = 5.4;
	float revive_time = 0;
	float death_time = DEATH_TIME;
	float invinceTime = 0;
	float timer = TURNT_TIME;
	float swing = CHOP_TIME;
	float hitTimer = HIT_TIME;
	int hp = 1;
	int charge = 20;
public:
	static Knight * create(const std::string& filename);

	bool getDeath() { return death; }
	void setDeath(bool die) { death = die; }
	void moveLightToKnight();
	static const int ENEMY_SPEED = 10;
	bool onGround();

	// Inherited via Enemy
	void AI(Player* player, float dt);
	void Hurt(int dmg);
	void Hit(Player * p);
	int getHp();

	// Inherited via Entity
	virtual bool HitDetect(Entity * other) override;
	virtual void Update(float dt) override;
	virtual void Move() override;
};

class Moth : public Enemy {
private:
	bool death = false;
	bool face_right = true;
	bool go_up = true;
	bool attacking = false;
	bool torchFound = false;
	bool paused = false;
	int turn;
	const float TURNT_TIME = 3;
	const float INVINCE_TIME = 1;
	const float DEATH_TIME = 0;
	float death_time = DEATH_TIME;
	float invinceTime = 0;
	float timer = TURNT_TIME;
	int hp = 1;
	cocos2d::Vector<Torch *> * torches;
public:
	static Moth * create(const std::string& filename, cocos2d::Vector<Torch *> * t);

	bool getDeath() { return death; }
	void setDeath(bool die) { death = die; }
	void setSpd(cocos2d::Vec2 m_spd);
	cocos2d::Vec2 getSpd();

	void setXDir(bool _x);
	void setYDir(bool _y);

	// Inherited via Enemy
	void AI(Player* player, float dt);
	void Hurt(int dmg);
	void Hit(Player * p);
	int getHp();

	// Inherited via Entity
	virtual bool HitDetect(Entity * other) override;
	virtual void Update(float dt) override;
	virtual void Move() override;
};

class Rat : public Enemy {
private:
	bool death = false;
	bool face_right = false;
	bool attacking = false;
	bool delay = false;
	bool paused = false;
	int turn;
	const float TURNT_TIME = 3;
	const float INVINCE_TIME = 1;
	const float DEATH_TIME = 0;
	float death_time = DEATH_TIME;
	float invinceTime = 0;
	float timer = TURNT_TIME;
	int hp = 1;
	Entity * platform;
	float pLeft;
	float pRight;
public:
	static Rat * create(const std::string& filename, Entity * Platform);

	bool getDeath() { return death; }
	void setDeath(bool die) { death = die; }

	// Inherited via Enemy
	void AI(Player* player, float dt);
	void Hurt(int dmg);
	void Hit(Player * p);
	int getHp();

	// Inherited via Entity
	virtual bool HitDetect(Entity * other) override;
	virtual void Update(float dt) override;
	virtual void Move() override;
};

class BossKnight : public Enemy {
private:

public:
	// Inherited via Enemy
	void AI(Player* player, float dt);
	void Hurt(int dmg);
	void Hit(Player * p);

	// Inherited via Entity
	virtual bool HitDetect(Entity * other) override;
	virtual void Update(float dt) override;
	virtual void Move() override;
};