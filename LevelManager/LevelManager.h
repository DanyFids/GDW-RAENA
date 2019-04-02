#pragma once
#include"Enums.h"
#include"cocos2d.h"

class GameplayScene;
class player_inventory;

class LevelManager {
private:
	static cocos2d::Vector<GameplayScene *> levels;
public:
	static GameplayScene* GetLevel(levelEnum id);
	static void AddLevels(cocos2d::Vector<GameplayScene *> l);
	static void setGlobalInv(player_inventory* inv);
	static void ResetLevels();
};