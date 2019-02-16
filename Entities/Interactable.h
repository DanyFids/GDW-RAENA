#pragma once
#include "Entities/CoreEntities.h"

enum InteractType {
	SWITCH
};

class Interactable : public Entity {

public :

	static Interactable* create(int x,int y, int w, int h,InteractType type);
	static Interactable* create(std::string filename, cocos2d::Vec2 p, InteractType type);



	//Inheirited
	virtual bool HitDetect(Entity * other) override;
	virtual void Update(float dt) override;
	virtual void Move() override;

	void Effect(InteractType t);
	InteractType getType();

private:
	bool Active = false;
	InteractType objectType;
	

};