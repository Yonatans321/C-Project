#include "Door.h"
#include <iostream>
#include <Windows.h>
#include "Screen.h"

// Static member initialization
bool Door::switchesAreOn = false;
bool Door::openDoors[10] = { false }; // static array to track open doors
static bool justWarned = false;

//DOOR LOGIC
bool Door::tryOpen(int keyId)
{
	if (open)// the door is already open
	{
		return true;
	}

	if (!switchesAreOn) // switches are not all on
	{
		return false; 
	}

	if (keyId == id) // macthed key for door
	{
		open = true;
		return true;
	}
	return false;
}

// Handle door interaction for the player
bool Door::handleDoor(Player& p, Screen& screen, int& currentLevel, char& foundDoor)
{
	if (!p.isActive()) return false; // do nothing if player is inactive

	Point playerPos = p.getPosition(); // get player's current position

	// Check directions of the player
	int dirX[] = { 1, -1, 0, 0 };
	int dirY[] = { 0, 0, 1, -1 };

	// Loop through each direction
	for (int i = 0; i < 4; i++)
	{
		Point checkPos = playerPos;
		checkPos.set(playerPos.getX() + dirX[i], playerPos.getY() + dirY[i]);// position next to player

		char cell = screen.getCharAt(checkPos);

		// Check if the cell is a door
		if (isDoorChar(cell))
		{
			int doorIndex = cell - '0';
			Door* door = screen.getDoor(checkPos);// get door object
			if (door == nullptr) continue;

			bool doorCanPass = false;
			if (doorIndex <= currentLevel)// door to previous or current level	
			{
				doorCanPass = true;
			}
			if (doorIndex != (currentLevel + 1)) // door to future level
			{
				if (door->isOpen() || openDoors[doorIndex] == true)
				{
					doorCanPass = true;
				}
			}

			// Check if the door is already open
			if (door->isOpen() || openDoors[doorIndex] == true)
			{
				doorCanPass = true;
			}
			// Try to use key for the door
			else
			{
				// Check if switches are on
				if (!Door::switchesAreOn)
				{
					Screen::drawAnimatedBox(10, 5, 50, 12);// draw message box
					gotoxy(13, 7);
					std::cout << "you cannot enter";
					gotoxy(15, 9);
					std::cout << "All switcehs must be ON!";
					Sleep(1200);
					Screen::closeAnimatedBox(10, 5, 50, 12);
					p.stepBack();// move player back
					screen.drawMap();
					p.draw();
					return false;
				}

				if (p.useKeyForDoor(cell))// check if player has the correct key
				{
					door->tryOpen(doorIndex);// open the door
					openDoors[doorIndex] = true;// mark door as open
					p.keyUsed();// mark key as used
					doorCanPass = true;
				}
				else
				{
					Screen::drawAnimatedBox(10, 5, 50, 12);
					gotoxy(17, 7);
					std::cout << "you need the correct key!";
					Sleep(1100);
					Screen::closeAnimatedBox(10, 5, 50, 12);
					p.stepBack();
					screen.drawMap();
					p.draw();
					return false;
				}
			}

			if (doorCanPass)
			{
				// Move the player through the door
				p.setInactive();
				p.erase();
				foundDoor = cell;
				return false;
			}
		}
	}
	return false;
}





