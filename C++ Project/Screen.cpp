#include "Screen.h"
#include "Rooms.h"
#include "Player.h"
#include <cstring>
#include <windows.h>
#include <cmath>

static int torchLastX = -1;
static int torchLastY = -1;
//   CONSTRUCTOR
Screen::Screen()
{
    for (int y = 0; y < MAP_HEIGHT; y++)
        for (int x = 0; x < WIDTH; x++)
            screen[y][x] = ' ';

    // init doors
    for (int i = 0; i < 10; i++)
        doors[i] = Door();
}

//   LOAD MAP
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


//   DRAW MAP
void Screen::drawMap() const
{
    for (int y = 0; y < MAP_HEIGHT; y++)
    {
        gotoxy(0, y);

        for (int x = 0; x < WIDTH; x++)
        {
            char c = screen[y][x];

            applyColor(c);
            std::cout << c;
        }
    }
    resetColor();
}




//   DRAWING HELPERS
void Screen::setCharAt(int x, int y, char ch)
{
    if (x < 0 || x >= WIDTH || y < 0 || y >= MAP_HEIGHT)
        return;

    screen[y][x] = ch;
    gotoxy(x, y);

    applyColor(ch);
    std::cout << ch;
    resetColor();
}
void Screen::setDark(bool isDark)
{
    dark = isDark;
}
bool Screen::isDark() const
{
    return dark;
}

void Screen::drawMapWithTorch(const Player& p) const
{
    if (p.getHeldItem() != '!')
    {
        drawDark();
        resetTorchState();
        return;
    }

    const int R = 6;

    int cx = p.getX();
    int cy = p.getY();

    // אם השחקן זז - נקה את ההילה הישנה
    if (torchLastX != -1 && (torchLastX != cx || torchLastY != cy))
    {
        for (int y = torchLastY - R; y <= torchLastY + R; y++)
        {
            if (y < 0 || y >= MAP_HEIGHT) continue;

            for (int x = torchLastX - R; x <= torchLastX + R; x++)
            {
                if (x < 0 || x >= WIDTH) continue;

                int dx = x - torchLastX, dy = y - torchLastY;
                if (dx * dx + dy * dy <= R * R)
                {
                    gotoxy(x, y);
                    std::cout << ' ';
                }
            }
        }
    }

    // צייר את ההילה החדשה
    for (int y = cy - R; y <= cy + R; y++)
    {
        if (y < 0 || y >= MAP_HEIGHT) continue;

        for (int x = cx - R; x <= cx + R; x++)
        {
            if (x < 0 || x >= WIDTH) continue;

            int dx = x - cx, dy = y - cy;
            if (dx * dx + dy * dy <= R * R)
            {
                gotoxy(x, y);
                char c = screen[y][x];
                applyColor(c);
                std::cout << c;
                resetColor();
            }
        }
    }

    torchLastX = cx;
    torchLastY = cy;
}

void Screen :: drawDark() const
{
    for (int y = 0; y < MAP_HEIGHT; y++)
    {
        gotoxy(0, y);
        for (int x = 0; x < WIDTH; x++)
        {
            std::cout << ' ';
        }
    }
}

void Screen::resetTorchState()
{
    torchLastX = -1;
    torchLastY = -1;
}
void Screen::setCharAt(const Point& p, char ch)
{
    setCharAt(p.getX(), p.getY(), ch);
}
// DRAW STATUS BAR
void Screen::drawStatusBar(
    char inv1, int lives1, int score1,
    char inv2, int lives2, int score2)
{
    // player 1 - first line
    gotoxy(0, MAP_HEIGHT);
    setColor(COLOR_LIGHT_CYAN);
    std::cout
        << "P1 & - Lives: " << lives1
        << "  Inv:" << (inv1 == 0 ? '-' : inv1)
        << "  Score: " << score1
        << "                                        ";

    // player 2 - second line
    gotoxy(0, MAP_HEIGHT + 1);
    std::cout
        << "P2 $ - Lives: " << lives2
        << "  Inv:"  << (inv2 == 0 ? '-' : inv2)
        << "  Score: " << score2
        << "                                        ";
    resetColor();
}
void Screen::drawBox(int x, int y, int w, int h)
{
    for (int row = 0; row < h; row++)
    {
        gotoxy(x, y + row);
        for (int col = 0; col < w; col++)
        {
            if (row == 0 || row == h - 1)
                std::cout << "-";
            else if (col == 0 || col == w - 1)
                std::cout << "|";
            else
                std::cout << " ";
        }
    }
}
//CLEAR BOX
void Screen::clearBox(int x, int y, int w, int h)
{
    for (int row = 0; row < h; row++)
    {
        gotoxy(x, y + row);
        for (int col = 0; col < w; col++)
            std::cout << " ";
    }
}
// DRAW ANIMATED BOX
void Screen::drawAnimatedBox(int x, int y, int w, int h)
{
    for (int i = 0; i <= h; i += 2)
    {
        drawBox(x, y, w, i);
        Sleep(30);
    }
}
// CLOSE ANIMATED BOX
void Screen::closeAnimatedBox(int x, int y, int w, int h)
{
    for (int i = h; i >= 0; i -= 2)
    {
        drawBox(x, y, w, i);
        Sleep(100);
    }
    clearBox(x, y, w, h);
}
// PRINT SOME TXT IN BOX (helped by AI)
void Screen::printInBox(int x, int y, const std::string& msg)
{
    size_t idx = 0;
    const int innerW = 50;
    const int innerH = 10;

    for (int row = 0; row < innerH && idx < msg.length(); row++)
    {
        gotoxy(x + 2, y + 2 + row);
        for (int col = 0; col < innerW && idx < msg.length(); col++)
        {
            std::cout << msg[idx++];
        }
    }
}
// SHOW MESSAGE (helped by AI)
void Screen::showMessage(const std::string& msg)
{
    int x = 20, y = 10, w = 40, h = 5;

    drawAnimatedBox(x, y, w, h);
    gotoxy(x + 2, y + 2);
    std::cout << msg;
    Sleep(1200);
    closeAnimatedBox(x, y, w, h);
}

//   MAP LOGIC
char Screen::getCharAt(int x, int y) const
{
    if (x < 0 || x >= WIDTH || y < 0 || y >= MAP_HEIGHT)
        return ' ';
    return screen[y][x];
}
//overload getcharat
char Screen::getCharAt(const Point& p) const
{
    return getCharAt(p.getX(), p.getY());
}
// checks if wall
bool Screen::isWall(const Point& p) const
{
    char c = getCharAt(p);
    return (c == 'W' || c == 'w');
}
// checks if obstacle
bool Screen::isObstacle(const Point& p) const
{
    return getCharAt(p) == '*';
}
// get door at point
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
// MAP DATA
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

void Screen::applyColor(char c) const
{
    switch (c)
    {
    case '\\':   // switch OFF
        setColor(COLOR_LIGHT_RED);
        break;

    case '/':    // switch ON
        setColor(COLOR_LIGHT_GREEN);
        break;

    case 's':    // temporary wall
        setColor(COLOR_YELLOW);
        break;

    case '*':    // obstacle
        setColor(COLOR_GRAY);
        break;

    case '?':    // riddle
        setColor(COLOR_LIGHT_PURPLE);
        break;

    case 'K':    // key
        setColor(COLOR_GOLD);
        break;
	case '@':    // Bomb
        setColor(COLOR_CYAN);
        break;
	case '!':    // Torch
        setColor(COLOR_RED);
        break;

    default:
        if (c >= '1' && c <= '9')   // door
            setColor(COLOR_LIGHT_CYAN);
        else
            resetColor();
        break;
    }
}