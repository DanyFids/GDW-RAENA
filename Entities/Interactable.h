#pragma once

#include "2d/CCScene.h"
#include "Entities/CoreEntities.h"
#include "Enums.h"

//USING_NS_CC;

class Interactable : public Entity {

public :

	//static Interactable* create(int x,int y, int w, int h,InteractType type, KeyType key = NONE);
	//static Interactable* create(std::string filename, cocos2d::Vec2 p, InteractType type, KeyType key = NONE);

	const float SwitchCD = 0.6;
	InteractType objectType; // What is this object? ex. A door? A switch?
	//Inheirited

	virtual bool HitDetect(Entity * other) = 0;
	virtual bool inRange(Entity * other) = 0;

	virtual void Update(float dt) = 0;
	virtual void Move() override;

	virtual void Effect(Entity * player, player_inventory * p_inv);	 //For Doors

	virtual void setCooldown() {
		if (CoolDownState == false) { CoolDownState = true; }

	}

	virtual bool getCooldown() = 0;

	virtual InteractType getType() = 0;

	//int getKeys(player_inventory*,KeyType k);
	//void editKeys(player_inventory*, KeyType k, int i);

	//void SceneReturnCallBack(Ref* pSender);

	//CREATE_FUNC(cocos2d::Scene);
	
	virtual void Land() override;

protected:

	//Scene * currScene;

	bool Active = false;
	bool CoolDownState = false;

	float CD;
	float temp_CD;

	KeyType requiredKey; // If this is a locked door what key does it use?

	//Door Variables;
	// Active == Open/Close , True == Open 
	bool locked = false;


	// Inherited via Entity

};