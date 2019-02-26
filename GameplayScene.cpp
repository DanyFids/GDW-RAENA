#include "GameplayScene.h"
#include "Effects/LightEffect.h"
#include "Effects/EffectLayer.h"
#include "Entities/Fireball.h"
#include "Constants.h"

USING_NS_CC;

Scene* GameplayScene::createScene() {
	return GameplayScene::create();
}

bool GameplayScene::init() {
	Texture2D::TexParams tp = { GL_NEAREST , GL_NEAREST, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE };

	if (!Scene::init()) {
		return false;
	}

	auto visibleSize = Director::getInstance()->getVisibleSize();
	//Center of screen
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	EffectSprite *_bgColor = EffectSprite::create("BG.png");
	_bgColor->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
	_bgColor->setScale(1);
	this->addChild(_bgColor, -10);

	player = Player ::create("test_dummy.png", this);
	player->setScale(SCALE);
	player->getTexture()->setTexParameters(tp);

	if (player != nullptr) {
		player->setPosition(Vec2((visibleSize.width / 2) - player->getBoundingBox().size.width / 2 + origin.x, (visibleSize.height / 2) - player->getBoundingBox().size.height / 2 + origin.y));

		this->addChild(player, 10);
	}
	else {
		return false;
	}

	knight = Knight::create("test_dummy.png");

	if (knight != nullptr) {
		knight->setPosition(Vec2((visibleSize.width / 2) - knight->getBoundingBox().size.width / 2 + origin.x, (visibleSize.height / 2) - knight->getBoundingBox().size.height / 2 + origin.y));
		knight->setPosition( 100, 250);
		this->addChild(knight);
	}
	else {
		return false;
		
	}
	//platforms
	terrain.pushBack(Block::create(0,0, 800, 200));
	terrain.pushBack(Block::create(500,200, 300, 75));
	terrain.pushBack(Block::create(280, 350, 180, 10));

	//ladders
	ladders.pushBack(Ladder::create(248, 200, 32, 160));

	for each (Entity* plat in terrain)
	{
		if (plat != nullptr) {
			this->addChild(plat);
		}
		else {
			return false;
		}
	}

	for each (Ladder* lad in ladders) {
		if (lad != nullptr) {
			this->addChild(lad);
		}
		else {
			return false;
		}
	}

	auto KeyHandler = EventListenerKeyboard::create();

	/*
	*	Key Down Event Handler
	*
	*/
	KeyHandler->onKeyPressed = [this](EventKeyboard::KeyCode key, Event * event) {
		switch (key) {
		case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
		case EventKeyboard::KeyCode::KEY_A:
			GAMEPLAY_INPUT.key_left = true;
			break;
		case EventKeyboard::KeyCode::KEY_UP_ARROW:
		case EventKeyboard::KeyCode::KEY_W:
			GAMEPLAY_INPUT.key_up = true;
			break;
		case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
		case EventKeyboard::KeyCode::KEY_D:
			GAMEPLAY_INPUT.key_right = true;
			break;
		case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
		case EventKeyboard::KeyCode::KEY_S:
			GAMEPLAY_INPUT.key_down = true;
			break;
		case EventKeyboard::KeyCode::KEY_SPACE:
			GAMEPLAY_INPUT.key_space = true;
			break;
		case EventKeyboard::KeyCode::KEY_C:
			GAMEPLAY_INPUT.key_jump = true;
			break;
		case EventKeyboard::KeyCode::KEY_X:
			GAMEPLAY_INPUT.key_crouch = true;
			break;
		}
	};

	KeyHandler->onKeyReleased = [this](EventKeyboard::KeyCode key, Event * event) {
		switch (key) {
		case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
		case EventKeyboard::KeyCode::KEY_A:
			GAMEPLAY_INPUT.key_left = false;
			break;
		case EventKeyboard::KeyCode::KEY_UP_ARROW:
		case EventKeyboard::KeyCode::KEY_W:
			GAMEPLAY_INPUT.key_up = false;
			break;
		case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
		case EventKeyboard::KeyCode::KEY_D:
			GAMEPLAY_INPUT.key_right = false;
			break;
		case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
		case EventKeyboard::KeyCode::KEY_S:
			GAMEPLAY_INPUT.key_down = false;
			break;
		case EventKeyboard::KeyCode::KEY_SPACE:
			GAMEPLAY_INPUT.key_space = false;
			GAMEPLAY_INPUT.key_space_p = false;
			break;
		case EventKeyboard::KeyCode::KEY_C:
			GAMEPLAY_INPUT.key_jump = false;
			GAMEPLAY_INPUT.key_jump_p = false;
			break;
		case EventKeyboard::KeyCode::KEY_X:
			GAMEPLAY_INPUT.key_crouch = false;
			GAMEPLAY_INPUT.key_crouch_p = false;
			break;
		}
	};

	this->_eventDispatcher->addEventListenerWithSceneGraphPriority(KeyHandler, player);

	// Lighting Tests
	auto _effect = LightEffect::create();
	_effect->retain();

	Vec3 l_pos(150, 300, 50);

	//_effect->addLight(l_pos);
	//_effect->addLight(Vec3(400, 250, 50));
	//_effect->addLight(Vec3(600, 250, 50));
	//_effect->addLight(Vec3(200, 250, 50));
	//_effect->addLight(Vec3(0, 250, 50));
	//_effect->addLight(Vec3(800, 250, 50));
	_effect->setLightCutoffRadius(250);
	_effect->setLightHalfRadius(0.5);
	_effect->setBrightness(0.7);
	_effect->setAmbientLightColor(Color3B(25, 25, 25));

	player->setEffect(_effect, "test_NM.png");
	_bgColor->setEffect(_effect, "layerNorm.png");

	player->switchLight();

	//Set Torches;
	torches.pushBack(Torch::create(cocos2d::Vec2(200, 230), _effect));
	torches.pushBack(Torch::create(cocos2d::Vec2(350, 400), _effect));
	torches.pushBack(Torch::create(cocos2d::Vec2(600, 310), _effect));

	for each (Torch* t in torches)
	{
		if (t != nullptr) {
			addChild(t, 2);
		}
		else {
			return false;
		}
	}

	this->scheduleUpdate();

	view = this->getDefaultCamera();

	return true;
}

