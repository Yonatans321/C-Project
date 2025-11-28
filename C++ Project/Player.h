#pragma once
#include "Point.h"
#include "Screen.h"
#include <cstddef>
#include <cstring>
#include 
class Player
{
private:
	static constexpr size_t NUM_KEYS = 6;
	Point position;
	char keys[NUM_KEYS];
	Screen& screen;
	char heldItem;
	char itemId;
public:
	Player(const Point& start_point, const char(&the_keys)[NUM_KEYS], Screen& theScreen)
		:position(start_point), screen(theScreen), heldItem(0), itemId(-1)
	{
		for (size_t i = 0; i < NUM_KEYS; ++i) {
			keys[i] = the_keys[i];
		}
	}

	// moving and drawing functions
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

// Item functions

bool hasItem(char item) {
	// Implementation for checking if the player has the specified item
	return heldItem == item;
}
bool hasKey(char key) {
	return heldItem == key;
}
bool hasItem() const
void GrabItem(char item, int id = -1) {
	if (hasItem(item)=='1')
		return; 
void DropItem(char item);

void hasKey(char key);
{
	return inventory == key;
}

