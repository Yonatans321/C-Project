#pragma once
#include <utility>
#include <cmath>
#include "Direction.h"

class Point {
	int x, y;
	Direction dir = Direction::directions[Direction::RIGHT];
	char ch;
	// option 1:
		// enum {MAX_X = 80, MAX_Y = 25};
	// option 2:
	static constexpr int MAX_X = 79;
	static constexpr int MAX_Y = 24;

	//Taken from lab exercise number 4
public:
	Point() :x(0),y(0),dir(Direction::directions[Direction::STAY]),ch('$'){}
	Point(int x1, int y1, const Direction& the_dir, char c) {
		x = x1;
		y = y1;
		dir = the_dir;
		ch = c;
	}
	void draw() {
		draw(ch);
	}
	void draw(char c);
	void move();
	void changeDirection(const Direction& new_dir) {
		dir = new_dir;
	}
	int getX() const {
		return x;
	}
	int getY() const {
		return y;
	}
	char getChar() const {
		return ch;
	}
	void erase() const;
	void set(int new_x, int new_y); 
};
