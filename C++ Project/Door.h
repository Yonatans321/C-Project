#pragma once
#include "Player.h"
class Player; // forward declaration
class Screen; // forward declaration
class Door
{
private:
	int id; // door number 1-9
	bool open; //  is the door open ?
	int destinationLevel; // level to which the door leads
public:
	Door() : id(-1), open(false), destinationLevel(-1) {}
	Door(int doorID,int dest=-1): id(doorID),open(false),destinationLevel(dest){}
	
	static bool switchesAreOn;
	static bool openDoors[10];
	//GET DATA
	int getId() const;
	bool isOpen() const;
	int getDestinationLevel() const;
	void setDestinationLevel(int level);

	bool tryOpen(int keyId);
	bool canPass() const;
	static bool isDoorChar(char c);
	static bool handleDoor(Player& p, Screen& screen, int& currentLevel, char& foundDoor);
	//static bool handleDoor(Player& p, Screen& screen, int& currentLevel);
	static Point findLocation(const Screen& screen, int doorIndex);
	static void allSwitchesAreOn();
};
