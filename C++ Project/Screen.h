#pragma once
#include <iostream>
#include <array>
#include <string>
#include "Point.h"
#include "Door.h"
#include "Utils.h"

// class to manage the screen layout and interactions
class Screen {
public:
	static constexpr int WIDTH = 80;
	static constexpr int HEIGHT = 25;
	static constexpr int MAP_HEIGHT = 23;
	static constexpr int NUM_MAPS = 4;

private:
	char screen[MAP_HEIGHT][WIDTH + 1];
	Door doors [10]; // array to hold doors

	static const char* const MAP_LAYOUTS[NUM_MAPS][MAP_HEIGHT];
public:
	Screen();

	// Map drawing
	void loadMap(int level);
	void drawMap() const;

	// Text drawing
	void setCharAt(const Point& p, char ch);
	void setCharAt(int x, int y, char ch);
	void drawStatusBar(char inventory, int health, int score);
	void drawBox(int x, int y, int width, int height);
	void clearBox(int x, int y, int width, int height);
	void drawAnimatedBox(int x, int y, int w, int h);
	void closeAnimatedBox(int x, int y, int w, int h);
	void printInBox(int x, int y, const std::string& message);
	void showMessage(const std::string& msg);

	//Map logic
	char getCharAt(const Point& p) const;
	char getCharAt(int x, int y) const;
	bool isWall(const Point& p) const;
	bool isObstacle(const Point& p) const;
	Door* getDoor(const Point& p);

	const char* getMapRow(int row) const; // get a specific row of the map
	/*void draw() const;
	void loadData(int mapIndex);
	void setDoor(int id, int destlevel);*/
};