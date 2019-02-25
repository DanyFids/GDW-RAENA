#include "GameplayScene.h"
#include "Effects/LightEffect.h"
#include "Effects/EffectLayer.h"

#include "Textbox.h"
#include "Prompt.h"

#include "Entities/Fireball.h"
#include "Constants.h"
#include "GamePad.h"

USING_NS_CC;

Gamepad* TheGamepad;

Scene* GameplayScene::createScene() {
	return GameplayScene::create();
}

bool GameplayScene::init() {
	Texture2D::TexParams tp = { GL_NEAREST , GL_NEAREST, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE };

	if (!Scene::init()) {
		return false;
	}

	auto Updatepad = new (std::nothrow) Gamepad;
	Updatepad->CheckConnection();
	TheGamepad = Updatepad;

	auto visibleSize = Director::getInstance()->getVisibleSize();
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

	//platforms
	platforms.pushBack(Block::create(0,0, 800, 200));
	platforms.pushBack(Block::create(500,200, 300, 75));
	platforms.pushBack(Block::create(280, 350, 180, 10));

	for each (Block* plat in platforms)
	{
		if (plat != nullptr) {
			this->addChild(plat);
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

		case EventKeyboard::KeyCode::KEY_C:
			GAMEPLAY_INPUT.key_jump = true;

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

		case EventKeyboard::KeyCode::KEY_C:
			GAMEPLAY_INPUT.key_jump = false;
			GAMEPLAY_INPUT.key_jump_p = false;

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


	

	return true;
}
Textbox* ActiveTextbox;
Prompt* ActivePrompt;

void GameplayScene::update(float dt) {
	player->Update(dt);
	TheGamepad->Refresh();

	if (GAMEPLAY_INPUT.key_left) {
		player->spd.x = -PLAYER_SPEED * dt;
	}
	if (GAMEPLAY_INPUT.key_right) {
		player->spd.x = PLAYER_SPEED * dt;
	}


	if (TheGamepad->CheckConnection() == true)
	{
		if (TheGamepad->leftStickX >= 0.2)
		{
			player->spd.x = TheGamepad->leftStickX * 100 * dt;
		}
		if (TheGamepad->leftStickX <= -0.2)
		{
			player->spd.x = TheGamepad->leftStickX * 100 * dt;
		}

		if (TheGamepad->IsPressed(XINPUT_GAMEPAD_DPAD_RIGHT))
		{
			player->spd.x = PLAYER_SPEED * dt;
		}
		if (TheGamepad->IsPressed(XINPUT_GAMEPAD_DPAD_LEFT))
		{
			player->spd.x = -PLAYER_SPEED * dt;
		}
		if (TheGamepad->IsPressed(XINPUT_GAMEPAD_A))
		{
			player->Jump();
		}
		if (TheGamepad->IsPressed(XINPUT_GAMEPAD_X))
		{
			player->Attack();
		}
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








	if (GAMEPLAY_INPUT.key_jump && !GAMEPLAY_INPUT.key_jump_p) {
		player->Jump();
		GAMEPLAY_INPUT.key_jump_p = true;
	}
	

	if (GAMEPLAY_INPUT.key_space && !GAMEPLAY_INPUT.key_space_p) {
		player->Attack();
		GAMEPLAY_INPUT.key_space_p = true;
	}

	








	for each (Block* platform in platforms)
	{
		platform->HitDetect(player);
	}

	for each (Torch* t in torches) {
		player->HitDetectEnem(t);
	}

	player->Move();
	player->moveLightToPlayer();
}