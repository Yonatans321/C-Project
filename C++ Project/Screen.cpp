#include <iostream>
#include "Screen.h"
#include "utils.h"

void Screen::draw() const {
	int y = 0;
	for (const auto& row : screen) {
		gotoxy(0, y++);
		std::cout << row << std::flush;
	}
}

char Screen::getCharAt(const Point& p) const {
	return screen[p.getY()][p.getX()];
}
void Screen::setCharAt(const Point& p, char ch) {
	char* row = const_cast<char*>(screen[p.getY()]);
	row[p.getX()] = ch;
}
bool Screen::isWall(const Point& p) const {
		return getCharAt(p) == 'w';
}
bool Screen::isObstacle(const Point& p) const {
		return getCharAt(p) == '*';
}
//Taken from Lab exercise 10.11.25