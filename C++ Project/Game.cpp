#include "Game.h"
#include <iostream>
#include <conio.h>
#include <windows.h>

using namespace std;

Game::Game() :
    currentLevel(0),
    player1(Point(2, 2,Direction::directions[Direction::STAY], '&'), { 'W', 'D', 'X', 'A', 'S', 'E' }, gameScreens[0]),
    player2(Point(77,2, Direction::directions[Direction::STAY], '$'), { 'I', 'L', 'M', 'J', 'K', 'O' }, gameScreens[0])
	
{
    hideCursor();
    currStatus = GameModes::MENU;
}
void Game::showMenu()
{
	while (_kbhit())
        _getch(); // Clear input buffer
	UIScreens::showMenu();
	bool  menu = true;

	while (menu) {
		char input = _getch();

		if (input == START_KEY) {
			currStatus = GameModes::NEW_GAME;
            menu = false;
		}
		else if (input == INSTRUCTIONS_KEY) {
			currStatus = GameModes::INSTRUCTIONS;
            menu = false;
		}
		else if (input == EXIT_KEY) {
			currStatus = GameModes::EXIT;
			cls();
            menu = false;
		}

	}
}
//AI generated func to show the intsurctions menu
void Game::showInstructions()
{
    UIScreens::showInstructions();
	_getch();
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
		//point to the current screen
        Screen& currentScreen = gameScreens[currentLevel];
        // Check for key presses
        if (_kbhit())
        {
            char ch = _getch();
            // Check for pause key (ESC)
            if (ch == 27) { // ESC key
				UIScreens::showPauseScreen();
                bool paused = true;
                while (paused) {
                    char pauseCh = _getch();
                    if (pauseCh == 27) { // ESC to resume
                        paused = false;
						cls();
						gameScreens[currentLevel].draw();
                        player1.setScreen(currentScreen);
                        player2.setScreen(currentScreen);
                        player1.draw();
						player2.draw();
                    }
                    else if (pauseCh == 'H' || pauseCh == 'h') { // H to return to main menu
                        currStatus = GameModes::MENU;
						return; // Exit game loop
                    }
                }
            }
            else
            {
                player1.keyPressed(ch);
                player2.keyPressed(ch);
            }
        }
    
        if(!gameRunning){
            break; // Exit game loop if game is no longer running
		}
		// Erase players from current position
		player1.erase();
		player2.erase();
        // Move players
        player1.move();
        player2.move();
		// Handle Tile interactions
		handleTile(player1);
        handleTile(player2);
        // Small delay to control game speed
        player1.draw();
        player2.draw();
        Sleep(100);
        
    }
}


void Game::handleTile(Player& player)
{
    Screen& currentScreen = gameScreens[currentLevel];
    Point pos = player.getPosition();
    char cell = currentScreen.getCharAt(pos);// Get the character at the player's position
    switch (cell)
    {
    case '?':
        riddleBank.handleRiddle(player,currentScreen,ui,currentLevel);
		break;
    case '1': case '2': case '3': case '4': case '5':case '6': case '7': case '8': case '9':
    {
        if (Door::handleDoor(player, currentScreen, ui, currentLevel))
        {
            initLevel(); // Reload level if changed
        }
        break;
	}
    case 'K':
        player.GrabItem('K',  cell - '0'); // Pick up key with id
        currentScreen.setCharAt(pos, ' '); // Remove key from screen
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
            while(_kbhit())
				_getch(); // Clear input buffer
            showMenu();
        }
        else if (currStatus == GameModes::INSTRUCTIONS)
        {
            showInstructions();
        }
        else
        {
            initLevel();
            gameLoop();

            currStatus = GameModes::MENU;// After game loop ends, return to menu
        }
    }
}