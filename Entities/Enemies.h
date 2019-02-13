#pragma once
#include"Entities/CoreEntities.h"
#include "Entities/Player.h"

class Knight : public Entity {
private:
	int knight_light;
	bool light_on = false;
	bool face_right = false;
	bool attacking = false;
	int turn;
	cocos2d::Vec2 spd = { 0, 0 };
	const float TURNT_TIME = 3;
	float timer = TURNT_TIME;
	int hp = 5;
public:
	static Knight * create(const std::string& filename);

	void switchLight();
	void moveLightToPlayer();
	static const int ENEMY_SPEED = 10;

	void AI(Player* player, float dt);
	void hurt(int dmg);
	int getHp();
	void hit(int dmg);

	// Inherited via Entity
	virtual bool HitDetect(Entity * other) override;
	virtual void Update(float dt) override;
	virtual void Move() override;
};