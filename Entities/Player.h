#pragma once
#include"Entities/CoreEntities.h"
#include "Enums.h"

namespace cocos2d {
	class Scene;
};

class Fireball;
class Ladder;

enum PlayerState {
	PS_Stand,
	PS_Jump,
	PS_Attack,
	PS_Climb,
	PS_Glide,
	PS_Crouch,
	PS_HURT
};

class Player : public Entity {
private:
	bool face_right = true;
	const float ATK_TIME = 0.3f;
	const float GLIDE_TIME = 2;
	const float KNOCK_TIME = 0.5f;
	const float INVINCE_TIME = 2.0f;

	const int DMG = 1;
	int player_light;
	bool light_on = false;
	int hp = 6;
	bool attacking = false;
	bool glide_used = false;
	float atk_timer = 0;
	float glide_timer=0;
	float knock_timer=0;
	float invince_timer = 0;

	PlayerState state = PS_Stand;

	//Number of Keys 
	int genKeys = 0; //General Key (can be used on most doors)

	cocos2d::Scene * scn = nullptr;

	Fireball * atk = nullptr;
	Ladder * climb_lad = nullptr;
public:
	static Player * create(const std::string& filename, cocos2d::Scene * s);

	void switchLight();
	void moveLightToPlayer();
	void hurt(int dmg);

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

	PlayerState getState() {
		return state;
	}


	void Climb(Ladder * lad);

	void ClimbDown(Ladder * lad);

	virtual void Land() override;

	void Crouch();
	void Stand();
	void Glide();

	bool isKnocked() {
		return knock_timer > 0;
	}
};