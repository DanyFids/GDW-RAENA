#include "Prompt.h"

Prompt * Prompt::create(int What, GameplayScene * scn)
{
	auto ret = new (std::nothrow) Prompt;

	ret->PP = What;
	ret->scene = scn;
	return ret;
}

void Prompt::Load()
{
	if (this->PP == 1)
	{
		auto TheBox = cocos2d::Sprite::create("A Prompt.png");
		TheBox->setPosition(cocos2d::Vec2(150, 0));
		TheBox->setScale(2.0);
		//TheBox->setScaleX(6.0);
		TheBox->setAnchorPoint(cocos2d::Vec2(0, 0));
		this->PBox = TheBox;
		scene->addChild(TheBox);

		this->PBox->setOpacity(255);
	}
	if (this->PP == 2)
	{
		auto TheBox = cocos2d::Sprite::create("B Prompt.png");
		TheBox->setPosition(cocos2d::Vec2(150, 0));
		TheBox->setScale(2.0);
		//TheBox->setScaleX(6.0);
		TheBox->setAnchorPoint(cocos2d::Vec2(0, 0));
		this->PBox = TheBox;
		scene->addChild(TheBox);

		this->PBox->setOpacity(255);
	}
	
	

}

void Prompt::Close()
{
	this->PBox->setOpacity(0);

}

void Prompt::Follow(Player* Target)
{
	this->PBox->setPosition(Target->getPosition());
}
