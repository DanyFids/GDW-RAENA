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

void Knight::moveLightToKnight()
{
	if (light_on) {
		getEffect()->moveLightTo(getPosition(), knight_light);
	}
}

bool Knight::onGround() {
	return on_ground;
}

void Knight::AI(Player* player, float dt) {

	/****************
	* All Movement
	****************/
	if (!swipe) {
		
		/*******************
		* Player Detection
		*******************/

		//Left
		if (!face_right) {
			if (player->getPosition().x <= this->getPosition().x && this->getPosition().x <= (player->getPosition().x + 300)) {
				if (player->getPosition().y <= this->getPosition().y + 100 || player->getPosition().y <= this->getPosition().y - 100) {
					this->spd.x -= charge * dt;
					attacking = true;
				}
			}

			//Turn Go Right
			else if (player->getPosition().x >= this->getPosition().x && this->getPosition().x >= (player->getPosition().x - 50)) {
				if (player->getPosition().y <= this->getPosition().y + 60 || player->getPosition().y <= this->getPosition().y - 60) {
					face_right = true;
					this->spd.x += charge * dt;
					attacking = true;
				}
			}

			else {
				charge = 20;
				attacking = false;
			}

			//attack left
			if (player->getPosition().x <= this->getPosition().x && this->getPosition().x <= (player->getPosition().x + 60)) {
				if (player->getPosition().y <= this->getPosition().y + 80 || player->getPosition().y <= this->getPosition().y - 80) {
					swipe = true;
				}
			}
		}

		//Right
		else if (face_right)
		{
			if (player->getPosition().x >= this->getPosition().x && this->getPosition().x >= (player->getPosition().x - 300)) {
				if (player->getPosition().y <= this->getPosition().y + 100 || player->getPosition().y <= this->getPosition().y - 100) {
					this->spd.x += charge * dt;
					attacking = true;
				}
			}

			//Turn Go Left
			else if (player->getPosition().x <= this->getPosition().x && this->getPosition().x <= (player->getPosition().x + 50)) {
				if (player->getPosition().y <= this->getPosition().y + 60 || player->getPosition().y <= this->getPosition().y - 60) {
					face_right = false;
					this->spd.x -= charge * dt;
					attacking = true;
				}
			}

			else {
				charge = 20;
				attacking = false;
			}

			//attack right
			if (player->getPosition().x >= this->getPosition().x && this->getPosition().x >= (player->getPosition().x - 60)) {
				if (player->getPosition().y <= this->getPosition().y + 80 || player->getPosition().y <= this->getPosition().y - 80) {
					swipe = true;
				}
			}
		}


		if (attacking && charge <= 100) {
			charge++;
		}

		/******************/

		/****************
		* Idle Movement
		****************/

		if (timer >= 0) {
			timer -= dt;
		}
		if (timer <= 0 && (!attacking))
		{
			std::random_device gen;
			std::uniform_int_distribution<> range(1, 40);
			turn = range(gen);

			switch (turn) {
			case 1:
				if (face_right) {
					face_right = false;
					timer = TURNT_TIME;
				}
				else {
					face_right = true;
					timer = TURNT_TIME;
				}
				break;
			case 25:
				timer = TURNT_TIME;
				break;
			}
		}

		if (!attacking) {
			if (face_right) {
				this->spd.x += 10 * dt;
			}
			else {
				this->spd.x -= 10 * dt;
			}
			if (turn == 25) {
				if (face_right) {
					this->spd.x -= 10 * dt;
				}
				else {
					this->spd.x += 10 * dt;
				}
			}
		}
		/***************/
	}
	else {
		swing -= dt;
		if (swing <= 0) {
			swing = CHOP_TIME;
			swipe = false;
		}

		if (hitTimer <= 0) {
			Hit(player);
			hitTimer = HIT_TIME;
		}
		else {
			hitTimer -= dt;
		}
		
	}
	/***************/

}


int Knight::getHp() {
	return hp;
}

void Knight::Hurt(int dmg) {
	hp--;
}

void Knight::Hit(Player * other) {

	if (other->getPosition().x >= this->getPosition().x && this->getPosition().x >= (other->getPosition().x - 75)) {
		if (other->getPosition().y <= this->getPosition().y + 40 || other->getPosition().y <= this->getPosition().y - 40) {
			other->hurt(4);
			other->spd.x = 5;
			other->spd.y = 2;
		}
	}
	else if (other->getPosition().x <= this->getPosition().x && this->getPosition().x <= (other->getPosition().x + 75)) {
		if (other->getPosition().y <= this->getPosition().y + 40 || other->getPosition().y <= this->getPosition().y - 40) {
			other->hurt(4);
			other->spd.x = -5;
			other->spd.y = 2;
		}
	}
}

bool Knight::HitDetect(Entity * other)
{
	float o_head = other->getPositionY() + (other->getBoundingBox().size.height / 2);
	float o_foot = other->getPositionY() - (other->getBoundingBox().size.height / 2);
	float o_left = other->getPositionX() - (other->getBoundingBox().size.width / 2);
	float o_right = other->getPositionX() + (other->getBoundingBox().size.width / 2);

	float t_head = this->getPositionY() + (this->getBoundingBox().size.height / 2);
	float t_foot = this->getPositionY() - (this->getBoundingBox().size.height / 2);
	float t_left = this->getPositionX() - (this->getBoundingBox().size.width / 2);
	float t_right = this->getPositionX() + (this->getBoundingBox().size.width / 2);

	if (o_head + other->spd.y > t_foot && o_foot + other->spd.y < t_head &&
		o_left < t_right && o_right > t_left) {
		if (other->spd.y > 0) {
			//other->spd.y = t_foot - o_head;
			other->spd.y = -1;
			return true;
		}
		else {
			//other->spd.y = t_head - o_foot;
			other->spd.y = 5;
			return true;
		}
	}

	if (o_head > t_foot && o_foot < t_head &&
		o_left + other->spd.x < t_right + spd.x && o_right + other->spd.x > t_left + spd.x) {
		if (other->spd.x > 0) { 
			//other->spd.x = (t_left + spd.x) - o_right;
			other->spd.y = 3;
			other->spd.x = -3;
			return true;
		}
		else {
			//other->spd.x = (t_right + spd.x) - o_left;
			other->spd.y = 3;
			other->spd.x = 3;
			return true;
		}
	}


	return false;
}

void Knight::Update(float dt)
{
	spd.y += GRAVITY * dt;
	if (spd.y < T_VELOCITY) {
		spd.y = T_VELOCITY;
	}

	//spd.y = 0;
	spd.x = 0;
}

void Knight::Move()
{
	setPosition(getPosition() + spd);

	if (spd.y != 0 && on_ground) {
		on_ground = false;
	}

}