void GameplayScene::update(float dt) {
	player->Update(dt);
	knight->Update(dt);
	knight->AI(player, dt);

	if (GAMEPLAY_INPUT.key_left) {
		if (player->getState() != PS_Climb) {
			player->spd.x = -PLAYER_SPEED * dt;
		}
	}
	if (GAMEPLAY_INPUT.key_right) {
		if (player->getState() != PS_Climb) {
			if (player->getState() == PS_Crouch) {
				player->spd.x = CROUCH_SPEED * dt;
			}
			player->spd.x = PLAYER_SPEED * dt;
		}
	}
	if (GAMEPLAY_INPUT.key_down) {
		if (player->getState() == PS_Climb) {
			player->spd.y = -PLAYER_SPEED * dt;
		}
	}

	if (GAMEPLAY_INPUT.key_up) {
		if (player->getState() == PS_Climb) {
			player->spd.y = PLAYER_SPEED * dt;
		}
	}

	if (GAMEPLAY_INPUT.key_space && ! GAMEPLAY_INPUT.key_space_p) {
		player->Attack();
		GAMEPLAY_INPUT.key_space_p = true;
	}

	if (GAMEPLAY_INPUT.key_jump && !GAMEPLAY_INPUT.key_jump_p) {
		player->Jump();
		GAMEPLAY_INPUT.key_jump_p = true;
	}

	for each (Block* platform in terrain)
	{
		platform->HitDetect(player);
		platform->HitDetect(knight);
		
	}

	for each (Torch* t in torches) {
		player->HitDetectEnem(t);
	}

	for each (Ladder* lad in ladders)
	{
		if (lad->HitDetect(player) && player->getState() != PS_Climb) {
			if (GAMEPLAY_INPUT.key_up && !lad->PlayerOnTop()) {
				player->Climb(lad);
			}
			else if(GAMEPLAY_INPUT.key_down && lad->PlayerOnTop()){
				player->ClimbDown(lad);
			}
		}
	}

	if (GAMEPLAY_INPUT.key_crouch && !GAMEPLAY_INPUT.key_crouch_p) {
		if (player->getState() == PS_Stand) {
			player->Crouch();
		}
		else if (player->getState() == PS_Crouch) {
			player->Stand();
		}
		GAMEPLAY_INPUT.key_crouch_p = true;
	}

	player->Move();
	player->moveLightToPlayer();

	knight->Move();
	if (knight->HitDetect(player)) {
		player->hurt(2);
	}

	// Move Camera
	if (player->getPositionX() >= (Director::getInstance()->getVisibleSize().width / 2) && player->getPositionX() <= STAGE_WIDTH - (Director::getInstance()->getVisibleSize().width/2)) {
		view->setPositionX(player->getPositionX());
	}

	if (player->getPositionY() >= (Director::getInstance()->getVisibleSize().height / 3) && player->getPositionY() <= STAGE_HEIGHT - (Director::getInstance()->getVisibleSize().height * 2 / 3)) {
		view->setPositionY(player->getPositionY() + (Director::getInstance()->getVisibleSize().height / 6));
	}
}