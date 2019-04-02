#include "Projectile.h"

float Projectile::degToRad = 3.14159f / 180.0f;
float Projectile::radToDeg = 180.0f / 3.14159f;
float Projectile::gravity = 98.0f;

Projectile * Projectile::create(float vel, float ang, int gro, int x, int y, cocos2d::Scene * sce)
{
	auto ret = new Projectile(vel, ang, gro);
	ret->projectile->setPosition(cocos2d::Vec2(x, y));
	ret->start = cocos2d::Vec2(x, y);
	ret->autorelease();
	sce->addChild(ret->projectile, 100);
	return ret;
}

Projectile::Projectile(float vel, float ang, int gro)
{
	initialVelocity = vel;
	angle = ang;
	projectile = cocos2d::DrawNode::create();
	projectile->drawDot(cocos2d::Vec2(0.0f, 0.0f), (diameterProjectile / 2), cocos2d::Color4F::RED);
	diameterProjectile = 10.0f;
	timeSinceLastShot = 0.0f;
	groundY = gro;
	Projx = (diameterProjectile / 2);
	Projy = groundY + (diameterProjectile / 2);
}

void Projectile::shootProjectile()
{
	//We are now shooting
	this->projectile->setPosition(this->start);
	this->isShooting = true;

	//Reset the time since the last projectile was shot
	this->timeSinceLastShot = 0.0f;
	

	//Set the velocity initially when shot
	this->spd = cocos2d::Vec2(this->initialVelocity * cos(this->angle * degToRad), this->initialVelocity * sin(this->angle * degToRad));
}

void Projectile::updateShotProjectile(float deltaTime)
{
	// Calculate the new velocity along the y-axis
	this->spd.y = (this->initialVelocity * sin(this->angle * degToRad) - this->accelerationY * this->timeSinceLastShot);

	// Move the projectile
	this->projectile->setPosition(this->projectile->getPosition() + (this->spd * deltaTime));

	// Resets the projectile when you hit the floor
	if (this->projectile->getPosition().y <= (this->groundY + (this->diameterProjectile / 2)))
	{
		this->isShooting = false;
		this->spd = cocos2d::Vec2(0.0f, 0.0f);
		this->timeSinceLastShot = 0.0f;
	}
	if (!this->isShooting)
	{
		this->shootProjectile();
	}

	if (this->isShooting)
	{
		this->timeSinceLastShot += deltaTime;
	}
}

bool Projectile::HitDetect(Entity * other)
{
	float o_TOP = other->getPositionY() + (other->getBoundingBox().size.height / 2); //This is for Objs with A Sprite (beccause it has a centered Origin)
	float o_BOT = other->getPositionY() - (other->getBoundingBox().size.height / 2);

	float o_LEFT = other->getPositionX() - (other->getBoundingBox().size.width / 2);
	float o_RIGHT = other->getPositionX() + (other->getBoundingBox().size.width / 2);

	float MAX_Y = this->projectile->getPositionY() + (this->projectile->getBoundingBox().size.height);                // For simple shapes
	float MIN_Y = this->projectile->getPositionY();

	float MAX_X = this->projectile->getPositionX() + (this->projectile->getBoundingBox().size.width);
	float MIN_X = this->projectile->getPositionX();

	if (o_TOP > MIN_Y && o_BOT < MAX_Y &&
		o_RIGHT + other->spd.x > MIN_X && o_LEFT + other->spd.x < MAX_X) {

		if (other->spd.x >= 0.0f) {
			other->spd.x = MIN_X - o_RIGHT;
			this->shootProjectile();
			return true;
		}
		else
		{
			other->spd.x = MAX_X - o_LEFT;
			this->shootProjectile();
			return true;
		}
	}

	return false;
}

void Projectile::Move()
{
}

void Projectile::Update(float dt)
{
}

void Projectile::Land()
{
}
