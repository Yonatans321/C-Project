#pragma once

class Key
{
private:
    int doorID;   // The Id of the door this key opens

public:
    Key(int id = -1): doorID(id) {}

    int getDoorID() const {
        return doorID;
    }

    bool matches(int d) const {
        return doorID == d;
    }

    bool isValid() const {
        return doorID != -1;
    }
};


