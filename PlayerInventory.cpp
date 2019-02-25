#include "PlayerInventory.h"

int PlayerInventory::getKeys(KeyType k)
{
	if (k == GEN_KEY) {
		return genKeys;
	}
}

void PlayerInventory::editKeys(KeyType k, int i)
{
	if (k == GEN_KEY)
	{
		if (i > 0) {
			genKeys += i;
		}
		else genKeys -= i;
	}
}
