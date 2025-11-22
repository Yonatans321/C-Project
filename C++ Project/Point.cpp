#include <iostream>
#include "Point.h"
#include "utils.h"

void Point::draw(char c) {
	gotoxy(x, y);
	std::cout << c;
}

void Point::move() {
	if (x + dirx < 0 || x + dirx > MAX_X) {
		dirx = -dirx;
	}
	if (y + diry < 0 || y + diry > MAX_Y) {
		diry = -diry;
	}
	x += dirx;
	y += diry;
}

