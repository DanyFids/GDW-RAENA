#pragma once
#include <string>
#include <vector>
#include "cocos2d.h"
#include "GameplayScene.h"

class Textbox : public cocos2d::Sprite
{
private:
	int Pages;
	std::vector<int> Character;
	std::vector<std::string> Text;
	int CurrPage;

	

	GameplayScene* scene;

public:
	
	Sprite* Box;
	cocos2d::Label* Yeet;

	static Textbox * create(int P, std::vector<int> C, std::vector<std::string> T, GameplayScene * scn);
	//Textbox(int P, std::vector<int> C, std::vector<std::string> T);
	void Flippage();
	void Load();
	void Close(/*cocos2d::Sprite del1, cocos2d::Label del2*/);
	int getPages();
	int getCurrPage();

};