#pragma once
#include "Classes/Entities/Interactable.h"

class Door : public	Interactable {

public:

	static Door* create(int x, int y, int w, int h, InteractType type, KeyType key = NONE); // Primitive,
	static Door* create(std::string filename, cocos2d::Vec2 p, InteractType type, KeyType key = NONE); //Sprite

	virtual bool HitDetect(Entity * other) override;
	virtual bool inRange(Entity * other) override;

	virtual void Update(float dt) override;
	virtual void Move() override {} ;

	virtual void Effect(InteractType t, Entity * player, player_inventory * p_inv) override;

	virtual void setCooldown() override {
		if (CoolDownState == false) { CoolDownState = true; }

	}

	virtual int getKeys(player_inventory *, KeyType k);
	virtual void editKeys(player_inventory*, KeyType k, int i);

	virtual bool getCooldown() { return CoolDownState; }

	virtual InteractType getType() { return objectType; };

private:
	KeyType requiredKey; // If this is a locked door what key does it use?

	//Door Variables;
	// Active == Open/Close , True == Open 
	bool locked = false;
};
