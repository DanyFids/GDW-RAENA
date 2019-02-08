#pragma once
#include "Entities/CoreEntities.h"

class Block : public Entity {
public:
	static Block* create(int x,int y,int w,int h);

	// Inherited via Entity
	virtual bool HitDetect(Entity * other) override;
	virtual void Update(float dt) override;
	virtual void Move() override;

	CREATE_FUNC(Block);
};