#include "Screen.h"
#include "Rooms.h"
#include "Player.h"
#include <cstring>
#include <windows.h>
#include <cmath>
#include <filesystem>
#include <fstream>


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
// CLEAR SCREEN BUFFER
void Screen::clearScreenBuffer() {
    // reset screen 
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            screen[y][x] = ' ';
        }
        screen[y][WIDTH] = '\0';
    }
}

// LOAD MAP FROM FILE helper function to load map and metadata from file- helped by AI
bool Screen::loadMapFromFile(const std::string& filename)
{
    roomMeta.clear();
    std::ifstream file(filename);
	if (!file.is_open()) { // file open error
        std::cout << "DEBUG: Failed to open file: " << filename << std::endl;
        return false;
    }
    legendPos = Point(-1, -1);
    clearScreenBuffer();

    std::string line;
    int y = 0;

    // ---------- READ MAP + METADATA (single pass) ----------
    while (std::getline(file, line))
    {
        // ----- METADATA -----
        if (!line.empty() && line[0] == '#')
        {
            roomMeta.loadFromLine(line);
            continue;
        }

        // ----- MAP -----
        if (y >= HEIGHT)
            break;

        for (int x = 0; x < WIDTH && x < (int)line.length(); x++)
        {
            char c = line[x];
            if (c == 'L')
            {
                if (!isLegendPositionValid(x, y, filename)) {
                    file.close();
                    return false;
                }
                legendPos = Point(x, y);
                screen[y][x] = ' ';
            }
            else
            {
                screen[y][x] = c;
            }
        }
        y++;
    }

    file.close();

	if (legendPos.getX() == -1) { // 'L' not found
        std::cout << "\n\n 'L' (Legend) is out of boundaries or not exist in: "
            << filename << std::endl;
        return false;
    }

    // ---------- BUILD DOORS FROM MAP ----------
    for (int i = 0; i < 10; i++)
        doors[i] = Door();

    for (int ty = 0; ty < MAP_HEIGHT; ty++) {
        for (int tx = 0; tx < WIDTH; tx++) {
            char c = screen[ty][tx];
            if (c >= '1' && c <= '9') {
                int id = c - '0';
                bool shouldBeOpen = roomMeta.isDoorOpen(id) || Door::openDoors[id];
                doors[id] = Door(id);
                if (shouldBeOpen) {
                    doors[id].setOpen();
                    Door::openDoors[id] = true;
                }
                int dest = roomMeta.getDoorLeadsTo(id);
                if (dest >= 0) {
                    doors[id].setDestinationLevel(dest);
                }
            }
        }
    }

    // ---------- APPLY DARK ----------
    setDark(roomMeta.isDark());

    // ---------- VALIDATE METADATA ----------
    if (!validateMetadata(filename))
        return false;

    return true;
}

// Helper function to validate key metadata
bool Screen::validateKey(const std::string& filename)
{
    int keyOpensId = roomMeta.getKeyOpens();
    if (keyOpensId == -1)
		return true; // no key defined, nothing to validate

	// check that the door the key opens exists in the map
    bool doorExists = false;
    for (int i = 1; i <= 9; i++)
    {
        if (doors[i].getId() == keyOpensId)
        {
            doorExists = true;
            break;
        }
    }

    if (!doorExists)
    {
        std::cout << "\n\nERROR in " << filename << std::endl;
        std::cout << "KEY OPENS=" << keyOpensId
            << " - but door " << keyOpensId << " doesn't exist in map!" << std::endl;
        return false;
    }

    return true;
}

