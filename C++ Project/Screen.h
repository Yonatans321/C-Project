#pragma once
#include <iostream>
#include "Point.h"

class Screen {
public:
	static constexpr int MAX_X = 80;
	static constexpr int MAX_Y = 25;
private:
	const char* screen[MAX_Y] = {
		//   01234567890123456789012345678901234567890123456789012345678901234567890123456789
			"wwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwww", // 0
			"w                                                                              w", // 1
			"w                                                                              w", // 2
			"w                                                                              w", // 3
			"w                                                                              w", // 4
			"w                                                                              w", // 5
			"w                                                                              w", // 6
			"w                                                                              w", // 7
			"w                                                                              w", // 8
			"w                                                                              w", // 9
			"w                                                                              w", // 10
			"w                                                                              w", // 11
			"w                                                                              w", // 12
			"w                                                                              w", // 13
			"w                                                                              w", // 14
			"w                                                                              w", // 15
			"w                                                                              w", // 16
			"w                                                                              w", // 17
			"w                                                                              w", // 18
			"w                                                                              w", // 19
			"w                                                                              w", // 20
			"w                                                                              w", // 21
			"w                                                                              w", // 22
			"w                                                                              w", // 23
			"wwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwww" // 24
	};
	char getCharAt(const Point& p) const;
public:
	void draw() const;
	bool isWall(const Point& p) const;
	bool isObstacle(const Point& p) const;
};