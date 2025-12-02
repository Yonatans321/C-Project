#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <cstring>
#include "Screen.h"
#include "Utils.h"

using namespace std;
const char* const Screen::MAP_LAYOUTS[Screen::NUM_MAPS][Screen::MAX_Y] = 
{

	// Map 0
	{//  01234567890123456789012345678901234567890123456789012345678901234567890123456789
		"WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW", // 0
		"W                                                                              W", // 1 
		"W          WWWWWWWWWW             WWWWWWWWWWWW                                 W", // 2
		"W          W        W             W          W                                 W", // 3
		"W                   W             W          W                                 W", // 4
		"W              ?    W             W    K     W                                 W", // 5
		"W          W        W             W          W                                 W", // 6
		"W          WWWWWWWWWW             WWWW  WWWWWW                                 W", // 7
		"W                                                                              W", // 8
		"W                                                                              W", // 9
		"W        WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW                          W", // 10
		"W                                                                              W", // 11
		"W                                                                              W", // 12
		"W                  WWWWWWWWWW                                                  W", // 13
		"W                  W        W                                                  W", // 14
		"W                  W   1    W                                                  W", // 15
		"W                  W        W                                                  W", // 16
		"W                  WWWWWWWWWW                                                  W", // 17
		"W                                                                              W", // 18
		"W                                                                              W", // 19
		"W                                                                              W", // 20
		"W                                                                              W", // 21
		"WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW"  // 22
	},

	// Map 1
	{
	"WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW", // 0
	"W                                                                              W", // 1
	"W                                                                              W", // 2
	"W          WWWWWWWWWWWWWWWWWWWW                                                W", // 3
	"W          W                  W                                                W", // 4
	"W          W        #         W                                                W", // 5
	"W          W                  W                                                W", // 6
	"W          WWWWWWWW  WWWWWWWWWW                                                W", // 7
	"W                  WW                                                          W", // 8
	"W                  WW                                                          W", // 9
	"W                  WW                                                          W", // 10
	"W                  WW                                                          W", // 11
	"W                  WW         WWWWWWWWWWWWWW                                   W", // 12
	"W                  WW         W            W                                   W", // 13
	"W                  WW         W     * W                                        W", // 14
	"W                             W            W                                   W", // 15
	"W                             WWWWWW  WWWWWW                                   W", // 16
	"W                                                                              W", // 17
	"W                                                                              W", // 18
	"W                                       WWWWWWWW                               W", // 19
	"W                                       W      W                               W", // 20
	"W                                       W  2   W                               W", // 21
	"WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW"  // 22
	},

	// Map 2
	{
	"WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW", // 0
	"W          W                  W                  W                             W", // 1
	"W          W                  W                  W                             W", // 2
	"WWWWWWW  WWWWWWWWWWWWWWW  WWWWWWWWWWWWWWWWW  WWWWWWWWWWWWWWWWWWWWWWWWWWWW  WWWWW", // 3
	"W                  W                  W                  W                     W", // 4
	"W        K         W        ?         W        !         W                     W", // 5
	"W                  W                  W                  W                     W", // 6
	"WWWWWWW  WWWWWWWWWWWWWWW  WWWWWWWWWWWWWWWWW  WWWWWWWWWWWWWWWWWWWWWWWWWWWW  WWWWW", // 7
	"W                                                                              W", // 8
	"W            WWWWWWWW                                                          W", // 9
	"W            W      W                                                          W", // 10
	"W            W      W                                                          W", // 11
	"W            W      W                                                          W", // 12
	"W            W      W                                                          W", // 13
	"W            WW    WW                                                          W", // 14
	"W                  WW                                                          W", // 15
	"W                  WW                                                          W", // 16
	"W                  WW                                                          W", // 17
	"W                  WW                                                          W", // 18
	"W        WWWWWWWWWWWWWWWWWWWWWW                                                W", // 19
	"W        W                    W                                                W", // 20
	"W        W         3          W                                                W", // 21
	"WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW"  // 24
	},
	// Map 3: THE WIN SCREEN 
	{
		"WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW",
		"W                                                                              W",
		"W                                                                              W",
		"W        W     W   WWWWWW   W     W      W       W   WWWWWW   W     W          W",
		"W         W   W    O    O   U     U      W       W     II     NN    N          W",
		"W          W W     O    O   U     U      W   W   W     II     N N   N          W",
		"W           W      O    O   U     U      W W W W W     II     N  N  N          W",
		"W           W      O    O   U     U      WW     WW     II     N   N N          W",
		"W           W      WWWWWW   WWWWWWW      W       W   WWWWWW   N    NN          W",
		"W                                                                              W",
		"W                                                                              W",
		"W                                                                              W",
		"W                                                                              W",
		"W                    WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW                          W",
		"W                    W      CONGRATULATIONS !!!     W                          W",
		"W                    WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW                          W",
		"W                                                                              W",
		"W                                                                              W",
		"W                                                                              W",
		"W                      (Press ESC to return to Menu)                           W",
		"W                                                                              W",
		"W                                                                              W",
		"WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW"
	}
};


// set door with id and destination level
void Screen::setDoor(int id, int destlevel)
{
	if (id >= 1 && id <= 9) {
		doors[id] = Door(id, destlevel);
	}

}
// load map data into screen
void Screen::loadData(int mapIndex) {
	if (mapIndex < 0 || mapIndex >= NUM_MAPS) {
		return;
	}
	for (int i = 0; i < MAX_Y; i++) {
		strncpy(screen[i], MAP_LAYOUTS[mapIndex][i], MAX_X);
		screen[i][MAX_X] = '\0';
	}
	
}



//Taken from Lab exercise 10.11.25 
void Screen::draw() const {
	int y = 0;
	for (int i = 0; i < MAX_Y; i++) {
		gotoxy(0, i);
		std::cout << screen[i];
	}
}

// get character at point p
char Screen::getCharAt(const Point& p) const
{
	
	if (p.getX() >= 0 && p.getX() < MAX_X && p.getY() >= 0 && p.getY() < MAX_Y) {
		return screen[p.getY()][p.getX()];
	}
	return ' ';
}
char Screen::getCharAt(int x, int y) const
{

	if (x >= 0 && x < MAX_X && y >= 0 && y < MAX_Y) {
		return screen[y][x];
	}
	return ' ';
}

// set character at point p
void Screen::setCharAt(const Point& p, char ch)
{
	if (p.getX() >= 0 && p.getX() < MAX_X && p.getY() >= 0 && p.getY() < MAX_Y) {
		
		screen[p.getY()][p.getX()] = ch;

		gotoxy(p.getX(), p.getY());
		std::cout << ch;
	}
}
void Screen::setCharAt(int x, int y, char ch)
{
	if (x >= 0 && x < MAX_X && y >= 0 && y < MAX_Y) {

		screen[y][x] = ch;

		gotoxy(x,y);
		std::cout << ch;
	}
}

bool Screen::isWall(const Point& p) const // check if the point is a wall
{
	if (getCharAt(p) == 'W'|| getCharAt(p) == 'w')
	{
		return true;
	}
		return false;
}
// check if the point is an obstacle
bool Screen::isObstacle(const Point& p) const
{
		return getCharAt(p) == '*';
}
// get door at point p
Door* Screen::getDoor(const Point& p)
{
	char ch = getCharAt(p);

	if (!Door::isDoorChar(ch))
	{
		return nullptr; // Not a door
	}
	int id = ch - '0';// Convert char to door id
	if(id >= 1 && id <= 9) {
		return &doors[id];
	}
	return nullptr;
}


	