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
	GameplayScene* scene;

public:
	static Prompt * create(int What, GameplayScene * scn);

	Sprite* PBox;

	void Load();
	void Close();
	void Follow(Player* Target);

};