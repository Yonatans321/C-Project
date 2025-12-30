#pragma once
#include <string>
#include <fstream>
#include "Key.h"
#include "Door.h"

class RoomMeta {
public:
    RoomMeta();

    void clear();
	//  load room metadata from a line
    void loadFromLine(const std::string& line);


    // setters
    void setDark(bool v);
    void setDoorOpen(int id, bool open);
    void setKeyOpens(int id);

    // getters
    bool isDark() const;
    bool isDoorOpen(int id) const;
    int getKeyOpens() const;
    int getDoorLeadsTo(int id) const;
    Key getRoomKey() const;

private:
    // ---- Room state ----
    bool dark;                 // Is the room initially dark
    bool doorOpen[10];         // doorOpen[1] -> door with id 1
	int doorLeadsTo[10];   // where each door leads
    Key roomKey; // the key available in the room

};