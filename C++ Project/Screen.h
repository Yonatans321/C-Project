#pragma once
#include <iostream>
#include "Point.h"
#include "Door.h"
#include <array>
#include "RoomScreenManager.h"

class Screen {
public:
	static constexpr int MAX_X = 80;
	static constexpr int MAX_Y = 23;
	static constexpr int NUM_MAPS = 4;

private:
	static const char* const MAP_LAYOUTS[NUM_MAPS][MAX_Y];// predefined map layouts
	char screen[MAX_Y][MAX_X + 1]; 
	Door doors [10]; // array to hold doors

public:
	
	
	char getCharAt(const Point& p) const;
	char getCharAt(int x, int y) const;
	void setCharAt(const Point& p, char ch);
	void setCharAt(int x, int y, char ch);
	void draw() const;
	bool isWall(const Point& p) const;
	bool isObstacle(const Point& p) const;
	
	void loadData(int mapIndex);
	void setDoor(int id, int destlevel);
	Door* getDoor(const Point& p);
};