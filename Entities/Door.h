#pragma once
#include "Classes/Entities/Interactable.h"

class Door : public	Interactable {

public:

	virtual bool HitDetect(Entity * other);
	virtual bool inRange(Entity * other);

	virtual void Update(float dt);
	virtual void Move() {};

	virtual void Effect(InteractType t, Entity * player, player_inventory * p_inv);

	virtual void setCooldown() {
		if (CoolDownState == false) { CoolDownState = true; }

	}

	virtual int getKeys(player_inventory *, KeyType k);
	virtual void editKeys(player_inventory*, KeyType k, int i);

	virtual bool getCooldown() { return CoolDownState; }

	virtual InteractType getType() { return objectType; };

private:

};
