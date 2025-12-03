#include "UIScreens.h"
#include <iostream>
#include <conio.h>
#include "Utils.h"

    using namespace std;

// פונקציית עזר להדפסה (נשארת אותו דבר)
void UIScreens::printScreen(const char** screen)
{
    cls();
    for (int i = 0; i < HEIGHT; i++)
    {
        gotoxy(0, i);
        cout << screen[i];
    }
}

// ================= MENU ==================

void UIScreens::showMenu()
{
    // המערך מוגדר כאן בפנים כ-static
    static const char* MENU_SCREEN[HEIGHT] =
    {
        "                                                                                ",
        "                          =================================                      ",
        "                                   TEXT ADVENTURE                                ",
        "                                        2025                                     ",
        "                          =================================                      ",
        "                                                                                ",
        "                                    MAIN  MENU                                   ",
        "                                                                                ",
        "                            [1]  Start a New Game                                ",
        "                            [8]  Instructions & Controls                         ",
        "                            [9]  Exit                                            ",
        "                                                                                ",
        "                                                                                ",
        "                       Tip: If you get lost that's on you.                      ",
        "                                                                                ",
        "                                                                                ",
        "                                                                                ",
        "                                                                                ",
        "                                                                                ",
        "                                                                                ",
        "                                                                                ",
        "                                                                                ",
        "                                                                                ",
        "                                                                                ",
        "                                                                                ",
    };

    printScreen(MENU_SCREEN);
}

// ================= INSTRUCTIONS ==================

void UIScreens::showInstructions()
{
    static const char* INSTRUCTIONS_SCREEN[HEIGHT] =
    {
        "                                                                                ", // 1
        "                                GAME INSTRUCTIONS                               ", // 2
        "                                                                                ", // 3
        "  A puzzle maze built for two players. Cooperate, explore, solve challenges,     ", // 4
        "  and escape together — because alone you won't survive (or think clearly).      ", // 5
        "                                                                                ", // 6
        "   PLAYER 1 ($)              |                PLAYER 2 (&)                      ", // 7
        "   W - Up                    |                I - Up                            ", // 8
        "   X - Down                  |                M - Down                          ", // 9
        "   A - Left                  |                J - Left                          ", // 10
        "   D - Right                 |                L - Right                         ", // 11
        "   S - Stay                  |                K - Stay                          ", // 12
        "   E - Action                |                O - Action                        ", // 13
        "                                                                                ", // 14
        "   ITEMS                     |                ELEMENTS                           ", // 15
        "   K - Key       | Opens doors. Simple.                                          ", // 16
        "   * - Obstacle  | Push together / spring-boost to move it.                      ", // 17
        "   ! - Torch     | Use it wisely... I'm scared of the dark.                      ", // 18
        "   ? - Riddle    | Solve it or stare at it awkwardly.                            ", // 19
        "   # - Spring    | Launches you like a cartoon hero.                             ", // 20
        "   /\\ - Switch  | ON/OFF. Surprisingly dramatic click.                          ", // 21
        "   S  - Signal   | Whatever the switch activates.                                 ", // 22
        "                                                                                ", // 23
        "                       Press ANY key to return to the Main Menu                 ", // 24
        "                                                                                "  // 25 (נוספה)
    };

    printScreen(INSTRUCTIONS_SCREEN);
}

// ================= PAUSE ==================

void UIScreens::showPauseScreen()
{
    static const char* PAUSE_SCREEN[HEIGHT] =
    {
        "                                                                                ", // 1
        "                                   GAME PAUSED                                  ", // 2
        "                                                                                ", // 3
        "                                 ================                                ", // 4
        "                                  The game is paused                             ", // 5
        "                                 ================                                ", // 6
        "                                                                                ", // 7
        "                      Press ESC to return to the game                            ", // 8
        "                      Press H   to return to the main menu                       ", // 9
        "                                                                                ", // 10
        "                                                                                ", // 11
        "                                                                                ", // 12
        "                                                                                ", // 13
        "                                                                                ", // 14
        "                                                                                ", // 15
        "                                                                                ", // 16
        "                                                                                ", // 17
        "                                                                                ", // 18
        "                                                                                ", // 19
        "                                                                                ", // 20
        "                                                                                ", // 21
        "                                                                                ", // 22
        "                                                                                ", // 23
        "                                                                                ", // 24
        "                                                                                "  // 25
    };

    printScreen(PAUSE_SCREEN);
}

// ================= WIN ==================

void UIScreens::showWinScreen()
{
    static const char* WIN_SCREEN[HEIGHT] =
    {
        "                                                                                ",
        "                                   YOU  WIN!                                     ",
        "                                                                                ",
        "                          =============================                          ",
        "                             Congratulations, team!                              ",
        "                          =============================                          ",
        "                                                                                ",
        "               You solved puzzles, pushed obstacles, flipped switches,           ",
        "               collected keys, avoided disasters, and escaped the maze!          ",
        "                                                                                ",
        "                     (Honestly? I didn't think you'd make it.)                   ",
        "                                                                                ",
        "                                                                                ",
        "                                                                                ",
        "                                                                                ",
        "                                                                                ",
        "                                                                                ",
        "                                                                                ",
        "                                                                                ",
        "                                                                                ",
        "                         Press ANY key to return to Main Menu                   ",
        "                                                                                ",
        "                                                                                ",
        "                                                                                ",
        "                                                                                "
    };

    printScreen(WIN_SCREEN);
    _getch();
}