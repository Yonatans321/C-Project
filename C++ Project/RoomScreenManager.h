#pragma once
#include <iostream>
#include "Point.h"
#include "Player.h"
#include <windows.h> 

class RoomScreenManager
{
public:
	static constexpr int WIDTH = 80;
	static constexpr int HEIGHT = 25;

	// Draw the room layout-MAP
	void drawRoom(const char* room[]);
	void setCharAt(const Point& p, char ch);
	void setCharAt(int x, int y, char ch);
	// Draw the status bar at the bottom
	void drawStatusBar(char inventory, int health, int score);

	// Text windows for Riddles and Messages
	void drawBox(int x, int y, int width, int height);
	void clearBox(int x, int y, int width, int height);
	void drawAnimatedBox(int x, int y, int w, int h);
	void closeAnimatedBox(int x, int y, int w, int h);
	void printInBox(int x, int y, const std::string& message);
};
