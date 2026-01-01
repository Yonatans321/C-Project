#pragma once
#include <string>
#include <vector>
#include <fstream>




class RoomMeta {
private:
    // ---- Room state ----
    bool dark;                 // Is the room initially dark
    bool doorOpen[10];         // doorOpen[1] -> door with id 1
    int keyOpensDoor;          // Which door the key opens (-1 if none)

    struct RiddlePosition {
        int riddleID;
        int x;
        int y;
    };
    RiddlePosition riddlePositions[6]; // Max 6 riddles per room
    int riddleCount;

public:
    RoomMeta();

    void clear();

    // קריאה מקובץ פתוח (אחרי המפה)
    void loadFromLine(const std::string& line);


    // setters
    void setDark(bool v);
    void setDoorOpen(int id, bool open);
    void setKeyOpens(int id);

    // getters
    bool isDark() const;
    bool isDoorOpen(int id) const;
    int getKeyOpens() const;
	
	// Riddle position management
    void addRiddlePosition(int riddleID, int x, int y);
    int getRiddleCount() const;
    int getRiddleID(int index) const;
    int getRiddleX(int index) const;
    int getRiddleY(int index) const;
   
};