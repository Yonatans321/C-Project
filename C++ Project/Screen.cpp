
#include "Screen.h"
#include "Rooms.h"
#include <cstring>
#include <windows.h>

using namespace std;

// =====================
//   CONSTRUCTOR
// =====================

Screen::Screen()
{
    for (int y = 0; y < MAP_HEIGHT; y++)
        for (int x = 0; x < WIDTH; x++)
            screen[y][x] = ' ';

    // init doors
    for (int i = 0; i < 10; i++)
        doors[i] = Door();
}

// =====================
//   LOAD MAP
// =====================

void Screen::loadMap(int level)
{
    const char* const* src = nullptr;

    switch (level)
    {
    case 0: src = ROOM0; break;
    case 1: src = ROOM1; break;
    case 2: src = ROOM2; break;
    default: return;
    }

    for (int y = 0; y < MAP_HEIGHT; y++)
    {
        strncpy_s(screen[y], src[y], WIDTH);
        screen[y][WIDTH] = '\0';
    }
    for (int y = 0; y < 23; y++)
    {
        for (int x = 0; x < 80; x++)
        {
            char c = screen[y][x];
            
            if (c >= '1' && c <= '9')
            {
                int id = c - '0'; 
                doors[id] = Door(id);
            }
        }
    }
}


// =====================
//   DRAW MAP
// =====================

void Screen::drawMap() const
{
    cls();

    for (int y = 0; y < MAP_HEIGHT; y++)
    {
        gotoxy(0, y);
        cout << screen[y];
    }
}

// =====================
//   DRAWING HELPERS
// =====================

void Screen::setCharAt(int x, int y, char ch)
{
    if (x < 0 || x >= WIDTH || y < 0 || y >= MAP_HEIGHT)
        return;

    screen[y][x] = ch;
    gotoxy(x, y);
    cout << ch;
}

void Screen::setCharAt(const Point& p, char ch)
{
    setCharAt(p.getX(), p.getY(), ch);
}

void Screen::drawStatusBar(char inventory, int health, int score)
{
    gotoxy(0, MAP_HEIGHT);
    cout << "Inventory: "
        << inventory
        << "  Health: " << health
        << "  Score: " << score
        << "                                        ";
}

void Screen::drawBox(int x, int y, int w, int h)
{
    for (int row = 0; row < h; row++)
    {
        gotoxy(x, y + row);
        for (int col = 0; col < w; col++)
        {
            if (row == 0 || row == h - 1)
                cout << "-";
            else if (col == 0 || col == w - 1)
                cout << "|";
            else
                cout << " ";
        }
    }
}

void Screen::clearBox(int x, int y, int w, int h)
{
    for (int row = 0; row < h; row++)
    {
        gotoxy(x, y + row);
        for (int col = 0; col < w; col++)
            cout << " ";
    }
}

void Screen::drawAnimatedBox(int x, int y, int w, int h)
{
    for (int i = 0; i <= h; i += 2)
    {
        drawBox(x, y, w, i);
        Sleep(30);
    }
}

void Screen::closeAnimatedBox(int x, int y, int w, int h)
{
    for (int i = h; i >= 0; i -= 2)
    {
        drawBox(x, y, w, i);
        Sleep(100);
    }
    clearBox(x, y, w, h);
}

void Screen::printInBox(int x, int y, const string& msg)
{
    int idx = 0;
    const int innerW = 50;
    const int innerH = 10;

    for (int row = 0; row < innerH && idx < msg.length(); row++)
    {
        gotoxy(x + 2, y + 2 + row);
        for (int col = 0; col < innerW && idx < msg.length(); col++)
        {
            cout << msg[idx++];
        }
    }
}

void Screen::showMessage(const string& msg)
{
    int x = 20, y = 10, w = 40, h = 5;

    drawAnimatedBox(x, y, w, h);
    gotoxy(x + 2, y + 2);
    cout << msg;
    Sleep(1200);
    closeAnimatedBox(x, y, w, h);
}

// =====================
//   MAP LOGIC
// =====================

char Screen::getCharAt(int x, int y) const
{
    if (x < 0 || x >= WIDTH || y < 0 || y >= MAP_HEIGHT)
        return ' ';
    return screen[y][x];
}

char Screen::getCharAt(const Point& p) const
{
    return getCharAt(p.getX(), p.getY());
}

bool Screen::isWall(const Point& p) const
{
    char c = getCharAt(p);
    return (c == 'W' || c == 'w');
}

bool Screen::isObstacle(const Point& p) const
{
    return getCharAt(p) == '*';
}

Door* Screen::getDoor(const Point& p)
{
    char ch = getCharAt(p);

    if (!Door::isDoorChar(ch))
        return nullptr;

    int id = ch - '0';
    if (id >= 1 && id <= 9)
        return &doors[id];

    return nullptr;
}
const char* Screen::getMapRow(int row) const
{
    if (row < 0 || row >= MAP_HEIGHT)
        return nullptr;
    return screen[row];
}

int Screen::getWidth() const
{
    return WIDTH;
}
int Screen::getHeight() const
{
    return MAP_HEIGHT;
}