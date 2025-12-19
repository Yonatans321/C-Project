#include "Game.h"
#include <conio.h>
#include <windows.h>
#include <iostream>
#include "Door.h"
#include "Obstacle.h"
#include "Switch.h"
#include "Utils.h"


bool Game::pauseRequestedFromRiddle = false; //stop in the middle of riddle

Game::Game() // initializer list
    : player1(Point(P1_START_X, P1_START_Y, Direction::directions[Direction::STAY], '&'),
        { 'W','D','X','A','S','E' }),
    player2(Point(P2_START_X, P2_START_Y, Direction::directions[Direction::STAY], '$'),
        { 'I','L','M','J','K','O' })
{
	hideCursor();// hide cursor at the start of the game
    currStatus = GameModes::MENU;
	// Load all maps
    for (int i = 0;i < MAX_LEVELS;i++)
    {
        gameScreens[i].loadMap(i);
    }
	// Assign initial screen to players
    player1.setScreen(gameScreens[0]);
    player2.setScreen(gameScreens[0]);
}

void Game::showMenu()
{
    clearInputBuffer();
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

            case Colors_ON_OFF: // Colors ON/OFF
                ColorsEnabled = !ColorsEnabled;
                UIScreens::showMenu(); 
                break;
            case EXIT_KEY:
                currStatus = GameModes::EXIT;
                menu = false;
                break;
            }
        }
    }

	clearInputBuffer();// clear any extra input
}


void Game::showInstructions()
{
    UIScreens::showInstructions();
	waitForKey(); // wait for any key press
   
    currStatus = GameModes::MENU;
}
// Initialize level 
void Game::initLevel(int specificDoor)
{
    Screen& currentScreen = gameScreens[currentLevel];

    cls();
    currentScreen.drawMap();
	riddleBank.attachPositionToRoom(currentScreen); // attach riddles to the current screen

    // Assign screen to players
    player1.setScreen(currentScreen);
    player2.setScreen(currentScreen);

	// Reset players' positions
    player1.activate();
    player2.activate();
    
    if (activeDoor >= '1' && activeDoor <= '9')
    {
        placePlayersAtEntrance(specificDoor);
    }
    else
    {
        player1.setPosition(Point(P1_START_X, P1_START_Y, Direction::directions[Direction::STAY], '&'));
        player2.setPosition(Point(P2_START_X, P2_START_Y, Direction::directions[Direction::STAY], '$'));
    }

    player1.draw();
    player2.draw();

    if (!Switch::exists(currentScreen))
    {
        Door::allSwitchesAreOn();
    }
}
// Handle pause function
void Game::handlePause(Screen& currentScreen, bool& gameRunning)
{
    UIScreens::showPauseScreen();
    clearInputBuffer();

    bool paused = true;
    while (paused)
    {
        if (_kbhit())
        {
            char c = _getch();
            if (c == ESC)
                paused = false;
            else if (c == 'H' || c == 'h')
            {
                currStatus = GameModes::MENU;
                gameRunning = false;
                return;
            }
        }
        Sleep(GAME_DELAY);
    }

    clearInputBuffer();
    currentScreen.drawMap();
    player1.draw();
    player2.draw();
}
// Main game loop 
void Game::gameLoop()
{
    bool gameRunning = true;

    while (gameRunning)
    {
        if (pauseRequestedFromRiddle) // stop in the middle of riddle
        {
            pauseRequestedFromRiddle = false;
            Screen& cs = gameScreens[currentLevel];
            handlePause(cs, gameRunning);
            clearInputBuffer();
            if (!gameRunning)
            {
                break;
            }
            cls();
            cs.drawMap();
            player1.draw();
            player2.draw();

            cs.drawStatusBar(player1.getHeldItem(), player1.getLives(), player1.getScore(), player2.getHeldItem(), player2.getLives(), player2.getScore());
        }

        Screen& currentScreen = gameScreens[currentLevel];

        // Handle input 
        if (_kbhit())
        {
            char ch = _getch();

            // PAUSE → Escape key
            if (ch == ESC)
            {
                handlePause(currentScreen, gameRunning);
                clearInputBuffer();

                if (!gameRunning)
                    break;

                // Redraw after pause
                currentScreen.drawMap();
                player1.draw();
                player2.draw();
                currentScreen.drawStatusBar(player1.getHeldItem(), player1.getLives(), player1.getScore(), player2.getHeldItem(), player2.getLives(), player2.getScore());
            }
            else
            {
                player1.keyPressed(ch);
                player2.keyPressed(ch);
            }
        }

        // --- Bomb Creation Logic ---
        // Check if any player requested to drop a bomb via flag
        if (player1.hasDroppedBomb() && activeBomb == nullptr)
        {
            activeBomb = new Bomb(player1.getLastDropPos());
            player1.clearBombRequest();
        }
        else if (player2.hasDroppedBomb() && activeBomb == nullptr)
        {
            activeBomb = new Bomb(player2.getLastDropPos());
            player2.clearBombRequest();
        }

        // Update players
        player1.erase();
        player2.erase();

        bool stop1 = handleTile(player1);
        bool stop2 = handleTile(player2);

        if (!stop1)
        {
            player1.move();
        }
        if (!stop2)
        {
            player2.move();
        }

        player1.draw();
        player2.draw();

        // --- Bomb Update Logic ---
        // Tick the bomb independently of keyboard input
        if (activeBomb != nullptr)
        {
            // Execute bomb logic from Bomb.cpp (timer update and explosion)
            if (activeBomb->tick(currentScreen, player1, player2))
            {
                delete activeBomb; // Explosion finished, clean memory
                activeBomb = nullptr;
            }
        }

        currentScreen.drawStatusBar(player1.getHeldItem(), player1.getLives(), player1.getScore(), player2.getHeldItem(), player2.getLives(), player2.getScore());

        if (checkLevel() == true)
        {
            gameRunning = false;      // level completed
            currStatus = GameModes::MENU; // return to menu
            break;                    // exit game loop
        }

        Sleep(GAME_DELAY);
    }
}


