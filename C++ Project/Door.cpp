#include "Door.h"

int Door::getId() const
{
	return id;
}
bool Door::isOpen() const
{
	return open;
}
bool Door::tryOpen(int keyId)
{
	if (keyId == id)
	{
		open = true;
		return true;
	}
	return false;
}
bool Door::canPass() const
{
	return open;
}
bool Door::isDoorChar(char c)
{
	return c >= '1' && c <= '9';
}
int Door::getDestinationLevel() const
{
	return destinationLevel;
}
void Door::setDestinationLevel(int level)
{
	destinationLevel = level;
}