#pragma once
#include "Point.h"
class Screen;
class Player;


class Bomb {
    Point position;
    int timer;
public:

    Bomb(Point p) : position(p), timer(50) {}
    bool tick(Screen& screen, Player& p1, Player& p2);
    void explode(Screen& screen, Player& p1, Player& p2);
    Point getPosition() const { return position; }
};

