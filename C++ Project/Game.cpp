#include "Game.h"
#include <conio.h>
#include <windows.h>
#include <iostream>
#include "Door.h"
#include "Obstacle.h"
#include "Switch.h"
#include "Utils.h"
#include "Torch.h"

bool Game::pauseRequestedFromRiddle = false; //stop in the middle of riddle

Game::Game() // initializer list
    : player1(Point(P1_START_X, P1_START_Y, Direction::directions[Direction::STAY], '&'),
        { 'W','D','X','A','S','E' }),
    player2(Point(P2_START_X, P2_START_Y, Direction::directions[Direction::STAY], '$'),
        { 'I','L','M','J','K','O' })
{
    hideCursor();// hide cursor at the start of the game
    currStatus = GameModes::MENU;

    getAllScreenFileNames(screenFileNames);

}
//taken from exrecise 7
void Game::getAllScreenFileNames(std::vector<std::string>& vec_to_fill) {
    namespace fs = std::filesystem;
    vec_to_fill.clear(); //clear the vector before filling it

    // Iterate through files in the current directory
    for (const auto& entry : fs::directory_iterator(fs::current_path())) {
        auto filename = entry.path().filename();
        auto filenameStr = filename.string();

        // Check if the filename starts with "adv-world" and ends with ".screen"
        if (filenameStr.find("adv-world") == 0 && filename.extension() == ".screen") {

            vec_to_fill.push_back(filenameStr);
        }
    }
}

