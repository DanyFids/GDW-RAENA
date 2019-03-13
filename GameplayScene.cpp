#include "GameplayScene.h"
#include "Effects/LightEffect.h"
#include "Effects/EffectLayer.h"
#include "Entities/Fireball.h"
#include "Constants.h"
#include <string>
#include "Textbox.h"
#include "Prompt.h"
#include "Enums.h"

#include "Entities/Door.h"
#include "GamePad.h"
#include "InventoryScene.h"
USING_NS_CC;
			  
Gamepad* TheGamepad;




Scene* GameplayScene::createScene() {
	return GameplayScene::create();
}

void GameplayScene::movePlayer(Entity * player,cocos2d::Vec2 move)
{
	player->setPosition(move);
}


bool GameplayScene::init() {
	Texture2D::TexParams tp = { GL_NEAREST , GL_NEAREST, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE };

	if (!Scene::init()) {
		return false;
	}

	auto Updatepad = new (std::nothrow) Gamepad;
	Updatepad->CheckConnection();
	TheGamepad = Updatepad;
	
	// Player ////////////////////////////
	player = Player ::create("test_dummy.png", this);
	player->setScale(SCALE);
	player->getTexture()->setTexParameters(tp);

	currInv = new player_inventory(1);

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
		case EventKeyboard::KeyCode::KEY_ENTER:
			Director::getInstance()->pushScene(InventoryScene::create());
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

	return true;
}
Textbox* ActiveTextbox;
Prompt* ActivePrompt;
bool promptInit = false;
bool overlap = false;

