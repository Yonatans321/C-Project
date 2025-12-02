#include "Game.h"
#include <iostream>
#include <conio.h>
#include <windows.h>

using namespace std;

// פונקציה שמרוקנת לחלוטין את הבאפר של המקלדת
static void clearBuffer()
{
    while (_kbhit())
        _getch();
}

Game::Game() :
    currentLevel(0),
    player1(Point(2, 2, Direction::directions[Direction::STAY], '&'),
        { 'W', 'D', 'X', 'A', 'S', 'E' }, gameScreens[0]),
    player2(Point(77, 2, Direction::directions[Direction::STAY], '$'),
        { 'I', 'L', 'M', 'J', 'K', 'O' }, gameScreens[0])
{
    hideCursor();
    currStatus = GameModes::MENU;
}

void Game::showMenu()
{
    clearBuffer();
    UIScreens::showMenu();

    bool menu = true;

    while (menu)
    {
        clearBuffer();           // מנקה תמיד לפני קליטת לחיצה
        char input = _getch();   // קולט רק את הלחיצה האמיתית

        if (input == START_KEY)
        {
            currStatus = GameModes::NEW_GAME;
            menu = false;
        }
        else if (input == INSTRUCTIONS_KEY)
        {
            currStatus = GameModes::INSTRUCTIONS;
            menu = false;
        }
        else if (input == EXIT_KEY)
        {
            currStatus = GameModes::EXIT;
            cls();
            menu = false;
        }
    }
}

void Game::showInstructions()
{
    UIScreens::showInstructions();
    _getch();
    clearBuffer();
    currStatus = GameModes::MENU;
}

void Game::initLevel()
{
    Screen& currentScreen = gameScreens[currentLevel];
    cls();
    setScreen(Screen::MAX_X + 1, Screen::MAX_Y + 1);

    currentScreen.loadData(currentLevel);
    currentScreen.draw();
    player1.setScreen(currentScreen);
    player2.setScreen(currentScreen);

    player1.draw();
    player2.draw();
}

void Game::gameLoop()
{
    bool gameRunning = true;

    while (gameRunning)
    {
        Screen& currentScreen = gameScreens[currentLevel];

        // קליטת מקשים
        if (_kbhit())
        {
            char ch = _getch();

            // ESC - Pause
            if (ch == 27)
            {
                clearBuffer();
                UIScreens::showPauseScreen();

                bool paused = true;

                while (paused)
                {
                    Sleep(100);

                    if (_kbhit())
                    {
                        clearBuffer();
                        char pauseCh = _getch();

                        if (pauseCh == 27)   // ESC שוב - Resume
                        {
                            paused = false;
                            clearBuffer();

                            cls();
                            currentScreen.draw();
                            player1.draw();
                            player2.draw();
                        }
                        else if (pauseCh == 'H' || pauseCh == 'h')
                        {
                            gameRunning = false;
                            paused = false;
                            currStatus = GameModes::MENU;
                        }
                    }
                }
            }
            else
            {
                player1.keyPressed(ch);
                player2.keyPressed(ch);
            }
        }

        if (!gameRunning) break;

        player1.erase();
        player2.erase();

        player1.move();
        player2.move();

        handleTile(player1);
        handleTile(player2);

        player1.draw();
        player2.draw();

        Sleep(100);
    }
}

void Game::handleTile(Player& player)
{
    Screen& currentScreen = gameScreens[currentLevel];
    Point pos = player.getPosition();
    char cell = currentScreen.getCharAt(pos);

    switch (cell)
    {
    case '?':
        riddleBank.handleRiddle(player, currentScreen, ui, currentLevel);
        break;

    case '1': case '2': case '3': case '4': case '5':
    case '6': case '7': case '8': case '9':
        if (Door::handleDoor(player, currentScreen, ui, currentLevel))
            initLevel();
        break;

    case 'K':
        player.GrabItem('K', cell - '0');
        currentScreen.setCharAt(pos, ' ');
        break;

    default:
        break;
    }
}

void Game::run()
{
    while (currStatus != GameModes::EXIT)
    {
        if (currStatus == GameModes::MENU)
        {
            cls();
            clearBuffer();
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