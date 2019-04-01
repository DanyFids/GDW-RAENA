#include "Player.h"
#include "Effects/LightEffect.h"
#include "Constants.h"
#include "Fireball.h"
#include "Ladder.h"
#include "Torch.h"

Player * Player::create(const std::string& filename, cocos2d::Scene * s)
{
	auto ret = new (std::nothrow) Player;
	if (ret && ret->initWithFile(filename)) {
		cocos2d::Vector<cocos2d::SpriteFrame *> stand_frames = { cocos2d::SpriteFrame::create("dragon_idle.png", cocos2d::Rect(0,0,38,64), false, {0,0}, {38, 64 }) };
		cocos2d::Vector<cocos2d::SpriteFrame *> crouch_frames = { cocos2d::SpriteFrame::create( "player_crouch.png", cocos2d::Rect(0,0,64,38), false, {0,0}, {64, 38 }) };

		ret->animations.pushBack(cocos2d::Animation::createWithSpriteFrames(stand_frames, 0.1f));
		ret->animations.pushBack(cocos2d::Animation::createWithSpriteFrames(crouch_frames, 0.1f));

		ret->runAction(cocos2d::RepeatForever::create(cocos2d::Animate::create(ret->animations.at(0))));

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
		player_light = this->getEffect()->addLight(cocos2d::Vec3(this->getPosition().x, this->getPosition().y, 20));
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
	if (state != PS_HURT && invince_timer <= 0) {
		hp -= dmg;
		knock_timer = KNOCK_TIME;
		invince_timer = INVINCE_TIME;
		state = PS_HURT;
		stopAllActions();
		runAction(cocos2d::RepeatForever::create(cocos2d::Animate::create(animations.at(0))));
	}
}

bool Player::HitDetect(Entity * other)
{
	return false;
}

void Player::Update(float dt)
{
	if (state == PS_Climb && climb_lad != nullptr) {
		if (!climb_lad->HitDetect(this)) {
			state = PS_Stand;
		}
		glide_used = false;
		glide_timer = GLIDE_TIME;
	}

	if (knock_timer > 0) {
		knock_timer -= dt;
		state = PS_HURT;
		if (knock_timer <= 0) {
			state = PS_Stand;
		}
	}

	if (state == PS_Glide) {
		glide_timer -= dt;
		if (glide_timer <= 0 && can_vert) {
			state = PS_Jump;
			this->stopAllActions();
			this->runAction(cocos2d::RepeatForever::create(cocos2d::Animate::create(animations.at(0))));
			glide_used = true;
		}
	}

	if (state != PS_Climb) {
		if (state != PS_Glide) {
			spd.y += GRAVITY * dt;
			if (spd.y < T_VELOCITY) {
				spd.y = T_VELOCITY;
			}
		}
		else {
			spd.y = GLIDE_V * dt;
		}
	}
	else {
		spd.y = 0;
	}

	if (knock_timer <= 0) {
		spd.x = 0;
	}

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

	if (knock_timer > 0) {
		knock_timer -= dt;
		state = PS_HURT;
		if (knock_timer <= 0) {
			state = PS_Stand;
		}
	}

	if (invince_timer > 0) {
		invince_timer -= dt;
		if (invince_timer <= 0) {
		}
	}
}

void Player::Move()
{
	if (!attacking) {
		setPosition(getPosition() + spd);

		if (spd.y != 0 && on_ground && state != PS_HURT && can_vert) {
			on_ground = false;
			state = PS_Jump;
			this->stopAllActions();
			this->runAction(cocos2d::RepeatForever::create(cocos2d::Animate::create(animations.at(0))));
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
	if (state == PS_Crouch) {
		if (!can_vert) {
			return;
		}
		Stand();
	}
	if ((on_ground || state == PS_Climb) && !attacking) {
		spd.y = JUMP_V;
		on_ground = false;
		state = PS_Jump;
	}
	else if (state == PS_Jump || state == PS_Glide) {
		Glide();
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
			atk_pos = cocos2d::Vec2(getPositionX() + (getBoundingBox().size.width / 2 + 18), getPositionY() + 18);
		}
		else {
			atk_pos = cocos2d::Vec2(getPositionX() - (getBoundingBox().size.width / 2 + 18), getPositionY() + 18);
		}

		atk = Fireball::create(atk_pos, getEffect());
		if (face_right)
		{
			atk->setFlipX(false);
		}
		else
		{
			atk->setFlipX(true);
		}
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

void Player::Climb(Ladder * lad)
{
	if (can_vert) {
		if (state == PS_Crouch) {
			Stand();
		}
		this->stopAllActions();
		this->runAction(cocos2d::RepeatForever::create(cocos2d::Animate::create(animations.at(0))));
		spd = { 0, 0 };
		climb_lad = lad;
		setPositionX(lad->getPositionX() + (lad->getBoundingBox().size.width / 2));
		state = PS_Climb;
	}
}

void Player::ClimbDown(Ladder * lad)
{
	if (state == PS_Crouch) {
		Stand();
	}
	this->stopAllActions();
	this->runAction(cocos2d::RepeatForever::create(cocos2d::Animate::create(animations.at(0))));
	spd = { 0, 0 };
	state = PS_Climb;
	


	climb_lad = lad;
	setPositionX(lad->getPositionX() + (lad->getBoundingBox().size.width / 2));
	setPositionY(this->getPositionY() - (this->getBoundingBox().size.height / 2));
}

int Player::getHP()
{
	return hp;
}

void Player::setHP(int i)
{
	hp = i;
}

void Player::Land() {
	if (!on_ground) {
		if (can_vert) {
			if (state == PS_Glide) {
				Stand();
			}
			this->stopAllActions();
			this->runAction(cocos2d::RepeatForever::create(cocos2d::Animate::create(animations.at(0))));
			on_ground = true;
			state = PS_Stand;
			glide_used = false;
		}
		else {
			this->stopAllActions();
			this->runAction(cocos2d::RepeatForever::create(cocos2d::Animate::create(animations.at(1))));
			on_ground = true;
			state = PS_Crouch;
			glide_used = false;
		}
	}
}

void Player::Crouch()
{
	if (can_horz) {
		float foot = this->getPositionY() - (getContentSize().height / 2);


		float stand_height = getContentSize().height;
		float crouch_height = getContentSize().width;
		this->stopAllActions();
		this->runAction(cocos2d::RepeatForever::create(cocos2d::Animate::create(animations.at(1))));

		state = PS_Crouch;

		float new_y = foot + (crouch_height / 2);

		setContentSize({ getContentSize().height , getContentSize().width });

		this->setPositionY(new_y);
	}
}

void Player::Stand()
{
	if (can_vert) {
		float crouch_height = getBoundingBox().size.height;
		float stand_height = getBoundingBox().size.width;
		this->stopAllActions();
		this->runAction(cocos2d::RepeatForever::create(cocos2d::Animate::create(animations.at(0))));
		state = PS_Stand;

		this->setPositionY(getPositionY() - (crouch_height / 2));
		this->setPositionY(getPositionY() + (stand_height / 2));
	}
}

void Player::Glide()
{
	if (!glide_used && can_horz) {
		this->stopAllActions();
		this->runAction(cocos2d::RepeatForever::create(cocos2d::Animate::create(animations.at(1))));
		state = PS_Glide;
		spd.y = 0;
		glide_timer = GLIDE_TIME;
		glide_used = true;
	}
	else if (state == PS_Glide && can_vert) {
		this->stopAllActions();
		this->runAction(cocos2d::RepeatForever::create(cocos2d::Animate::create(animations.at(0))));
		state = PS_Jump;
	}
}

void Player::DetectObstruction(Entity * other)
{
	float largest_size = (getBoundingBox().size.width > getBoundingBox().size.height) ? getBoundingBox().size.width : getBoundingBox().size.height;
	float small_size = (getBoundingBox().size.width < getBoundingBox().size.height) ? getBoundingBox().size.width : getBoundingBox().size.height;
	
	float t_head;
	float t_foot;
	float t_left;
	float t_right;

	if (state == PS_Crouch) {
		t_foot = getPositionY() - (getBoundingBox().size.height/2);

		t_head = t_foot + largest_size;
		t_left = getPositionX() - (largest_size / 2.0f);
		t_right = getPositionX() + (largest_size / 2.0f);
	}
	else {
		t_head = getPositionY() + (largest_size/2.0f);
		t_foot = getPositionY() - (largest_size / 2.0f);
		t_left = getPositionX() - (largest_size / 2.0f);
		t_right = getPositionX() + (largest_size / 2.0f);
	}

	cocos2d::Vec2 anchorP = other->getAnchorPoint();

	float o_foot = other->getPositionY() - (other->getBoundingBox().size.height * anchorP.y);
	float o_head = other->getPositionY() + (other->getBoundingBox().size.height - (other->getBoundingBox().size.height * anchorP.y));
	float o_left = other->getPositionX() - (other->getBoundingBox().size.width * anchorP.x);
	float o_right = other->getPositionX() + (other->getBoundingBox().size.width - (other->getBoundingBox().size.width * anchorP.x));

	if (t_foot < o_head && t_head > o_foot && t_left < o_right && t_right > o_left) {
		switch (state) {
		case PS_Glide:
		case PS_Crouch:
			can_vert = false;
			break;
		case PS_Stand:
			if (o_foot < t_foot + small_size) {
				can_horz = false;
			}
			break;
		default:
			can_horz = false;
			break;
		}
	}
}


