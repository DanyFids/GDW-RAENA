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
#include "Entities/PuzzleInteractable.h"
#include "Entities/Pickups.h"
#include "GamePad.h"
#include "InventoryScene.h"
#include "PauseMenu.h"
#include "LevelManager/LevelManager.h"
USING_NS_CC;
			 

Scene* GameplayScene::createScene() {
	return GameplayScene::create();
}

//void GameplayScene::setUp(cocos2d::Vec2 movePlayer)
//{
//	player->setPosition(movePlayer);
//}

void GameplayScene::movePlayer(cocos2d::Vec2 move)
{
	this->clearKeys();
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
	player = Player ::create ("test_dummy.png", this);
	player->setScale(SCALE);
	player->getTexture()->setTexParameters(tp);

	//creates the audio
	auto audioF = CocosDenshion::SimpleAudioEngine::getInstance();
	TheAudioF = audioF;
	auto audioD = CocosDenshion::SimpleAudioEngine::getInstance();
	TheAudioD = audioD;
	auto audioB = CocosDenshion::SimpleAudioEngine::getInstance();
	TheAudioB = audioB;
	auto audioT = CocosDenshion::SimpleAudioEngine::getInstance();
	TheAudioT = audioT;

	if (!this->audioinitT)
	{
		TheAudioT->preloadBackgroundMusic("RAENA SOUNDSCAPE/Music/RaenaTitle.wav");
		TheAudioT->playBackgroundMusic("RAENA SOUNDSCAPE/Music/RaenaTitle.wav", true);
	}
	audioinitT = true;

	
 	this->addChild(player,10);
	
	currInv = new player_inventory(1);

	auto KeyHandler = EventListenerKeyboard::create();

	/*
	*	Key Down Event Handler
	*
	*/
		KeyHandler->onKeyPressed = [this](EventKeyboard::KeyCode key, Event * event) {
			if (!cutScene && player->getState() != PS_HURT) {
				switch (key) {
				case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
					GAMEPLAY_INPUT.key_left = true;
					break;
				case EventKeyboard::KeyCode::KEY_UP_ARROW:
					GAMEPLAY_INPUT.key_up = true;
					break;
				case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
					GAMEPLAY_INPUT.key_right = true;
					break;
				case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
					GAMEPLAY_INPUT.key_down = true;
					break;
				case EventKeyboard::KeyCode::KEY_SPACE:
					GAMEPLAY_INPUT.key_space = true;
					break;
				case EventKeyboard::KeyCode::KEY_C:
					GAMEPLAY_INPUT.key_jump = true;
					break;
				case EventKeyboard::KeyCode::KEY_D:
					GAMEPLAY_INPUT.key_interact = true;
					break;
				case EventKeyboard::KeyCode::KEY_X:
					GAMEPLAY_INPUT.key_crouch = true;
					break;
				case EventKeyboard::KeyCode::KEY_ENTER:
					this->clearKeys();
					Director::getInstance()->pushScene(InventoryScene::createScene(this));
					break;
			//	case EventKeyboard::KeyCode::KEY_1:
			//		GAMEPLAY_INPUT.key_one = true;
			//		break;
		case EventKeyboard::KeyCode::KEY_ESCAPE:
			Director::getInstance()->pushScene(PauseMenu::create());
				}
			}
		};

		KeyHandler->onKeyReleased = [this](EventKeyboard::KeyCode key, Event * event) {
			switch (key) {
			case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
				GAMEPLAY_INPUT.key_left = false;
				break;
			case EventKeyboard::KeyCode::KEY_UP_ARROW:
				GAMEPLAY_INPUT.key_up = false;
				break;
			case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
				GAMEPLAY_INPUT.key_right = false;
				break;
			case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
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
			case EventKeyboard::KeyCode::KEY_D:
				GAMEPLAY_INPUT.key_interact = false;
				break;
			case EventKeyboard::KeyCode::KEY_X:
				GAMEPLAY_INPUT.key_crouch = false;
				GAMEPLAY_INPUT.key_crouch_p = false;
				break;
			//case EventKeyboard::KeyCode::KEY_1:
			//	GAMEPLAY_INPUT.key_one = false;
			//	GAMEPLAY_INPUT.key_oneP = false;
			//	break;
			}
		};

		if (player->getState() == PS_HURT) {
			GAMEPLAY_INPUT.key_crouch = false;
			GAMEPLAY_INPUT.key_crouch_p = false;

			GAMEPLAY_INPUT.key_interact = false;

			GAMEPLAY_INPUT.key_jump = false;
			GAMEPLAY_INPUT.key_jump_p = false;

			GAMEPLAY_INPUT.key_space = false;
			GAMEPLAY_INPUT.key_space_p = false;

			GAMEPLAY_INPUT.key_down = false;

			GAMEPLAY_INPUT.key_right = false;

			GAMEPLAY_INPUT.key_up = false;

			GAMEPLAY_INPUT.key_left = false;
		}

	this->_eventDispatcher->addEventListenerWithSceneGraphPriority(KeyHandler, player);

	return true;
}


