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
