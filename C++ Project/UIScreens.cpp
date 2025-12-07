#include "UIScreens.h"
#include <iostream>
#include <conio.h>
#include "Utils.h"
#include <windows.h>

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
        "                                                                                ", // 0
        "                          G A M E   I N S T R U C T I O N S                      ", // 1
        "                                                                                ", // 2
        "  A cooperative maze for two players. Push, pull, think, panic together —        ", // 3
        "  because teamwork makes the dream work (and prevents unnecessary respawns).     ", // 4
        "                                                                                ", // 5
        "   PLAYER 1 ($)                             PLAYER 2 (&)                         ", // 6
        "   W - Up        D - Right                 I - Up        L - Right               ", // 7
        "   X - Down      A - Left                  M - Down      J - Left                ", // 8
        "   S - Stay      E - Action                K - Stay      O - Action              ", // 9
        "                                                                                ", // 10
        "   ITEMS & ELEMENTS                                                             ", // 11
        "                                                                                ", // 12
        "   K    - Key         | Opens doors.you didnt expect something special...       ", // 13
        "   *    - Obstacle    | Push together (or cry).                                  ", // 14
        "   @    - Bomb        | Big boom. Try not to be in the blast radius.             ", // 15
        "   !    - Torch       | Lights the way. Batteries not included.                  ", // 16
        "   ?    - Riddle      | Solve it or stare until it solves you.                   ", // 17
        "   #    - Spring      | Launches you like you're in a cartoon.                   ", // 18
        "   /\\   - Switch     | ON/OFF. Dramatic click guaranteed.                       ", // 19
        "   ssss - Wall       | Temporary wall. Controlled by a switch… usually.          ", // 20
        "                                                                                ", // 21
        "                     Press ANY key to return to the Main Menu...                ", // 22
        "                                                                                ", // 23
        "                                                                                "  // 24
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

void UIScreens::showExitMessage()
{
    cls();
    cout << "\n\n";
    cout << "=============================================\n";
    cout << "           THANKS FOR PLAYING! :)\n";
    cout << "=============================================\n\n";

    Sleep(1200); // רגע קטן לראות את ההודעה ואז סוגרים
}