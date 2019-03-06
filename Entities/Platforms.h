#pragma once
#include "Entities/CoreEntities.h"
#include <string>

class Platform : public Entity {
public:
	static Platform* create( std::string filename, float x, float y);
	static Platform* create(std::string filename, cocos2d::Vec2 p);

	static Platform* create(std::string filename, float x,float y, float xSpeed, float ySpeed);
	static Platform* create(std::string filename, cocos2d::Vec2 p, float xSpeed, float ySpeed);



	//Inheirited
	virtual bool HitDetect(Entity * other) override;
	virtual void Update(float dt) override;
	virtual void Move() override;
	virtual void Land() override;
private:
	cocos2d::Vec2 origin;

};