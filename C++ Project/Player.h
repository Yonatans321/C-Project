#pragma once
#include "Point.h"
#include <cstddef>// for size_t

class Screen; // forward declaration

class Player
{
private:
	static constexpr size_t NUM_KEYS = 6;
	Point position;
	char keys[NUM_KEYS];
	Screen* screen;
	char heldItem;
	int itemId;
	int points=0;
	int lives=3;
public:
	Player(const Point& start_point, const char(&the_keys)[NUM_KEYS])
		:position(start_point), screen(nullptr), heldItem(0), itemId(-1)
	{
		for (size_t i = 0; i < NUM_KEYS; ++i) {
			keys[i] = the_keys[i];
		}
	}

	// moving and drawing functions
	void draw();
	void move();
	void keyPressed(char ch);
	void erase() const;
	int getX() const;
	int getY() const;
	char getChar() const;
	Point getPosition() const { return position; }// get player's current position
	void setScreen(Screen& newScreen);// set player's current screen

	// Item functions
	bool hasItem() const;
	bool hasItem(char item) const;
	char getHeldItem() const;
	int getItemId() const;
	void DropItem();
	void GrabItem(char item, int id = -1);
	bool useKeyForDoor(char doorChar);
	void setPosition(const Point& pos);// set player's position
	void keyUsed();
	//bool hasKeyForDoor(char doorChar) const;
	
	
	// Score and Lives functions
	void addPoints(int pts);
	void losePoints(int pts);
	int getScore() const;
	void addLives();
	int getLives() const;
	void loseLife();
	bool isDead() const;
};

