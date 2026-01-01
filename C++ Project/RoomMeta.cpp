#include "RoomMeta.h"
#include <sstream>

// ---------- Helper functions ----------

// Converts string value to boolean (used only while reading metadata)
static bool stringToBool(const std::string& value)
{
    return value == "true" || value == "TRUE";
}

// ---------- RoomMeta implementation ----------

RoomMeta::RoomMeta()
{
    clear();
}

void RoomMeta::clear()
{
    dark = false;
    keyOpensDoor = -1;
    riddleCount = 0;
    for (int i = 0; i < 10; i++)
        doorOpen[i] = false;
}

void RoomMeta::loadFromLine(const std::string& line)
{
    // Skip empty lines and non-metadata lines
    if (line.empty() || line[0] != '#')
        return;

    std::istringstream iss(line);
    std::string hash, key;

    iss >> hash >> key;

    // ----- DARK -----
    if (key == "DARK")
    {
        std::string value;
        iss >> value;
        dark = stringToBool(value);
    }

    // ----- KEY -----
    else if (key == "KEY")
    {
        // Expected format: # KEY opens=1
        std::string token;
        iss >> token;

        if (token.find("opens=") == 0)
            keyOpensDoor = token.back() - '0';
    }

    // ----- DOOR -----
    else if (key == "DOOR")
    {
        // Expected format: # DOOR id=1 state=open
        std::string token;
        int id = -1;
        bool open = false;

        while (iss >> token)
        {
            if (token.find("id=") == 0)
                id = token.back() - '0';
            else if (token.find("state=") == 0)
                open = (token.find("open") != std::string::npos);
        }

        if (id >= 0 && id < 10)
            doorOpen[id] = open;
    }
	// ----- RIDDLE -----
    else if (key == "RIDDLE")
    {
        // Expected format: # RIDDLE x y id
        // Example: # RIDDLE 4 22 1
        int x, y, riddleID;
        if (iss >> x >> y >> riddleID)
        {
            addRiddlePosition(riddleID, x, y);
        }
    }
    // Future metadata (SWITCH, etc.) will be added here
}

// ---------- Setters ----------

void RoomMeta::setDark(bool value)
{
    dark = value;
}

void RoomMeta::setDoorOpen(int id, bool open)
{
    if (id >= 0 && id < 10)
        doorOpen[id] = open;
}

void RoomMeta::setKeyOpens(int id)
{
    keyOpensDoor = id;
}

// ---------- Getters ----------

bool RoomMeta::isDark() const
{
    return dark;
}

bool RoomMeta::isDoorOpen(int id) const
{
    if (id >= 0 && id < 10)
        return doorOpen[id];
    return false;
}

int RoomMeta::getKeyOpens() const
{
    return keyOpensDoor;
}



// ---------- Riddle position management ----------
void RoomMeta::addRiddlePosition(int riddleID, int x, int y)
{
    if (riddleCount < 20)
    {
        riddlePositions[riddleCount].riddleID = riddleID;
        riddlePositions[riddleCount].x = x;
        riddlePositions[riddleCount].y = y;
        riddleCount++;
    }
}

int RoomMeta::getRiddleCount() const
{
    return riddleCount;
}

int RoomMeta::getRiddleID(int index) const
{
    if (index >= 0 && index < riddleCount)
        return riddlePositions[index].riddleID;
    return -1;
}

int RoomMeta::getRiddleX(int index) const
{
    if (index >= 0 && index < riddleCount)
        return riddlePositions[index].x;
    return -1;
}

int RoomMeta::getRiddleY(int index) const
{
    if (index >= 0 && index < riddleCount)
        return riddlePositions[index].y;
    return -1;
}