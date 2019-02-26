#pragma once

#include "2d/CCScene.h"
#include "Entities/CoreEntities.h"
#include "Enums.h"

USING_NS_CC;

class Interactable : public Entity {

public :


	static Interactable* create(int x,int y, int w, int h,InteractType type, KeyType key = NONE);
	static Interactable* create(std::string filename, cocos2d::Vec2 p, InteractType type, KeyType key = NONE);

	const float SwitchCD = 0.6;

	//Inheirited
	virtual bool HitDetect(Entity * other) override;
	bool inRange(Entity * other);

	virtual void Update(float dt) override;
	virtual void Move() override;

	void Effect(InteractType t,Entity * p,player_inventory * p_inv = nullptr, Scene * scn = nullptr);

	void setCooldown() {
		if (CoolDownState == false) { CoolDownState = true; } 
	}
	bool getCooldown() { return CoolDownState; }

	InteractType getType();

	int getKeys(player_inventory*,KeyType k);
	void editKeys(player_inventory*, KeyType k, int i);

	void SceneReturnCallBack(Ref* pSender);

	CREATE_FUNC(cocos2d::Scene);
	virtual void Land() override;

private:

	Scene * currScene;

	bool Active = false;
	bool CoolDownState = false;

	float CD;
	float temp_CD;

	InteractType objectType; // What is this object? ex. A door? A switch? 
	KeyType requiredKey; // If this is a locked door what key does it use?

	//Door Variables;
	// Active == Open/Close , True == Open 
	bool locked = false;


	// Inherited via Entity

};