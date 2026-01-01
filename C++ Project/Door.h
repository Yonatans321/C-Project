#pragma once
#include "Player.h"
// forward declaration
class Screen; 
class RoomMeta;
class Door
{
private:
	int id; // door number 1-9
	bool open; //  is the door open ?
	int destinationLevel; // level to which the door leads
public:
	Door() : id(-1), open(false), destinationLevel(-1) {}
	Door(int doorID, int dest = -1) : id(doorID), open(false), destinationLevel(dest) {}

	static bool switchesAreOn;
	static bool openDoors[10];

	//GET DATA
	int getId() const { return id; }
	bool isOpen() const { return open; }
	int getDestinationLevel() const { return destinationLevel; }
	void setDestinationLevel(int level) { destinationLevel = level; }

	//DOOR LOGIC
	bool tryOpen(int keyId);
	bool canPass() const { return open; }
	static bool isDoorChar(char c) { return (c >= '1' && c <= '9'); }
	static bool handleDoor(Player& p, Screen& screen, char& foundDoor);
	void setOpen() { open = true; }
   //SET AWITCHES ON
	static void allSwitchesAreOn() { switchesAreOn = true; }

};



