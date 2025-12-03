#include "Door.h"
#include "Player.h"
#include "Screen.h"
#include "RoomScreenManager.h" 

using namespace std;
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

bool Door::handleDoor(Player& p, Screen& screen,int& currentLevel)
{	
	Point doorPos = p.getPosition();
	char cell = screen.getCharAt(doorPos);
	if (!isDoorChar(cell))
	{
		return false; // not a door
	}
	Door* door = screen.getDoor(doorPos);
	if (door == nullptr)
	{
		return false; // door not found
	}
	if(!door->isOpen())
	{
		if (p.useKeyForDoor(cell))
		{
			if (door->tryOpen(cell - '0'))
			{
				screen.showMessage("You unlocked the door!.");
				p.keyUsed();
				return true; // door interaction handled
			}
			else
			{
				screen.showMessage("The key doesn't fit the door .");
				return true; // door interaction handled
			}
		}
		else
		{
			screen.showMessage("The door is locked. You need the matching key.");
			return true; // door interaction handled
		}
	}
	int dest = door->getDestinationLevel();
	if (dest != -1)
	{
		currentLevel = dest;
		return true; // tell Game it should reload the level
	}

	return false;
}