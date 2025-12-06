#include <iostream>
#include "Point.h"
#include "utils.h"
#include "Screen.h"

void Point::draw(char c) {
	gotoxy(x, y);
	std::cout << c;
}

void Point::move() {
	if (x + dir.dirx < 0 || x + dir.dirx > Screen::WIDTH) {
		dir.dirx = -dir.dirx;
	}
	if (y + dir.diry < 0 || y + dir.diry > Screen::HEIGHT) {
		dir.diry = -dir.diry;
	}
	x += dir.dirx;
	y += dir.diry;
}
void Point::erase() const {
	gotoxy(x, y);
	std::cout << ' ';
}
void Point::set(int new_x, int new_y) {
	if (new_x >= 0 && new_x <= Screen::WIDTH) {
		x = new_x;
	}
	if (new_y >= 0 && new_y <= Screen::HEIGHT) {
		y = new_y;
	}
}