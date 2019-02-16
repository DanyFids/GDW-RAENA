#pragma once
#include "Entities/CoreEntities.h"

enum InteractType {
	SWITCH,
	DOOR,
	S_DOOR
};



class Interactable : public Entity {

public :

	static Interactable* create(int x,int y, int w, int h,InteractType type);
	static Interactable* create(std::string filename, cocos2d::Vec2 p, InteractType type);

	const float SwitchCD = 0.6;

	//Inheirited
	virtual bool HitDetect(Entity * other) override;
	virtual void Update(float dt) override;
	virtual void Move() override;

	void Effect(InteractType t);

	void setCooldown() {
		if (CoolDownState == false) { CoolDownState = true; } 
	}

	bool getCooldown() { return CoolDownState; }

	InteractType getType();

private:

	bool Active = false;
	bool CoolDownState = false;

	float CD;
	float temp_CD;

	InteractType objectType;

	//Door Variables;
	// Active == Open/Close , True == Open 
	bool locked = false;

	

};