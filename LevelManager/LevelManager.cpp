#include "LevelManager.h"
#include "GameplayScene.h"

cocos2d::Vector<GameplayScene *> LevelManager::levels = {
    TutRoom1::create()	 
};

GameplayScene * LevelManager::GetLevel(levelEnum id)
{
	return levels.at(id);
}
