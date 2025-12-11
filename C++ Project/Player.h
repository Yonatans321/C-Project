#pragma once
#include "Point.h"
#include <cstddef> //for size_t

// forward declarations
class Screen; 
class Key;   

class Player
{
private:
	static constexpr size_t NUM_KEYS = 6; //numbers of controlled keys
	Point position; // current pos
	Point prevPos; // prev pos
	char keys[NUM_KEYS]; // keys for controlling player
	Screen* screen;// current screen
	char heldItem; // current held item
	int itemId; // id of the held item
	int points=0; // score
	int lives=3; // lives
	Key* myKey= nullptr; // player's key
	bool hasKey = false; // does the player have a key
	bool active = true;// does the player is active

	
public:
	//constructor
	Player(const Point& start_point, const char(&the_keys)[NUM_KEYS])
		:position(start_point), screen(nullptr), heldItem(0), itemId(-1),points(0),
		lives(3),myKey(nullptr),hasKey(false),active(true)
	{
		// initialize control keys
		for (size_t i = 0; i < NUM_KEYS; ++i) {
			keys[i] = the_keys[i];
		}
	}

	// moving and drawing functions
	void draw(); // 
	void move();
	void keyPressed(char ch);
	void erase() const;
	int getX() const;
	int getY() const;
	char getChar() const;
	const Point getPosition() const { return position; }// get player's current position
	void setScreen(Screen& newScreen);// set player's current screen
	void rememberPosition();
	void stepBack();// move player back to previous position

	// Item functions
	bool hasItem() const;
	bool hasItem(char item) const;
	char getHeldItem() const;
	int getItemId() const;
	void dropHeldItem();
	void DropItem();
	void GrabItem(char item, int id = -1);
	void setPosition(const Point& pos);// set player's position

	// Key functions
	void setKey(bool val) { hasKey = val; }
	bool useKeyForDoor(char doorChar) const;
	void keyUsed();
	//bool hasKeyForDoor(char doorChar) const;
	
	//Directions functions
	Direction getDirection() const
	{
		return position.getDirection();
	}
	Direction getOppositeDirection() const
	{
		return position.getOppositeDirection();
	}
	// Score and Lives functions
	void addPoints(int pts);
	void losePoints(int pts);
	int getScore() const;
	void addLives();
	int getLives() const;
	void loseLife();
	bool isDead() const;

	// Player active status
	bool isActive() const { return active; }
	void setInactive() { active = false; }
	void activate() {active = true;}
};

