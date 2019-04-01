#pragma once
#include "Classes/Entities/Interactable.h"

class Door : public	Interactable {

public:

	static Door* create(int x, int y, int w, int h, KeyType key = NONE); // Primitive,
	static Door* create(std::string filename, cocos2d::Vec2 p, KeyType key = NONE); //Sprite

	virtual bool HitDetect(Entity * other) override;
	virtual bool inRange(Entity * other) override;

	virtual void Update(float dt) override;
	virtual void Move() override {} ;

	virtual void Effect(Entity * player, player_inventory * p_inv);

	virtual void setCooldown() override {
		if (CoolDownState == false) { CoolDownState = true; }

	}

	virtual int getKeys(player_inventory *, KeyType k);
	virtual void editKeys(player_inventory*, KeyType k, int i);

	virtual bool getCooldown() { return CoolDownState; }

	virtual InteractType getType() { return objectType; };

protected:

	;
};


//////////////////////////////////////////////////

class SceneDoor : public Door {
public:
	static SceneDoor* create(std::string filename, cocos2d::Vec2 p, cocos2d::Vec2 pPos, levelEnum lvl, KeyType key = NONE);

	virtual void setCooldown() override {
		if (CoolDownState == false) { CoolDownState = true; }

	}

	virtual bool HitDetect(Entity * other) override { return false; };

	virtual void Effect(Entity * player, player_inventory * p_inv);

	virtual bool getCooldown() { return CoolDownState; }

	virtual InteractType getType() { return objectType; };
	levelEnum goTo;
	cocos2d::Vec2 movePlayer;

	//cocos2d::Scene* getScenePointer() {return goTo;}
protected:
	
	//cocos2d::Scene* goTo;
};

class LoadZone : public SceneDoor {
public:
	static LoadZone* create(int x, int y, int w, int h, levelEnum dest,cocos2d::Vec2 pPos); // Primitive,

	   virtual void setCooldown() override{}

	   virtual bool getCooldown() { return false; }

	   virtual bool HitDetect(Entity * other) override;

	   virtual void Effect(Entity * player);

	   virtual InteractType getType() { return objectType; }

};