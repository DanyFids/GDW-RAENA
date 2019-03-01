#include "Enemies.h"
#include "Effects/LightEffect.h"
#include "Constants.h"
auto ret = new (std::nothrow) Knight;
Knight * Knight::create(const std::string& filename)
{
	if (ret && ret->initWithFile(filename)) {
			cocos2d::Vector<cocos2d::SpriteFrame *> walk_frames = { cocos2d::SpriteFrame::create("knightwalkyboi0000.png", cocos2d::Rect(0,0,64,100), false, {0,0}, {64,100}),
																	cocos2d::SpriteFrame::create("knightwalkyboi0001.png", cocos2d::Rect(0,0,64,100), false, {0,0}, {64,100}),
																	cocos2d::SpriteFrame::create("knightwalkyboi0002.png", cocos2d::Rect(0,0,56,100), false, {0,0}, {64,100}),
																	cocos2d::SpriteFrame::create("knightwalkyboi0003.png", cocos2d::Rect(0,0,64,100), false, {0,0}, {64,100}) };
			ret->animations.pushBack(cocos2d::Animation::createWithSpriteFrames(walk_frames, 0.3f));

		ret->runAction(cocos2d::RepeatForever::create(cocos2d::Animate::create(ret->animations.at(0))));
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
			ret->setFlipX(true);
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
			ret->setFlipX(false);
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
			swipe = false;
			swing = CHOP_TIME;
		}
	}
	/***************/

}

void Knight::greatSword() {

}

int Knight::getHp() {
	return hp;
}

void Knight::Hurt(int dmg) {

}

void Knight::Hit(Player * other) {

}

bool Knight::HitDetect(Entity * other)
{
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