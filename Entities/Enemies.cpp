#include "Enemies.h"
#include "Effects/LightEffect.h"
#include "Constants.h"

Knight * Knight::create(const std::string& filename)
{
	auto ret = new (std::nothrow) Knight;
	if (ret && ret->initWithFile(filename)) {
		ret->autorelease();
		return ret;
	}
	CC_SAFE_RELEASE(ret);
	return nullptr;
}

void Knight::switchLight()
{
	if (!light_on) {
		knight_light = this->getEffect()->addLight(cocos2d::Vec3(this->getPosition().x, this->getPosition().y, 25));
		light_on = true;
	}
	else {
		this->getEffect()->rmvLight(knight_light);
		light_on = false;
	}
}

void Knight::moveLightToPlayer()
{
	if (light_on) {
		getEffect()->moveLightTo(getPosition(), knight_light);
	}
}

void Knight::AI(Player* player, float dt) {

	/*******************
	* Player Detection
	*******************/
	if (player->getPosition().x <= this->getPosition().x && this->getPosition().x <= (player->getPosition().x + 200)) {
		if (player->getPosition().y <= this->getPosition().y + 200 || player->getPosition().y <= this->getPosition().y - 200) {
			this->spd.x -= 50 * dt;
			attacking = true;
		}
	}
	else if (player->getPosition().x >= this->getPosition().x && this->getPosition().x >= (player->getPosition().x - 200)) {
		if (player->getPosition().y <= this->getPosition().y + 200 || player->getPosition().y <= this->getPosition().y - 200) {
			this->spd.x += 50 * dt;
			attacking = true;
		}
	}
	if (attacking && spd.x <= 0) {
		attacking = false;
	}
	/******************/

	/****************
	* Idle Movement
	****************/
	if (timer >= 0) {
		timer -= dt;
	}
	if (timer <= 0)
	{
		std::random_device gen;
		std::uniform_int_distribution<> range(1, 40);
		turn = range(gen);

		if (turn == 1) {
			if (face_right) {
				face_right = false;
				timer = TURNT_TIME;
			}
			else {
				face_right = true;
				timer = TURNT_TIME;
			}
		}
	}

	if (!attacking) {
			if (face_right) {
				this->spd.x += 10 * dt;
			}
			else {
				this->spd.x -= 10 * dt;
			}
	}
	/***************/

	/*******************
	* Attacking Player 
	*******************/
	if (this->HitDetect(player)) {
		player->hurt(2);
	}
	/******************/

}

void Knight::hurt(int dmg) {
	hp -= dmg;
}

int Knight::getHp() {
	return hp;
}

bool Knight::HitDetect(Entity * other)
{
	return false;
}

void Knight::Update(float dt)
{
	//spd.y += GRAVITY * dt;
	//if (spd.y < T_VELOCITY) {
	//	spd.y = T_VELOCITY;
	//}

	spd.y = 0;
	spd.x = 0;
}

void Knight::Move()
{
	setPosition(getPosition() + spd);
}