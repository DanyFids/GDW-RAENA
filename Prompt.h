#pragma once
#include <string>
#include <vector>
#include "cocos2d.h"
#include "GameplayScene.h"
#include "Entities/Player.h"

class Prompt : public cocos2d::Sprite
{
private:
	int PP;
	GameplayScene* scene = nullptr;

public:
	static Prompt * create(int What, GameplayScene * scn);

	Sprite* PBox;

	void Load();
	void Close();
	void Hide();
	void Show();
	void Follow(Player* Target);

};