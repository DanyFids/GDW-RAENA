#pragma once
#include "Effects/EffectSprite.h"
#include "cocos2d.h"
#include "Enums.h"

struct inventoryItem
{
	int itemId = 0;
	std::string itemName = "NA";
	std::string itemPic = "NA";
	std::string itemDescription = "NA";
	itemEnum Val = I_NONE;
};

struct player_inventory {
public:
	player_inventory(int gen_keys) : general_keys(gen_keys) {

	}

	std::vector<inventoryItem> items;

	int general_keys;
};

class Entity : public EffectSprite {
protected:
	bool on_ground;

	cocos2d::Vector<cocos2d::Animation * > animations;
public:
	cocos2d::Vec2 spd;

	virtual void Move() = 0;
	virtual bool HitDetect(Entity * other) = 0;
	virtual void Update(float dt) = 0;
	//bool getonground();

	void SetOnGround(bool b) {on_ground = b;}
	void ChangeAnimation(int anim_id) {
		if (anim_id >= 0 && anim_id < animations.size()) {
			stopAllActions();
			runAction(cocos2d::RepeatForever::create(cocos2d::Animate::create(animations.at(anim_id))));
		}
		else {
			throw "Woah there friend that'sa no good!";
		}
	}

	bool isOnGround() { return on_ground; }
	virtual void Land() = 0;
};



class Player;

class Enemy: public Entity {
protected:
	int hp;
public:
	virtual void AI(Player * p, float dt) = 0;
	virtual void Hit(Player * p) = 0;

	virtual void Hurt(int d) = 0;

	virtual void Land() override{
		on_ground = true;
	}
};

