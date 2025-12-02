#include "RoomScreenManager.h"
#include <iostream>
#include "Utils.h"

using namespace std;

void RoomScreenManager::drawRoom(const char* room[])
{
	cls();
	for (int y = 0; y < HEIGHT-1; ++y)
	{
		gotoxy(0, y);
		cout << room[y];
	}
}

void RoomScreenManager::drawStatusBar(char inventory, int health, int score)
{
	gotoxy(0, HEIGHT);
	cout << "Inventory: " << inventory << "  Health: " << health << "  Score: " << score << "                              ";
}

void RoomScreenManager::setCharAt(const Point& p, char ch)
{
	gotoxy(p.getX(), p.getY());
	cout << ch;
}
void RoomScreenManager::setCharAt(int x, int y, char ch)
{
	gotoxy(x, y);
	cout << ch;
}

void RoomScreenManager::drawBox(int x, int y, int width, int height)
{
	for (int row = 0; row < height; row++)
	{
		gotoxy(x, y + row);
		for (int col = 0; col < width; col++)
		{
			if (row == 0 || row == height - 1)
				cout << "-";
			else if (col == 0 || col == width - 1)
				cout << "|";
			else
				cout << " ";
		}
	}
}

void RoomScreenManager::clearBox(int x, int y, int width, int height)
{
	for (int row = 0; row < height; row++)
	{
		gotoxy(x, y + row);
		for (int col = 0; col < width; col++)
		{
			cout << " ";
		}
	}
}
void RoomScreenManager::drawAnimatedBox(int x, int y, int w, int h)
{
	for (int i = 0; i <= h; i += 2)
	{
		drawBox(x, y, w, i);
		Sleep(50);
	}
}

void RoomScreenManager::closeAnimatedBox(int x, int y, int w, int h)
{
	for (int i = 0; i <= h; i += 2)
	{
		drawBox(x, y, w, i);
		Sleep(50);
	}
	clearBox(x, y, w, h);
}

void RoomScreenManager::printInBox(int x, int y, const std::string& message)
{
	int boxWidth = 50;
	int boxHeight = 10;
	int startX = x + 2;
	int startY = y + 2;
	int msgIndex = 0;
	for (int row = 0; row < boxHeight - 4 && msgIndex < message.length(); row++)
	{
		gotoxy(startX, startY + row);
		for (int col = 0; col < boxWidth - 4 && msgIndex < message.length(); col++)
		{
			cout << message[msgIndex++];
		}
	}
}