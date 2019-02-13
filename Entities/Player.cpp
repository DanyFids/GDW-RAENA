#include "Player.h"
#include "Effects/LightEffect.h"
#include "Constants.h"
#include "Fireball.h"

Player * Player::create(const std::string& filename, cocos2d::Scene * s)
{
	auto ret = new (std::nothrow) Player;
	if (ret && ret->initWithFile(filename)) {
		ret->autorelease();
		ret->SetScene(s);
		//ret->switchLight();
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

	if (attacking) {
		if (atk_timer > 0) {
			atk_timer -= dt;
		}
		else {
			attacking = false;
			atk->Die();
			scn->removeChild(atk, true);
			atk = nullptr;

			switchLight();
		}
	}
}

void Player::Move()
{
	if (!attacking) {
		setPosition(getPosition() + spd);

		if (spd.y != 0 && on_ground) {
			on_ground = false;
		}

		if (spd.x > 0) {
			face_right = true;
		}
		else if (spd.x < 0) {
			face_right = false;
		}
	}
}

void Player::Jump()
{
	if (on_ground && !attacking) {
		spd.y = JUMP_V;
		on_ground = false;
	}
}

void Player::Attack()
{
	if (!attacking && on_ground) {
		switchLight();

		attacking = true;

		atk_timer = ATK_TIME;

		cocos2d::Vec2 atk_pos;
		if (face_right) {
			atk_pos = cocos2d::Vec2(getPositionX() + (getBoundingBox().size.width / 2 + 30), getPositionY() + 5);
		}
		else {
			atk_pos = cocos2d::Vec2(getPositionX() - (getBoundingBox().size.width / 2 + 30), getPositionY() + 5);
		}

		atk = Fireball::create(atk_pos, getEffect());

		scn->addChild(atk, 10);


	}

}

void Player::HitDetectEnem(Enemy * e)
{
	if (attacking && atk != nullptr) {
		if (atk->HitDetect(e)) {
			e->Hurt(DMG);
		}
	}
}
