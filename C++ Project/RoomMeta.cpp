#include "RoomMeta.h"
#include <sstream>
#include <iostream>
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
    roomKey = Key(-1);

    for (int i = 0; i < 10; i++)
    {
		doorOpen[i] = false; // all doors closed by default
		doorLeadsTo[i] = -1; // initialize to -1 (no destination)
    }
}

void RoomMeta::loadFromLine(const std::string& line) // parse a metadata line helped by AI
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
        std::string token;
        iss >> token;
        if (token.find("opens=") == 0)
        {
            std::string numStr = token.substr(6);
            if (!numStr.empty())
            {
                int doorId = std::stoi(numStr);
                roomKey = Key(doorId);
            }
        }
    }

    // ----- DOOR -----
    else if (key == "DOOR")
	{ // Expected format: # DOOR id=1 state=open leads=2
        std::string token; 
        int id = -1; 
        bool open = false;
        int leads = -1;

        while (iss >> token) // Read each token
        {
            if (token.find("id=") == 0)
            {// door ID
				std::string numStr = token.substr(3);  // take after "id="
                id = std::stoi(numStr); // Extract ID from the end of the token
            }
			// door state
            else if (token.find("state=") == 0)
				open = (token.find("open") != std::string::npos); // Check if state is "open"

			// where the door leads
            else if (token.find("leads=") == 0)
            {
                std::string numStr = token.substr(6);  // take after "leads="
                leads = std::stoi(numStr)-1;
            }
        }

		// Set door properties if ID is valid
        if (id >= 0 && id < 10)
        {
			doorOpen[id] = open;        // door state
			doorLeadsTo[id] = leads;    // destination room ID
        }
    }
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
	roomKey = Key(id);  // Create key that opens the specified door
}

int RoomMeta::getKeyOpens() const
{
	return roomKey.getDoorID();  // Return the door ID that the key opens
}

Key RoomMeta::getRoomKey() const  // getter for room key
{
    return roomKey;
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

int RoomMeta::getDoorLeadsTo(int id) const
{
    if (id >= 0 && id < 10)
        return doorLeadsTo[id];
    return -1;
}