// Show main menu and handle input
void Game::showMenu()
{
    clearInputBuffer();

    // Check if riddles loaded successfully before showing menu
    if (!riddleBank.isLoaded()) {
        cls();
        std::cout << "\n========================================" << std::endl;
        std::cout << "ERROR: Riddles.txt file is missing!" << std::endl;
        std::cout << "The game cannot start without riddles." << std::endl;
        std::cout << "========================================\n" << std::endl;
        std::cout << "Press any key to return to menu..." << std::endl;
        waitForKey();
        currStatus = GameModes::MENU; // Stay in menu
        UIScreens::showMenu();
    }
    else {
        UIScreens::showMenu();
    }

    bool menu = true;
    while (menu)
    {
        if (_kbhit())
        {
            char input = _getch();
            switch (input)
            {
            case START_KEY:
                // Check if riddles are loaded before starting game
                if (!riddleBank.isLoaded()) {
                    cls();
                    std::cout << "\n========================================" << std::endl;
                    std::cout << "ERROR: Cannot start game!" << std::endl;
                    std::cout << "Riddles.txt file is missing." << std::endl;
                    std::cout << "========================================\n" << std::endl;
                    std::cout << "Press any key to return to menu..." << std::endl;
                    waitForKey();
                    UIScreens::showMenu();
                }
                else {
                    currStatus = GameModes::NEW_GAME;
                    menu = false;
                }
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

// Show instructions screen
void Game::showInstructions()
{
    UIScreens::showInstructions();
    waitForKey(); // wait for any key press

    currStatus = GameModes::MENU;
}
// Initialize level
void Game::initLevel(const std::string& filename, int specificDoor)
{
    cls();
    drawCurrentScreen();

    riddleBank.attachPositionToRoom(currentScreen);
    player1.setScreen(currentScreen);
    player2.setScreen(currentScreen);

    player1.activate();
    player2.activate();

    if (specificDoor != -1)
        placePlayersAtEntrance(specificDoor);
    else
    {
        player1.setPosition(Point(P1_START_X, P1_START_Y, Direction::directions[Direction::STAY], '&'));
        player2.setPosition(Point(P2_START_X, P2_START_Y, Direction::directions[Direction::STAY], '$'));
    }

    player1.draw();
    player2.draw();

    if (!Switch::exists(currentScreen))
        Door::allSwitchesAreOn();
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
    if (currentScreen.isDark())
        currentScreen.drawMapWithTorch(player1);
    else
        currentScreen.drawMap();;    player1.draw();
    player2.draw();
}
// Helper function to handle screen drawing based on torch / dark state
void Game::drawCurrentScreen()
{
    if (currentScreen.isDark())
    {
        bool hasTorch = Torch::playerHasTorch(player1) || Torch::playerHasTorch(player2);

        if (hasTorch)
        {
            if (Torch::playerHasTorch(player1))
                currentScreen.drawMapWithTorch(player1);
            else
                currentScreen.drawMapWithTorch(player2);
        }
        else
        {
            currentScreen.drawDark();
            currentScreen.resetTorchState();
        }
    }
    else
    {
        currentScreen.resetTorchState();
        currentScreen.drawMap();
    }
}

// Helper function to redraw everything after pause or game state change
void Game::redrawGame()
{
    drawCurrentScreen();
    player1.draw();
    player2.draw();
    currentScreen.drawStatusBar(
        player1.getHeldItem(), player1.getLives(), player1.getScore(),
        player2.getHeldItem(), player2.getLives(), player2.getScore());
}

// Minimal redraw - just update what changed
void Game::updateDisplay()
{
    // Only redraw players and status bar (much faster)
    player1.draw();
    player2.draw();
    currentScreen.drawStatusBar(
        player1.getHeldItem(), player1.getLives(), player1.getScore(),
        player2.getHeldItem(), player2.getLives(), player2.getScore());
}

// Handle bomb creation
void Game::updateBomb()
{
    if (player1.hasDroppedBomb() && activeBomb == nullptr)
    {
        activeBomb = new Bomb(player1.getLastDropPos(), player1.getChar());
        player1.clearBombRequest();
    }
    else if (player2.hasDroppedBomb() && activeBomb == nullptr)
    {
        activeBomb = new Bomb(player2.getLastDropPos(), player2.getChar());
        player2.clearBombRequest();
    }

    if (activeBomb != nullptr)
    {
        if (activeBomb->tick(currentScreen, player1, player2))
        {
            delete activeBomb;
            activeBomb = nullptr;
        }
    }
}

// Handle player movement and collisions
void Game::updatePlayers()
{
    player1.erase();
    player2.erase();

    bool stop1 = handleTile(player1);
    bool stop2 = handleTile(player2);

    if (!stop1)
        player1.move();
    if (!stop2)
        player2.move();
}

// Check game end conditions
bool Game::checkGameOver()
{
    if (player1.isDead() || player2.isDead())
    {
        UIScreens::showGameOverMessage();
        resetGame();
        currStatus = GameModes::MENU;
        return true;
    }

    if (checkLevel())
    {
        currStatus = GameModes::MENU;
        return true;
    }

    return false;
}

// Main game loop 
void Game::gameLoop()
{
    bool gameRunning = true;
    redrawGame(); // Draw full screen at start
    while (gameRunning)
    {
        // Handle pause request from riddle
        if (pauseRequestedFromRiddle)
        {
            pauseRequestedFromRiddle = false;
            handlePause(currentScreen, gameRunning);
            clearInputBuffer();

            if (!gameRunning)
                break;

            redrawGame();
            continue;
        }

        // Handle input 
        if (_kbhit())
        {
            char ch = _getch();

            if (ch == ESC)
            {
                handlePause(currentScreen, gameRunning);
                clearInputBuffer();

                if (!gameRunning)
                    break;

                redrawGame();
                continue;
            }
            else
            {
                player1.keyPressed(ch);
                player2.keyPressed(ch);
            }
        }

<<<<<<<<< Temporary merge branch 1
        // --- Bomb Creation Logic ---
        if (player1.hasDroppedBomb() && activeBomb == nullptr)
        {
            activeBomb = new Bomb(player1.getLastDropPos(), player1.getChar(), currentLevelIdx);
            player1.clearBombRequest();
        }
        else if (player2.hasDroppedBomb() && activeBomb == nullptr)
        {
            activeBomb = new Bomb(player2.getLastDropPos(), player2.getChar(),currentLevelIdx);
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
       
        // צייר רק בחדר חשוך אם יש לפיד בפועל
        if (currentScreen.isDark())
        {
            // בדוק אם מישהו מחזיק לפיד
            bool hasTorch = Torch::playerHasTorch(player1) || Torch::playerHasTorch(player2);

            if (hasTorch)
            {
                // צייר הילה רק אם יש מחזיק לפיד
                if (Torch::playerHasTorch(player1))
                    currentScreen.drawMapWithTorch(player1);
                else
                    currentScreen.drawMapWithTorch(player2);
            }
            else
            {
                currentScreen.drawDark();
                currentScreen.resetTorchState();
            }
        }
        //  Bomb Update Logic
        if (activeBomb != nullptr)
        {
            if (activeBomb->tick(currentScreen, player1, player2,currentLevelIdx))
            {
                delete activeBomb;
                activeBomb = nullptr;
            }
        }
=========
        // Update game state
        updateBomb();
        updatePlayers();

        // Minimal redraw - only update players and status bar
        updateDisplay();

        // Check end conditions
        if (checkGameOver())
        {
            gameRunning = false;
        }

        Sleep(GAME_DELAY);
    }
}

bool Game::handleTile(Player& player)// handle tile interaction for a player
{
    // get reference to the other player
   // Screen& currentScreen = gameScreens[currentLevel];
    Point pos = player.getPosition();
    Point targetPos = pos;
    targetPos.move();
    char cell = currentScreen.getCharAt(pos);
    char targetCell = currentScreen.getCharAt(targetPos);

    if (targetCell == 'K' || targetCell == '@' || targetCell == '!') {
        if (player.getHeldItem() != ' ' && player.getHeldItem() != 0) {
            UIScreens::showInventoryFullMessage(currentScreen);

            if (currentScreen.isDark())
                currentScreen.drawMapWithTorch(player);
            else
                currentScreen.drawMap();

            player.stepBack();
            player.draw();
            return true;
        }
    }
    switch (cell)
    {
    case '?':// riddle
        riddleBank.handleRiddle(player, currentScreen, currentLevelIdx);
        break;

    case 'K':// key
        if (player.getHeldItem() == ' ' || player.getHeldItem() == 0) {
            player.GrabItem('K', currentLevelIdx + 1);
            currentScreen.setCharAt(pos, ' ');
        }
        else
            return true;
        break;

    case '!': // Torch
        if (player.getHeldItem() == ' ' || player.getHeldItem() == 0)
        {
            player.GrabItem('!');
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
        bool doorOpened = Door::handleDoor(player, currentScreen, currentLevelIdx, activeDoor);// try to handle door
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
            cls();              // clear screen
            clearInputBuffer(); // clear any extra input
            showMenu();         // show menu
        }
        else if (currStatus == GameModes::INSTRUCTIONS)
        {
            showInstructions();
        }
        else if (currStatus == GameModes::NEW_GAME) // start new game
        {
            //get all screen file names from the directory
            getAllScreenFileNames(screenFileNames);

            // check if any screen files were found
            if (screenFileNames.empty()) {
                cls();
                std::cout << "No screen files found (adv-world*.screen) in the directory!" << std::endl;
                waitForKey();
                currStatus = GameModes::MENU;
                continue;
            }

            // load all levels into memory
            allLevels.clear();
            bool successload = true;
            for (const auto& fileName : screenFileNames) {
                Screen tempScreen;
                if (!tempScreen.loadMapFromFile(fileName)) {
                    successload = false;
                    break;
                }
                allLevels.push_back(tempScreen);
            }
            if (!successload) {
                std::cout << "\nPress any key to return to Menu..." << std::endl;
                waitForKey();
                currStatus = GameModes::MENU;
                continue;
            }

            //review loaded levels
            resetGame();

            // start from level 0
            currentLevelIdx = 0;

            // set current screen
            currentScreen = allLevels[currentLevelIdx];

            //start the first level
            initLevel(screenFileNames[currentLevelIdx]);

            //enter the main game loop
            gameLoop();

            currStatus = GameModes::MENU;
        }
    }
    UIScreens::showExitMessage();
}
bool Game::checkLevel()
{
    if (!player1.isActive() && !player2.isActive())
    {
        allLevels[currentLevelIdx] = currentScreen;
        int doorId = activeDoor - '0';
        int nextLevelIdx = currentLevelIdx;

        // בדיקה אם להתקדם או לחזור אחורה
        if (doorId > currentLevelIdx)
        {
            if (currentLevelIdx + 1 < (int)screenFileNames.size()) {
                nextLevelIdx++;
                cls();
                std::cout << "\n\n\n\t\tMoving to NEXT Level..." << std::endl;
                std::cout << "\t\tLoading: " << screenFileNames[nextLevelIdx] << std::endl;
                Sleep(2000); //delay for loading next level 2 seconds
            }
            else {
                showWinScreen();
                return true;
            }
        }
        else if (doorId <= currentLevelIdx)
        {
            if (currentLevelIdx > 0) {
                nextLevelIdx--;
                cls();
                std::cout << "\n\n\n\t\tGoing BACK to previous Level..." << std::endl;
                Sleep(1500); // delay for loading previous level 1.5 seconds
            }
        }

        // עדכון האינדקס וטעינת השלב
        currentLevelIdx = nextLevelIdx;
        currentScreen = allLevels[currentLevelIdx];
        initLevel(screenFileNames[currentLevelIdx], doorId);
        activeDoor = ' ';

        return false;
    }
    return false;
}

void Game::placePlayersAtEntrance(int specificDoor) // place players next to the door they entered from (manager)
{
    // Screen& currentScreen = gameScreens[currentLevel];

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
    currentLevelIdx = 0;
    activeDoor = ' ';
    Door::switchesAreOn = false;

    player1.resetStats();
    player2.resetStats();


    // reset riddles
    riddleBank.resetAllRiddles();

    // reset doors
    for (int i = 0; i < 10; i++)
        Door::openDoors[i] = false;

	//reset bombs
    if (activeBomb != nullptr)
    {
        delete activeBomb;
        activeBomb = nullptr;
	}

}

