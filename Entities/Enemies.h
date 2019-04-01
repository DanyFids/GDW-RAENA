#pragma once
#include"Entities/CoreEntities.h"
#include "Entities/Player.h"
#include "Torch.h"

class Knight : public Enemy {
private:
	int knight_light;
	bool light_on = false;
	bool face_right = true;
	bool attacking = false;
	bool paused = false;
	bool swipe = false;
	bool delay = false;
	int turn;
	const float TURNT_TIME = 3;
	const float CHOP_TIME = 1;
	const float HIT_TIME = 0.5;
	float timer = TURNT_TIME;
	float swing = CHOP_TIME;
	float hitTimer = HIT_TIME;
	int hp = 5;
	int charge = 20;
public:
	static Knight * create(const std::string& filename);

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
	bool face_right = true;
	bool go_up = true;
	bool attacking = false;
	bool delay = false;
	bool paused = false;
	int turn;
	const float TURNT_TIME = 3;
	float timer = TURNT_TIME;
	int hp = 5;
public:
	static Moth * create(const std::string& filename);

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
	bool face_right = true;
	bool attacking = false;
	bool delay = false;
	bool paused = false;
	int turn;
	const float TURNT_TIME = 3;
	float timer = TURNT_TIME;
	int hp = 2;
	Entity * platform;
public:
	static Rat * create(const std::string& filename, Entity * Platform);

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