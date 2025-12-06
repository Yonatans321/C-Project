#include "Door.h"
#include "Player.h"
#include "Screen.h"
#include "RoomScreenManager.h" 
#include <iostream>

using std::cout;

static bool openDoors[10] = { false }; // static array to track open doors

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
	return (c >= '1' && c <= '9');
}
int Door::getDestinationLevel() const
{
	return destinationLevel;
}
void Door::setDestinationLevel(int level)
{
	destinationLevel = level;
}


bool Door::handleDoor(Player& p, Screen& screen, int& currentLevel, char& foundDoor)
{
	if (!p.isActive()) return false;

	Point playerPos = p.getPosition();

	// Check directions of the player
	int dirX[] = { 1, -1, 0, 0 };
	int dirY[] = { 0, 0, 1, -1 };

	// Loop through each direction
	for (int i = 0; i < 4; i++)
	{
		Point checkPos = playerPos;
		checkPos.set(playerPos.getX() + dirX[i], playerPos.getY() + dirY[i]);

		char cell = screen.getCharAt(checkPos);

		// Check if the cell is a door
		if (isDoorChar(cell))
		{
			int doorIndex = cell - '0';

			Door* door = screen.getDoor(checkPos);
			if (door == nullptr) continue;

			bool doorCanPass = false;
			// Check if the door is already open	
			if (door->isOpen()||openDoors[doorIndex]==true)
			{
				doorCanPass = true;
			}
			// Try to use key for the door
			else if (p.useKeyForDoor(cell))
			{
				door->tryOpen(doorIndex);     
				openDoors[doorIndex] = true;  
				p.keyUsed();                 
				doorCanPass = true;
			}
			
			if (doorCanPass)
			{
				
				// Move the player through the door
				p.setInactive();
				p.erase();
				foundDoor = cell;
		
				// int dest = door->getDestinationLevel();
				// if (dest != -1) currentLevel = dest;
				return false;
			}
		}
	}
	return false;
}

Point Door::findLocation(const Screen& screen, int doorIndex) {
	char targetChar = '0' + doorIndex;

	
	for (int y = 0; y < 23; y++) {
		for (int x = 0; x < 80; x++) {

			
			if (screen.getCharAt(x, y) == targetChar) {
				return Point(x, y, Direction::directions[Direction::STAY], targetChar);
			}
		}
	}

	
	return Point(-1, -1, Direction::directions[Direction::STAY], ' ');
}


