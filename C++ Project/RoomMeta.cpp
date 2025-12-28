#include "RoomMeta.h"

RoomMeta::RoomMeta() {
    clear();
}

void RoomMeta::clear() {
    dark = false;
    keyOpensDoor = -1;
    for (int i = 0; i < 10; i++)
        doorOpen[i] = false;
}

void RoomMeta::setDark(bool v) {
    dark = v;
}

void RoomMeta::setDoorOpen(int id, bool open) {
    if (id >= 0 && id < 10)
        doorOpen[id] = open;
}

void RoomMeta::setKeyOpens(int id) {
    keyOpensDoor = id;
}

bool RoomMeta::isDark() const {
    return dark;
}

bool RoomMeta::isDoorOpen(int id) const {
    if (id >= 0 && id < 10)
        return doorOpen[id];
    return false;
}

int RoomMeta::getKeyOpens() const {
    return keyOpensDoor;
}
