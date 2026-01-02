#pragma once
#include <string>
#include <vector>
#include <fstream>
#include "Key.h"
#include "Door.h"




class RoomMeta {
private:
    // ---- Room state ----
    bool dark;                 // Is the room initially dark
    bool doorOpen[10];         // doorOpen[1] -> door with id 1
    int doorLeadsTo[10];   // where each door leads
    bool doorInvalidState[10];
    Key roomKey;              // Key object for the room
	// ---- Riddle positions ----
    struct RiddlePosition {
        int riddleID;
        int x;
        int y;
    };
    RiddlePosition riddlePositions[20]; // Max 6 riddles per room
    int riddleCount;

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

    bool isDoorStateInvalid(int id) const;
        // Riddle position management
    void addRiddlePosition(int riddleID, int x, int y);
    int getRiddleCount() const;
    int getRiddleID(int index) const;
    int getRiddleX(int index) const;
    int getRiddleY(int index) const;
   
};