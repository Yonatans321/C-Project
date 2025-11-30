#include <iostream>
#include "Screen.h"
#include "Utils.h"

//Taken from Lab exercise 10.11.25
void Screen::draw() const {
	int y = 0;
	for (const auto& row : screen) {
		gotoxy(0, y++);
		std::cout << row << std::flush;
	}
}

// get character at point p
char Screen::getCharAt(const Point& p) const
{
	
	if (p.getX() >= 0 && p.getX() < MAX_X && p.getY() >= 0 && p.getY() < MAX_Y) {
		return screen[p.getY()][p.getX()];
	}
	return ' ';
}
// set character at point p
void Screen::setCharAt(const Point& p, char ch)
{
	if (p.getX() >= 0 && p.getX() < MAX_X && p.getY() >= 0 && p.getY() < MAX_Y) {
		
		screen[p.getY()][p.getX()] = ch;

		gotoxy(p.getX(), p.getY());
		std::cout << ch;
	}
}


bool Screen::isWall(const Point& p) const // check if the point is a wall
{
	if (getCharAt(p) == 'W'|| getCharAt(p) == 'w')
	{
		return true;
	}
		return false;
}
// check if the point is an obstacle
bool Screen::isObstacle(const Point& p) const
{
		return getCharAt(p) == '*';
}
// get door at point p
Door* Screen::getDoor(const Point& p)
{
	char ch = getCharAt(p);

	if (!Door::isDoorChar(ch))
	{
		return nullptr; // Not a door
	}
	int id = ch - '0';// Convert char to door id
	if(id >= 1 && id <= 9) {
		return &doors[id];
	}
	return nullptr;
}