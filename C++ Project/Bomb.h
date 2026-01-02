#pragma once
#include "Point.h"
class Screen;
class Player;


class Bomb {
    Point position;
    int timer;
    char playerChar;
	int roomID;


public:

    Bomb(Point p, char pChar,int ID) : position(p), timer(50), playerChar(pChar),roomID(ID) {}
    bool tick(Screen& screen, Player& p1, Player& p2,int currentRoomID);
    void explode(Screen& screen, Player& p1, Player& p2,bool isInCurrentRoom);
    Point getPosition() const { return position; }
	int getRoomID() const { return roomID; }
	int getTimer() const { return timer; }
};

