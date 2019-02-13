#include "GameplayScene.h"
#include "Effects/LightEffect.h"
#include "Effects/EffectLayer.h"

USING_NS_CC;

Scene* GameplayScene::createScene() {
	return GameplayScene::create();
}

bool GameplayScene::init() {
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

	player = Player::create("test_dummy.png");

	if (player != nullptr) {
		player->setPosition(Vec2((visibleSize.width / 2) - player->getBoundingBox().size.width / 2 + origin.x, (visibleSize.height / 2) - player->getBoundingBox().size.height / 2 + origin.y));

		this->addChild(player);
	}
	else {
		return false;
	}

	knight = Knight::create("test_dummy.png");

	if (knight != nullptr) {
		knight->setPosition(Vec2((visibleSize.width / 2) - knight->getBoundingBox().size.width / 2 + origin.x, (visibleSize.height / 2) - knight->getBoundingBox().size.height / 2 + origin.y));
		knight->setPosition(700, 50);
		this->addChild(knight);
	}
	else {
		return false;
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
		}
	};

	this->_eventDispatcher->addEventListenerWithSceneGraphPriority(KeyHandler, player);

	// Lighting Tests
	auto _effect = LightEffect::create();
	_effect->retain();

	Vec3 l_pos(150, 300, 50);

	//_effect->addLight(l_pos);
	_effect->addLight(Vec3(500, 300, 50));
	_effect->setLightCutoffRadius(250);
	_effect->setLightHalfRadius(0.1);
	_effect->setBrightness(1.0);
	_effect->setAmbientLightColor(Color3B(25, 25, 25));

	player->setEffect(_effect, "test_NM.png");
	_bgColor->setEffect(_effect, "layerNorm.png");

	this->scheduleUpdate();

	return true;
}

void GameplayScene::update(float dt) {
	if (GAMEPLAY_INPUT.key_up) {
		player->setPosition(player->getPosition() + Vec2(0, PLAYER_SPEED * dt));
	}
	if (GAMEPLAY_INPUT.key_down) {
		player->setPosition(player->getPosition() + Vec2(0, -PLAYER_SPEED * dt));
	}
	if (GAMEPLAY_INPUT.key_left) {
		player->setPosition(player->getPosition() + Vec2(-PLAYER_SPEED * dt, 0));
	}
	if (GAMEPLAY_INPUT.key_right) {
		player->setPosition(player->getPosition() + Vec2(PLAYER_SPEED * dt, 0));
	}

	if (GAMEPLAY_INPUT.key_space && ! GAMEPLAY_INPUT.key_space_p) {
		player->switchLight();
		GAMEPLAY_INPUT.key_space_p = true;
	}

	player->moveLightToPlayer();
	knight->AI(player, dt);
	knight->Move();
	knight->Update(dt);
}