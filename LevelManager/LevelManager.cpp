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