void GameplayScene::update(float dt) {
	player->Update(dt);

	TheGamepad->Refresh();

	if (Projectiles.size() > 0)	//Are there projectiles on this map?
	{
		for each (Projectile* i in Projectiles) {
			i->updateShotProjectile(dt);
			if (i->HitDetect(player))
			{
				player->hurt(1);
			}
		}
	}

	if (knight != nullptr) {
		knight->Update(dt);
		knight->AI(player, dt);
	}

	if (bossKnight != nullptr) {
		bossKnight->Update(dt);
		bossKnight->AI(player, dt);
	}

	if (moth.size() > 0) {
		for each (Moth* m in moth) {
			m->Update(dt);
			m->AI(player, dt);
		}
	}

	if (rat.size() > 0) {
		for each (Rat* r in rat) {
			r->Update(dt);
			r->AI(player, dt);
		}
	}

	if (knight != nullptr) {
		if (knight->getHp() <= 0) {

		}
	}

	
	for (int m = 0; m < moth.size(); m++) {
		if (moth.at(m)->getHp() <= 0) {
			this->removeChild(moth.at(m));
			moth.erase(m);
		}
	}

	for (int r = 0; r < rat.size(); r++) {
		if (rat.at(r)->getHp() <= 0) {
			this->removeChild(rat.at(r));
			rat.erase(r);
		}
	}

	PNode->setPosition(view->getPosition());

	if (interactables.size() > 0) {
		for each (Interactable* i in interactables) { //Showing Prompts?
			if (i->inRange(player) && i->getType() != LOAD_ZONE) {
				if (promptInit == false) {
					auto Prompt1 = Prompt::create(1, (this));
					//Prompt1->Load();
					if (ActivePrompt)
					{
						ActivePrompt->Close();
					}
					ActivePrompt = Prompt1;
					promptInit = true;
					addChild(ActivePrompt, 10);
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

		if (GAMEPLAY_INPUT.key_interact || TheGamepad->IsPressed(XINPUT_GAMEPAD_Y)) {	//When the Interact Key is pressed, it looks through to see if the player is close enough to any interactables
			for each (Interactable* i in interactables) {
				if (i->inRange(player)) {
					InteractType curr_thing = i->getType();
				}
			}
		}

		if (GAMEPLAY_INPUT.key_interact || TheGamepad->IsPressed(XINPUT_GAMEPAD_Y) && TheGamepad->CheckConnection()) {	//When the Interact Key is pressed, it looks through to see if the player is close enough to any interactables
			for (int i = 0; i < interactables.size(); i++) {
				if (interactables.at(i)->inRange(player)) {
					InteractType curr_thing = interactables.at(i)->getType(); //Getting the type
					switch (curr_thing) {
					case DOOR:
						((Door*)i)->Effect(player, currInv);
						interactables.at(i)->setCooldown();
						break;
					case SWITCH:
						break;
					case S_DOOR:
						((SceneDoor*)interactables.at(i))->Effect(player, currInv);
						interactables.at(i)->setCooldown();
						break;
					case PUZZLE:
						((PuzzleInteract*)interactables.at(i))->Effect(player, currInv, this);
						//if (((PuzzleInteract*)i)->checkPuzzle(Princess1) ) {
						//	this->addChild(knight);
						//}
						interactables.at(i)->setCooldown();
						break;
					case PICKUP:
						((Pickup*)interactables.at(i))->Effect(player, currInv);
						this->removeChild(interactables.at(i));
						interactables.erase(i);
						break;
					}
				}
			}

		for each (Interactable* i in interactables) {
			i->Update(dt);
		}
	}

	if (Pushables.size() > 0)
	{
		for each (Pushable* P in Pushables)
		{
			P->Update(dt);
		}
	}

	if (player->getState() != PS_HURT) {
		if (GAMEPLAY_INPUT.key_left || TheGamepad->leftStickX <= -0.2 && TheGamepad->CheckConnection()) {
			if (player->getState() != PS_Climb) {
				player->spd.x = -PLAYER_SPEED * dt;
				if (player->getState() == PS_Crouch) {
					player->spd.x = -CROUCH_SPEED * dt;
				}
			}
		}


		if (GAMEPLAY_INPUT.key_right || TheGamepad->leftStickX >= 0.2 && TheGamepad->CheckConnection()) {
			if (player->getState() != PS_Climb) {
				if (player->getState() == PS_Crouch) {
					player->spd.x = CROUCH_SPEED * dt;
				}
				player->spd.x = PLAYER_SPEED * dt;
			}
		}

		if (GAMEPLAY_INPUT.key_down || TheGamepad->leftStickY <= -0.2 && TheGamepad->CheckConnection()) {
			if (player->getState() == PS_Climb) {
				player->spd.y = -PLAYER_SPEED * dt;
			}
		}

		if (GAMEPLAY_INPUT.key_up || TheGamepad->leftStickY >= 0.2 && TheGamepad->CheckConnection()) {
			if (player->getState() == PS_Climb) {
				player->spd.y = PLAYER_SPEED * dt;
			}
		}

		if (GAMEPLAY_INPUT.key_inv || TheGamepad->IsPressed(XINPUT_GAMEPAD_START))
		{
			Director::getInstance()->pushScene(InventoryScene::create());
		}

		if (GAMEPLAY_INPUT.key_right != GAMEPLAY_INPUT.key_left && player->getState() != PS_HURT && !player->isAttacking()) {
			if (GAMEPLAY_INPUT.key_right) {
				player->setFacingRight(true);
				player->setFlipX(false);
			}
			else if (GAMEPLAY_INPUT.key_left) {
				player->setFacingRight(false);
				player->setFlipX(true);
			}
		}
	}

	//////////////////////////////////////////////////////////////////////////////////

	if (player->getState() != PS_HURT) {
		if (GAMEPLAY_INPUT.key_one && !GAMEPLAY_INPUT.key_oneP)
		{
			auto Textbox1 = Textbox::create(1, { 1 }, { "Hello World!" }, (this));
			addChild(Textbox1, 10);
			Textbox1->Load();
			if (ActiveTextbox)
			{
				ActiveTextbox->Close();
			}


			ActiveTextbox = Textbox1;

			GAMEPLAY_INPUT.key_oneP = true;
		}
	}
	if (player->getState() != PS_HURT) {
		if (GAMEPLAY_INPUT.key_F && !GAMEPLAY_INPUT.key_FP)
		{

			GAMEPLAY_INPUT.key_FP = true;
		}

		if (ActivePrompt)
		{
			ActivePrompt->Follow(player);
		}


		if (!TheGamepad->IsPressed(XINPUT_GAMEPAD_A) && !GAMEPLAY_INPUT.key_jump && TheGamepad->CheckConnection())
		{
			GAMEPLAY_INPUT.key_jump_p = false;
		}
		if ((GAMEPLAY_INPUT.key_jump && !GAMEPLAY_INPUT.key_jump_p) || (TheGamepad->IsPressed(XINPUT_GAMEPAD_A) && !GAMEPLAY_INPUT.key_jump_p)) {
			player->Jump();
			GAMEPLAY_INPUT.key_jump_p = true;
		}
	}

	if (knight != nullptr)
	{
		if (knight->HitDetect(player)) {
			player->hurt(2);
		}
	}
	
	player->ResetObstruction();
	
	if (interactables.size() > 0)	//Are there interactables on this map?
	{
		for each (Interactable* i in interactables) {
			if (i->getType() == DOOR) {	//Add all interactable types that actually collide with the player here.
				if (!i->getActive()) {
					player->DetectObstruction(i);
				}
				i->HitDetect(player);

				if (knight != nullptr)
				{
					i->HitDetect(knight);
				}

				if (moth.size() > 0) {
					for each (Moth* m in moth) {
						i->HitDetect(m);
					}
				}
				
				if (rat.size() > 0) {
					for each (Rat* r in rat) {
						i->HitDetect(r);
					}
				}
			}

			if (i->getType() == LOAD_ZONE)
			{
				((LoadZone*)i)->HitDetect(player);
			}

		}
	}

	if (player->getState() != PS_HURT) {

		if (!TheGamepad->IsPressed(XINPUT_GAMEPAD_X) && !GAMEPLAY_INPUT.key_space)
		{
			GAMEPLAY_INPUT.key_space_p = false;
		}
		//if (GAMEPLAY_INPUT.key_space && !GAMEPLAY_INPUT.key_space_p || (TheGamepad->IsPressed(XINPUT_GAMEPAD_X) && !GAMEPLAY_INPUT.key_space_p)) {

		if (!TheGamepad->IsPressed(XINPUT_GAMEPAD_X) && !GAMEPLAY_INPUT.key_space && TheGamepad->CheckConnection())
		{
			GAMEPLAY_INPUT.key_space_p = false;
		}
		if (GAMEPLAY_INPUT.key_space && !GAMEPLAY_INPUT.key_space_p || (TheGamepad->IsPressed(XINPUT_GAMEPAD_X) && !GAMEPLAY_INPUT.key_space_p) && TheGamepad->CheckConnection()) {

			player->Attack();
			GAMEPLAY_INPUT.key_space_p = true;
		}
	}

	for each (Block* platform in terrain)
	{
		player->DetectObstruction(platform);
		platform->HitDetect(player);
		if (knight != nullptr) {
			platform->HitDetect(knight);
		}

		if (bossKnight != nullptr) {
			platform->HitDetect(bossKnight);
		}

		if (moth.size() > 0) {
			for each (Moth* m in moth) {
				platform->HitDetect(m);
			}
		}

		if (rat.size() > 0) {
			for each (Rat* r in rat) {
				platform->HitDetect(r);
			}
		}
	}

	if (Pushables.size() > 0)
	{
		for each (Pushable* Push in Pushables)
		{
			player->DetectObstruction(Push);
			Push->HitDetect(player);
		}
	}

		
	if (ActualPlatforms.size() > 0)
	{
		for each (Platform* p in ActualPlatforms) {
			player->DetectObstruction(p);
			p->HitDetect(player);

			if (knight != nullptr)
			{
				p->HitDetect(knight);
			}

			if (moth.size() > 0) {
				for each (Moth* m in moth) {
					p->HitDetect(m);
				}
			}

			if (rat.size() > 0) {
				for each (Rat* r in rat) {
					p->HitDetect(r);
				}
			}
		}
	}

	if (torches.size() > 0)
	{
		for each (Torch* t in torches) {
			player->HitDetectEnem(t);
		}
	}

	if (knight != nullptr) {
		player->HitDetectEnem(knight);
	}

	if (moth.size() > 0)
	{
		for each (Moth* m in moth) {
			player->HitDetectEnem(m);
		}
	}

	if (rat.size() > 0)
	{
		for each (Rat* r in rat) {
			player->HitDetectEnem(r);
		}
	}

	if (player->getState() != PS_HURT) {
		if (ladders.size())
		{
			for each (Ladder* lad in ladders)
			{
				if ((lad->HitDetect(player) && player->getState() != PS_Climb)) {
					if ((GAMEPLAY_INPUT.key_up && !lad->PlayerOnTop()) || (TheGamepad->leftStickY >= 0.2 && player->getState() != PS_Climb)) {
						player->Climb(lad);
					}
					else if (GAMEPLAY_INPUT.key_down && lad->PlayerOnTop() || (TheGamepad->leftStickY <= -0.2 && player->getState() != PS_Climb)) {
						player->ClimbDown(lad);
					}
				}
			}
		}
	}

		if (!TheGamepad->IsPressed(XINPUT_GAMEPAD_B) && !GAMEPLAY_INPUT.key_crouch && TheGamepad->CheckConnection())
		{
			GAMEPLAY_INPUT.key_crouch_p = false;
		}
		if ((GAMEPLAY_INPUT.key_crouch && !GAMEPLAY_INPUT.key_crouch_p && player->isOnGround() && player->getSpeedY() == 0)
			|| (TheGamepad->IsPressed(XINPUT_GAMEPAD_B) && !GAMEPLAY_INPUT.key_crouch_p) && player->isOnGround() && player->getSpeedY() == 0 && TheGamepad->CheckConnection()) {
			if (player->getState() == PS_Stand) {
				player->Crouch();
			}
			else if (player->getState() == PS_Crouch) {
				player->Stand();
			}
			GAMEPLAY_INPUT.key_crouch_p = true;
		}
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

	if (bossKnight != nullptr) {
		bossKnight->Move();
	}

	if (moth.size() > 0) {
		for each (Moth* m in moth) {
			m->Move();
		}
	}

	if (rat.size() > 0) {
		for each (Rat* r in rat) {
			r->Move();
		}
	}

	if (knight != nullptr && player->getState() != PS_HURT) {
		if (knight->HitDetect(player)) {
			player->hurt(2);
		}
	}

	if (bossKnight != nullptr && player->getState() != PS_HURT) {
		bossKnight->HitDetect(player);
	}

	if (moth.size() > 0) {
		for each (Moth* m in moth) {
			if (m->HitDetect(player)) {
				m->Hit(player);
			}
		}
	}

	if (rat.size() > 0) {
		for each (Rat* r in rat) {
			if (r->HitDetect(player)) {
				r->Hit(player);
			}
		}
	}

	// Move Camera
	if (!cutScene) {
		//if (!cutSceneC) {
			if (player->getPositionX() >= (Director::getInstance()->getVisibleSize().width / 2) && player->getPositionX() <= STAGE_WIDTH - (Director::getInstance()->getVisibleSize().width / 2)) {
				view->setPositionX(player->getPositionX());
			}

			if (player->getPositionY() >= (Director::getInstance()->getVisibleSize().height / 3) && player->getPositionY() <= STAGE_HEIGHT - (Director::getInstance()->getVisibleSize().height * 2 / 3)) {
				view->setPositionY(player->getPositionY() + (Director::getInstance()->getVisibleSize().height / 6));
			}
		//}
	}
}

bool A1_R1::init()
{
	//STAGE_HEIGHT = 600;
	//STAGE_WIDTH = 1000;		Defaults

	STAGE_WIDTH = 900;

	//removeAllChildren();

	if (GameplayScene::init()) {

		//RAENA SOUNDSCAPE
		auto visibleSize = Director::getInstance()->getVisibleSize();
		//Center of screen ///////////////////////////////////////////////////////////////////////
		Vec2 origin = Director::getInstance()->getVisibleOrigin();

		//Parallax Stuff
		auto paraNode = ParallaxNode::create();
		PNode = paraNode;

		EffectSprite *_bgColor1 = EffectSprite::create("Parallax Background/BlueBG_Updated.png");
		EffectSprite *_bgColor2 = EffectSprite::create("Parallax Background/LightBlueBG_Updated.png");
		EffectSprite *_bgColor3 = EffectSprite::create("Parallax Background/DarkGreenBG_Updated.png");
		EffectSprite *_bgColor4 = EffectSprite::create("Parallax Background/GreenBG_Updated.png");
		EffectSprite *_bgColor5 = EffectSprite::create("Parallax Background/LightGreenBG_Updated.png");

		_bgColor1->setAnchorPoint(cocos2d::Vec2(0, 0));
		_bgColor1->setScale(1);

		_bgColor2->setAnchorPoint(cocos2d::Vec2(0, 0));
		_bgColor2->setScale(1);

		_bgColor3->setAnchorPoint(cocos2d::Vec2(0, 0));
		_bgColor3->setScale(1);

		_bgColor4->setAnchorPoint(cocos2d::Vec2(0, 0));
		_bgColor4->setScale(1);

		_bgColor5->setAnchorPoint(cocos2d::Vec2(0, 0));
		_bgColor5->setScale(1);

		
		paraNode->addChild(_bgColor3, 5, Vec2(0, 0), Vec2(100, 200));		 //Frontmost
		paraNode->addChild(_bgColor4, 4, Vec2(0.1f, 0), Vec2(-100, 200));
		paraNode->addChild(_bgColor5, 3, Vec2(0.2f, 0), Vec2(-100, 200));
		paraNode->addChild(_bgColor2, 2, Vec2(0.3f, 0), Vec2(-100, 200));
		paraNode->addChild(_bgColor1, 1, Vec2(0.4f, 0), Vec2(-100, 200));

		EffectSprite * tileSet = EffectSprite::create("Levels/A1_R1.png");
		tileSet->setAnchorPoint(Vec2(0, 0));

		this->addChild(tileSet, 5);

		this->addChild(paraNode);
		paraNode->setPosition(0,0);

		// Lighting Tests
		auto _effect = LightEffect::create();
		_effect->retain();

		_effect->setLightCutoffRadius(250);
		_effect->setLightHalfRadius(0.5);
		_effect->setBrightness(0.7);
		_effect->setAmbientLightColor(Color3B(255, 255, 255));  //255 = no shadow, 0 = black

		player->setEffect(_effect, "test_NM.png");
		
		player->switchLight();

		//Entities
		if (player != nullptr) {
			player->setPosition(Vec2(200,245));

			//this->addChild(player, 10);
		}
		else {
			return false;
		}
									  // x,y w,h
		terrain.pushBack(Block::create(0, 0, 600, 200)); //Ground

		terrain.pushBack(Block::create(503, 0, 600, 250)); //Ground

		terrain.pushBack(Block::create(0, 200, 100, 750)); //Tree

		for each (Entity* plat in terrain)
		{
			if (plat != nullptr) {
				this->addChild(plat,0);
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

		interactables.pushBack(Pickup::create("Rose.png", cocos2d::Vec2(240, 200), ROSE));
		//interactables.pushBack(SceneDoor::create("closed_door.png", Vec2(700,270),A1_R2)); // Scene Door
		interactables.pushBack(LoadZone::create(905,270,30,1000, A1_R2,cocos2d::Vec2(50,230))); // LoadZone
		

		for each (Interactable* inter in interactables) {
			if (inter != nullptr) {
				this->addChild(inter,7);
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
	
	if (!this->audioinitF)
	{
		TheAudioT->stopBackgroundMusic("RAENA SOUNDSCAPE/Music/RaenaTitle.wav");
		TheAudioF->preloadBackgroundMusic("RAENA SOUNDSCAPE/Music/Forest Waltz.mp3");
		TheAudioF->setBackgroundMusicVolume(0.5);
		TheAudioF->playBackgroundMusic("RAENA SOUNDSCAPE/Music/Forest Waltz.mp3", true);

	}
	audioinitF = true;

	GameplayScene::update(dt);
}


// A1_R2

bool A1_R2::init()
{
	//STAGE_HEIGHT = 600;
	//STAGE_WIDTH = 1000;		Defaults

	STAGE_HEIGHT = 900;

	//removeAllChildren();

	if (GameplayScene::init()) {

		auto visibleSize = Director::getInstance()->getVisibleSize();
		//Center of screen ///////////////////////////////////////////////////////////////////////
		Vec2 origin = Director::getInstance()->getVisibleOrigin();

		//Parallax Stuff
		auto paraNode = ParallaxNode::create();
		PNode = paraNode;
		
		EffectSprite *_bgColor1 = EffectSprite::create("Parallax Background/BlueBG_Updated.png");
		EffectSprite *_bgColor2 = EffectSprite::create("Parallax Background/LightBlueBG_Updated.png");
		EffectSprite *_bgColor3 = EffectSprite::create("Parallax Background/DarkGreenBG_Updated.png");
		EffectSprite *_bgColor4 = EffectSprite::create("Parallax Background/GreenBG_Updated.png");
		EffectSprite *_bgColor5 = EffectSprite::create("Parallax Background/LightGreenBG_Updated.png");

		_bgColor1->setAnchorPoint(cocos2d::Vec2(0, 0));
		_bgColor1->setScale(1.5);

		_bgColor2->setAnchorPoint(cocos2d::Vec2(0, 0));
		_bgColor2->setScale(1.5);

		_bgColor3->setAnchorPoint(cocos2d::Vec2(0, 0));
		_bgColor3->setScale(1.5);

		_bgColor4->setAnchorPoint(cocos2d::Vec2(0, 0));
		_bgColor4->setScale(1.5);

		_bgColor5->setAnchorPoint(cocos2d::Vec2(0, 0));
		_bgColor5->setScale(1.5);

		paraNode->addChild(_bgColor3, 5, Vec2(0, 0), Vec2(-300, 200));		 //Frontmost
		paraNode->addChild(_bgColor4, 4, Vec2(0.1f, 0), Vec2(-150, 200));
		paraNode->addChild(_bgColor5, 3, Vec2(0.2f, 0), Vec2(-150, 200));
		paraNode->addChild(_bgColor2, 2, Vec2(0.3f, 0), Vec2(-150, 200));
		paraNode->addChild(_bgColor1, 1, Vec2(0.4f, 0), Vec2(-150, 200));

		EffectSprite * tileSet = EffectSprite::create("Levels/A1_R2.png");
		tileSet->setAnchorPoint(Vec2(0, 0));
		
		this->addChild(tileSet, 4);


		this->addChild(paraNode);

		// Lighting Tests
		auto _effect = LightEffect::create();
		_effect->retain();

		_effect->setLightCutoffRadius(250);
		_effect->setLightHalfRadius(0.5);
		_effect->setBrightness(0.7);
		_effect->setAmbientLightColor(Color3B(255, 255, 255));

		player->setEffect(_effect, "test_NM.png");

		player->switchLight();

		//Entities
		if (player != nullptr) {
			//player->setPosition(Vec2(50,245));

			//this->addChild(player, 10);
		}
		else {
			return false;
		}


		// x,y w,h
		terrain.pushBack(Block::create(0, 0, 600, 199)); //Ground
		terrain.pushBack(Block::create(603, 0, 400,695)); //Ground
		terrain.pushBack(Block::create(253, 205, 45, 43)); //Rock
		
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

		interactables.pushBack(LoadZone::create(-10, 205, 10, 1000, A1_R1, Vec2(800, 280))); // LoadZone
		interactables.pushBack(LoadZone::create(1000, 670, 10, 1000, A1_R3, Vec2(50, 248))); // LoadZone

		for each (Interactable* inter in interactables) {
			if (inter != nullptr) {
				this->addChild(inter);
			}
			else {
				return false;
			}
		}

		//Platforms
		ActualPlatforms.pushBack(Platform::create("terrain/leafy_platform.png", cocos2d::Vec2(400,300)));

		ActualPlatforms.pushBack(Platform::create("terrain/leafy_platform.png", cocos2d::Vec2(290, 390)));

		ActualPlatforms.pushBack(Platform::create("terrain/leafy_platform.png", cocos2d::Vec2(450, 430)));

		ActualPlatforms.pushBack(Platform::create("terrain/leafy_platform.png", cocos2d::Vec2(340, 520)));

		ActualPlatforms.pushBack(Platform::create("terrain/leafy_platform.png", cocos2d::Vec2(240, 580)));

		ActualPlatforms.pushBack(Platform::create("terrain/leafy_platform.png", cocos2d::Vec2(500, 600)));

		for each (Platform* p in ActualPlatforms) {
			if (p != nullptr) {
		
				/*p->setScale(SCALE);
				p->getTexture()->setTexParameters(tp);*/
		
				this->addChild(p,6);
			}
		}

		// RAT STUFF

		//rat.pushBack(Rat::create("Rat Death Animation/Rat_Death_Animation1.png", ActualPlatforms.at(3)));
		//rat.at(0)->setPosition(ActualPlatforms.at(3)->getPositionX(), ActualPlatforms.at(3)->getPositionY() + 32);
		//
		//for each (Rat* r in rat) {
		//	if (r != nullptr) {
		//
		//		this->addChild(r);
		//
		//	}
		//}


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

	removeAllChildren();

	

	if (GameplayScene::init()) {


		auto visibleSize = Director::getInstance()->getVisibleSize();
		//Center of screen ///////////////////////////////////////////////////////////////////////
		Vec2 origin = Director::getInstance()->getVisibleOrigin();

		//Parallax Stuff
		auto paraNode = ParallaxNode::create();
		PNode = paraNode;

		EffectSprite *_bgColor1 = EffectSprite::create("Parallax Background/BlueBG_Updated.png");
		EffectSprite *_bgColor2 = EffectSprite::create("Parallax Background/LightBlueBG_Updated.png");
		EffectSprite *_bgColor3 = EffectSprite::create("Parallax Background/DarkGreenBG_Updated.png");
		EffectSprite *_bgColor4 = EffectSprite::create("Parallax Background/GreenBG_Updated.png");
		EffectSprite *_bgColor5 = EffectSprite::create("Parallax Background/LightGreenBG_Updated.png");

		_bgColor1->setAnchorPoint(cocos2d::Vec2(0, 0));
		_bgColor1->setScale(1.2);

		_bgColor2->setAnchorPoint(cocos2d::Vec2(0, 0));
		_bgColor2->setScale(1.2);

		_bgColor3->setAnchorPoint(cocos2d::Vec2(0, 0));
		_bgColor3->setScale(1.2);

		_bgColor4->setAnchorPoint(cocos2d::Vec2(0, 0));
		_bgColor4->setScale(1.2);

		_bgColor5->setAnchorPoint(cocos2d::Vec2(0, 0));
		_bgColor5->setScale(1.2);

		paraNode->addChild(_bgColor3, 5, Vec2(0, 0), Vec2(-100, 100));		 //Frontmost
		paraNode->addChild(_bgColor4, 4, Vec2(0.1f, 0), Vec2(-150, 100));
		paraNode->addChild(_bgColor5, 3, Vec2(0.2f, 0), Vec2(-150, 100));
		paraNode->addChild(_bgColor2, 2, Vec2(0.3f, 0), Vec2(-150, 100));
		paraNode->addChild(_bgColor1, 1, Vec2(0.4f, 0), Vec2(-150, 100));

		EffectSprite * tileSet = EffectSprite::create("Levels/A1_R3.png");
		tileSet->setAnchorPoint(Vec2(0, 0));

		this->addChild(tileSet, 4);

		this->addChild(paraNode);

		// Lighting Tests
		auto _effect = LightEffect::create();
		_effect->retain();

		_effect->setLightCutoffRadius(250);
		_effect->setLightHalfRadius(0.5);
		_effect->setBrightness(0.7);
		_effect->setAmbientLightColor(Color3B(255, 255, 255));

		player->setEffect(_effect, "test_NM.png");
		//_bgColor->setEffect(_effect, "test_NM.png");

		player->switchLight();

		//Entities
		if (player != nullptr) {
			player->setPosition(Vec2(50,300));

			//this->addChild(player, 10);
		}
		else {
			return false;
		}


		// x,y w,h
		terrain.pushBack(Block::create(0, 0, 300, 200)); //Ground

		terrain.pushBack(Block::create(300, 0, 400, 100)); //Ground

		terrain.pushBack(Block::create(703, 0, 400, 250)); //Ground

		terrain.pushBack(Block::create(1103, 0, 120, 500)); //Crouch block

		terrain.pushBack(Block::create(300, 0, 50, 150)); //Ground	*stopper block

		terrain.pushBack(Block::create(803, 290, 44, 400)); //Floating
		terrain.pushBack(Block::create(803, 503, 420, 400)); //Roof

		

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
		//Pushables.pushBack(Pushable::create(470,140,80,80, Vec2(400,140),Vec2(660,140)));
		Pushables.pushBack(Pushable::create("PushableRock.png", Vec2(470, 140), Vec2(400, 140), Vec2(660, 140) ) );

		for each (Pushable* push in Pushables) {
			if (push != nullptr) {
				this->addChild(push);
			}
			else
				return false;
			
		}

		interactables.pushBack(LoadZone::create(-10, 205, 10, 900, A1_R2, Vec2(900, 735))); // LoadZone
		interactables.pushBack(SceneDoor::create("terrain/inner_cave_door.png", Vec2(1000, 250), Vec2(70,600), A1_R4));	//SceneDoor

		for each (Interactable* inter in interactables) {
			if (inter != nullptr) {
				this->addChild(inter,5);
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

	removeAllChildren();

	

	if (GameplayScene::init()) {


		auto visibleSize = Director::getInstance()->getVisibleSize();
		//Center of screen ///////////////////////////////////////////////////////////////////////
		Vec2 origin = Director::getInstance()->getVisibleOrigin();

		//Parallax Stuff
		auto paraNode = ParallaxNode::create();
		PNode = paraNode;

		EffectSprite *_bgColor1 = EffectSprite::create("Parallax Background/BlueBG_Updated.png");
		EffectSprite *_bgColor2 = EffectSprite::create("Parallax Background/LightBlueBG_Updated.png");
		EffectSprite *_bgColor3 = EffectSprite::create("Parallax Background/DarkGreenBG_Updated.png");
		EffectSprite *_bgColor4 = EffectSprite::create("Parallax Background/GreenBG_Updated.png");
		EffectSprite *_bgColor5 = EffectSprite::create("Parallax Background/LightGreenBG_Updated.png");

		_bgColor1->setAnchorPoint(cocos2d::Vec2(0, 0));
		_bgColor1->setScale(2);

		_bgColor2->setAnchorPoint(cocos2d::Vec2(0, 0));
		_bgColor2->setScale(2);

		_bgColor3->setAnchorPoint(cocos2d::Vec2(0, 0));
		_bgColor3->setScale(2);

		_bgColor4->setAnchorPoint(cocos2d::Vec2(0, 0));
		_bgColor4->setScale(2);

		_bgColor5->setAnchorPoint(cocos2d::Vec2(0, 0));
		_bgColor5->setScale(2);


		paraNode->addChild(_bgColor3, 5, Vec2(0, 0), Vec2(-100, 100));		 //Frontmost
		paraNode->addChild(_bgColor4, 4, Vec2(0.1f, 0), Vec2(-150, 100));
		paraNode->addChild(_bgColor5, 3, Vec2(0.2f, 0), Vec2(-150, 100));
		paraNode->addChild(_bgColor2, 2, Vec2(0.3f, 0), Vec2(-150, 100));
		paraNode->addChild(_bgColor1, 1, Vec2(0.4f, 0), Vec2(-150, 100));

		EffectSprite * tileSet = EffectSprite::create("Levels/A1_R4.png");
		tileSet->setAnchorPoint(Vec2(0, 0));

		this->addChild(tileSet, 4);

		this->addChild(paraNode);

		// Lighting Tests
		auto _effect = LightEffect::create();
		_effect->retain();

		_effect->setLightCutoffRadius(250);
		_effect->setLightHalfRadius(0.5);
		_effect->setBrightness(0.7);
		_effect->setAmbientLightColor(Color3B(255, 255, 255));

		player->setEffect(_effect, "test_NM.png");

		player->switchLight();

		//Entities
		if (player != nullptr) {
			player->setPosition(Vec2(150,575));

			//this->addChild(player, 10);
		}
		else {
			return false;
		}

		// x,y w,h
		terrain.pushBack(Block::create(0, 0, 1000, 150)); //Ground
		terrain.pushBack(Block::create(275, 900, 450, 10)); //Cieling

		terrain.pushBack(Block::create(0, 150, 296, 400)); //Ground 1

		terrain.pushBack(Block::create(-100, 150, 100, 1000)); //Left Barrier


		terrain.pushBack(Block::create(0, 700, 195, 300)); //Mountain

		terrain.pushBack(Block::create(753, 150, 500, 300)); //Ground 2

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
		ladders.pushBack(Ladder::create(365, 200, 32, 900));

		ladders.pushBack(Ladder::create(600, 400, 32, 500));

		for each (Ladder* lad in ladders) {
			if (lad != nullptr) {
				this->addChild(lad);
			}
			else {
				return false;
			}
		}

	
		interactables.pushBack(LoadZone::create(1000, 300, 10, 900, A1_R5, Vec2(50, 245))); // LoadZone
		interactables.pushBack(SceneDoor::create("terrain/outer_cave_door.png", Vec2(50, 550), Vec2(980,280), A1_R3));	//SceneDoor

		for each (Interactable* inter in interactables) {
			if (inter != nullptr) {
				this->addChild(inter,5);
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
		view->setPositionY(view->getPositionY() + 200);
		

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

	STAGE_HEIGHT = 1200;
	STAGE_WIDTH = 1600;

	removeAllChildren();



	if (GameplayScene::init()) {


		auto visibleSize = Director::getInstance()->getVisibleSize();
		//Center of screen ///////////////////////////////////////////////////////////////////////
		Vec2 origin = Director::getInstance()->getVisibleOrigin();

		//Parallax Stuff
		auto paraNode = ParallaxNode::create();
		PNode = paraNode;
		EffectSprite *_bgColor1 = EffectSprite::create("Parallax Background/BlueBG_Updated.png");
		EffectSprite *_bgColor2 = EffectSprite::create("Parallax Background/LightBlueBG_Updated.png");
		EffectSprite *_bgColor3 = EffectSprite::create("Parallax Background/DarkGreenBG_Updated.png");
		EffectSprite *_bgColor4 = EffectSprite::create("Parallax Background/GreenBG_Updated.png");
		EffectSprite *_bgColor5 = EffectSprite::create("Parallax Background/LightGreenBG_Updated.png");

		_bgColor1->setAnchorPoint(cocos2d::Vec2(0, 0));
		_bgColor1->setScale(2.5);

		_bgColor2->setAnchorPoint(cocos2d::Vec2(0, 0));
		_bgColor2->setScale(2.5);

		_bgColor3->setAnchorPoint(cocos2d::Vec2(0, 0));
		_bgColor3->setScale(2.5);

		_bgColor4->setAnchorPoint(cocos2d::Vec2(0, 0));
		_bgColor4->setScale(2.5);

		_bgColor5->setAnchorPoint(cocos2d::Vec2(0, 0));
		_bgColor5->setScale(2.5);


		paraNode->addChild(_bgColor3, 5, Vec2(0, 0), Vec2(-100, 90));		 //Frontmost
		paraNode->addChild(_bgColor4, 4, Vec2(0.1f, 0), Vec2(-150, 90));
		paraNode->addChild(_bgColor5, 3, Vec2(0.2f, 0), Vec2(-150, 90));
		paraNode->addChild(_bgColor2, 2, Vec2(0.3f, 0), Vec2(-150, 90));
		paraNode->addChild(_bgColor1, 1, Vec2(0.4f, 0), Vec2(-150, 90));

		EffectSprite * tileSet = EffectSprite::create("Levels/A1_R5.png");
		tileSet->setAnchorPoint(Vec2(0, 0));

		this->addChild(tileSet, 4);

		this->addChild(paraNode);

		// Lighting Tests
		auto _effect = LightEffect::create();
		_effect->retain();

		_effect->setLightCutoffRadius(250);
		_effect->setLightHalfRadius(0.5);
		_effect->setBrightness(0.7);
		_effect->setAmbientLightColor(Color3B(255, 255, 255));

		player->setEffect(_effect, "test_NM.png");
		//_bgColor->setEffect(_effect, "test_NM.png");

		player->switchLight();

		//Entities
		if (player != nullptr) {
			player->setPosition(Vec2(50,250));

			//this->addChild(player, 10);
		}
		else {
			return false;
		}


		// x,y w,h
		terrain.pushBack(Block::create(0, 0, 1600, 93)); //Base Ground

		terrain.pushBack(Block::create(-100, 93, 247, 100)); //Ground 1
		terrain.pushBack(Block::create(150, 93, 97, 50)); //Ground 2

		terrain.pushBack(Block::create(653, 90, 60, 53)); //Ground 2

		terrain.pushBack(Block::create(703, 225, 244, 198)); //floating 3
		terrain.pushBack(Block::create(703, 100, 293, 93)); //Ground 3
		

		terrain.pushBack(Block::create(1403, 100, 290, 442)); //Ground 4

		//Projectiles.pushBack(Projectile::create(100, 45, 0, 100, 500, this));

		for each (Entity* plat in terrain)
		{
			if (plat != nullptr) {
				this->addChild(plat);
			}
			else {
				return false;
			}
		}

		//Pushables.pushBack(Pushable::create(558, 133, 80, 80, Vec2(289, 133), Vec2(600, 133)));
		Pushables.pushBack(Pushable::create("PushableRock.png", cocos2d::Vec2(558, 133), cocos2d::Vec2(289, 133), cocos2d::Vec2(610, 133)));


		for each (Pushable* push in Pushables) {
			if (push != nullptr) {
				this->addChild(push);
			}
			else
				return false;

		}

		//x y w h
		ladders.pushBack(Ladder::create(390, 290, 32, 320));

		//ladders.pushBack(Ladder::create(600, 400, 32, 500));
		
		for each (Ladder* lad in ladders) {
			if (lad != nullptr) {
				this->addChild(lad);
			}
			else {
				return false;
			}
		}

		interactables.pushBack(LoadZone::create(-10, 165, 10, 1000, A1_R4, Vec2(940, 495))); // LoadZone
		interactables.pushBack(LoadZone::create(1620, 550, 10, 1000, A1_R6, Vec2(50, 250))); // LoadZone

		for each (Interactable* inter in interactables) {
			if (inter != nullptr) {
				this->addChild(inter);
			}
			else {
				return false;
			}
		}

		//Platforms
		ActualPlatforms.pushBack(Platform::create("terrain/leafy_platform.png", cocos2d::Vec2(400, 590.6)));

		ActualPlatforms.pushBack(Platform::create("terrain/leafy_platform.png", cocos2d::Vec2(560, 645)));

		ActualPlatforms.pushBack(Platform::create("terrain/leafy_platform.png", cocos2d::Vec2(360, 720)));

		ActualPlatforms.pushBack(Platform::create("terrain/leafy_platform.png", cocos2d::Vec2(590, 770)));

		ActualPlatforms.pushBack(Platform::create("terrain/leafy_platform.png", cocos2d::Vec2(975, 660)));

		ActualPlatforms.pushBack(Platform::create("terrain/leafy_platform.png", cocos2d::Vec2(1175, 520)));


		for each (Platform* p in ActualPlatforms) {
			if (p != nullptr) {
		
				/*p->setScale(SCALE);
				p->getTexture()->setTexParameters(tp);*/
		
				this->addChild(p,6);
			}
		}





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

bool A1_R6::init()	//Pushable And Crouch Tutorial
{
	//STAGE_HEIGHT = 600;
	//STAGE_WIDTH = 1000;		Defaults

	STAGE_HEIGHT = 650;
	STAGE_WIDTH = 1200;

	removeAllChildren();

	

	if (GameplayScene::init()) {


		auto visibleSize = Director::getInstance()->getVisibleSize();
		//Center of screen ///////////////////////////////////////////////////////////////////////
		Vec2 origin = Director::getInstance()->getVisibleOrigin();

		//Parallax Stuff
		auto paraNode = ParallaxNode::create();
		PNode = paraNode;
		EffectSprite *_bgColor1 = EffectSprite::create("Parallax Background/Flower_BlueBG.png");
		EffectSprite *_bgColor2 = EffectSprite::create("Parallax Background/Flower_LightBlueBG.png");
		EffectSprite *_bgColor3 = EffectSprite::create("Parallax Background/Flower_DarkGreenBG.png");
		EffectSprite *_bgColor4 = EffectSprite::create("Parallax Background/Flower_GreenBG.png");
		EffectSprite *_bgColor5 = EffectSprite::create("Parallax Background/Flower_LightGreenBG.png");
		EffectSprite *_bgColor6 = EffectSprite::create("Parallax Background/Flower_TowerBG.png");


		_bgColor1->setAnchorPoint(cocos2d::Vec2(0, 0));
		_bgColor1->setScale(1);

		_bgColor2->setAnchorPoint(cocos2d::Vec2(0, 0));
		_bgColor2->setScale(1);

		_bgColor3->setAnchorPoint(cocos2d::Vec2(0, 0));
		_bgColor3->setScale(1);

		_bgColor4->setAnchorPoint(cocos2d::Vec2(0, 0));
		_bgColor4->setScale(1);

		_bgColor5->setAnchorPoint(cocos2d::Vec2(0, 0));	
		_bgColor5->setScale(1);

		_bgColor6->setAnchorPoint(cocos2d::Vec2(0, 0));
		_bgColor6->setScale(1);

		paraNode->addChild(_bgColor3, 6, Vec2(0, 0), Vec2(-100, 165));		 //Frontmost
		paraNode->addChild(_bgColor4, 5, Vec2(0.1f, 0), Vec2(-150, 180));
		paraNode->addChild(_bgColor5, 4, Vec2(0.2f, 0), Vec2(-150, 180));
		paraNode->addChild(_bgColor2, 1, Vec2(0.3f, 0), Vec2(-150, 180));
		paraNode->addChild(_bgColor1, 2, Vec2(0.4f, 0), Vec2(-150, 180));
		paraNode->addChild(_bgColor6, 3, Vec2(0.3f, 0), Vec2(-280, 180));



		EffectSprite * tileSet = EffectSprite::create("Levels/A1_R6.png");
		tileSet->setAnchorPoint(Vec2(0, 0));

		this->addChild(tileSet, 4);

		this->addChild(paraNode);

		// Lighting Tests
		auto _effect = LightEffect::create();
		_effect->retain();

		_effect->setLightCutoffRadius(250);
		_effect->setLightHalfRadius(0.5);
		_effect->setBrightness(0.7);												
		_effect->setAmbientLightColor(Color3B(255, 255, 255));

		player->setEffect(_effect, "test_NM.png");
		
		player->switchLight();

		//Entities
		if (player != nullptr) {
			player->setPosition(Vec2((visibleSize.width / 2) - player->getBoundingBox().size.width / 2 + origin.x, (visibleSize.height / 2) - player->getBoundingBox().size.height / 2 + origin.y));

			//this->addChild(player, 10);
		}
		else {
			return false;
		}

		//moth = Moth::create("Mothboi.png");
		//
		//if (moth != nullptr) {
		//	moth->setPosition(Vec2((visibleSize.width / 2) - player->getBoundingBox().size.width / 2 + origin.x, (visibleSize.height / 2) - player->getBoundingBox().size.height / 2 + origin.y));
		//	moth->setPosition(600, 250);
		//
		//	this->addChild(moth, 10);
		//}
		//else {
		//	return false;
		//}

		//rat.pushBack(Rat::create("test_dummy_2.png")); //Rat Test
		////Push_back
		//for each (Rat* r in rat) {
		//	if (r != nullptr) {
		//		r->setPosition(Vec2((visibleSize.width / 2) - player->getBoundingBox().size.width / 2 + origin.x, (visibleSize.height / 2) - player->getBoundingBox().size.height / 2 + origin.y));
		//		r->setPosition(600, 250);
		//		this->addChild(r);
		//	}
		//	else {
		//		return false;
		//	}
		//}

		// x,y w,h
		terrain.pushBack(Block::create(0, 0, 1400, 200)); //Ground
		terrain.pushBack(Block::create(1402, 0, 1500, 650)); //wall


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

		//for each (Pushable* push in Pushables) {
		//	if (push != nullptr) {
		//		this->addChild(push);
		//	}
		//	else
		//		return false;
		//
		//}

		interactables.pushBack(LoadZone::create(-10, 200, 10, 400, A1_R5, Vec2(1500, 605))); // LoadZone
		//interactables.pushBack(SceneDoor::create("closed_door.png", Vec2(1050, 275), Vec2(50, 200), A1_R4));	//SceneDoor
		interactables.pushBack(PuzzleInteract::create("Princess_Standing.png", Vec2(1275, 200),Princess1, ROSE));
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

void A1_R6::update(float dt)
{
	GameplayScene::update(dt);


	if (player->getPosition().x >= 800 && !cutSceneC) {
		cutScene = true;
		GAMEPLAY_INPUT.key_left = false;
		GAMEPLAY_INPUT.key_up = false;
		GAMEPLAY_INPUT.key_right = false;
		GAMEPLAY_INPUT.key_down = false;
		GAMEPLAY_INPUT.key_space = false;
		GAMEPLAY_INPUT.key_space_p = false;
		GAMEPLAY_INPUT.key_jump = false;
		GAMEPLAY_INPUT.key_jump_p = false;
		GAMEPLAY_INPUT.key_interact = false;
		GAMEPLAY_INPUT.key_crouch = false;
		GAMEPLAY_INPUT.key_crouch_p = false;
	}

	if (view->getPositionX() <= 1000 && cutScene) {
		view->setPositionX(view->getPositionX() + 75 * dt);
	}
	else if (view->getPositionX() >= 1000 && !cutSceneC) {
		cutScene = false;
		cutSceneC = true;
	}

	if (player->getHP() <= 0) {
		cocos2d::Director::getInstance()->replaceScene(LevelManager::GetLevel(A2_R1));
		player->setHP(6);
	}
}

bool A2_R1::init()
{

	//STAGE_HEIGHT = 600;
	//STAGE_WIDTH = 1000;		Defaults

	STAGE_HEIGHT = 650;
	STAGE_WIDTH = 1700;

	removeAllChildren();


	if (GameplayScene::init()) {

		//Parallax & Background //////////////////////////////////////////////////////////////////////////////
		auto paraNode = ParallaxNode::create();
		PNode = paraNode;
		EffectSprite *_bgColor = EffectSprite::create("BGP1.png");

		_bgColor->setScale(1);

		paraNode->addChild(_bgColor, 1, Vec2(0.4f, 0.5f), Vec2::ZERO);

		EffectSprite *_bgColor2 = EffectSprite::create("BGP2.png");

		_bgColor2->setScale(1);
		paraNode->addChild(_bgColor2, -1, Vec2(1.4f, 1.5f), Vec2::ZERO);

		this->addChild(paraNode);

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
		_effect->setAmbientLightColor(Color3B(0, 0, 0));

		player->setEffect(_effect, "layerNorm.png");
		_bgColor->setEffect(_effect, "layerNorm.png");
		_bgColor2->setEffect(_effect, "layerNorm.png");

		auto visibleSize = Director::getInstance()->getVisibleSize();
		//Center of screen ///////////////////////////////////////////////////////////////////////
		Vec2 origin = Director::getInstance()->getVisibleOrigin();

		//Player Loc /////////////////////////////////////////////////////////////////////////
		if (player != nullptr) {
			player->setPosition(Vec2((visibleSize.width / 2) - player->getBoundingBox().size.width / 2 + origin.x, (visibleSize.height / 2) - player->getBoundingBox().size.height / 2 + origin.y));

			//this->addChild(player, 10);
		}
		else {
			return false;
		}

		//platforms ////////////////////////////////////////////////////////////////////////////////
		terrain.pushBack(Block::create(0, 0, 64, 700));		//Left Boundary

		terrain.pushBack(Block::create(0, 0, 1800, 100));	//Base ground

		terrain.pushBack(Block::create(500, 600, 1000, 200));	//Roof
		terrain.pushBack(Block::create(-50, 600, 300, 200));	//Roof

		terrain.pushBack(Block::create(200, 100, 450, 50));	//Meaty Pile 1
		terrain.pushBack(Block::create(300, 150, 175, 50));	//Meaty Pile 1

		terrain.pushBack(Block::create(650, 100, 450, 25));	//Meaty Pile 2

		terrain.pushBack(Block::create(1100, 100, 300, 75));	//Meaty Pile 3
		terrain.pushBack(Block::create(1400, 100, 100, 125));	//Meaty Pile 3
		terrain.pushBack(Block::create(1500, 100, 400, 175));	//Meaty Pile 3

		terrain.pushBack(Block::create(1675, 100, 50, 800));	//Right Boundary

		//terrain.pushBack(Block::create(500, 200, 300, 75));
		//terrain.pushBack(Block::create(280, 350, 180, 10));
		for each (Entity* plat in terrain)
		{
			if (plat != nullptr) {
				plat->setEffect(_effect, "layerNorm.png");
				this->addChild(plat);
			}
			else {
				return false;
			}
		}
		//ladders
		ladders.pushBack(Ladder::create(1600, 275, 32, 600));

		for each (Ladder* lad in ladders) {
			if (lad != nullptr) {
				this->addChild(lad);
			}
			else {
				return false;
			}
		}

		//Interactables /////////////////////////////////////////////////////////////////////////////////////////
		interactables.pushBack(LoadZone::create(1500, 650, 500, 300, A2_R2, Vec2(500, 130)));
		for each (Interactable* inter in interactables) {
			if (inter != nullptr) {
				this->addChild(inter);
			}
			else {
				return false;
			}
		}
		//Actual Platforms ///////////////////////////////////////////////////////////////////////////////////////////


		//actualPlatforms.pushBack(Platform::create("Platform1.png", cocos2d::Vec2(100, 280)));
		for each (Platform* p in ActualPlatforms) {
			if (p != nullptr) {

				/*p->setScale(SCALE);
				p->getTexture()->setTexParameters(tp);*/
				p->setEffect(_effect, "layerNorm.png");
				this->addChild(p);
			}
			else
				return false;
		}



		//Set Torches;
		torches.pushBack(Torch::create(cocos2d::Vec2(420, 250), _effect));
		
		torches.pushBack(Torch::create(cocos2d::Vec2(900, 170), _effect));
		torches.pushBack(Torch::create(cocos2d::Vec2(1275, 225), _effect));

	


		for each (Torch* t in torches)
		{
			if (t != nullptr) {
				addChild(t, 2);
			}
			else {
				return false;
			}
		}

		moth.pushBack(Moth::create("MothBoi.png", &torches));
		moth.at(0)->setPosition(cocos2d::Vec2(850, 200 + (moth.at(0)->getBoundingBox().size.height / 2)));
		moth.at(0)->setPosition(500, 300);

		for each (Moth* m in moth) {
			if (m != nullptr) {

				this->addChild(m);

			}
		}

		player->switchLight();
		view = this->getDefaultCamera();

		this->scheduleUpdate();
		return true;
	}

	return false;
}

void A2_R1::update(float dt)
{
	GameplayScene::update(dt);
}

bool A2_R2::init()
{

	//STAGE_HEIGHT = 600;
	//STAGE_WIDTH = 1000;		Defaults

	STAGE_HEIGHT = 650;
	STAGE_WIDTH = 1600;

	removeAllChildren();


	if (GameplayScene::init()) {

		//Parallax & Background //////////////////////////////////////////////////////////////////////////////
		auto paraNode = ParallaxNode::create();
		PNode = paraNode;
		EffectSprite *_bgColor = EffectSprite::create("BGP1.png");

		_bgColor->setScale(1);

		paraNode->addChild(_bgColor, 1, Vec2(0.4f, 0.5f), Vec2::ZERO);

		EffectSprite *_bgColor2 = EffectSprite::create("BGP2.png");

		_bgColor2->setScale(1);
		paraNode->addChild(_bgColor2, -1, Vec2(1.4f, 1.5f), Vec2::ZERO);

		this->addChild(paraNode);

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
		_effect->setAmbientLightColor(Color3B(0, 0, 0));

		player->setEffect(_effect, "layerNorm.png");
		_bgColor->setEffect(_effect, "layerNorm.png");
		_bgColor2->setEffect(_effect, "layerNorm.png");

		auto visibleSize = Director::getInstance()->getVisibleSize();
		//Center of screen ///////////////////////////////////////////////////////////////////////
		Vec2 origin = Director::getInstance()->getVisibleOrigin();

		//Player Loc /////////////////////////////////////////////////////////////////////////
		if (player != nullptr) {
			player->setPosition(Vec2((visibleSize.width / 2) - player->getBoundingBox().size.width / 2 + origin.x, (visibleSize.height / 2) - player->getBoundingBox().size.height / 2 + origin.y));

			//this->addChild(player, 10);
		}
		else {
			return false;
		}

		//platforms ////////////////////////////////////////////////////////////////////////////////
		terrain.pushBack(Block::create(0, 0, 64, 700));			//Left Boundary

		terrain.pushBack(Block::create(0, 0, 770, 100));		//Base ground Left
		
		terrain.pushBack(Block::create(770, 68, 100, 32));		//Middle Block

		terrain.pushBack(Block::create(830, 0, 770, 100));		//Base ground Right

		terrain.pushBack(Block::create(1570, 0, 64, 700));		//Left Boundary

		//terrain.pushBack(Block::create(500, 200, 300, 75));
		//terrain.pushBack(Block::create(280, 350, 180, 10));
		for each (Entity* plat in terrain)
		{
			if (plat != nullptr) {
				plat->setEffect(_effect, "layerNorm.png");
				this->addChild(plat);
			}
			else {
				return false;
			}
		}
		//ladders

		//ladders.pushBack(Ladder::create(1600, 275, 32, 600));

		for each (Ladder* lad in ladders) {
			if (lad != nullptr) {
				this->addChild(lad);
			}
			else {
				return false;
			}
		}

		//Interactables /////////////////////////////////////////////////////////////////////////////////////////
		//interactables.pushBack(LoadZone::create(1500, 650, 500, 300, A2_R2, Vec2(50, 205)));
		interactables.pushBack(SceneDoor::create("closed_door.png", Vec2(400, 100), Vec2(400, 140), A2_R3));	//SceneDoor
		interactables.pushBack(SceneDoor::create("closed_door.png", Vec2(1270, 100), Vec2(400, 140), A2_R4));

		for each (Interactable* inter in interactables) {
			if (inter != nullptr) {
				this->addChild(inter);
			}
			else {
				return false;
			}
		}
		//Actual Platforms ///////////////////////////////////////////////////////////////////////////////////////////


		//ActualPlatforms.pushBack(Platform::create("terrain/Wooden_Platform.png", cocos2d::Vec2(800,85)));
		for each (Platform* p in ActualPlatforms) {
			if (p != nullptr) {

				/*p->setScale(SCALE);
				p->getTexture()->setTexParameters(tp);*/
				p->setEffect(_effect, "layerNorm.png");
				this->addChild(p);
			}
			else
				return false;
		}

		//Set Torches;
		torches.pushBack(Torch::create(cocos2d::Vec2(180, 150), _effect));
		torches.pushBack(Torch::create(cocos2d::Vec2(650, 150), _effect));
		torches.pushBack(Torch::create(cocos2d::Vec2(950, 150), _effect));
		torches.pushBack(Torch::create(cocos2d::Vec2(1454, 150), _effect));

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

void A2_R2::update(float dt)
{
	GameplayScene::update(dt);
}

bool A2_R3::init()
{
	//STAGE_HEIGHT = 600;
	//STAGE_WIDTH = 1000;		Defaults

	STAGE_HEIGHT = 600;
	STAGE_WIDTH = 800;

	removeAllChildren();


	if (GameplayScene::init()) {

		//Parallax & Background //////////////////////////////////////////////////////////////////////////////
		auto paraNode = ParallaxNode::create();
		PNode = paraNode;
		EffectSprite *_bgColor = EffectSprite::create("BGP1.png");

		_bgColor->setScale(1);

		paraNode->addChild(_bgColor, 1, Vec2(0.4f, 0.5f), Vec2::ZERO);

		EffectSprite *_bgColor2 = EffectSprite::create("BGP2.png");

		_bgColor2->setScale(1);
		paraNode->addChild(_bgColor2, -1, Vec2(1.4f, 1.5f), Vec2::ZERO);

		this->addChild(paraNode);

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
		_effect->setAmbientLightColor(Color3B(0, 0, 0));

		player->setEffect(_effect, "layerNorm.png");
		_bgColor->setEffect(_effect, "layerNorm.png");
		_bgColor2->setEffect(_effect, "layerNorm.png");

		auto visibleSize = Director::getInstance()->getVisibleSize();
		//Center of screen ///////////////////////////////////////////////////////////////////////
		Vec2 origin = Director::getInstance()->getVisibleOrigin();

		//Player Loc /////////////////////////////////////////////////////////////////////////
		if (player != nullptr) {
			player->setPosition(Vec2((visibleSize.width / 2) - player->getBoundingBox().size.width / 2 + origin.x, (visibleSize.height / 2) - player->getBoundingBox().size.height / 2 + origin.y));

			//this->addChild(player, 10);
		}
		else {
			return false;
		}

		//platforms ////////////////////////////////////////////////////////////////////////////////
		terrain.pushBack(Block::create(0, 0, 20, 700));			//Left Boundary

		terrain.pushBack(Block::create(0, 0, 800, 100));		//Base ground Left
		terrain.pushBack(Block::create(0, 500, 800, 100));		//roof ground Right

		terrain.pushBack(Block::create(0, 500, 800, 100));   // Middle Section

		terrain.pushBack(Block::create(780, 0, 25, 700));		//Left Boundary

		//terrain.pushBack(Block::create(500, 200, 300, 75));
		//terrain.pushBack(Block::create(280, 350, 180, 10));
		for each (Entity* plat in terrain)
		{
			if (plat != nullptr) {
				plat->setEffect(_effect, "layerNorm.png");
				this->addChild(plat);
			}
			else {
				return false;
			}
		}
		//ladders

		//ladders.pushBack(Ladder::create(1600, 275, 32, 600));

		for each (Ladder* lad in ladders) {
			if (lad != nullptr) {
				this->addChild(lad);
			}
			else {
				return false;
			}
		}

		//Interactables /////////////////////////////////////////////////////////////////////////////////////////
		//interactables.pushBack(LoadZone::create(1500, 650, 500, 300, A2_R2, Vec2(50, 205)));
		interactables.pushBack(SceneDoor::create("closed_door.png", Vec2(400, 100), Vec2(400, 140), A2_R2));
		interactables.pushBack(Pickup::create("Bandages.png", cocos2d::Vec2(200, 120), BANDAGE));

		for each (Interactable* inter in interactables) {
			if (inter != nullptr) {
				this->addChild(inter);
			}
			else {
				return false;
			}
		}
		//Actual Platforms ///////////////////////////////////////////////////////////////////////////////////////////


		//	ActualPlatforms.pushBack(Platform::create("Platform1.png", cocos2d::Vec2(100, 280)));
		for each (Platform* p in ActualPlatforms) {
			if (p != nullptr) {

				/*p->setScale(SCALE);
				p->getTexture()->setTexParameters(tp);*/
				p->setEffect(_effect, "layerNorm.png");
				this->addChild(p);
			}
			else
				return false;
		}

		//Set Torches;
		torches.pushBack(Torch::create(cocos2d::Vec2(360, 150), _effect));
		//torches.pushBack(Torch::create(cocos2d::Vec2(350, 400), _effect));
		//torches.pushBack(Torch::create(cocos2d::Vec2(600, 310), _effect));

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

void A2_R3::update(float dt)
{
	GameplayScene::update(dt);
}

bool A2_R4::init()
{
	STAGE_HEIGHT = 600;
	STAGE_WIDTH = 800;

	removeAllChildren();


	if (GameplayScene::init()) {

		//Parallax & Background //////////////////////////////////////////////////////////////////////////////
		auto paraNode = ParallaxNode::create();
		PNode = paraNode;
		EffectSprite *_bgColor = EffectSprite::create("BGP1.png");

		_bgColor->setScale(1);

		paraNode->addChild(_bgColor, 1, Vec2(0.4f, 0.5f), Vec2::ZERO);

		EffectSprite *_bgColor2 = EffectSprite::create("BGP2.png");

		_bgColor2->setScale(1);
		paraNode->addChild(_bgColor2, -1, Vec2(1.4f, 1.5f), Vec2::ZERO);

		this->addChild(paraNode);

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
		_effect->setAmbientLightColor(Color3B(0, 0, 0));

		player->setEffect(_effect, "layerNorm.png");
		_bgColor->setEffect(_effect, "layerNorm.png");
		_bgColor2->setEffect(_effect, "layerNorm.png");

		auto visibleSize = Director::getInstance()->getVisibleSize();
		//Center of screen ///////////////////////////////////////////////////////////////////////
		Vec2 origin = Director::getInstance()->getVisibleOrigin();

		//Player Loc /////////////////////////////////////////////////////////////////////////
		if (player != nullptr) {
			player->setPosition(Vec2((visibleSize.width / 2) - player->getBoundingBox().size.width / 2 + origin.x, (visibleSize.height / 2) - player->getBoundingBox().size.height / 2 + origin.y));

			//this->addChild(player, 10);
		}
		else {
			return false;
		}

		//platforms ////////////////////////////////////////////////////////////////////////////////
		terrain.pushBack(Block::create(0, 0, 20, 700));			//Left Boundary

		terrain.pushBack(Block::create(0, 0, 800, 100));		//Base ground
		terrain.pushBack(Block::create(0, 500, 700, 100));		//roof 

		terrain.pushBack(Block::create(780, 0, 25, 700));		//Left Boundary
		terrain.pushBack(Block::create(690, 0, 120, 250));		//Left Unmoving Box/ledge

																//terrain.pushBack(Block::create(500, 200, 300, 75));
																//terrain.pushBack(Block::create(280, 350, 180, 10));
		for each (Entity* plat in terrain)
		{
			if (plat != nullptr) {
				plat->setEffect(_effect, "layerNorm.png");
				this->addChild(plat);
			}
			else {
				return false;
			}
		}
		//ladders

		ladders.pushBack(Ladder::create(720, 300, 32, 400));

		for each (Ladder* lad in ladders) {
			if (lad != nullptr) {
				this->addChild(lad);
			}
			else {
				return false;
			}
		}

		//Interactables /////////////////////////////////////////////////////////////////////////////////////////
		interactables.pushBack(LoadZone::create(700, 600, 500, 300, A2_R5, Vec2(100, 200)));
		interactables.pushBack(SceneDoor::create("closed_door.png", Vec2(400, 100), Vec2(1270, 140), A2_R2));
		
		for each (Interactable* inter in interactables) {
			if (inter != nullptr) {
				this->addChild(inter);
			}
			else {
				return false;
			}
		}
		//Actual Platforms ///////////////////////////////////////////////////////////////////////////////////////////


		//	ActualPlatforms.pushBack(Platform::create("Platform1.png", cocos2d::Vec2(100, 280)));
		for each (Platform* p in ActualPlatforms) {
			if (p != nullptr) {

				/*p->setScale(SCALE);
				p->getTexture()->setTexParameters(tp);*/
				p->setEffect(_effect, "layerNorm.png");
				this->addChild(p);
			}
			else
				return false;
		}

		//Set Torches;
		//torches.pushBack(Torch::create(cocos2d::Vec2(200, 230), _effect));
		//torches.pushBack(Torch::create(cocos2d::Vec2(350, 400), _effect));
		//torches.pushBack(Torch::create(cocos2d::Vec2(600, 310), _effect));

		for each (Torch* t in torches)
		{
			if (t != nullptr) {
				addChild(t, 2);
			}
			else {
				return false;
			}
		}

		Pushables.pushBack(Pushable::create("PushableRock.png", Vec2(100, 140), Vec2(100, 140), Vec2(650, 140)));

		for each (Pushable* push in Pushables) {
			if (push != nullptr) {
				this->addChild(push);
			}
			else
				return false;

		}

		player->switchLight();
		view = this->getDefaultCamera();

		this->scheduleUpdate();
		return true;
	}

	return false;
}

void A2_R4::update(float dt)
{
	GameplayScene::update(dt);
}

bool A2_R5::init()
{
	STAGE_HEIGHT = 600;
	STAGE_WIDTH = 1600;

	removeAllChildren();


	if (GameplayScene::init()) {

		//Parallax & Background //////////////////////////////////////////////////////////////////////////////
		auto paraNode = ParallaxNode::create();
		PNode = paraNode;
		EffectSprite *_bgColor = EffectSprite::create("BGP1.png");

		_bgColor->setScale(1);

		paraNode->addChild(_bgColor, 1, Vec2(0.4f, 0.5f), Vec2::ZERO);

		EffectSprite *_bgColor2 = EffectSprite::create("BGP2.png");

		_bgColor2->setScale(1);
		paraNode->addChild(_bgColor2, -1, Vec2(1.4f, 1.5f), Vec2::ZERO);

		this->addChild(paraNode);

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
		_effect->setAmbientLightColor(Color3B(0, 0, 0));

		player->setEffect(_effect, "layerNorm.png");
		_bgColor->setEffect(_effect, "layerNorm.png");
		_bgColor2->setEffect(_effect, "layerNorm.png");

		auto visibleSize = Director::getInstance()->getVisibleSize();
		//Center of screen ///////////////////////////////////////////////////////////////////////
		Vec2 origin = Director::getInstance()->getVisibleOrigin();

		//Player Loc /////////////////////////////////////////////////////////////////////////
		if (player != nullptr) {
			player->setPosition(Vec2((visibleSize.width / 2) - player->getBoundingBox().size.width / 2 + origin.x, (visibleSize.height / 2) - player->getBoundingBox().size.height / 2 + origin.y));

			//this->addChild(player, 10);
		}
		else {
			return false;
		}

		//platforms ////////////////////////////////////////////////////////////////////////////////
		terrain.pushBack(Block::create(0, 0, 20, 300));			//Left Boundary

		terrain.pushBack(Block::create(0, 0, 1600, 100));		//Base ground
		terrain.pushBack(Block::create(0, 300, 1600, 300));		//roof 

		terrain.pushBack(Block::create(1580, 0, 25, 300));		//Left Boundary
		//terrain.pushBack(Block::create(690, 0, 120, 250));		//Left Unmoving Box/ledge

																//terrain.pushBack(Block::create(500, 200, 300, 75));
																//terrain.pushBack(Block::create(280, 350, 180, 10));
		for each (Entity* plat in terrain)
		{
			if (plat != nullptr) {
				plat->setEffect(_effect, "layerNorm.png");
				this->addChild(plat);
			}
			else {
				return false;
			}
		}
		//ladders

		//ladders.pushBack(Ladder::create(720, 300, 32, 400));

		for each (Ladder* lad in ladders) {
			if (lad != nullptr) {
				this->addChild(lad);
			}
			else {
				return false;
			}
		}

		//Interactables /////////////////////////////////////////////////////////////////////////////////////////
		interactables.pushBack(LoadZone::create(1500, 650, 500, 300, A2_R2, Vec2(100, 200)));
		interactables.pushBack(SceneDoor::create("closed_door.png", Vec2(400, 100), Vec2(1270, 140), A2_R2));

		for each (Interactable* inter in interactables) {
			if (inter != nullptr) {
				this->addChild(inter);
			}
			else {
				return false;
			}
		}
		//Actual Platforms ///////////////////////////////////////////////////////////////////////////////////////////


		//	ActualPlatforms.pushBack(Platform::create("Platform1.png", cocos2d::Vec2(100, 280)));
		for each (Platform* p in ActualPlatforms) {
			if (p != nullptr) {

				/*p->setScale(SCALE);
				p->getTexture()->setTexParameters(tp);*/
				p->setEffect(_effect, "layerNorm.png");
				this->addChild(p);
			}
			else
				return false;
		}

		//Set Torches;
		//torches.pushBack(Torch::create(cocos2d::Vec2(200, 230), _effect));
		//torches.pushBack(Torch::create(cocos2d::Vec2(350, 400), _effect));
		//torches.pushBack(Torch::create(cocos2d::Vec2(600, 310), _effect));

		for each (Torch* t in torches)
		{
			if (t != nullptr) {
				addChild(t, 2);
			}
			else {
				return false;
			}
		}

		Pushables.pushBack(Pushable::create("PushableRock.png", Vec2(100, 140), Vec2(100, 140), Vec2(650, 140)));

		for each (Pushable* push in Pushables) {
			if (push != nullptr) {
				this->addChild(push);
			}
			else
				return false;

		}

		player->switchLight();
		view = this->getDefaultCamera();

		this->scheduleUpdate();
		return true;
	}

	return false;
}

void A2_R5::update(float dt)
{
	GameplayScene::update(dt);
}
