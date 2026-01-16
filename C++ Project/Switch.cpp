#include "Switch.h"

bool Switch::isSwitch(char c) //determines if the char is switch
{
    return c == '\\' || c == '/';
}

Switch::State Switch::stateFromChar(char c) // decoder of switch state from char
{
    return (c == '\\' ? OFF : ON);
}

void Switch::toggleSwitchAt(Point pos, Screen& screen) // changes the switch position
{
    char c = screen.getCharAt(pos.getX(), pos.getY());

    // only OFF to ON
    if (c == '\\')
    {
        // Update the map data
        screen.setCharAtSilent(pos.getX(), pos.getY(), '/');
        if (Screen::isSilent) return;
        // Check if room is dark
        bool isDark = screen.isDark();

        // Redraw the position
        gotoxy(pos.getX(), pos.getY());

        if (isDark)
        {
            // In dark rooms: draw as space (invisible)
            std::cout << ' ';
        }
        else
        {
            // In lit rooms: draw the new switch
            setColor(COLOR_LIGHT_GREEN);
            std::cout << '/';
            resetColor();
        }
    }
}
bool Switch::allSwitchesOn(const Screen& screen) //checks if ALL switches are on
{
    bool foundAnySwitch = false;
    // scan the screen for switches
    for (int y = 0; y < screen.getHeight(); y++)
    {
        for (int x = 0; x < screen.getWidth(); x++)
        {
            char c = screen.getCharAt(x, y);

            if (isSwitch(c))
            {
                foundAnySwitch = true;

                if (c == '\\')   // switch OFF
                    return false;
            }
        }
    }
    if (!foundAnySwitch) // if no switche found
        return true;

    return true; //all the switches are ON
}
// deletes all the 's' barriers from the screen
void Switch::clearBarriers(Screen& screen)
{
    for (int y = 0; y <screen.getHeight(); y++)
    {
        for (int x = 0; x < screen.getWidth(); x++)
        {
            if (screen.getCharAt(x, y) == 's')
                screen.setCharAt(x, y, ' ');
        }
    }
}
// responsible for handling the switch logic
void Switch::handleSwitch(Player& player, Screen& screen)
{
    Point pos = player.getPosition();
    char c = screen.getCharAt(pos.getX(), pos.getY());

    if (c=='/')
    {
        return;
    }

	// change the switch position
    toggleSwitchAt(pos, screen);

	// checks if all switches are ON
    if (allSwitchesOn(screen))
    {
        clearBarriers(screen);    //deletes all th 's' narriers
        Door::allSwitchesAreOn();    //can open the doors (if have the right key)
    }
}

bool Switch::exists(const Screen& screen)  // checks if there is any switch on the screen
{
    for (int y = 0; y < screen.getHeight(); y++)
    {
        for (int x = 0; x < screen.getWidth(); x++)
        {
            char c = screen.getCharAt(x, y);
            if (isSwitch(c))
                return true;
        }
    }
    return false;
}