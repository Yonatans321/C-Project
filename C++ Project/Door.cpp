#include "Door.h"
#include <iostream>
#include <Windows.h>
#include "Screen.h"

bool Door::switchesAreOn = false;

static bool openDoors[10] = { false }; // static array to track open doors
static bool justWarned = false;

int Door::getId() const
{
	return id;
}
bool Door::isOpen() const
{
	return open;
}
int Door::getDestinationLevel() const
{
	return destinationLevel;
}
void Door::setDestinationLevel(int level)
{
	destinationLevel = level;
}

bool Door::tryOpen(int keyId)
{
	if (open)
	{
		return true;
	}
	if (!switchesAreOn)
	{
		return false;
	}
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


bool Door::handleDoor(Player& p, Screen& screen, int& currentLevel)
{
	if (!p.isActive()) return false;

	Point playerPos = p.getPosition();

	// Check directions of the player
	int dirX[] = { 1, -1, 0, 0 };
	int dirY[] = { 0, 0, 1, -1 };

	bool touchingDoor = false;

	// Loop through each direction
	for (int i = 0; i < 4; i++)
	{
		Point checkPos = playerPos;
		checkPos.set(playerPos.getX() + dirX[i], playerPos.getY() + dirY[i]);

		char cell = screen.getCharAt(checkPos);

		// Check if the cell is a door
		if (isDoorChar(cell))
		{
			touchingDoor = true;
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
			else
			{
				if (!Door::switchesAreOn)
				{
					if (!justWarned)
					{
						Screen::drawAnimatedBox(10, 5, 50, 12);
						gotoxy(15, 7);
						std::cout << "you cannot enter.\n All switcehs must be ON!";
						Sleep(400);
						Screen::closeAnimatedBox(10, 5, 50, 12);
						screen.drawMap();
						p.draw();
						justWarned = true;
					}
					return false;
				}
				if (p.useKeyForDoor(cell))
				{
					door->tryOpen(doorIndex);
					openDoors[doorIndex] = true;
					p.keyUsed();
					doorCanPass = true;
				}
				else
				{
					if (!justWarned)
					{
						Screen::drawAnimatedBox(10, 5, 50, 12);
						gotoxy(15, 7);
						std::cout << "you need the correct key!";
						Sleep(400);
						Screen::closeAnimatedBox(10, 5, 50, 12);
						screen.drawMap();
						p.draw();
						justWarned = true;
					}
				}
			}
			
			if (doorCanPass)
			{
				
				// Move the player through the door
				p.setInactive();
				p.erase();

		
				// int dest = door->getDestinationLevel();
				// if (dest != -1) currentLevel = dest;
				return false;
			}
		}
	}
	if (!touchingDoor)
	{
		justWarned = false;
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

void Door::allSwitchesAreOn()
{
	switchesAreOn = true;
}

