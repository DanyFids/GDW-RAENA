#include "Enemies.h"
#include "Effects/LightEffect.h"
#include "Constants.h"


auto mothret = new (std::nothrow) Moth;
auto ratret = new (std::nothrow) Rat;

/*****************************/
/*       Knight    Code      */
/*****************************/

Knight * Knight::create(const std::string& filename)
{
	auto ret = new (std::nothrow) Knight;
	float h = 101;
	float w = 57;

	if (ret && ret->initWithFile(filename)) {
		cocos2d::Vector<cocos2d::SpriteFrame *> walk_frames = { cocos2d::SpriteFrame::create("knightwalkyboi0000.png", cocos2d::Rect(0, 0, w, h), false, { 0,0 }, { w,h }),
															cocos2d::SpriteFrame::create("knightwalkyboi0001.png", cocos2d::Rect(0, 0, w, h), false, { 0,0 }, { w,h }),
															cocos2d::SpriteFrame::create("knightwalkyboi0002.png", cocos2d::Rect(0, 0, w, h), false, { 0,0 }, { w,h }),
															cocos2d::SpriteFrame::create("knightwalkyboi0003.png", cocos2d::Rect(0, 0, w, h), false, { 0,0 }, { w,h }) };

		//if (ret && ret->initWithFile(filename)) {
		//	cocos2d::Vector<cocos2d::SpriteFrame *> walk_frames = { cocos2d::SpriteFrame::create("Knight_Idle_Resized.png", cocos2d::Rect(0, 0, w, h), false, { 0,0 }, { w,h }),
		//														cocos2d::SpriteFrame::create("Knight_Idle_Resized.png", cocos2d::Rect(0, 0, w, h), false, { 0,0 }, { w,h }),
		//														cocos2d::SpriteFrame::create("Knight_Idle_Resized.png", cocos2d::Rect(0, 0, w, h), false, { 0,0 }, { w,h }),
		//														cocos2d::SpriteFrame::create("Knight_Idle_Resized.png", cocos2d::Rect(0, 0, w, h), false, { 0,0 }, { w,h }) };

			ret->animations.pushBack(cocos2d::Animation::createWithSpriteFrames(walk_frames, 0.3f));

		ret->runAction(cocos2d::RepeatForever::create(cocos2d::Animate::create(ret->animations.at(0))));
		ret->autorelease();

		auto prim = cocos2d::DrawNode::create();
		//prim->drawRect({ -(w/2),-(h/2) }, { w/2, h/2 }, cocos2d::Color4F::GREEN);
		prim->drawRect({ 0,0 }, { w, h }, cocos2d::Color4F::GREEN);

		ret->addChild(prim);
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
		if (!face_right && !delay) {
			this->setFlipX(true);
			//attack left
			if (player->getPosition().x <= this->getPosition().x && this->getPosition().x <= (player->getPosition().x + 60)) {
				if (player->getPosition().y <= this->getPosition().y + 80 || player->getPosition().y <= this->getPosition().y - 80) {
					swipe = true;
					attacking = true;
				}
			}

			else if (player->getPosition().x <= this->getPosition().x && this->getPosition().x <= (player->getPosition().x + 500)) {
			
				if (player->getPosition().y <= this->getPosition().y + 100 || player->getPosition().y <= this->getPosition().y - 100) {
					this->spd.x -= charge * dt;
					attacking = true;
				}
			}

			//Turn Go Right
			else if (player->getPosition().x >= this->getPosition().x && this->getPosition().x >= (player->getPosition().x - 100)) {
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
		}

		//Right
		else if (face_right && !delay)
		{
			this->setFlipX(false);
			//attack right
			if (player->getPosition().x >= this->getPosition().x && this->getPosition().x >= (player->getPosition().x - 60)) {
				if (player->getPosition().y <= this->getPosition().y + 80 || player->getPosition().y <= this->getPosition().y - 80) {
					swipe = true;
					attacking = true;
				}
			}

			else if (player->getPosition().x >= this->getPosition().x && this->getPosition().x >= (player->getPosition().x - 500)) {
			
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
		}
		else {
			swing -= dt;
			if (swing <= 0) {
				swing = CHOP_TIME;
				delay = false;
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
			delay = true;
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
	hp -= dmg;
}

void Knight::Hit(Player * other) {

	if (other->getPosition().x >= this->getPosition().x && this->getPosition().x >= (other->getPosition().x - 125)) {
		if (other->getPosition().y <= this->getPosition().y + 40 || other->getPosition().y <= this->getPosition().y - 40) {
			if (other->getState() != PS_HURT && other->getInvince() <= 0) {
				other->hurt(4);
				other->spd.x = 5;
				other->spd.y = 2;
			}
		}
	}
	else if (other->getPosition().x <= this->getPosition().x && this->getPosition().x <= (other->getPosition().x + 125)) {
		if (other->getPosition().y <= this->getPosition().y + 40 || other->getPosition().y <= this->getPosition().y - 40) {
			if (other->getState() != PS_HURT && other->getInvince() <= 0) {
				other->hurt(4);
				other->spd.x = -5;
				other->spd.y = 2;
			}
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
			if (((Player *)other)->getState() != PS_HURT && ((Player *)other)->getInvince() <= 0) {
				other->spd.y = -1;
			}
			return true;
		}
		else {
			if (((Player *)other)->getState() != PS_HURT && ((Player *)other)->getInvince() <= 0) {
				other->spd.y = 5;
			} 
			return true;
		}
	}

	if (o_head > t_foot && o_foot < t_head &&
		o_left + other->spd.x < t_right + spd.x && o_right + other->spd.x > t_left + spd.x) {
		if (other->spd.x > 0) {
			if (((Player *)other)->getState() != PS_HURT && ((Player *)other)->getInvince() <= 0) {
				other->spd.y = 3;
				other->spd.x = -3;
			}
			return true;
		}
		else {
			if (((Player *)other)->getState() != PS_HURT && ((Player *)other)->getInvince() <= 0) {
				other->spd.y = 3;
				other->spd.x = 3;
			}
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

/*****************************/
/*        Moth    Code       */
/*****************************/

Moth * Moth::create(const std::string & filename)
{
	if (mothret && mothret->initWithFile(filename)) {
		cocos2d::Vector<cocos2d::SpriteFrame *> fly_frames = { cocos2d::SpriteFrame::create("MothFlyingAnimation0000.png", cocos2d::Rect(0,0,29,41), false, {0,0}, {29,41}),
		 cocos2d::SpriteFrame::create("MothFlyingAnimation0002.png", cocos2d::Rect(0,0,29,41), false, {0,0}, {29,41}),
		 cocos2d::SpriteFrame::create("MothFlyingAnimation0004.png", cocos2d::Rect(0,0,29,41), false, {0,0}, {29,41}),
		 cocos2d::SpriteFrame::create("MothFlyingAnimation0006.png", cocos2d::Rect(0,0,29,41), false, {0,0}, {29,41}) };
		mothret->animations.pushBack(cocos2d::Animation::createWithSpriteFrames(fly_frames, 0.1f));

		mothret->runAction(cocos2d::RepeatForever::create(cocos2d::Animate::create(mothret->animations.at(0))));
		mothret->autorelease();
		return mothret;
	}
	CC_SAFE_RELEASE(mothret);
	return nullptr;
}

void Moth::setSpd(cocos2d::Vec2 m_spd) {
	spd = m_spd;
}

cocos2d::Vec2 Moth::getSpd() {
	return spd;
}

void Moth::setXDir(bool _x)
{
	face_right = _x;
}

void Moth::setYDir(bool _y)
{
	go_up = _y;
}

void Moth::AI(Player * player, float dt)
{

	//Left
	if (!face_right && !delay) {
		//attack left
		//if tere is an attack animation
		if (player->getPosition().x == this->getPosition().x) {

			if (player->getPosition().y > this->getPosition().y && player->getPosition().y < this->getPosition().y + 250) {
				attacking = true;
				this->spd.y += 50 * dt;
			}
			else if (player->getPosition().y < this->getPosition().y && player->getPosition().y > this->getPosition().y - 250) {
				attacking = true;
				this->spd.y -= 50 * dt;
			}
		}

		else if (player->getPosition().x <= this->getPosition().x && this->getPosition().x <= (player->getPosition().x + 300)) {

			if (player->getPosition().y == this->getPosition().y) {
				this->spd.x -= 60 * dt;
				attacking = true;
			}
			else if (player->getPosition().y > this->getPosition().y && player->getPosition().y < this->getPosition().y + 200){
				this->spd.x -= 60 * dt;
				attacking = true;
				this->spd.y += 50 * dt;
			}
			else if (player->getPosition().y < this->getPosition().y && player->getPosition().y > this->getPosition().y - 200) {
				this->spd.x -= 60 * dt;
				attacking = true;
				this->spd.y -= 50 * dt;
			}
		}

		//Turn Go Right
		else if (player->getPosition().x >= this->getPosition().x && this->getPosition().x >= (player->getPosition().x - 50)) {

			if (player->getPosition().y == this->getPosition().y) {
				face_right = true;
				this->spd.x += 60 * dt;
				attacking = true;
			}
			else if (player->getPosition().y > this->getPosition().y && player->getPosition().y < this->getPosition().y + 120) {
				face_right = true;
				this->spd.x += 60 * dt;
				attacking = true;
				this->spd.y += 50 * dt;
			}
			else if (player->getPosition().y < this->getPosition().y && player->getPosition().y > this->getPosition().y - 120) {
				face_right = true;
				this->spd.x += 60 * dt;
				attacking = true;
				this->spd.y -= 50 * dt;
			}
		}

		else {
			attacking = false;
		}
	}

	//Right
	else if (face_right && !delay)
	{
		//attack right
		//If there is attack code

		if (player->getPosition().x == this->getPosition().x) {

			if (player->getPosition().y > this->getPosition().y && player->getPosition().y < this->getPosition().y + 250) {
				attacking = true;
				this->spd.y += 50 * dt;
			}
			else if (player->getPosition().y < this->getPosition().y && player->getPosition().y > this->getPosition().y - 250) {
				attacking = true;
				this->spd.y -= 50 * dt;
			}
		}

		else if (player->getPosition().x >= this->getPosition().x && this->getPosition().x >= (player->getPosition().x - 300)) {

			if (player->getPosition().y == this->getPosition().y) {
				this->spd.x += 60 * dt;
				attacking = true;
			}
			else if (player->getPosition().y > this->getPosition().y && player->getPosition().y < this->getPosition().y + 200) {
				this->spd.x += 60 * dt;
				attacking = true;
				this->spd.y += 50 * dt;
			}
			else if (player->getPosition().y < this->getPosition().y && player->getPosition().y > this->getPosition().y - 200) {
				this->spd.x += 60 * dt;
				attacking = true;
				this->spd.y -= 50 * dt;
			}
		}

		//Turn Go Left
		else if (player->getPosition().x <= this->getPosition().x && this->getPosition().x <= (player->getPosition().x + 50)) {

			if (player->getPosition().y == this->getPosition().y) {
				face_right = false;
				this->spd.x -= 60 * dt;
				attacking = true;
			}
			else if (player->getPosition().y > this->getPosition().y && player->getPosition().y < this->getPosition().y + 120) {
				face_right = false;
				this->spd.x -= 60 * dt;
				attacking = true;
				this->spd.y += 50 * dt;
			}
			else if (player->getPosition().y < this->getPosition().y && player->getPosition().y > this->getPosition().y - 120) {
				face_right = false;
				this->spd.x -= 60 * dt;
				attacking = true;
				this->spd.y -= 50 * dt;
			}
		}

		else {
			attacking = false;
		}
	}


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
		case 3:
			go_up = true;
			break;
		case 5:
			go_up = false;
			break;
		}
	}

	if (!attacking) {
		if (face_right) {
			this->spd.x += 60 * dt;
		}
		else {
			this->spd.x -= 60 * dt;
		}
		if (go_up) {
			this->spd.y += 30 * dt;
		}
		else {
			this->spd.y -= 30 * dt;
		}
	}

	if (face_right) {
		setFlipX(false);
	}
	else {
		setFlipX(true);
	}
}

void Moth::Hurt(int dmg)
{
	hp -= dmg;
}

void Moth::Hit(Player * p)
{
	float o_head = p->getPositionY() + (p->getBoundingBox().size.height / 2);
	float o_foot = p->getPositionY() - (p->getBoundingBox().size.height / 2);
	float o_left = p->getPositionX() - (p->getBoundingBox().size.width / 2);
	float o_right = p->getPositionX() + (p->getBoundingBox().size.width / 2);

	float t_head = this->getPositionY() + (this->getBoundingBox().size.height / 2);
	float t_foot = this->getPositionY() - (this->getBoundingBox().size.height / 2);
	float t_left = this->getPositionX() - (this->getBoundingBox().size.width / 2);
	float t_right = this->getPositionX() + (this->getBoundingBox().size.width / 2);

	if (p->getState() != PS_HURT && p->getInvince() <= 0) {

		if (o_head + p->spd.y > t_foot && o_foot + p->spd.y < t_head &&
			o_left < t_right && o_right > t_left) {
			if (p->spd.y > 0) {
				p->spd.y = -3;
				this->spd.x = 0;
				this->spd.y = 0;
				p->hurt(1);
			}
			else {
				p->spd.y = 5;
				this->spd.x = 0;
				this->spd.y = 0;
				p->hurt(1);
			}
		}

		if (o_head > t_foot && o_foot < t_head &&
			o_left + p->spd.x < t_right + spd.x && o_right + p->spd.x > t_left + spd.x) {
			if (p->spd.x > 0) {
				p->spd.y = 3;
				p->spd.x = -3;
				this->spd.x = 0;
				this->spd.y = 0;
				p->hurt(1);
			}
			else {
				//other->spd.x = (t_right + spd.x) - o_left;
				p->spd.y = 3;
				p->spd.x = 3;
				this->spd.x = 0;
				this->spd.y = 0;
				p->hurt(1);
			}
		}

	}
}

int Moth::getHp()
{
	return hp;
}

bool Moth::HitDetect(Entity * other)
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
			return true;
		}
		else {
			return true;
		}
	}

	if (o_head > t_foot && o_foot < t_head &&
		o_left + other->spd.x < t_right + spd.x && o_right + other->spd.x > t_left + spd.x) {
		if (other->spd.x > 0) {
			return true;
		}
		else {
			return true;
		}
	}


	return false;
}

void Moth::Update(float dt)
{
	spd.y = 0;
	spd.x = 0;
}

void Moth::Move()
{
	setPosition(getPosition() + spd);
}

/*****************************/
/*        Rat    Code        */
/*****************************/

Rat * Rat::create(const std::string & filename, Entity * Platform)
{
	if (ratret && ratret->initWithFile(filename)) {
		cocos2d::Vector<cocos2d::SpriteFrame *> fly_frames = { cocos2d::SpriteFrame::create("test_dummy_2.png", cocos2d::Rect(0,0,60,38), false, {0,0}, {29,41}) };
		ratret->platform = Platform;

		ratret->pLeft = Platform->getPositionX() - (Platform->getBoundingBox().size.width / 2);
		ratret->pRight = Platform->getPositionX() + (Platform->getBoundingBox().size.width / 2);

		ratret->setScale(0.8);
		//ratret->runAction(cocos2d::RepeatForever::create(cocos2d::Animate::create(ratret->animations.at(0))));
		//ratret->autorelease();
		return ratret;
	}
	CC_SAFE_RELEASE(ratret);
	return nullptr;
}

	void Rat::AI(Player * player, float dt)
	{
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
				this->spd.x += 6 * dt;
			}
			else {
				this->spd.x -= 6 * dt;
			}
			if (turn == 25) {
				if (face_right) {
					this->spd.x -= 6 * dt;
				}
				else {
					this->spd.x += 6 * dt;
				}
			}
		}

		if (this->getPositionX() >= pRight) {
			spd.x = spd.x * (-1);
			face_right = false;
		}
		else if (this->getPositionX() <= pLeft) {
			spd.x = spd.x * (-1);
			face_right = true;
		}
		/***************/
	}

	void Rat::Hurt(int dmg)
	{
		this->hp -= dmg;
	}

	void Rat::Hit(Player * p)
	{
		float o_head = p->getPositionY() + (p->getBoundingBox().size.height / 2);
		float o_foot = p->getPositionY() - (p->getBoundingBox().size.height / 2);
		float o_left = p->getPositionX() - (p->getBoundingBox().size.width / 2);
		float o_right = p->getPositionX() + (p->getBoundingBox().size.width / 2);

		float t_head = this->getPositionY() + (this->getBoundingBox().size.height / 2);
		float t_foot = this->getPositionY() - (this->getBoundingBox().size.height / 2);
		float t_left = this->getPositionX() - (this->getBoundingBox().size.width / 2);
		float t_right = this->getPositionX() + (this->getBoundingBox().size.width / 2);

		if (p->getState() != PS_HURT && p->getInvince() <= 0) {

			if (o_head + p->spd.y > t_foot && o_foot + p->spd.y < t_head &&
				o_left < t_right && o_right > t_left) {
				if (p->spd.y > 0) {
					
					p->spd.y = -3;
					this->spd.x = 0;
					p->hurt(1);
				}
				else {
					p->spd.y = 5;
					this->spd.x = 0;
					p->hurt(1);
				}
			}

			if (o_head > t_foot && o_foot < t_head &&
				o_left + p->spd.x < t_right + spd.x && o_right + p->spd.x > t_left + spd.x) {
				if (p->spd.x > 0) {
					p->spd.y = 3;
					p->spd.x = -3;
					this->spd.x = 0;
					p->hurt(1);
				}
				else {
					//other->spd.x = (t_right + spd.x) - o_left;
					p->spd.y = 3;
					p->spd.x = 3;
					this->spd.x = 0;
					p->hurt(1);
				}
			}

		}
	}

	int Rat::getHp()
	{
		return hp;
	}

	bool Rat::HitDetect(Entity * other)
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
				return true;
			}
			else {
				return true;
			}
		}

		if (o_head > t_foot && o_foot < t_head &&
			o_left + other->spd.x < t_right + spd.x && o_right + other->spd.x > t_left + spd.x) {
			if (other->spd.x > 0) {
				return true;
			}
			else {
				return true;
			}
		}


		return false;
	}

	void Rat::Update(float dt)
	{
		spd.y += GRAVITY * dt;
		if (spd.y < T_VELOCITY) {
			spd.y = T_VELOCITY;
		}

		//spd.y = 0;
		spd.x = 0;
	}

	void Rat::Move()
	{
		setPosition(getPosition() + spd);
	}