bool Game::handleTile(Player& player)// handle tile interaction for a player
{
	// get reference to the other player
    Screen& currentScreen = gameScreens[currentLevel];
    Point pos = player.getPosition();
    Point targetPos = pos;
    targetPos.move();
    char cell = currentScreen.getCharAt(pos);
    char targetCell = currentScreen.getCharAt(targetPos);

    if (targetCell == 'K' || targetCell == '@') {
        if (player.getHeldItem() != ' ' && player.getHeldItem() != 0) {
            UIScreens::showInventoryFullMessage(gameScreens[currentLevel]);
            player.stepBack();
            player.draw();
            return true;
        }
    }
    switch (cell)
    {
	case '?':// riddle
        riddleBank.handleRiddle(player, currentScreen, currentLevel);
        break;

	case 'K':// key
        if (player.getHeldItem() == ' ' || player.getHeldItem() == 0) {
            player.GrabItem('K', currentLevel + 1);
            currentScreen.setCharAt(pos, ' ');
        }
        else
            return true;
        break;
        case '@':// bomb
            if (player.getHeldItem() == ' ' || player.getHeldItem() == 0) {
                player.GrabItem('@');
                currentScreen.setCharAt(pos, ' ');
            }
            else
                return true;
            break; 
    // switch
    case '\\':
    case '/':
        Switch::handleSwitch(player, currentScreen);
        return false;
    }

	switch (targetCell)// check target cell
    {
    case '1': case '2': case '3': case '4': case '5':
    case '6': case '7': case '8': case '9':
    {
		bool doorOpened = Door::handleDoor(player, currentScreen, currentLevel, activeDoor);// try to handle door
        if (doorOpened)
        {
			player.setPosition(targetPos);// move player through door
            return true;
        }
        break;

    }
	case '*':// obstacle
    {
		Obstacle::handleObstacle(player1, player2, currentScreen);// try to push obstacle

		char afterPush = currentScreen.getCharAt(targetPos);// check if obstacle was moved

        if (afterPush == '*')
			return true;    // cannot move, obstacle not moved

		player.setPosition(targetPos);// move player into obstacle's previous position
        return true;
    }

	case 's':// switch wall
        return true;
        break;

    }

	return false; // the player can move
    
}

