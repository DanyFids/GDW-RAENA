#include "Textbox.h"
#include "cocos2d.h"
#include "GameplayScene.h"


Textbox * Textbox::create(int P, std::vector<int> C, std::vector<std::string> T, cocos2d::Scene * scn)
{
	auto ret = new (std::nothrow) Textbox;

	if (ret->initWithFile("TextBox.png")) {
		ret->Pages = P;
		ret->Character = C;
		ret->Text = T;
		ret->scene = scn;
		((GameplayScene*)ret->scene)->inDialogue = true;
		ret->CurrPage = 0;

		ret->setAnchorPoint(cocos2d::Vec2(0, 0));
		ret->setPosition(cocos2d::Vec2(scn->getDefaultCamera()->getPositionX() - 250, 20));

		auto TheText = cocos2d::Label::createWithSystemFont(ret->Text[ret->CurrPage], "Arial", 24);
		TheText->enableShadow();
		TheText->setAnchorPoint(cocos2d::Vec2(0, 0));
		TheText->setPosition(cocos2d::Vec2(50, 100));
		ret->Yeet = TheText;
		ret->addChild(TheText);

		return ret;
	}

	CC_SAFE_RELEASE(ret);
	return nullptr;
}

void Textbox::Flippage()
{
	this->CurrPage++;
	if (CurrPage < Pages) {
		Yeet->setString(Text[CurrPage]);
	}
	else {
		Close();
	}
}

void Textbox::Load()
{
	if (this->Box)
	{
		auto TheBox = cocos2d::Sprite::create("TextBox.png");
		TheBox->setPosition(cocos2d::Vec2(150, 0));
		TheBox->setScale(4.0);
		TheBox->setScaleX(6.0);
		TheBox->setAnchorPoint(cocos2d::Vec2(0, 0));
		this->Box = TheBox;
		scene->addChild(TheBox);
	}
	else 
	{
		this->Box->setOpacity(255);
	}

	auto TheText = cocos2d::Label::createWithSystemFont(Text[CurrPage], "Arial", 24);
	TheText->enableShadow();
	TheText->setAnchorPoint(cocos2d::Vec2(0, 0));
	TheText->setPosition(cocos2d::Vec2(180, 140));
	this->Yeet = TheText;
	scene->addChild(TheText);

}

void Textbox::Close(/*cocos2d::Sprite del1, cocos2d::Label del2*/)
{
	scene->removeChild(this);
	((GameplayScene*)scene)->inDialogue = false;
	//this->Box->setOpacity(0);
}

int Textbox::getPages()
{
	return this->Pages;
}

int Textbox::getCurrPage()
{
	return this->CurrPage;
}
