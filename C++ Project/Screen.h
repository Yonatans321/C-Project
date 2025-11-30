#pragma once
#include <iostream>
#include "Point.h"
#include "Door.h"

class Screen {
public:
	static constexpr int MAX_X = 80;
	static constexpr int MAX_Y = 25;


private:
	 char screen[MAX_Y][MAX_X+1] = {
		//   01234567890123456789012345678901234567890123456789012345678901234567890123456789
			"WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW", // 0
			"W                                                                              W", // 1
			"W                                                                              W", // 2
			"W                                                                              W", // 3
			"W                                                                              W", // 4
			"W                                                                              W", // 5
			"W                                                                              W", // 6
			"W                                                                              W", // 7
			"W                                                                              W", // 8
			"W                                                                              W", // 9
			"W                                                                              W", // 10
			"W                                                                              W", // 11
			"W                                                                              W", // 12
			"W                                                                              W", // 13
			"W                                                                              W", // 14
			"W                                                                              W", // 15
			"W                                                                              W", // 16
			"W                                                                              W", // 17
			"W                                                                              W", // 18
			"W                                                                              W", // 19
			"W                                                                              W", // 20
			"W                                                                              W", // 21
			"W                                                                              W", // 22
			"W                                                                              W", // 23
			"WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW"  // 24
	};
	Door doors[10]; // array to hold doors

public:
	char getCharAt(const Point& p) const;
	void setCharAt(const Point& p, char ch);

	void draw() const;
	bool isWall(const Point& p) const;
	bool isObstacle(const Point& p) const;

	void setdoor(int id,int destlevel) {
		
		if (id >= 1 && id <= 9) {
		 doors[id] = Door(id, destlevel);
			
		}
	}
	Door* getDoor(const Point& p);
};