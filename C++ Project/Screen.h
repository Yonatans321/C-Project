#pragma once
#include <iostream>
#include <array>
#include <string>
#include <vector>
#include "Point.h"
#include "Door.h"
#include "Utils.h"
#include "RoomMeta.h"


enum class RoomLevel {
	ROOM_1 = 0,      // room 1 leadsTo=1
	ROOM_2 = 1,      // room 2 leadsTo=1
	ROOM_3 = 2,      // room 3 leadsTo=1
	ROOM_4 = 3,      //  WIN SCREEN
	NUM_ROOMS = 4,   // total number of rooms
	INVALID = -1     // invalid room
};

class Player; // forward declaration

// class to manage the screen layout and interactions
class Screen {
public:
	static constexpr int WIDTH = 80;
	static constexpr int HEIGHT = 25;
	static constexpr int MAP_HEIGHT = 23;
	static constexpr int NUM_MAPS = 4;
	static constexpr int MAX_DOORS = 10;

private:
	
	Door doors [10]; // array to hold doors
	bool dark = false; // is the screen dark
	Point legendPos; // position of the legend
	void clearScreenBuffer(); // clears the screen buffer
	bool isLegendPositionValid(int x, int y, const std::string& filename); // checks if the legend position is valid
	RoomMeta roomMeta; // metadata for the room

public:
	Screen();
	char screen[HEIGHT][WIDTH + 1];
	// Map drawing
	bool loadMapFromFile(const std::string& filename); // returns true if loaded successfully
	//void loadMap(int level);
	void drawMap() const;
	Point getLegendPos() const { return legendPos; }
	void drawMapWithTorch(const Player& p1) const;
	bool isDark() const;
	void setDark(bool isDark);
	void drawDark() const;
	static void resetTorchState();
	// Text drawing
	void setCharAt(const Point& p, char ch);
	void setCharAt(int x, int y, char ch);
	void setCharAtSilent(int x, int y, char ch);
	void drawStatusBar(char inv1, int lives1, int score1,char inv2, int lives2, int score2 ,int timeRemaining, int gameTimer=-1);
	static void drawBox(int x, int y, int width, int height);
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
	Door* getDoorById(int doorId);
	//Map data
	int getWidth() const;
	int getHeight() const;
	const char* getMapRow(int row) const; // get a specific row of the map

	void applyColor(char c) const;

	RoomMeta getRoomMeta() const
	{
		return roomMeta;
	}

	void setRoomMeta(const RoomMeta& meta)
	{
		roomMeta = meta;
	}
	bool validateMetadata(const std::string& filename);
	bool validateKey(const std::string& filename);
	bool validateDoors (const std::string& filename);
};