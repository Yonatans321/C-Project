#pragma once
#include <string>

class RoomMeta {
public:
    RoomMeta();

    void clear();

    // setters
    void setDark(bool v);
    void setDoorOpen(int id, bool open);
    void setKeyOpens(int id);

    // getters
    bool isDark() const;
    bool isDoorOpen(int id) const;
    int getKeyOpens() const;

private:
    bool dark;
    bool doorOpen[10];   // [1] בשביל דלת '1'
    int keyOpensDoor;   // איזה דלת המפתח פותח
};
