#pragma once
#include "Screen.h"
#include "Player.h"
#include "Door.h"

class Switch
{
public:
    enum State { OFF, ON };

    //determines if the char is switch
    static bool isSwitch(char c);

    // decoder of switch state from char
    static State stateFromChar(char c);

    // changes the switch position
    static void toggleSwitchAt(Point pos, Screen& screen);

	// responsible for handling the switch logic
    static void handleSwitch(Player& player, Screen& screen);
    // checks if there is any switch on the screen
    static bool exists(const Screen& screen);

private:
    //checks if all switches are on
    static bool allSwitchesOn(const Screen& screen);

    // deletes all the 's' barriers from the screen
    static void clearBarriers(Screen& screen);

};
