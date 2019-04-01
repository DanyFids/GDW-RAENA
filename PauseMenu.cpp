#include "PauseMenu.h"
#include "GameplayScene.h"
#include "Options.h"
#include "ui/CocosGUI.h"

USING_NS_CC;

static void problemLoading(const char* filename);

Scene * PauseMenu::createScene()
{
	return PauseMenu::create();
}

bool PauseMenu::init()
{
	if (!Scene::init()) {
		return false;
	}

	pauseLabel = Label::createWithTTF("GAME IS PAUSED", "fonts/horrendo.ttf", 36);
	pauseLabel->setColor(Color3B(200, 180, 150));
	pauseLabel->enableGlow(Color4B::RED);
	pauseLabel->enableBold();
	pauseLabel->setPosition(cocos2d::Vec2(400.0f, 500.0f));
	addChild(pauseLabel, 1);

	auto options = cocos2d::ui::Button::create("Play.png", "PlayClicked");

	options->addTouchEventListener([&](Ref* sender, cocos2d::ui::Widget::TouchEventType type) {
		switch (type)
		{
		case cocos2d::ui::Widget::TouchEventType::BEGAN:
			Director::getInstance()->pushScene(Options::createScene());
			break;
		case cocos2d::ui::Widget::TouchEventType::ENDED:
			break;
		default:
			break;
		}
	});
	auto visibleSize = Director::getInstance()->getVisibleSize();

	auto quit = MenuItemImage::create(
		"Quit.png",
		"QuitClicked.png",
		CC_CALLBACK_1(PauseMenu::menuCloseCallback, this));

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

	quit->setScale(2);
	options->setScale(2);
	auto menu = Menu::create(quit, NULL);
	menu->setPosition(Vec2::ZERO);
	options->setPosition(Vec2(400.0f, 300.0f));
	this->addChild(menu, 1);
	this->addChild(options, 1);

	this->scheduleUpdate();
	return true;
}

void PauseMenu::update(float dt)
{
	exitTimer -= dt;
	if (GetAsyncKeyState(VK_ESCAPE) != 0)
	{
		if (exitTimer == 0)
		{
			exitTimer = 20;
			Director::getInstance()->popScene();
		}
	}
}

static void problemLoading(const char* filename)
{
	printf("Error while loading: %s\n", filename);
	printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

void PauseMenu::menuReturnCallback(Ref* pSender) {
	Director::getInstance()->replaceScene(GameplayScene::create());
}

void PauseMenu::menuCloseCallback(Ref* pSender)
{
	//Close the cocos2d-x game scene and quit the application
	Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
}
