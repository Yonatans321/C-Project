#include "Game.h"
#include <conio.h>
#include <windows.h>
#include <iostream>

using namespace std;

// ============================
//       CONSTRUCTOR
// ============================

Game::Game()
    : player1(Point(2, 2, Direction::directions[Direction::STAY], '&'),
        { 'W','D','X','A','S','E' }),
    player2(Point(77, 2, Direction::directions[Direction::STAY], '$'),
        { 'I','L','M','J','K','O' })
{
    hideCursor();
    currStatus = GameModes::MENU;
    
	gameScreens[0].loadMap(0);
	player1.setScreen(gameScreens[0]);
	player2.setScreen(gameScreens[0]);
}

// ============================
//     SHOW MAIN MENU
// ============================

void Game::showMenu()
{
    while (_kbhit()) _getch();
    UIScreens::showMenu();

    bool menu = true;

    while (menu)
    {
        if (_kbhit())
        {
            char input = _getch();

            switch (input)
            {
            case START_KEY:
                currStatus = GameModes::NEW_GAME;
                menu = false;
                break;

            case INSTRUCTIONS_KEY:
                currStatus = GameModes::INSTRUCTIONS;
                menu = false;
                break;

            case EXIT_KEY:
                currStatus = GameModes::EXIT;
                menu = false;
                break;
            }
        }
    }

    while (_kbhit()) _getch();
}

// ============================
//       INSTRUCTIONS
// ============================

void Game::showInstructions()
{
    UIScreens::showInstructions();
    while (_kbhit()) _getch(); // פונקציית עזר CLEANBUFFER ב UTILLS להוסיףףףףףףף
    [[maybe_unused]]
    auto i =_getch();  // Press any key לעשות פונקציית עזר WAIT FOR KEY שיבינו מה עושים
    while (_kbhit()) _getch();

    currStatus = GameModes::MENU;
}

// ============================
//       INIT LEVEL
// ============================

void Game::initLevel()
{
    Screen& currentScreen = gameScreens[currentLevel];

    cls();
    currentScreen.loadMap(currentLevel);
    currentScreen.drawMap();

    // Assign screen to players
    player1.setScreen(currentScreen);
    player2.setScreen(currentScreen);

    player1.draw();
    player2.draw();
}

// ============================
//        GAME LOOP
// ============================

void Game::gameLoop()
{
    bool gameRunning = true;

    while (gameRunning)
    {
        Screen& currentScreen = gameScreens[currentLevel];

        // Handle input
        if (_kbhit())
        {
            char ch = _getch();

            // PAUSE → Escape key
            if (ch == 27) // להחליף בקבוע ESC ( יותר קריא בשביל בודק התרגליים)
            {
                UIScreens::showPauseScreen();
                while (_kbhit()) _getch();

                bool paused = true;

                while (paused)
                {
                    if (_kbhit())
                    {
                        char c = _getch();

                        if (c == 27)          // ESC → return to game
                        {
                            paused = false;
                        }
                        else if (c == 'H' || c == 'h') // go to menu
                        {
                            currStatus = GameModes::MENU;
                            paused = false;
                            gameRunning = false;
                        }
                    }

                    Sleep(50);
                }

                while (_kbhit()) _getch();

                if (!gameRunning)
                    break;

                cls();
                currentScreen.drawMap();
                player1.draw();
                player2.draw();
            }
            else
            {
                player1.keyPressed(ch);
                player2.keyPressed(ch);
            }
        }

        // Update players
        player1.erase();
        player2.erase();

        player1.move();
        player2.move();

        handleTile(player1);
        handleTile(player2);

        player1.draw();
        player2.draw();

        Sleep(80);
    }
}

// ============================
//       HANDLE TILE
// ============================

void Game::handleTile(Player& player)
{
    Screen& currentScreen = gameScreens[currentLevel];
    Point pos = player.getPosition();
    char cell = currentScreen.getCharAt(pos);

    switch (cell)
    {
    case '?':
        riddleBank.handleRiddle(player, currentScreen, currentLevel);
        break;

    case '1': case '2': case '3': case '4': case '5':
    case '6': case '7': case '8': case '9':
        if (Door::handleDoor(player, currentScreen, currentLevel))
        {
            initLevel();
        }
        break;

    case 'K':
        player.GrabItem('K', 0);
        currentScreen.setCharAt(pos, ' ');
        break;

    default:
        break;
    }
}

// ============================
//       WIN SCREEN
// ============================

void Game::showWinScreen()
{
    UIScreens::showWinScreen();
    _getch();
}

// =========================
//          RUN
// ============================

void Game::run()
{
    while (currStatus != GameModes::EXIT)
    {
        if (currStatus == GameModes::MENU)
        {
            cls();
            while (_kbhit()) _getch();
            showMenu();
        }
        else if (currStatus == GameModes::INSTRUCTIONS)
        {
            showInstructions();
        }
        else if (currStatus == GameModes::NEW_GAME)
        {
            initLevel();
            gameLoop(); 

            currStatus = GameModes::MENU;
        }
    }
}
