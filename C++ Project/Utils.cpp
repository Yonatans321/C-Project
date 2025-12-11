#include <windows.h>
#include <iostream>
#include <cstdlib>
#include "Utils.h"
#include <conio.h>
#include <limits>

using std::cout; 
using std::endl;   

//Taken from lab exercise 10.11.25
void gotoxy(int x, int y) {
    std::cout.flush();
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}
//Taken from lab exercise 10.11.25
void hideCursor() {
    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(consoleHandle, &cursorInfo);
    cursorInfo.bVisible = FALSE; // Hide the cursor
    SetConsoleCursorInfo(consoleHandle, &cursorInfo);
}
//Taken from lab exercise 10.11.25
void cls() {
    system("cls");
}

//setscreen taken from AI
void setScreen(int width, int height) {
    HANDLE hScreen = GetStdHandle(STD_OUTPUT_HANDLE);
    
    SMALL_RECT windowSize = { 0, 0, (short)(width - 1), (short)(height - 1) };
    SetConsoleWindowInfo(hScreen, TRUE, &windowSize);
  
    COORD bufferSize = { (short)width, (short)height };
    SetConsoleScreenBufferSize(hScreen, bufferSize);
}
//AI generated function to change text color in console
void setColor(int color) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}
void resetColor() {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7); // 7 is the default color
}
// clear the input from any previous key presses
void clearInputBuffer()
{
    while (_kbhit()) _getch();
}
// wait for a key press from the user
void waitForKey()
    {
        clearInputBuffer(); 
		auto i = _getch();            
    }
