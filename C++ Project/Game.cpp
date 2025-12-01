#include "Game.h"
#include <iostream>
#include <conio.h>
#include <windows.h>
#include "Utils.h"
#include "Direction.h"
#include "Screen.h"
#include "Point.h"
#include "Player.h"
#include <cstring>
#include "Riddle.h"
#include "RiddleBank.h"


using namespace std;

Game::Game() :
    currentLevel(0),
    player1(Point(2, 2,Direction::directions[Direction::STAY], '&'), { 'W', 'D', 'X', 'A', 'S', 'E' }, gameScreens[0]),
    player2(Point(77,2, Direction::directions[Direction::STAY], '$'), { 'I', 'L', 'M', 'J', 'K', 'O' }, gameScreens[0])
	
{
    hideCursor();
    //setScreen(Screen::MAX_X, Screen::MAX_Y);
    currStatus = GameModes::MENU;
}
void Game::showMenu()
{

	cls();
	gotoxy(25,12);
	cout << "==============================" << endl;
    gotoxy(22,13);
	cout << "      TEXT ADVENTURE GAME 2025      " << endl;
    gotoxy(25,14);
    cout << "==============================" << endl;
    gotoxy(29,16);
	cout << "(1) Start a new game" << endl;
    gotoxy(23,17);
	cout << "(8) Present instructions and keys" << endl;
    gotoxy(36,18);
	cout << "(9) EXIT" << endl;

	bool  gameRunning = true;

	while (gameRunning) {
		char input = _getch();

		if (input == START_KEY) {
			currStatus = GameModes::NEW_GAME;
            gameRunning = false;
		}
		else if (input == INSTRUCTIONS_KEY) {
			currStatus = GameModes::INSTRUCTIONS;
            gameRunning = false;
		}
		else if (input == EXIT_KEY) {
			currStatus = GameModes::EXIT;
			cls();
			gameRunning = false;
		}

	}
}
//AI generated func to show the intsurctions menu
void Game::showInstructions()
{
    cls();

    // Title
    setColor(11); // Cyan
    gotoxy(20, 3);
    cout << "===============================================" << endl;
    gotoxy(20, 4);
    cout << "                 GAME INSTRUCTIONS              " << endl;
    gotoxy(20, 5);
    cout << "===============================================" << endl;
    setColor(7); // White

    // Intro paragraph
    gotoxy(20, 7);
    cout << "The game is a cooperative 2-player adventure.";
    gotoxy(20, 8);
    cout << "Your mission is to collect keys and move between rooms.";
    gotoxy(20, 9);
    cout << "You will encounter walls, riddles and obstacles that require";
    gotoxy(20, 10);
    cout << "creativity and smart use of items. Good luck!";

    // Player 1 block
    setColor(10); // Green
    gotoxy(20, 13);
    cout << "@  Player 1 Controls:" << endl;
    setColor(7);

    gotoxy(20, 14);
    cout << "  W - Move Up" << endl;
    gotoxy(20, 15);
    cout << "  X - Move Down" << endl;
    gotoxy(20, 16);
    cout << "  A - Move Left" << endl;
    gotoxy(20, 17);
    cout << "  D - Move Right" << endl;
    gotoxy(20, 18);
    cout << "  S - Stay" << endl;
    gotoxy(20, 19);
    cout << "  E - Use Item" << endl;

    // Player 2 block
    setColor(12); // Red
    gotoxy(50, 13);
    cout << "#  Player 2 Controls:" << endl;
    setColor(7);

    gotoxy(50, 14);
    cout << "  I - Move Up" << endl;
    gotoxy(50, 15);
    cout << "  M - Move Down" << endl;
    gotoxy(50, 16);
    cout << "  J - Move Left" << endl;
    gotoxy(50, 17);
    cout << "  L - Move Right" << endl;
    gotoxy(50, 18);
    cout << "  K - Stay" << endl;
    gotoxy(50, 19);
    cout << "  O - Use Item" << endl;

    // Items
    setColor(14); // Yellow
    gotoxy(20, 22);
    cout << "Pickable Items:" << endl;
    setColor(7);

    gotoxy(20, 23);
    cout << "  K - Key" << endl;
    gotoxy(20, 24);
    cout << "  @ - Bomb" << endl;
    gotoxy(20, 25);
    cout << "  ! - Torch" << endl;

    // Game elements
    setColor(9); // Light Blue
    gotoxy(50, 22);
    cout << "Game Elements:" << endl;
    setColor(7);

    gotoxy(50, 23);
    cout << "  W - Wall" << endl;
    gotoxy(50, 24);
    cout << "  * - Obstacle" << endl;
    gotoxy(50, 25);
    cout << "  # - Spring" << endl;
    gotoxy(50, 26);
    cout << "  /\\ - Switch" << endl;
    gotoxy(50, 27);
    cout << "  1-9 - Doors" << endl;
    gotoxy(50, 28);
    cout << "  ? - Riddle" << endl;

    // Bottom text
    setColor(11);
    gotoxy(20, 31);
    cout << "Press ANY key to return to the main menu..." << endl;
    setColor(7);

    _getch();
    currStatus = GameModes::MENU;

}
void Game::run()
{
	while (currStatus !=GameModes::EXIT )
	{
		if (currStatus == GameModes::MENU) 
        {
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

void Game::GamePaused()
{
	cls();
	gotoxy(30, 12);
	cout << "===================" << endl;
	cout << "Game Paused"<< endl;
	cout << "===================" << endl;
	cout << "Press ESC to return to the game" << endl;
	cout << "Press H to return to the main menu" << endl;
}

void Game::initLevel()
{
    Screen& currentScreen = gameScreens[currentLevel];
    cls();
    setScreen(Screen::MAX_X + 1, Screen::MAX_Y + 1);
	currentScreen.loadData(currentLevel);

    

    currentScreen.draw();
    player1.draw();
    player2.draw();
 }



void Game::gameLoop()
{
    Screen& currentScreen = gameScreens[currentLevel];
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
                GamePaused();
                bool paused = true;
                while (paused) {
                    char pauseCh = _getch();
                    if (pauseCh == 27) { // ESC to resume
                        paused = false;
                        initLevel(); // Redraw the level
                    }
                    else if (pauseCh == 'H' || pauseCh == 'h') { // H to return to main menu
                        currStatus = GameModes::MENU;
                        gameRunning = false;
                        paused = false;
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
		// Handle door interactions
		handleDoor(player1);
		handleDoor(player2);
        // Small delay to control game speed
        player1.draw();
        player2.draw();
        Sleep(100);
        
    }
}

void Game::handleDoor(Player& p)
{
    Screen& currentScreen = gameScreens[currentLevel];
    Point doorPos = p.getPosition();
    char cell = currentScreen.getCharAt(doorPos);// Get the character at the player's position

    if (!Door::isDoorChar(cell))
    {
        return; // Not a door
    }
    Door* d = currentScreen.getDoor(doorPos);// Get the door at the player's position
    if (!d)// If no door found 
    {
        return;
    }

    if (!d->isOpen())// If the door is not open
    {
        if (p.useKeyForDoor(cell))// If the player has the key for the door
        {
            if (d->tryOpen(p.getItemId()))// Try to open the door with the key 
            {
                p.DropItem();// Remove the key from the player
            }

        }
		if (!d->isOpen())// If the door is still not open
        {
            return; // Door is still closed
        }
    }
    if (d->isOpen())// If the door is open
    {
        int destLevel = d->getDestinationLevel();// Get the destination level

        if (destLevel != -1)// If destination level is the last level
        {
			currentLevel = destLevel;// Change to the new level

            initLevel();// Initialize the new level
        }
    }
}

void Game::handleRiddle(Player& player)
{
    int x = player.getX();
    int y = player.getY();
    char tile = screen.getCharAt(x, y);

    if (tile != '?')
        return;

    int riddleID = currentLevel; 

    cout << "\nYou stepped on a RIDDLE!" << endl;
    cout << "Answer it? (Y/N): ";

    char* choice;
	getline(cin, choice);

    if (choice.empty()|| choice[0]=='N'|| choice[0] =='n') {
        cout << "You walk away from the riddle.\n";
        return;
    }

    RiddleOutcome result = riddleBank.askRiddleInteractive(riddleID);

	switch (result) {
   case RiddleOutcome::Correct:
       cout << "Correct! You may proceed.\n" << endl;
       cout << "+100 poitnts ! \n" << endl;
       player.addPoints(100);
        break;
    case RiddleOutcome::Incorrect:
        cout << "Incorrect! You lose a life.\n";
        player.loseLife();
        break;
    case RiddleOutcome::AlreadySolved:
        cout << "You have already solved this riddle.\n";
		screen.setCharAt(x, y, ' '); // Remove the riddle from the screen
        break;
    case RiddleOutcome::NotFound:
        cout << "Riddle not found.\n";
		break;  
}