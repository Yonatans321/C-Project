#pragma once
#include "Point.h"
class Screen;
class Player;


class Bomb {
    Point position;
    int timer;
    char playerChar;
public:

    Bomb(Point p, char pChar) : position(p), timer(50), playerChar(pChar) {}
    bool tick(Screen& screen, Player& p1, Player& p2);
    void explode(Screen& screen, Player& p1, Player& p2);
    Point getPosition() const { return position; }
};

