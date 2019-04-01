#include "Projectile.h"

float Projectile::degToRad = 3.14159f / 180.0f;
float Projectile::radToDeg = 180.0f / 3.14159f;
float Projectile::gravity = 98.0f;

void Projectile::shootProjectile()
{
	//We are now shooting
	isShooting = true;

	//Reset the time since the last projectile was shot
	timeSinceLastShot = 0.0f;

	//Set the velocity initially when shot
	spd = cocos2d::Vec2(initialVelocity * cos(angle * degToRad), initialVelocity * sin(angle * degToRad));
}

void Projectile::updateShotProjectile(float deltaTime)
{
	if (!isShooting)
		return;

	// Calculate the new velocity along the y-axis
	spd.y = (initialVelocity * sin(angle * degToRad) - accelerationY * timeSinceLastShot);

	// Move the projectile
	projectile->setPosition(projectile->getPosition() + (spd * deltaTime));

	// Resets the projectile when you hit the floor
	if (projectile->getPosition().y <= (groundY + (diameterProjectile / 2)))
	{
		isShooting = false;
		spd = cocos2d::Vec2(0.0f, 0.0f);
		timeSinceLastShot = 0.0f;
	}
}
