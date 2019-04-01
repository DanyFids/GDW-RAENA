#include "Options.h"
#include "GameplayScene.h"
#include "ui/CocosGUI.h"

USING_NS_CC;

static void problemLoading(const char* filename);

Scene * Options::createScene()
{
	return Options::create();
}

bool Options::init()
{
	if (!Scene::init())
	{
		return false;
	}
	optionsLabel = Label::createWithTTF("OPTIONS", "fonts/horrendo.ttf", 36);
	optionsLabel->setColor(Color3B(200, 180, 150));
	optionsLabel->enableGlow(Color4B::RED);
	optionsLabel->enableBold();
	optionsLabel->setPosition(cocos2d::Vec2(400.0f, 500.0f));
	addChild(optionsLabel, 1);
	auto slider = cocos2d::ui::Slider::create();
	slider->loadBarTexture("");
	slider->loadSlidBallTextures("");
	slider->loadProgressBarTexture("");

	slider->addTouchEventListener([&](Ref* sender, cocos2d::ui::Widget::TouchEventType type) {
		switch (type)
		{
		case cocos2d::ui::Widget::TouchEventType::BEGAN:
			break;
		case cocos2d::ui::Widget::TouchEventType::ENDED:
			break;
		default:
			break;
		}
	});
	return true;
}

void Options::update(float dt)
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
