#pragma once
#include "Entities/Interactable.h"
#include "InventoryScene.h"

class Pickup : public Interactable
{
private:
	itemEnum ItemType;
public:

	static Pickup* create(std::string filename, cocos2d::Vec2 p, itemEnum = I_NONE);

	virtual bool HitDetect(Entity * other) {
		return false;
	};

	virtual bool inRange(Entity * other);

	virtual void Update(float dt) {};
	virtual void Move() override {};

	virtual void Effect(Entity * player, player_inventory * p_inv);

	virtual void setCooldown() {
		if (CoolDownState == false) { CoolDownState = true; }

	}

	virtual bool getCooldown() { return CoolDownState; }

	virtual InteractType getType() { return objectType; }
	virtual itemEnum getItemType() { return ItemType; }

	virtual bool getActive() {
		return false;
	}
};
