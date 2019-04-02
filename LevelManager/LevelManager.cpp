#include "LevelManager.h"
#include "GameplayScene.h"

cocos2d::Vector<GameplayScene *> LevelManager::levels = {
};

GameplayScene * LevelManager::GetLevel(levelEnum id)
{
	return levels.at(id);
}

void LevelManager::AddLevels(cocos2d::Vector<GameplayScene*> l)
{
	for each (GameplayScene* lvl in l)
	{
		levels.pushBack(l);
	}
}

void LevelManager::setGlobalInv(player_inventory * inv)
{
	for each (GameplayScene* scn in levels)
	{
		scn->SetInventory(inv);
	}
}

void LevelManager::ResetLevels()
{
	while (levels.size() > 0) {
		levels.popBack();
	}

	cocos2d::Vector<GameplayScene*> levels = {
		A1_R1::create(),
		A1_R2::create(),
		A1_R3::create(),
		A1_R4::create(),
		A1_R5::create(),
		A1_R6::create(),
		A2_R1::create(),
		A2_R2::create(),
		A2_R3::create(),
		A2_R4::create(),
		A2_R5::create(),
		A2_AT1::create(),
		BOSS_R1::create()
	};

	AddLevels(levels);
}
