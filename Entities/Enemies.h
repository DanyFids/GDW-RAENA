#pragma once
#include"Entities/CoreEntities.h"
#include "Entities/Player.h"

class Knight : public Enemy {
private:
	int knight_light;
	bool light_on = false;
	bool face_right = false;
	bool attacking = false;
	bool paused = false;
	int turn;
	cocos2d::Vec2 spd = { 0, 0 };
	const float TURNT_TIME = 3;
	float timer = TURNT_TIME;
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