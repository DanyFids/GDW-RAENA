#include "Player.h"
#include "Effects/LightEffect.h"
#include "Constants.h"

Player * Player::create(const std::string& filename)
{
	auto ret = new (std::nothrow) Player;
	if (ret && ret->initWithFile(filename)) {
		ret->autorelease();
		return ret;
	}
	CC_SAFE_RELEASE(ret);
	return nullptr;
}

void Player::switchLight()
{
	if (!light_on) {
		player_light = this->getEffect()->addLight(cocos2d::Vec3(this->getPosition().x, this->getPosition().y, 25));
		light_on = true;
	}
	else {
		this->getEffect()->rmvLight(player_light);
		light_on = false;
	}
}

void Player::moveLightToPlayer()
{
	if (light_on) {
		getEffect()->moveLightTo(getPosition(), player_light);
	}
}

void Player::hurt(int dmg) {
	hp -= dmg;
}

bool Player::HitDetect(Entity * other)
{
	return false;
}

void Player::Update(float dt)
{
	spd.y += GRAVITY * dt;
	if (spd.y < T_VELOCITY) {
		spd.y = T_VELOCITY;
	}

	spd.x = 0;
}

void Player::Move()
{
	setPosition(getPosition() + spd);
}
