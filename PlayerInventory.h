#pragma once
#include "Enums.h"



//I hope this works

//Player inventory Values YAY
class PlayerInventory {
public:
	PlayerInventory();

	friend int getKeys(KeyType k);
	friend void editKeys(KeyType k, int i);
private:
	static int genKeys;
};

