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

void Game::handleDoor(Player& p)
{
    Screen& currentScreen = gameScreens[currentLevel];
    Point doorPos = p.getPosition();
    char cell = currentScreen.getCharAt(doorPos);// Get the character at the player's position

    if (!Door::isDoorChar(cell))
    {
        return; // Not a door
    }
    Door* d = currentScreen.getDoor(doorPos);// Get the door at the player's position
    if (!d)// If no door found 
    {
        return;
    }

    if (!d->isOpen())// If the door is not open
    {
        if (p.useKeyForDoor(cell))// If the player has the key for the door
        {
            if (d->tryOpen(p.getItemId()))// Try to open the door with the key 
            {
                p.DropItem();// Remove the key from the player
            }

        }
        if (!d->isOpen())// If the door is still not open
        {
            return; // Door is still closed
        }
    }
    if (d->isOpen())// If the door is open
    {
        int destLevel = d->getDestinationLevel();// Get the destination level

        if (destLevel != -1)// If destination level is the last level
        {
            currentLevel = destLevel;// Change to the new level

            initLevel();// Initialize the new level
        }
    }
}