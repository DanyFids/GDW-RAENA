#pragma once
#include"Entities/CoreEntities.h"
#include "cocos2d.h"

class Projectile : public Entity
{
public:
	static Projectile* create(float vel, float ang, int gro, int x, int y, cocos2d::Scene * sce);

	float initialVelocity;
	float angle;
	float Projx;
	float Projy;
	static float degToRad;
	static float radToDeg;
	static float gravity;
	float accelerationY = 98.0f;
	cocos2d::DrawNode* projectile;
	bool isShooting = false;
	float diameterProjectile = 10.0f;
	float timeSinceLastShot;
	//the y coord that stops the projectile
	float groundY;
	float time;
	cocos2d::Vec2 start;

	Projectile(float vel, float ang, int gro);
	void shootProjectile();
	void updateShotProjectile(float deltaTime);
	//cocos2d::Vec2 velocity;
	virtual void Move() override;
	virtual bool HitDetect(Entity * other) override;
	virtual void Update(float dt) override;
	virtual void Land() override;
};