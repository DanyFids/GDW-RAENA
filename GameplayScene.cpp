#include "GameplayScene.h"
#include "Effects/LightEffect.h"
#include "Effects/EffectLayer.h"
#include "Textbox.h"
#include "Prompt.h"

USING_NS_CC;



Scene* GameplayScene::createScene() {
	return GameplayScene::create();
}

bool GameplayScene::init() {
	if (!Scene::init()) {
		return false;
	}

	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	EffectSprite *_bgColor = EffectSprite::create("BG.png");
	_bgColor->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
	_bgColor->setScale(1);
	this->addChild(_bgColor, -10);

	player = Player ::create("test_dummy.png");

	if (player != nullptr) {
		player->setPosition(Vec2((visibleSize.width / 2) - player->getBoundingBox().size.width / 2 + origin.x, (visibleSize.height / 2) - player->getBoundingBox().size.height / 2 + origin.y));

		this->addChild(player);
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
		case EventKeyboard::KeyCode::KEY_M:
			GAMEPLAY_INPUT.key_one = true;
			break;
		case EventKeyboard::KeyCode::KEY_F:
			GAMEPLAY_INPUT.key_F = true;
			break;
		case EventKeyboard::KeyCode::KEY_L:
			GAMEPLAY_INPUT.key_two = true;
			break;
		case EventKeyboard::KeyCode::KEY_1:
			GAMEPLAY_INPUT.key_P1 = true;
			break;
		case EventKeyboard::KeyCode::KEY_2:
			GAMEPLAY_INPUT.key_P2 = true;
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
		case EventKeyboard::KeyCode::KEY_M:
			GAMEPLAY_INPUT.key_one = false;
			GAMEPLAY_INPUT.key_oneP = false;
			break;
		case EventKeyboard::KeyCode::KEY_F:
			GAMEPLAY_INPUT.key_F = false;
			GAMEPLAY_INPUT.key_FP = false;
			break;
		case EventKeyboard::KeyCode::KEY_L:
			GAMEPLAY_INPUT.key_two = false;
			GAMEPLAY_INPUT.key_twoP = false;
			break;
		case EventKeyboard::KeyCode::KEY_1:
			GAMEPLAY_INPUT.key_P1 = false;
			GAMEPLAY_INPUT.key_P1P = false;
			break;
		case EventKeyboard::KeyCode::KEY_2:
			GAMEPLAY_INPUT.key_P2 = false;
			GAMEPLAY_INPUT.key_P2P = false;
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
Textbox* ActiveTextbox;
Prompt* ActivePrompt;

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
	if (GAMEPLAY_INPUT.key_one && !GAMEPLAY_INPUT.key_oneP)
	{
		auto Textbox1 = Textbox::create(1, { 1 }, { "What up fuckbois" }, (this));
		addChild(Textbox1, 10);
		Textbox1->Load();
		if (ActiveTextbox)
		{
			ActiveTextbox->Close();
		}
		

		ActiveTextbox = Textbox1;
		
		GAMEPLAY_INPUT.key_oneP = true;

		
	}
	
	

	if (GAMEPLAY_INPUT.key_two && !GAMEPLAY_INPUT.key_twoP)
	{
		auto Textbox2 = Textbox::create(2, { 1,1 }, { "Yeet", "Get Dabbed on" }, (this));
		addChild(Textbox2, 10);
		Textbox2->Load();
		if (ActiveTextbox)
		{
			ActiveTextbox->Close();
		}
		ActiveTextbox = Textbox2;

		GAMEPLAY_INPUT.key_twoP = true;
	
	}

	if (GAMEPLAY_INPUT.key_P1 && !GAMEPLAY_INPUT.key_P1P)
	{
		auto Prompt1 = Prompt::create(1, (this));
		addChild(Prompt1, 10);
		Prompt1->Load();
		if (ActivePrompt)
		{
			ActivePrompt->Close();
		}
		ActivePrompt = Prompt1;

		GAMEPLAY_INPUT.key_P1P = true;
	}
	if (GAMEPLAY_INPUT.key_P2 && !GAMEPLAY_INPUT.key_P2P)
	{
		auto Prompt2 = Prompt::create(2, (this));
		addChild(Prompt2, 10);
		Prompt2->Load();
		if (ActivePrompt)
		{
			ActivePrompt->Close();
		}
		ActivePrompt = Prompt2;

		GAMEPLAY_INPUT.key_P2P = true;
	}

	if (GAMEPLAY_INPUT.key_F && !GAMEPLAY_INPUT.key_FP)
	{
		
		//ActiveTextbox->Load();
		ActiveTextbox->Close();
		int ActiveCurrPage = ActiveTextbox->getCurrPage();
		int ActivePages = ActiveTextbox->getPages();
		if (!(ActiveCurrPage == ActivePages - 1))
		{
			ActiveTextbox->Flippage();
			ActiveTextbox->Load();
		}
		
		GAMEPLAY_INPUT.key_FP = true;
	}

	if (ActivePrompt)
	{
		ActivePrompt->Follow(player);
	}

	player->moveLightToPlayer();
}