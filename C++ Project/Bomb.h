#pragma once
#include "Point.h"
class Screen;
class Player;


class Bomb {
	Point position; // Bomb position
	int timer; // Countdown timer
	char playerChar; // Character of the player who placed the bomb
	int roomID; // ID of the room where the bomb is placed


public:

    Bomb(Point p, char pChar,int ID) : position(p), timer(50), playerChar(pChar),roomID(ID) {}
	bool tick(Screen& screen, Player& p1, Player& p2, int currentRoomID); // returns true if exploded
	void explode(Screen& screen, Player& p1, Player& p2, bool isInCurrentRoom); // Updated signature
	Point getPosition() const { return position; } // Getter for position
	int getRoomID() const { return roomID; } // Getter for room ID
	int getTimer() const { return timer; } // Getter for timer
};

//GUY PALATIN 