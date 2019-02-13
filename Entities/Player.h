#pragma once
#include"Entities/CoreEntities.h"

class Player : public Entity {
private:
	int player_light;
	bool light_on = false;
	int hp = 6;
public:
	static Player * create(const std::string& filename);

	void switchLight();
	void moveLightToPlayer();
	void hurt(int dmg);

	// Inherited via Entity
	virtual bool HitDetect(Entity * other) override;
	virtual void Update(float dt) override;
	virtual void Move() override;
};