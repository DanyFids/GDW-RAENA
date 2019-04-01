#pragma once
#include "Enums.h"
#include "Entities/Interactable.h"
#include "cocos2d.h"
#include "InventoryScene.h";

class GameplayScene;

class PuzzleInteract : public Interactable {
public:
	static PuzzleInteract* create(std::string filename, cocos2d::Vec2 p,PuzzleType puz, itemEnum item = I_NONE); //Sprite

	virtual bool HitDetect(Entity * other) override { return false; }

	virtual bool inRange(Entity* other) override;

	virtual void Effect(Entity * player, player_inventory * p_inv, GameplayScene* scn);

	virtual void Update(float dt) override;

	virtual void setCooldown() {
		if (CoolDownState == false)
		{
			CoolDownState = true;
		}
	}

	virtual bool getCooldown() {
		return CoolDownState;
	}

	virtual InteractType getType() {
		return objectType;
	}

	virtual bool getActive() {
		return Active;
	}

	bool checkItem(itemEnum i);
	bool checkPuzzle(PuzzleType p);
	PuzzleType puzzle;
private:
	itemEnum requiredItem;
	
};



