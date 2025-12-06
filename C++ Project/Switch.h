#pragma once
#include "Screen.h"
#include "Player.h"
#include "Door.h"

class Switch
{
public:
    enum State { OFF, ON };

    // קובע האם התו הוא סוויצ'
    static bool isSwitch(char c);

    // מפענח מצב מתוך תו
    static State stateFromChar(char c);

    // משנה את הסוויצ' עצמו
    static void toggleSwitchAt(Point pos, Screen& screen);

    // הפונקציה שה-Game קורא — עושה הכל
    static void handleSwitch(Player& player, Screen& screen);

    static bool exists(const Screen& screen);

private:
    // בדיקה האם כל הסוויצ'ים ON
    static bool allSwitchesOn(const Screen& screen);

    // מוחקת מחסומי 's'
    static void clearBarriers(Screen& screen);

};
