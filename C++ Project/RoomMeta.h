#pragma once
#include <string>
#include <fstream>

class RoomMeta {
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

private:
    // ---- Room state ----
    bool dark;                 // Is the room initially dark
    bool doorOpen[10];         // doorOpen[1] -> door with id 1
    int keyOpensDoor;          // Which door the key opens (-1 if none)
};