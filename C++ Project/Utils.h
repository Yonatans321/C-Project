#pragma once
// declare the color constants
#define COLOR_YELLOW       14
#define COLOR_LIGHT_CYAN   11
#define COLOR_LIGHT_GREEN  10
#define COLOR_LIGHT_RED    12
void gotoxy(int x, int y);// go to the position x,y in the screen

extern bool ColorsEnabled;

void hideCursor(); //hide the console cursor

void cls();// clear the screen

void setScreen(int width, int height); // set the screen size

void setColor(int color);// set the text color

void resetColor(); // reset the text color

void clearInputBuffer(); // clear the input from any previous key presses

void waitForKey(); // wait for a key press from the user