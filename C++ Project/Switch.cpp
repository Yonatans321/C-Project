#include "Switch.h"

bool Switch::isSwitch(char c)
{
    return c == '\\' || c == '/';
}

Switch::State Switch::stateFromChar(char c)
{
    return (c == '\\' ? OFF : ON);
}

void Switch::toggleSwitchAt(Point pos, Screen& screen)
{
    char c = screen.getCharAt(pos.getX(), pos.getY());

    // רק OFF → ON (אין חזור)
    if (c == '\\')
    {
        screen.setCharAt(pos.getX(), pos.getY(), '/');

        // ציור מחדש (ON = ירוק)
        setColor(10);
        gotoxy(pos.getX(), pos.getY());
        std::cout << "/";
        resetColor();
    }
}
bool Switch::allSwitchesOn(const Screen& screen)
{
    bool foundAnySwitch = false;

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

    // לא מצאנו אפילו Switch אחד → אין צורך להתנות
    if (!foundAnySwitch)
        return true;

    return true; // כל הסוויצ'ים ON
}

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

void Switch::handleSwitch(Player& player, Screen& screen)
{
    Point pos = player.getPosition();

    // 1. הופכים את הסוויצ'
    toggleSwitchAt(pos, screen);

    // 2. האם כל ה-switch ON?
    if (allSwitchesOn(screen))
    {
        clearBarriers(screen);    // מוחק את כל ה־s
        Door::allSwitchesAreOn();    // מסמן לדלת שאפשר לפתוח עם מפתח
    }
}

bool Switch::exists(const Screen& screen)
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