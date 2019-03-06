#include "GameplayScene.h"
#include "Effects/LightEffect.h"
#include "Effects/EffectLayer.h"
#include "Entities/Fireball.h"
#include "Constants.h"
#include <string>
#include "Textbox.h"
#include "Prompt.h"
#include "Enums.h"

#include "Door.h"
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
	//Center of screen
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	EffectSprite *_bgColor = EffectSprite::create("BG.png");
	_bgColor->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
	_bgColor->setScale(1);
	this->addChild(_bgColor, -10);

	player = Player ::create("test_dummy.png", this);
	player->setScale(SCALE);
	player->getTexture()->setTexParameters(tp);

	 currInv = new player_inventory(1);

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
		knight->setPosition( 700, 300);
		this->addChild(knight);
	}
	else {
		return false;
		
	}
	//platforms
	terrain.pushBack(Block::create(0,0, 800, 200));
	terrain.pushBack(Block::create(500,200, 300, 75));
	terrain.pushBack(Block::create(280, 350, 180, 10));


	//interactables.pushBack(Interactable::create(100, 200, 50, 50,SWITCH));	//Some Switch thing

	interactables.pushBack(Door::create(320, 200, 30, 70, DOOR)); //Normal Door

	//interactables.pushBack(Interactable::create(550,270, 30, 70, S_DOOR)); // Scene Door

	interactables.pushBack(Door::create(210, 200, 20, 80, DOOR, GEN_KEY)); // KeyDoor with general Key.
	//interactables.pushBack(Interactable::create(50, 200, 20, 80, DOOR, GEN_KEY)); // KeyDoor with general Key.


	std::string plat1_file = "Platform1.png";
	ActualPlatforms.pushBack(Platform::create(plat1_file, cocos2d::Vec2(100,280)));

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

	for each (Interactable* inter in interactables) {
		if (inter != nullptr) {
			this->addChild(inter);
		}
		else {
			return false;
		}
	}

	for each (Platform* p in ActualPlatforms) {
		if (p != nullptr) {

			/*p->setScale(SCALE);
			p->getTexture()->setTexParameters(tp);*/

			this->addChild(p);
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
		case EventKeyboard::KeyCode::KEY_E:
			GAMEPLAY_INPUT.key_interact = true;
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
		case EventKeyboard::KeyCode::KEY_E:
			GAMEPLAY_INPUT.key_interact = false;
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
Textbox* ActiveTextbox;
Prompt* ActivePrompt;
bool promptInit = false;
bool overlap = false;

void GameplayScene::update(float dt) {
	player->Update(dt);

	TheGamepad->Refresh();
	knight->Update(dt);
	knight->AI(player, dt);



	for each (Interactable* i in interactables) { //Showing Prompts?
		if (i->inRange(player)) {
			if (promptInit == false) {
				auto Prompt1 = Prompt::create(1, (this));
				addChild(Prompt1, 10);
				Prompt1->Load();
				if (ActivePrompt)
				{
					ActivePrompt->Close();
				}
				ActivePrompt = Prompt1;
				promptInit = true;
			}
			overlap = true;
			promptInit = true;
			ActivePrompt->Show();
			break;
		}
		else
		{
			overlap = false;

		}

		if (!(i->inRange(player) || i->getCooldown() )) {
			if (ActivePrompt && !(overlap)) { ActivePrompt->Hide();}
		}
	}

	if (GAMEPLAY_INPUT.key_interact) {	//When the Interact Key is pressed, it looks through to see if the player is close enough to any interactables
		for each (Interactable* i in interactables) {
			if (i->inRange(player) ) {
				InteractType curr_thing = i->getType();
				switch (curr_thing) {
				case DOOR:
					i->Effect(i->getType(), player, currInv);
					break;
				case SWITCH:
					break;
				}
				
				i->setCooldown();
				break;
			}
		}
	}

	for each (Interactable* i in interactables) {
		i->Update(dt);
	}

	if (knight->getLanded()) {
		player->knockBack(true);
		knockTimer = true;
	}
	if (knockTimer) {
		timer -= dt;
		if (timer <= 0) {
			knockTimer = false;
			knight->setLanded(false);
			player->knockBack(false);
		}
	}
	else {
		timer = maxTime;
	}
	if (!player->getKnockback()) {
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


		if (player->isHurt() && knight->HitDetect(player)) {
			player->hurt(2);
			player->knockBack(true);
		}
	}
	else {
	knockTimer = true;
	}

	for each (Interactable* i in interactables) {
		if (i->getType() == DOOR) {	//Add all interactable types that actually collide with the player here.
			i->HitDetect(player);
			i->HitDetect(knight);
		}
	}

	if (GAMEPLAY_INPUT.key_space && !GAMEPLAY_INPUT.key_space_p) {
		player->Attack();
		GAMEPLAY_INPUT.key_space_p = true;
	}

	for each (Block* platform in terrain)
	{
		platform->HitDetect(player);
		platform->HitDetect(knight);
		
	}

	for each (Platform* p in ActualPlatforms) {
		p->HitDetect(player);
		p->HitDetect(knight);
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