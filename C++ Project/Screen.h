#pragma once
#include <iostream>
#include <array>
#include <string>
#include <vector>
#include "Point.h"
#include "Door.h"
#include "Utils.h"

class Player; // forward declaration

// class to manage the screen layout and interactions
class Screen {
public:
	static constexpr int WIDTH = 80;
	static constexpr int HEIGHT = 25;
	static constexpr int MAP_HEIGHT = 23;
	static constexpr int NUM_MAPS = 4;
	
private:
	char screen[HEIGHT][WIDTH + 1];
	Door doors [10]; // array to hold doors
	
	bool dark = false; // is the screen dark
	Point legendPos;
	void clearScreenBuffer();
	bool isLegendPositionValid(int x, int y, const std::string& filename);
public:
	Screen();

	// Map drawing
	bool loadMapFromFile(const std::string& filename); // returns true if loaded successfully
	//void loadMap(int level);
	void drawMap() const;

	void drawMapWithTorch(const Player& p1) const;
	bool isDark() const;
	void setDark(bool isDark);
	void drawDark() const;
	static void resetTorchState();
	// Text drawing
	void setCharAt(const Point& p, char ch);
	void setCharAt(int x, int y, char ch);
	void drawStatusBar(char inv1, int lives1, int score1,char inv2, int lives2, int score2);static void drawBox(int x, int y, int width, int height);
	static void clearBox(int x, int y, int width, int height);
	static void drawAnimatedBox(int x, int y, int w, int h);
	static void closeAnimatedBox(int x, int y, int w, int h);
	void printInBox(int x, int y, const std::string& message);
	void showMessage(const std::string& msg);

	//Map logic
	char getCharAt(const Point& p) const;
	char getCharAt(int x, int y) const;
	bool isWall(const Point& p) const;
	bool isObstacle(const Point& p) const;
	Door* getDoor(const Point& p);

	//Map data
	int getWidth() const;
	int getHeight() const;
	const char* getMapRow(int row) const; // get a specific row of the map
	Point getLegendPos() const { return legendPos; } // Getter ìîé÷åí ä-Legend

	void applyColor(char c) const;
};