void GameplayScene::update(float dt) {
	player->Update(dt);

	TheGamepad->Refresh();

	if (knight != nullptr) {
		knight->Update(dt);
		knight->AI(player, dt);
	}

	PNode->setPosition(view->getPosition());

	if (interactables.size() > 0) {
		for each (Interactable* i in interactables) { //Showing Prompts?
			if (i->inRange(player) && i->getType() != LOAD_ZONE) {
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

			if (!(i->inRange(player) || i->getCooldown())) {
				if (ActivePrompt && !(overlap)) { ActivePrompt->Hide(); }
			}
		}


		if (GAMEPLAY_INPUT.key_interact) {	//When the Interact Key is pressed, it looks through to see if the player is close enough to any interactables
			for each (Interactable* i in interactables) {
				if (i->inRange(player)) {
					InteractType curr_thing = i->getType();
					switch (curr_thing) {
					case DOOR:
						((Door*)i)->Effect(player, currInv);
						i->setCooldown();
						break;
					case SWITCH:
						break;
					case S_DOOR:
						((SceneDoor*)i)->Effect(player, currInv);
						i->setCooldown();
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
	}

	if (!player->isKnocked()) {
		if (GAMEPLAY_INPUT.key_left) {
			if (player->getState() != PS_Climb) {
				player->spd.x = -PLAYER_SPEED * dt;
			}
		}
	}

	if (Pushables.size() > 0)
	{
		for each (Pushable* P in Pushables)
		{
			P->Update(dt);
		}
	}

	if (GAMEPLAY_INPUT.key_left || TheGamepad->leftStickX <= -0.2) {
		if (player->getState() != PS_Climb) {
			player->setFlipX(true);
			player->spd.x = -PLAYER_SPEED * dt;
		}
	}

	if (GAMEPLAY_INPUT.key_right || TheGamepad->leftStickX >= 0.2) {
		if (player->getState() != PS_Climb) {
			if (player->getState() == PS_Crouch) {
				player->spd.x = CROUCH_SPEED * dt;
			}
			player->setFlipX(false);
			player->spd.x = PLAYER_SPEED * dt;
		}
	}

	if (GAMEPLAY_INPUT.key_down || TheGamepad->leftStickY <= -0.2) {
		if (player->getState() == PS_Climb) {
			player->spd.y = -PLAYER_SPEED * dt;
		}
	}

	if (GAMEPLAY_INPUT.key_up || TheGamepad->leftStickY >= 0.2) {
		if (player->getState() == PS_Climb) {
			player->spd.y = PLAYER_SPEED * dt;
		}
	}

	//GamePad///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//if (TheGamepad->CheckConnection() == true)
	//{
	//	if (TheGamepad->leftStickX >= 0.2)
	//	{
	//		//player->spd.x = TheGamepad->leftStickX * 100 * dt;
	//		if (player->getState() != PS_Climb) {
	//			if (player->getState() == PS_Crouch) {
	//				player->spd.x = CROUCH_SPEED * dt;
	//			}
	//			player->setFlipX(false);
	//			player->spd.x = PLAYER_SPEED * dt;
	//		}
	//	}
	//	if (TheGamepad->leftStickX <= -0.2)
	//	{
	//		if (player->getState() != PS_Climb) {
	//			player->setFlipX(true);
	//			player->spd.x = -PLAYER_SPEED * dt;
	//		}
	//	}
	//	if (TheGamepad->leftStickY <= -0.2)
	//	{
	//		if (player->getState() == PS_Climb) {
	//			player->spd.y = -PLAYER_SPEED * dt;
	//		}
	//	}

	//	if (TheGamepad->leftStickY >= 0.2)
	//	{
	//		if (player->getState() == PS_Climb) {
	//			player->spd.y = PLAYER_SPEED * dt;
	//		}
	//	}

	//	/*if (TheGamepad->IsPressed(XINPUT_GAMEPAD_DPAD_RIGHT))
	//	{
	//		player->spd.x = PLAYER_SPEED * dt;
	//	}
	//	if (TheGamepad->IsPressed(XINPUT_GAMEPAD_DPAD_LEFT))
	//	{
	//		player->spd.x = -PLAYER_SPEED * dt;
	//	}*/
	//	if (TheGamepad->IsPressed(XINPUT_GAMEPAD_A ) && !Bpress1)
	//	{	
	//		Bpress1 = true;
	//		player->Jump();
	//	}
	//	else if (!(TheGamepad->IsPressed(XINPUT_GAMEPAD_A)))
	//	{
	//		Bpress1 = false;
	//	}
	//	if (TheGamepad->IsPressed(XINPUT_GAMEPAD_X))
	//	{
	//		player->Attack();
	//	}
	//	if (TheGamepad->IsPressed(XINPUT_GAMEPAD_START))
	//	{
	//		Director::getInstance()->pushScene(InventoryScene::create());
	//	}
	//	if (TheGamepad->IsPressed(XINPUT_GAMEPAD_B) && !Bpress2)
	//	{
	//		if (player->getState() == PS_Stand) {
	//			player->Crouch();
	//		}
	//		else if (player->getState() == PS_Crouch) {
	//			player->Stand();
	//		}
	//		Bpress2 = true;
	//	}
	//	else if (!(TheGamepad->IsPressed(XINPUT_GAMEPAD_B)))
	//	{
	//		Bpress2 = false;
	//	}
	//	if (TheGamepad->IsPressed(XINPUT_GAMEPAD_Y))
	//	{
	//		//When the Interact Key is pressed, it looks through to see if the player is close enough to any interactables
	//		for each (Interactable* i in interactables) 
	//		{
	//			if (i->inRange(player)) 
	//			{
	//				InteractType curr_thing = i->getType();
	//				switch (curr_thing) 
	//				{
	//				case DOOR:
	//					((Door*)i)->Effect(player, currInv);
	//					i->setCooldown();
	//					break;
	//				case SWITCH:
	//					break;
	//				case S_DOOR:
	//					((SceneDoor*)i)->Effect(player, currInv);
	//					i->setCooldown();
	//					break;
	//				}

	//				i->setCooldown();
	//				break;
	//			}
	//		}			
	//	}
	//}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

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





	if (GAMEPLAY_INPUT.key_jump && !GAMEPLAY_INPUT.key_jump_p || TheGamepad->IsPressed(XINPUT_GAMEPAD_A && !GAMEPLAY_INPUT.key_jump_p)) {
		player->Jump();
		GAMEPLAY_INPUT.key_jump_p = true;
	}

	if (knight != nullptr)
	{
		if (knight->HitDetect(player)) {
			player->hurt(2);
		}
	}
	
	
	if (interactables.size() > 0)	//Are there interactables on this map?
	{
		for each (Interactable* i in interactables) {
			if (i->getType() == DOOR) {	//Add all interactable types that actually collide with the player here.
				i->HitDetect(player);

				if (knight != nullptr)
				{
					i->HitDetect(knight);
				}
			}

			if (i->getType() == LOAD_ZONE)
			{
				((LoadZone*)i)->HitDetect(player);
			}

		}
	}

	if (GAMEPLAY_INPUT.key_space && !GAMEPLAY_INPUT.key_space_p || TheGamepad->IsPressed(XINPUT_GAMEPAD_X) && !GAMEPLAY_INPUT.key_space_p) {
		player->Attack();
		GAMEPLAY_INPUT.key_space_p = true;
	}

	for each (Block* platform in terrain)
	{
		platform->HitDetect(player);

		if (knight != nullptr) {
			platform->HitDetect(knight);
		}
	}

	if (Pushables.size() > 0)
	{
		for each (Pushable* Push in Pushables)
		{
			Push->HitDetect(player);
			//platform->HitDetect(Push);
			//Push->HitDetect(player);
		}
	}

		
	if (ActualPlatforms.size() > 0)
	{
		for each (Platform* p in ActualPlatforms) {
			p->HitDetect(player);

			if (knight != nullptr)
			{
				p->HitDetect(knight);
			}
		}
	}

	if (torches.size() > 0)
	{
		for each (Torch* t in torches) {
			player->HitDetectEnem(t);
		}
	}

	if (ladders.size())
	{
		for each (Ladder* lad in ladders)
		{
			if (lad->HitDetect(player) && player->getState() != PS_Climb) {
				if (GAMEPLAY_INPUT.key_up && !lad->PlayerOnTop()) {
					player->Climb(lad);
				}
				else if (GAMEPLAY_INPUT.key_down && lad->PlayerOnTop()) {
					player->ClimbDown(lad);
				}
			}
		}
	}

	if (GAMEPLAY_INPUT.key_crouch && !GAMEPLAY_INPUT.key_crouch_p || TheGamepad->IsPressed(XINPUT_GAMEPAD_B) && !GAMEPLAY_INPUT.key_crouch_p) {
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

	if (Pushables.size() > 0)
	{
		for each (Pushable* P in Pushables)
		{
			P->Move();
		}
	}
	

	if (knight != nullptr) {
		knight->Move();
	}

	if (knight != nullptr) {
		if (knight->HitDetect(player)) {
			player->hurt(2);
		}
	}

	// Move Camera
	if (player->getPositionX() >= (Director::getInstance()->getVisibleSize().width / 2) && player->getPositionX() <= STAGE_WIDTH - (Director::getInstance()->getVisibleSize().width/2)) {
		view->setPositionX(player->getPositionX());
	}

	if (player->getPositionY() >= (Director::getInstance()->getVisibleSize().height / 3) && player->getPositionY() <= STAGE_HEIGHT - (Director::getInstance()->getVisibleSize().height * 2 / 3)) {
		view->setPositionY(player->getPositionY() + (Director::getInstance()->getVisibleSize().height / 6));
	}

}

bool TutRoom1::init()
{
	if (GameplayScene::init()) {



		return true;
	}
	return false;
}

void TutRoom1::update(float dt)
{
	GameplayScene::update(dt);
}

bool TestRoom1::init()
{
	if (GameplayScene::init()) {

		//Paralax & Background //////////////////////////////////////////////////////////////////////////////
		auto paraNode = ParallaxNode::create();
		PNode = paraNode;
		EffectSprite *_bgColor = EffectSprite::create("BGP1.png");

		_bgColor->setScale(1);

		paraNode->addChild(_bgColor, 1, Vec2(0.4f, 0.5f), Vec2::ZERO);

		EffectSprite *_bgColor2 = EffectSprite::create("BGP2.png");

		_bgColor2->setScale(1);
		paraNode->addChild(_bgColor2, -1, Vec2(1.4f, 1.5f), Vec2::ZERO);

		this->addChild(paraNode);

		auto visibleSize = Director::getInstance()->getVisibleSize();
		//Center of screen ///////////////////////////////////////////////////////////////////////
		Vec2 origin = Director::getInstance()->getVisibleOrigin();

		//Player Loc /////////////////////////////////////////////////////////////////////////
		if (player != nullptr) {
			player->setPosition(Vec2((visibleSize.width / 2) - player->getBoundingBox().size.width / 2 + origin.x, (visibleSize.height / 2) - player->getBoundingBox().size.height / 2 + origin.y));

			this->addChild(player, 10);
		}
		else {
			return false;
		}
		//Knight //////////////////////////////////////////////////////////////////////////////////////
		knight = Knight::create("test_dummy.png");

		if (knight != nullptr) {
			knight->setPosition(Vec2((visibleSize.width / 2) - knight->getBoundingBox().size.width / 2 + origin.x, (visibleSize.height / 2) - knight->getBoundingBox().size.height / 2 + origin.y));
			knight->setPosition(700, 300);
			this->addChild(knight);
		}
		else {
			return false;
		}
		
		//platforms ////////////////////////////////////////////////////////////////////////////////
		terrain.pushBack(Block::create(0, 0, 800, 200));
		terrain.pushBack(Block::create(500, 200, 300, 75));
		terrain.pushBack(Block::create(280, 350, 180, 10));
		for each (Entity* plat in terrain)
		{
			if (plat != nullptr) {
				this->addChild(plat);
			}
			else {
				return false;
			}
		}
		//ladders
		ladders.pushBack(Ladder::create(248, 200, 32, 160));
		
		for each (Ladder* lad in ladders) {
			if (lad != nullptr) {
				this->addChild(lad);
			}
			else {
				return false;
			}
		}

		//Interactables /////////////////////////////////////////////////////////////////////////////////////////
		//interactables.pushBack(Interactable::create(100, 200, 50, 50,SWITCH));	//Some Switch thing

		//interactables.pushBack(Door::create(320, 200, 30, 70, DOOR)); //Normal Door

		

		//interactables.pushBack(Door::create(210, 200, 20, 80, DOOR, GEN_KEY)); // KeyDoor with general Key.

		//interactables.pushBack(Interactable::create(50, 200, 20, 80, DOOR, GEN_KEY)); // KeyDoor with general Key.
		for each (Interactable* inter in interactables) {
			if (inter != nullptr) {
				this->addChild(inter);
			}
			else {
				return false;
			}
		}
		//Actual Platforms ///////////////////////////////////////////////////////////////////////////////////////////
		

		ActualPlatforms.pushBack(Platform::create("Platform1.png", cocos2d::Vec2(100, 280)));
		for each (Platform* p in ActualPlatforms) {
			if (p != nullptr) {

				/*p->setScale(SCALE);
				p->getTexture()->setTexParameters(tp);*/

				this->addChild(p);
			}
		}

		// Lighting Tests
		auto _effect = LightEffect::create();
		_effect->retain();

		Vec3 l_pos(150, 300, 50);

		_effect->addLight(l_pos);
		//_effect->addLight(Vec3(400, 250, 50));
		//_effect->addLight(Vec3(600, 250, 50));
		//_effect->addLight(Vec3(200, 250, 50));
		//_effect->addLight(Vec3(0, 250, 50));
		//_effect->addLight(Vec3(800, 250, 50));
		_effect->setLightCutoffRadius(250);
		_effect->setLightHalfRadius(0.5);
		_effect->setBrightness(0.7);
		_effect->setAmbientLightColor(Color3B(0, 0, 0));

		player->setEffect(_effect, "test_NM.png");
		_bgColor->setEffect(_effect, "test_NM.png");
		_bgColor2->setEffect(_effect, "test_NM.png");

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
		player->switchLight();
		view = this->getDefaultCamera();

		this->scheduleUpdate();
		return true;
	}

	return false;
}

void TestRoom1::update(float dt)
{
	GameplayScene::update(dt);
}



bool A1_R1::init()
{
	//STAGE_HEIGHT = 600;
	//STAGE_WIDTH = 1000;		Defaults

	STAGE_WIDTH = 900;

	if (GameplayScene::init()) {


		auto visibleSize = Director::getInstance()->getVisibleSize();
		//Center of screen ///////////////////////////////////////////////////////////////////////
		Vec2 origin = Director::getInstance()->getVisibleOrigin();

		//Parallax Stuff
		auto paraNode = ParallaxNode::create();
		PNode = paraNode;
		EffectSprite *_bgColor = EffectSprite::create("BGP1.png");

		_bgColor->setAnchorPoint(cocos2d::Vec2(0, 0));
		_bgColor->setScale(1);

		paraNode->addChild(_bgColor, 1, Vec2(0.4f, 0.5f), Vec2::ZERO);


		this->addChild(paraNode);

		// Lighting Tests
		auto _effect = LightEffect::create();
		_effect->retain();

		_effect->setLightCutoffRadius(250);
		_effect->setLightHalfRadius(0.5);
		_effect->setBrightness(0.7);
		_effect->setAmbientLightColor(Color3B(255, 255, 255));  //255 = no shadow, 0 = black

		player->setEffect(_effect, "test_NM.png");
		_bgColor->setEffect(_effect, "test_NM.png");
		
		player->switchLight();

		//Entities
		if (player != nullptr) {
			player->setPosition(Vec2((visibleSize.width / 2) - player->getBoundingBox().size.width / 2 + origin.x, (visibleSize.height / 2) - player->getBoundingBox().size.height / 2 + origin.y));

			this->addChild(player, 10);
		}
		else {
			return false;
		}
									  // x,y w,h
		terrain.pushBack(Block::create(0, 0, 600, 200)); //Ground

		terrain.pushBack(Block::create(600, 0, 400, 270)); //Ground

		terrain.pushBack(Block::create(0, 200, 100, 750)); //Tree

		for each (Entity* plat in terrain)
		{
			if (plat != nullptr) {
				this->addChild(plat);
			}
			else {
				return false;
			}
		}

		//Push_back
		//for each (Ladder* lad in ladders) {
		//	if (lad != nullptr) {
		//		this->addChild(lad);
		//	}
		//	else {
		//		return false;
		//	}
		//}

		//interactables.pushBack(SceneDoor::create("closed_door.png", Vec2(700,270),A1_R2)); // Scene Door
		interactables.pushBack(LoadZone::create(900,270,30,400, A1_R2,Vec2(50,205))); // LoadZone

		for each (Interactable* inter in interactables) {
			if (inter != nullptr) {
				this->addChild(inter);
			}
			else {
				return false;
			}
		}

		//Platforms
		//ActualPlatforms.pushBack(Platform::create("Platform1.png", cocos2d::Vec2(100, 280)));
		//for each (Platform* p in ActualPlatforms) {
		//	if (p != nullptr) {
		//
		//		/*p->setScale(SCALE);
		//		p->getTexture()->setTexParameters(tp);*/
		//
		//		this->addChild(p);
		//	}
		//}



		view = this->getDefaultCamera();


		scheduleUpdate();

		return true;
	}


	return false;
}

void A1_R1::update(float dt)
{
	GameplayScene::update(dt);
}


// A1_R2

bool A1_R2::init()
{
	//STAGE_HEIGHT = 600;
	//STAGE_WIDTH = 1000;		Defaults

	STAGE_HEIGHT = 900;

	if (GameplayScene::init()) {


		auto visibleSize = Director::getInstance()->getVisibleSize();
		//Center of screen ///////////////////////////////////////////////////////////////////////
		Vec2 origin = Director::getInstance()->getVisibleOrigin();

		//Parallax Stuff
		auto paraNode = ParallaxNode::create();
		PNode = paraNode;
		EffectSprite *_bgColor = EffectSprite::create("BGP1.png");

		_bgColor->setAnchorPoint(cocos2d::Vec2(0, 0));
		_bgColor->setScale(1);

		paraNode->addChild(_bgColor, 1, Vec2(0.4f, 0.5f), Vec2::ZERO);

		this->addChild(paraNode);

		// Lighting Tests
		auto _effect = LightEffect::create();
		_effect->retain();

		_effect->setLightCutoffRadius(250);
		_effect->setLightHalfRadius(0.5);
		_effect->setBrightness(0.7);
		_effect->setAmbientLightColor(Color3B(255, 255, 255));

		player->setEffect(_effect, "test_NM.png");
		_bgColor->setEffect(_effect, "test_NM.png");

		player->switchLight();

		//Entities
		if (player != nullptr) {
			player->setPosition(Vec2((visibleSize.width / 2) - player->getBoundingBox().size.width / 2 + origin.x, (visibleSize.height / 2) - player->getBoundingBox().size.height / 2 + origin.y));

			this->addChild(player, 10);
		}
		else {
			return false;
		}


		// x,y w,h
		terrain.pushBack(Block::create(0, 0, 600, 200)); //Ground
		terrain.pushBack(Block::create(600, 0, 400,670)); //Ground
		terrain.pushBack(Block::create(260, 200, 70, 70)); //Rock
		
		for each (Entity* plat in terrain)
		{
			if (plat != nullptr) {
				this->addChild(plat);
			}
			else {
				return false;
			}
		}

		//Push_back
		//for each (Ladder* lad in ladders) {
		//	if (lad != nullptr) {
		//		this->addChild(lad);
		//	}
		//	else {
		//		return false;
		//	}
		//}

		interactables.pushBack(LoadZone::create(-10, 205, 10, 400, A1_R1, Vec2(50, 205))); // LoadZone
		interactables.pushBack(LoadZone::create(1000, 670, 10, 400, A1_R1, Vec2(50, 205))); // LoadZone

		for each (Interactable* inter in interactables) {
			if (inter != nullptr) {
				this->addChild(inter);
			}
			else {
				return false;
			}
		}

		//Platforms
		ActualPlatforms.pushBack(Platform::create("Platform1.png", cocos2d::Vec2(200, 380)));
		for each (Platform* p in ActualPlatforms) {
			if (p != nullptr) {
		
				/*p->setScale(SCALE);
				p->getTexture()->setTexParameters(tp);*/
		
				this->addChild(p);
			}
		}



		view = this->getDefaultCamera();


		scheduleUpdate();

		return true;
	}


	return false;
}

void A1_R2::update(float dt)
{
	GameplayScene::update(dt);
}


//A1_R3
bool A1_R3::init()	//Pushable And Crouch Tutorial
{
	//STAGE_HEIGHT = 600;
	//STAGE_WIDTH = 1000;		Defaults

	STAGE_HEIGHT = 650;
	STAGE_WIDTH = 1200;

	if (GameplayScene::init()) {


		auto visibleSize = Director::getInstance()->getVisibleSize();
		//Center of screen ///////////////////////////////////////////////////////////////////////
		Vec2 origin = Director::getInstance()->getVisibleOrigin();

		//Parallax Stuff
		auto paraNode = ParallaxNode::create();
		PNode = paraNode;
		EffectSprite *_bgColor = EffectSprite::create("BGP1.png");

		_bgColor->setAnchorPoint(cocos2d::Vec2(0, 0));
		_bgColor->setScale(1);

		paraNode->addChild(_bgColor, 1, Vec2(0.4f, 0.5f), Vec2::ZERO);

		this->addChild(paraNode);

		// Lighting Tests
		auto _effect = LightEffect::create();
		_effect->retain();

		_effect->setLightCutoffRadius(250);
		_effect->setLightHalfRadius(0.5);
		_effect->setBrightness(0.7);
		_effect->setAmbientLightColor(Color3B(255, 255, 255));

		player->setEffect(_effect, "test_NM.png");
		_bgColor->setEffect(_effect, "test_NM.png");

		player->switchLight();

		//Entities
		if (player != nullptr) {
			player->setPosition(Vec2((visibleSize.width / 2) - player->getBoundingBox().size.width / 2 + origin.x, (visibleSize.height / 2) - player->getBoundingBox().size.height / 2 + origin.y));

			this->addChild(player, 10);
		}
		else {
			return false;
		}


		// x,y w,h
		terrain.pushBack(Block::create(0, 0, 300, 200)); //Ground
		terrain.pushBack(Block::create(300, 0, 400, 100)); //Ground
		terrain.pushBack(Block::create(700, 0, 400, 275)); //Ground
		terrain.pushBack(Block::create(1100, 0, 120, 500)); //Ground

		terrain.pushBack(Block::create(300, 0, 60, 120)); //Ground	*stopper block

		terrain.pushBack(Block::create(800, 308, 60, 400)); //Floating
		terrain.pushBack(Block::create(800, 500, 420, 400)); //Roof

		

		for each (Entity* plat in terrain)
		{
			if (plat != nullptr) {
				this->addChild(plat);
			}
			else {
				return false;
			}
		}

		//Push_back
		//for each (Ladder* lad in ladders) {
		//	if (lad != nullptr) {
		//		this->addChild(lad);
		//	}
		//	else {
		//		return false;
		//	}
		//}
										 //x,y,w,h  , Vec2 MaxLeft, Vec2 MaxRight
		Pushables.pushBack(Pushable::create(470,140,80,80, Vec2(400,140),Vec2(660,140)));

		for each (Pushable* push in Pushables) {
			if (push != nullptr) {
				this->addChild(push);
			}
			else
				return false;
			
		}

		interactables.pushBack(LoadZone::create(-10, 205, 10, 400, A1_R1, Vec2(50, 205))); // LoadZone
		interactables.pushBack(SceneDoor::create("closed_door.png", Vec2(1050, 275), Vec2(50,200), A1_R4));	//SceneDoor

		for each (Interactable* inter in interactables) {
			if (inter != nullptr) {
				this->addChild(inter);
			}
			else {
				return false;
			}
		}

		//Platforms
		//ActualPlatforms.pushBack(Platform::create("Platform1.png", cocos2d::Vec2(200, 380)));
		//for each (Platform* p in ActualPlatforms) {
		//	if (p != nullptr) {
		//
		//		/*p->setScale(SCALE);
		//		p->getTexture()->setTexParameters(tp);*/
		//
		//		this->addChild(p);
		//	}
		//}



		view = this->getDefaultCamera();


		scheduleUpdate();

		return true;
	}


	return false;
}

void A1_R3::update(float dt)
{
	GameplayScene::update(dt);
}


// A1_R4

bool A1_R4::init()
{
	//STAGE_HEIGHT = 600;
	//STAGE_WIDTH = 1000;		Defaults

	STAGE_HEIGHT = 900;



	if (GameplayScene::init()) {


		auto visibleSize = Director::getInstance()->getVisibleSize();
		//Center of screen ///////////////////////////////////////////////////////////////////////
		Vec2 origin = Director::getInstance()->getVisibleOrigin();

		//Parallax Stuff
		auto paraNode = ParallaxNode::create();
		PNode = paraNode;
		EffectSprite *_bgColor = EffectSprite::create("BGP1.png");

		_bgColor->setAnchorPoint(cocos2d::Vec2(0, 0));
		_bgColor->setScale(1);

		paraNode->addChild(_bgColor, 1, Vec2(0.4f, 0.5f), Vec2::ZERO);

		this->addChild(paraNode);

		// Lighting Tests
		auto _effect = LightEffect::create();
		_effect->retain();

		_effect->setLightCutoffRadius(250);
		_effect->setLightHalfRadius(0.5);
		_effect->setBrightness(0.7);
		_effect->setAmbientLightColor(Color3B(255, 255, 255));

		player->setEffect(_effect, "test_NM.png");
		_bgColor->setEffect(_effect, "test_NM.png");

		player->switchLight();

		//Entities
		if (player != nullptr) {
			player->setPosition(Vec2((visibleSize.width / 2) - player->getBoundingBox().size.width / 2 + origin.x, (visibleSize.height / 2) - player->getBoundingBox().size.height / 2 + origin.y));

			this->addChild(player, 10);
		}
		else {
			return false;
		}


		// x,y w,h
		terrain.pushBack(Block::create(0, 0, 1000, 150)); //Ground
		terrain.pushBack(Block::create(275, 900, 450, 10)); //Cieling

		terrain.pushBack(Block::create(0, 150, 275, 400)); //Ground 1
		terrain.pushBack(Block::create(750, 150, 500, 300)); //Ground 2

		for each (Entity* plat in terrain)
		{
			if (plat != nullptr) {
				this->addChild(plat);
			}
			else {
				return false;
			}
		}
						 //x y w h
		ladders.pushBack(Ladder::create(300, 200, 32, 900));

		ladders.pushBack(Ladder::create(600, 400, 32, 500));

		for each (Ladder* lad in ladders) {
			if (lad != nullptr) {
				this->addChild(lad);
			}
			else {
				return false;
			}
		}

		interactables.pushBack(LoadZone::create(-10, 400, 10, 400, A1_R3, Vec2(50, 205))); // LoadZone
		interactables.pushBack(LoadZone::create(1000, 300, 10, 400, A1_R5, Vec2(50, 205))); // LoadZone

		for each (Interactable* inter in interactables) {
			if (inter != nullptr) {
				this->addChild(inter);
			}
			else {
				return false;
			}
		}

		//Platforms
		//ActualPlatforms.pushBack(Platform::create("Platform1.png", cocos2d::Vec2(200, 380)));
		//for each (Platform* p in ActualPlatforms) {
		//	if (p != nullptr) {
		//
		//		/*p->setScale(SCALE);
		//		p->getTexture()->setTexParameters(tp);*/
		//
		//		this->addChild(p);
		//	}
		//}



		view = this->getDefaultCamera();


		scheduleUpdate();

		return true;
	}


	return false;
}

void A1_R4::update(float dt)
{
	GameplayScene::update(dt);
}

//A1_R5

bool A1_R5::init()
{
	//STAGE_HEIGHT = 600;
	//STAGE_WIDTH = 1000;		Defaults

	STAGE_HEIGHT = 900;
	STAGE_WIDTH = 1600;



	if (GameplayScene::init()) {


		auto visibleSize = Director::getInstance()->getVisibleSize();
		//Center of screen ///////////////////////////////////////////////////////////////////////
		Vec2 origin = Director::getInstance()->getVisibleOrigin();

		//Parallax Stuff
		auto paraNode = ParallaxNode::create();
		PNode = paraNode;
		EffectSprite *_bgColor = EffectSprite::create("BGP1.png");

		_bgColor->setAnchorPoint(cocos2d::Vec2(0, 0));
		_bgColor->setScale(1);

		paraNode->addChild(_bgColor, 1, Vec2(0.4f, 0.5f), Vec2::ZERO);

		this->addChild(paraNode);

		// Lighting Tests
		auto _effect = LightEffect::create();
		_effect->retain();

		_effect->setLightCutoffRadius(250);
		_effect->setLightHalfRadius(0.5);
		_effect->setBrightness(0.7);
		_effect->setAmbientLightColor(Color3B(255, 255, 255));

		player->setEffect(_effect, "test_NM.png");
		_bgColor->setEffect(_effect, "test_NM.png");

		player->switchLight();

		//Entities
		if (player != nullptr) {
			player->setPosition(Vec2((visibleSize.width / 2) - player->getBoundingBox().size.width / 2 + origin.x, (visibleSize.height / 2) - player->getBoundingBox().size.height / 2 + origin.y));

			this->addChild(player, 10);
		}
		else {
			return false;
		}


		// x,y w,h
		terrain.pushBack(Block::create(0, 0, 1600, 100)); //Base Ground

		terrain.pushBack(Block::create(-100, 100, 250, 75)); //Ground 1
		terrain.pushBack(Block::create(150, 100, 75, 40)); //Ground 2
		terrain.pushBack(Block::create(640, 100, 60, 40)); //Ground 2

		terrain.pushBack(Block::create(700, 100, 300, 75)); //Ground 3
		terrain.pushBack(Block::create(700, 210, 225, 200)); //floating 3

		terrain.pushBack(Block::create(1400, 100, 300, 450)); //Ground 3

		for each (Entity* plat in terrain)
		{
			if (plat != nullptr) {
				this->addChild(plat);
			}
			else {
				return false;
			}
		}

		Pushables.pushBack(Pushable::create(559, 140, 80, 80, Vec2(265, 140), Vec2(600, 140)));

		for each (Pushable* push in Pushables) {
			if (push != nullptr) {
				this->addChild(push);
			}
			else
				return false;

		}

		//x y w h
		//ladders.pushBack(Ladder::create(300, 200, 32, 900));

		//ladders.pushBack(Ladder::create(600, 400, 32, 500));
		//
		//for each (Ladder* lad in ladders) {
		//	if (lad != nullptr) {
		//		this->addChild(lad);
		//	}
		//	else {
		//		return false;
		//	}
		//}

		interactables.pushBack(LoadZone::create(-10, 100, 10, 400, A1_R4, Vec2(50, 205))); // LoadZone
		//interactables.pushBack(LoadZone::create(1000, 300, 10, 400, A1_R6, Vec2(50, 205))); // LoadZone

		for each (Interactable* inter in interactables) {
			if (inter != nullptr) {
				this->addChild(inter);
			}
			else {
				return false;
			}
		}

		//Platforms
		//ActualPlatforms.pushBack(Platform::create("Platform1.png", cocos2d::Vec2(200, 380)));
		//for each (Platform* p in ActualPlatforms) {
		//	if (p != nullptr) {
		//
		//		/*p->setScale(SCALE);
		//		p->getTexture()->setTexParameters(tp);*/
		//
		//		this->addChild(p);
		//	}
		//}





		view = this->getDefaultCamera();


		scheduleUpdate();

		return true;
	}


	return false;
}

void A1_R5::update(float dt)
{
	GameplayScene::update(dt);
}
