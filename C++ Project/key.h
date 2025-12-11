#pragma once

class Key
{
private:
    int doorID;   // The Id of the door this key opens

public:
    Key(int id = -1): doorID(id) {} // constractor

    bool isKey() const {return doorID != -1;} // checks if it is a key

    int getDoorID() const {return doorID; } // returns the door id

    bool matches(int d) const {return doorID == d;} // checks if the key matches the door id

    bool isValid() const {return doorID != -1;}  // check if the key is valid
};


