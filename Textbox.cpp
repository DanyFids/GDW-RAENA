#include "Textbox.h"
#include "cocos2d.h"
#include "GameplayScene.h"


Textbox * Textbox::create(int P, std::vector<int> C, std::vector<std::string> T, GameplayScene * scn)
{
	auto ret = new (std::nothrow) Textbox;

	ret->Pages = P;
	ret->Character = C;
	ret->Text = T;
	ret->scene = scn;
	ret->CurrPage = 0;

	return ret;
}

void Textbox::Flippage()
{
	
	
	this->CurrPage++;
	
	
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
	scene->removeChild(this->Yeet);
	scene->removeChild(this->Box);
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
