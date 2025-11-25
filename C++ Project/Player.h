#pragma once
#include "Point.h"
#include "Screen.h"

class Player
{
private:
	static constexpr size_t NUM_KEYS = 6;
	Point position;
	char keys[NUM_KEYS];
	Screen& screen;
public:
	Player(const Point& start_point, const char(&the_keys)[NUM_KEYS], Screen& theScreen)
		:position(start_point), screen(theScreen)
	{
		for (size_t i = 0; i < NUM_KEYS; ++i) {
			keys[i] = the_keys[i];
		}
	}
	void draw() {
		position.draw();
	}
	void move();
	void keyPressed(char ch);
	int getX() const {
		return position.getX();
	}
	int getY() const {
		return position.getY();
	}
	char getChar() const {
		return position.getChar();
	}
	void erase() const;
};



