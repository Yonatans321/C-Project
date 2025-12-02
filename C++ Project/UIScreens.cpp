#include "UIScreens.h"
#include <iostream>
#include <conio.h>
#include "Utils.h"

using namespace std;

// ================== PRINT FUNCTION ==================
void UIScreens::printScreen(const char* screen[HEIGHT])
{
    cls();
    for (int i = 0; i < HEIGHT; i++)
    {
        gotoxy(0, i);
        std::cout << screen[i];
    }
}

GameModes UIScreens::showMenu()
{
    printScreen(MENU_SCREEN);
    while (true)
    {
        char input = _getch();
        if (input == '1') {
            return GameModes::NEW_GAME;
        }
        else if (input == '8') {
            return GameModes::INSTRUCTIONS;
        }
        else if (input == '9') {
            return GameModes::EXIT;
        }
    }
}

const char* UIScreens::MENU_SCREEN[HEIGHT] =
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

void UIScreens::showInstructions()
{
    printScreen(INSTRUCTIONS_SCREEN);
    _getch();
}

// ================== INSTRUCTIONS SCREEN ==================
const char* UIScreens::INSTRUCTIONS_SCREEN[HEIGHT] =
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
"                  Press ANY key to return to the Main Menu                      "
};


GameModes UIScreens::showPauseScreen()
{
    printScreen(PAUSE_SCREEN);
    while (true)
    {
        char input = _getch();
        if (input == 27) { // ESC key
            return GameModes::RETURN_TO_THE_GAME;
        }
        else if (input == 'H' || input == 'h') {
            return GameModes::MENU;
        }
    }
}

// ================== PAUSE SCREEN ==================
const char* UIScreens::PAUSE_SCREEN[HEIGHT] =
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

void UIScreens::showWinScreen()
{
    printScreen(WIN_SCREEN);
    _getch();
}
const char* UIScreens::WIN_SCREEN[HEIGHT] =
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
