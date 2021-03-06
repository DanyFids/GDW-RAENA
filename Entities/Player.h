#pragma once
#include"Entities/CoreEntities.h"

namespace cocos2d {
	class Scene;
};

class Fireball;

class Player : public Entity {
private:
	const float ATK_TIME = 0.3f;
	const int DMG = 1;

	int player_light;
	bool light_on = false;
	bool attacking = false;
	bool face_right = true;
	float atk_timer = 0;

	cocos2d::Scene * scn = nullptr;

	Fireball * atk = nullptr;
public:
	static Player * create(const std::string& filename, cocos2d::Scene * s);

	void switchLight();
	void moveLightToPlayer();

	// Inherited via Entity
	virtual bool HitDetect(Entity * other) override;
	virtual void Update(float dt) override;
	virtual void Move() override;

	void Jump();
	void Attack();

	void HitDetectEnem(Enemy * e);

	void SetScene(cocos2d::Scene * s) {
		scn = s;
	};
	cocos2d::Scene * GetScene() {
		return scn;
	}
};