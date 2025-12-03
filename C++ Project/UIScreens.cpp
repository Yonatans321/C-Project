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
        "                       Tip: If you get lost… that's on you.                      ",
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
        "                                                                                ",
        "                                GAME INSTRUCTIONS                               ",
        "                                                                                ",
        "  A puzzle maze built for two players. Cooperate, explore, solve challenges,     ",
        "  and escape together — because alone you won't survive (or think clearly).      ",
        "                                                                                ",
        "   PLAYER 1 ($)              |                PLAYER 2 (&)                      ",
        "   W - Up                    |                I - Up                            ",
        "   X - Down                  |                M - Down                          ",
        "   A - Left                  |                J - Left                          ",
        "   D - Right                 |                L - Right                         ",
        "   S - Stay                  |                K - Stay                          ",
        "   E - Action                |                O - Action                        ",
        "                                                                                ",
        "   ITEMS                     |                ELEMENTS                           ",
        "   K - Key       | Opens doors. Simple.                                          ",
        "   * - Obstacle  | Push together / spring-boost to move it.                      ",
        "   ! - Torch     | Use it wisely... I'm scared of the dark.                      ",
        "   ? - Riddle    | Solve it or stare at it awkwardly.                            ",
        "   # - Spring    | Launches you like a cartoon hero.                             ",
        "   /\\ - Switch  | ON/OFF. Surprisingly dramatic click.                          ",
        "   S  - Signal   | Whatever the switch activates.                                 ",
        "                                                                                ",
        "                       Press ANY key to return to the Main Menu                 ",
    };

    printScreen(INSTRUCTIONS_SCREEN);
}

// ================= PAUSE ==================

void UIScreens::showPauseScreen()
{
    static const char* PAUSE_SCREEN[HEIGHT] =
    {
        "                                                                                ",
        "                                   GAME PAUSED                                  ",
        "                                                                                ",
        "                                 ================                                ",
        "                                  The game is paused                             ",
        "                                 ================                                ",
        "                                                                                ",
        "                      Press ESC to return to the game                            ",
        "                      Press H   to return to the main menu                       ",
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
        "                                                                                ",
        "                                                                                ",
        "                                                                                ",
        "                                                                                "
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