#pragma once
class Door
{
private:
	int id; // door number 1-9
	bool open; //  is the door open ?
	int destinationLevel; // level to which the door leads

public:
	Door(int doorID,int dest=-1): id(doorID),open(false),destinationLevel(dest){}
	
	int getId() const;
	bool isOpen() const;
	bool tryOpen(int keyId);
	bool canPass() const;
	static bool isDoorChar(char c);
	int getDestinationLevel() const; 
	void setDestinationLevel(int level);
};

