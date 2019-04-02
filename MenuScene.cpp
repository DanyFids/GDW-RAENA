#include "cocos2d.h"
#include "MenuScene.h"
#include "GameplayScene.h"
#include "InventoryScene.h"
#include "LevelManager/LevelManager.h"

USING_NS_CC;

static void problemLoading(const char* filename);

Scene* MenuScene::createScene() {
	return MenuScene::create();
}

bool MenuScene::init()
{
	if (!Scene::init()) {
		return false;
	}

	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto KeyHandler = EventListenerKeyboard::create();

	cocos2d::Texture2D::TexParams tp = { GL_NEAREST, GL_NEAREST, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE };

	auto BG = Sprite::create();
	BG->initWithFile("Raena.png");
	BG->getTexture()->setTexParameters(tp);
	BG->initWithFile("Raena2.png");
	BG->getTexture()->setTexParameters(tp);
	BG->initWithFile("Raena3.png");
	BG->getTexture()->setTexParameters(tp);
	cocos2d::Vector<cocos2d::SpriteFrame *> title_frames = {
		cocos2d::SpriteFrame::create("Raena.png", {0,0, 94, 39}, false, {0,0},{94, 39}),
		cocos2d::SpriteFrame::create("Raena2.png", {0,0, 94, 39}, false, {0,0},{94, 39}),
		cocos2d::SpriteFrame::create("Raena3.png", {0,0, 94, 39}, false, {0,0},{94, 39}),
	};

	auto anim = cocos2d::Animation::createWithSpriteFrames(title_frames, 0.2);
	BG->runAction(cocos2d::RepeatForever::create(cocos2d::Animate::create(anim)));

	// Center of the image
	BG->setPosition({origin.x + (visibleSize.width/2), origin.y + (visibleSize.height / 2)});
	// Resize if picture is too small
	BG->setScale(5);

	// Return to different menu
	auto play = MenuItemImage::create(
		"Play.png",
		"PlayClicked.png",
		CC_CALLBACK_1(MenuScene::menuReturnCallback, this));

	auto quit = MenuItemImage::create(
		"Quit.png",
		"QuitClicked.png",
		CC_CALLBACK_1(MenuScene::menuCloseCallback, this));

	if (play == nullptr ||
		play->getContentSize().width <= 0 ||
		play->getContentSize().height <= 0)
	{
		problemLoading(" 'Highscores Unselected.png' and the other one");
	}
	else
	{
		// Decide where the button goes
		float x = visibleSize.width / 2 + play->getContentSize().width / 2;//origin.x + visibleSize.width - closeItem->getContentSize().width / 2;
		float y = visibleSize.height / 2 - play->getContentSize().height / 2 - visibleSize.height / 4;//origin.y + closeItem->getContentSize().height / 2;
		play->setPosition(Vec2(x, y));
	}

	if (quit == nullptr ||
		quit->getContentSize().width <= 0 ||
		quit->getContentSize().height <= 0)
	{
		problemLoading(" 'Highscores Unselected.png' and the other one");
	}
	else
	{
		// Decide where the button goes
		float x = visibleSize.width / 2 + quit->getContentSize().width / 2;//origin.x + visibleSize.width - closeItem->getContentSize().width / 2;
		float y = visibleSize.height / 2 - quit->getContentSize().height / 2 - visibleSize.height / 3;//origin.y + closeItem->getContentSize().height / 2;
		quit->setPosition(Vec2(x, y));
	}

	// create menu, it's an autorelease object
	play->setScale(2);
	quit->setScale(2);
	auto menu = Menu::create(play, NULL);
	auto menu2 = Menu::create(quit, NULL);
	menu->setPosition(Vec2::ZERO);
	menu2->setPosition(Vec2::ZERO);
	this->addChild(menu, 1);
	this->addChild(menu2, 1);

	addChild(BG);

	return true;
}

static void problemLoading(const char* filename)
{
	printf("Error while loading: %s\n", filename);
	printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

void MenuScene::menuReturnCallback(Ref* pSender) {
	Director::getInstance()->replaceScene(LevelManager::GetLevel(A1_R3));  //GAME START
}

void MenuScene::menuCloseCallback(Ref* pSender)
{
	//Close the cocos2d-x game scene and quit the application
	Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
}