void Game::showWinScreen()
{
    UIScreens::showWinScreen();
	waitForKey(); // wait for any key press
}


void Game::run() // main game loop
{
    while (currStatus != GameModes::EXIT)
    {
        if (currStatus == GameModes::MENU)
        {
			cls();// clear screen
			clearInputBuffer();// clear any extra input
			showMenu();// show menu
        }
        else if (currStatus == GameModes::INSTRUCTIONS)
        {
            showInstructions();
        }
		else if (currStatus == GameModes::NEW_GAME) // start new game
        {
			resetGame();// reset game state
			initLevel();// initialize first level
			gameLoop(); //  start game loop

            currStatus = GameModes::MENU;
        }
    }
    UIScreens::showExitMessage();
}

bool Game::checkLevel() // check if level is completed
{
	if (!player1.isActive() && !player2.isActive()) // both players went through a door
    {
		int doorId = activeDoor - '0'; // convert char to int
		int targetDoor = -1; // door to place players at

		if (activeDoor == '3') // final door (for now)
        {
            showWinScreen();
            activeDoor = ' ';
            return true;
        }

		if (doorId == currentLevel + 1) // going to next level
        {
            currentLevel++;
            targetDoor = -1;
        }
		else if (doorId == currentLevel) // going back to previous level
        {
			currentLevel--;
            targetDoor = doorId;
        }
        
		initLevel(targetDoor); // initialize new level
        activeDoor = ' ';
    }
    return false;
}

void Game::placePlayersAtEntrance(int specificDoor) // place players next to the door they entered from (manager)
{
    Screen& currentScreen = gameScreens[currentLevel];

	int lowestDoor = 10; // higher than any door id
    Point targetDoorPos;
    bool found = false;

	// Search for doors in the current screen (helped by AI)
    for (int y = 0; y < Screen::MAP_HEIGHT; y++)
    {
        for (int x = 0; x < Screen::WIDTH; x++)
        {
			char c = currentScreen.getCharAt(x, y);// get character at position
            if (c >= '1' && c <= '9')
            {
                int id = c - '0';
                if (specificDoor != -1)
                {
					if (id == specificDoor)// found the specific door
                    {
						targetDoorPos = Point(x, y, Direction::directions[Direction::STAY], ' '); // set target position
						found = true;  // mark as found
						goto EndSearch;// exit both loops (helped by AI)
                    }
                }
				else if (id < lowestDoor) // find the lowest door id    
                {
                    lowestDoor = id;
                    targetDoorPos = Point(x, y, Direction::directions[Direction::STAY], ' ');
                    found = true;
                }
            }
        }
    }
    
EndSearch:// exit point for goto (helped by AI)
	if (!found) return;

	placeNextToDoor(targetDoorPos); // place players next to the found door
      
}

void Game::placeNextToDoor(const Point& targetDoorPos)
{
    int px = targetDoorPos.getX();
    int py = targetDoorPos.getY();

    // player 1
    Point p1(px + 1, py, Direction::directions[Direction::STAY], '&');
    player1.setPosition(p1);

    // player 2
    Point p2(px + 2, py, Direction::directions[Direction::STAY], '$');
    player2.setPosition(p2);

    player1.draw();
    player2.draw();
}

void Game::resetGame()
{
	// reset game state
    currentLevel = 0;
    activeDoor = ' ';
    Door::switchesAreOn = false;

    player1.resetStats();
    player2.resetStats();

	// reset riddles
    riddleBank.resetAllRiddles();

	// reset doors
    for (int i = 0; i < 10; i++)
        Door::openDoors[i] = false;

	// reload maps
    for (int i = 0; i < MAX_LEVELS; i++)   
        gameScreens[i].loadMap(i);

}