// Helper function to validate door metadata
bool Screen::validateDoors(const std::string& filename)
{
    for (int i = 1; i <= 9; i++)
    {
        int leadsTo = roomMeta.getDoorLeadsTo(i);

		// if LEADS is not defined, skip
        if (leadsTo == -1)
            continue;

		// check that the door exists in the map
        bool doorExistsInMap = false;
        for (int y = 0; y < MAP_HEIGHT; y++)
        {
            for (int x = 0; x < WIDTH; x++)
            {
                if (screen[y][x] == ('0' + i))
                {
                    doorExistsInMap = true;
                    break;
                }
            }
            if (doorExistsInMap) break;
        }

        if (!doorExistsInMap)
        {
            std::cout << "\n\nERROR in " << filename << std::endl;
            std::cout << "DOOR ID=" << i
                << " defined in metadata but doesn't exist in map!" << std::endl;
            return false;
        }

		// check that LEADS points to a valid room number
        if (leadsTo < 0 || leadsTo >= 4)
        {
            std::cout << "\n\nERROR in " << filename << std::endl;
            std::cout << "DOOR ID=" << i << " LEADS=" << (leadsTo + 1)
                << " - invalid room number! Only rooms 1-4 exist." << std::endl;
            return false;
        }
    }
    for (int i = 1; i <= 9; i++)
    {
        if (roomMeta.getDoorLeadsTo(i) != -1)
        {
			if (doors[i].getId() == -1)  // state not defined
            {
                std::cout << "\n\nERROR in " << filename << std::endl;
                std::cout << "DOOR ID=" << i << " has invalid state!" << std::endl;
                std::cout << "Valid states are: \"open\" or \"closed\"" << std::endl;
                return false;
            }
        }
    }

    return true;
}

// Main validation function
bool Screen::validateMetadata(const std::string& filename)
{
    if (!validateKey(filename))
        return false;

    if (!validateDoors(filename))
        return false;

    return true;
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

bool Screen::isLegendPositionValid(int x, int y, const std::string& filename) // helper function to validate legend position
{
	if (x < 0 || x >= WIDTH - 1 || y < 0 || y >= HEIGHT - 1) { // out of bounds
        std::cout << "\n\nError: Legend (L) is out of 80x25 screen bounds in:" << filename << std::endl;
    return false;
}
	if (y < MAP_HEIGHT) { // legend must be below game arena
		std::cout << "\n\nError: Legend (L) must be below the game arena in:" << filename << std::endl;
        return false;
    }
	else if (x + 41 >= WIDTH) { // legend too far right
        std::cout << "\n\nError: Legend (L) is too far right in:" << filename << std::endl;
        return false;
	}
	return true;



}


//   DRAWING HELPERS
void Screen::setCharAt(int x, int y, char ch)
{
    if (x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT)
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

void Screen::drawMapWithTorch(const Player& p) const // draw map with torchlight effect
{
	if (p.getHeldItem() != '!') // no torch
    {
        drawDark();
        resetTorchState();
        return;
    }

    const int R = 6;

    int cx = p.getX();
    int cy = p.getY();

	// if the player moved, clear the previous halo
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
                    char c = screen[y][x];

                    if ((c >= '1' && c <= '9') || c == '!')
                    {
                        applyColor(c);
                        std::cout << c;
                        resetColor();
                    }
                    else
                    {
                        std::cout << ' ';
                    }
                }
            }
        }
    }

	// draw current halo
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

void Screen::drawDark() const // draw completely dark screen
{
    for (int y = 0; y < MAP_HEIGHT; y++)
    {
        gotoxy(0, y);
        for (int x = 0; x < WIDTH; x++)
        {
			char c = screen[y][x]; // get character at position

			if ((c >= '1' && c <= '9') || c == '!') // show doors and torches even in dark
            {
                applyColor(c);
                std::cout << c;
                resetColor();
            }
            else
            {
                std::cout << ' ';
            }
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
	// get legend position
    int startX = legendPos.getX();
    int startY = legendPos.getY();

    // player 1 - first line
    gotoxy(startX, startY);
    setColor(COLOR_LIGHT_CYAN);
    std::cout
        << "P1 & - Lives: " << lives1
        << "  Inv:" << (inv1 == 0 ? '-' : inv1)
        << "  Score: " << score1
        << "             ";

    // player 2 - second line
    gotoxy(startX, startY + 1);
    std::cout
        << "P2 $ - Lives: " << lives2
        << "  Inv:"  << (inv2 == 0 ? '-' : inv2)
        << "  Score: " << score2
        << "             ";
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

Door* Screen::getDoorById(int id)
{
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
