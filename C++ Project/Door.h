#pragma once
#include "Player.h"
class Player; // forward declaration
class Screen; // forward declaration
class RoomScreenManager; // forward declaration

class Door
{
private:
	int id; // door number 1-9
	bool open; //  is the door open ?
	int destinationLevel; // level to which the door leads

public:
	Door() : id(-1), open(false), destinationLevel(-1) {}
	Door(int doorID,int dest=-1): id(doorID),open(false),destinationLevel(dest){}
	
	int getId() const;
	bool isOpen() const;
	bool tryOpen(int keyId);
	bool canPass() const;
	static bool isDoorChar(char c);
	int getDestinationLevel() const; 
	void setDestinationLevel(int level);
	static bool handleDoor(Player& p, Screen& screen, int& currentLevel, char& foundDoor);
	static Point findLocation(const Screen& screen, int doorIndex);

};
