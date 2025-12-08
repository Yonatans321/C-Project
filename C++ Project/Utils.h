#pragma once

#define COLOR_YELLOW       14
#define COLOR_LIGHT_CYAN   11
#define COLOR_LIGHT_GREEN  10
#define COLOR_LIGHT_RED    12
void gotoxy(int x, int y);

void hideCursor();

void cls();

void setScreen(int width, int height);

void setColor(int color);

void resetColor();

void clearInputBuffer();