#pragma once
#include"Enums.h"
#include"cocos2d.h"

class GameplayScene;

class LevelManager {
private:
	static cocos2d::Vector<GameplayScene *> levels;
public:
	static GameplayScene* GetLevel(levelEnum id);
};