#include "Game.h"
#include <conio.h>
#include <windows.h>
#include <iostream>
#include "Door.h"
#include "Key.h"
#include "Obstacle.h"
#include "Switch.h"
#include "Utils.h"
#include "Torch.h"

bool Game::pauseRequestedFromRiddle = false; //stop in the middle of riddle

Game::Game() // initializer list
    : player1(Point(P1_START_X, P1_START_Y, Direction::directions[Direction::STAY], '&'),
        { 'W','D','X','A','S','E' }),
    player2(Point(P2_START_X, P2_START_Y, Direction::directions[Direction::STAY], '$'),
        { 'I','L','M','J','K','O' }),
	player1LastPos(-1, -1),player2LastPos(-1,-1),
    gameTimer(0), maxGameTime(420), timerActive(false)
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
					currStatus = GameModes::NEW_GAME; // start new game
                    menu = false;
                }
                break;

            case INSTRUCTIONS_KEY:
				currStatus = GameModes::INSTRUCTIONS; // show instructions
                menu = false;
                break;

            case Colors_ON_OFF: // Colors ON/OFF
                ColorsEnabled = !ColorsEnabled;
				UIScreens::showMenu(); // redraw menu to show updated color status
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
	currentRoomMeta = currentScreen.getRoomMeta();// get current room metadata
	currentScreen.resetTorchState();// reset torch state
	Key::placeFromMetadata(currentScreen);// place key from metadata
	currentRoomMeta.placeLightSwitchFromMetadata(currentScreen);// place light switch from metadata
    // Start timer on first level
    if (specificDoor == -1 && currentLevelIdx == 0)
    {
        gameTimer = maxGameTime;
        timerActive = true;
    }
    drawCurrentScreen();
	// Attach riddle positions to the room
    riddleBank.attachPositionToRoom(currentScreen);
    player1.setScreen(currentScreen);
    player2.setScreen(currentScreen);

	if (!player1.isActive()) // activate players
        player1.activate();
	if (!player2.isActive()) // activate players
        player2.activate();

	if (specificDoor != -1) // if coming from a door, place players there
        placePlayersAtEntrance(specificDoor); 
    else
    {
        player1.setPosition(Point(P1_START_X, P1_START_Y, Direction::directions[Direction::STAY], '&'));
        player2.setPosition(Point(P2_START_X, P2_START_Y, Direction::directions[Direction::STAY], '$'));
    }
    drawCurrentScreen();
    player1.draw();
    player2.draw();

	if (!Switch::exists(currentScreen)) // if no switches, set allSwitchesAreOn to true
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
	if (currentScreen.isDark()) // redraw based on dark/torch state
        currentScreen.drawMapWithTorch(player1);
    else
        currentScreen.drawMap();;    player1.draw();
    player2.draw();
}
// Helper function to handle screen drawing based on torch / dark state
void Game::drawCurrentScreen()
{
    bool isDark = currentScreen.getRoomMeta().isDark();

    if (isDark)
    {
		bool hasTorch = Torch::playerHasTorch(player1) || Torch::playerHasTorch(player2); // check if any player has torch

		if (hasTorch) // draw with torch
        {
            if (Torch::playerHasTorch(player1))
                currentScreen.drawMapWithTorch(player1);
            else if (Torch::playerHasTorch(player2))
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
	if (player1.isActive())
        player1.draw();
	if (player2.isActive())
        player2.draw();

    int bombTimerDisplay = -1;
    if (activeBomb != nullptr)
    {
        bombTimerDisplay = activeBomb->getTimer();
    }

    currentScreen.drawStatusBar(
        player1.getHeldItem(), player1.getLives(), player1.getScore(),
        player2.getHeldItem(), player2.getLives(), player2.getScore(),
        bombTimerDisplay, timerActive ? gameTimer : -1);
}

// Minimal redraw - just update what changed
void Game::updateDisplay()
{
	// get bomb timer
	int bombTimerDisplay = -1;  // -1 = if no bomb
    if (activeBomb != nullptr)
    {
        bombTimerDisplay = activeBomb->getTimer();
    }

	// draw status bar
    currentScreen.drawStatusBar(
        player1.getHeldItem(), player1.getLives(), player1.getScore(),
        player2.getHeldItem(), player2.getLives(), player2.getScore(),
        bombTimerDisplay, timerActive ? gameTimer : -1);

	// draw players if active
    if (player1.isActive())
        player1.draw();
    if (player2.isActive())
        player2.draw();

	// draw bomb if active
    if (activeBomb != nullptr)
    {
        Point bombPos = activeBomb->getPosition();
        gotoxy(bombPos.getX(), bombPos.getY());
        setColor(COLOR_RED);
        std::cout << '@';
        resetColor();
    }
}

// Handle bomb creation
void Game::updateBomb()
{
    if (activeBomb != nullptr)
    {
        int bombRoomID = activeBomb->getRoomID();
        if (bombRoomID == currentLevelIdx)
        {
            if (activeBomb->tick(currentScreen, player1, player2, currentLevelIdx))
            {
                delete activeBomb;
                activeBomb = nullptr;
            }
        }
        else 
        {       
            delete activeBomb;
            activeBomb = nullptr;
   
        }
    }
    if (player1.hasDroppedBomb() && activeBomb == nullptr)
    {
        activeBomb = new Bomb(player1.getPosition(), player1.getChar(),currentLevelIdx);
        player1.clearBombRequest();
    }
    else if (player2.hasDroppedBomb() && activeBomb == nullptr)
    {
        activeBomb = new Bomb(player2.getPosition(), player2.getChar(), currentLevelIdx);
        player2.clearBombRequest();
    }    
}

// Handle player movement and collisions
void Game::updatePlayers()
{
	char underPlayer1 = ' ';// to track what is under each player
    char underPlayer2 = ' ';

    if (player1.isActive())
    {
        underPlayer1 = currentScreen.getCharAt(player1.getX(), player1.getY());
        player1.erase();
        // Restore 'A' if it was there
        if (underPlayer1 == 'A')
            currentScreen.setCharAt(player1.getX(), player1.getY(), 'A');
    }
        

    if (player2.isActive())
    {
        underPlayer2 = currentScreen.getCharAt(player2.getX(), player2.getY());
        player2.erase();
        // Restore 'A' if it was there
        if (underPlayer2 == 'A')
            currentScreen.setCharAt(player2.getX(), player2.getY(), 'A');
    }

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
    Point p1PosLastFrame = player1.getPosition();
    Point p2PosLastFrame = player2.getPosition();
	player1LastPos = p1PosLastFrame;
	player2LastPos = p2PosLastFrame;
	// timer tracker (taken from AI)
    DWORD lastTickTime = GetTickCount();
    const DWORD timerInterval = 1000;
    redrawGame(); // Draw full screen at start

    while (gameRunning)
    {
        // Update timer
        if (timerActive) {
            DWORD currentTime = GetTickCount();
            if (currentTime - lastTickTime >= timerInterval) {
                gameTimer--;
                lastTickTime = currentTime;

                // Check if time's up
                if (gameTimer <= 0) {
                    UIScreens::showGameOverMessage();
                    resetGame();
                    currStatus = GameModes::MENU;
                    gameRunning = false;
                    break;
                }
            }
        }
        // Handle pause request from riddle
        if (pauseRequestedFromRiddle)
        {
            pauseRequestedFromRiddle = false;
            handlePause(currentScreen, gameRunning);
            clearInputBuffer();
            if (!gameRunning) break;
            redrawGame();
            p1PosLastFrame = player1.getPosition();
            p2PosLastFrame = player2.getPosition();
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
                if (!gameRunning) break;
                redrawGame();
                p1PosLastFrame = player1.getPosition();
                p2PosLastFrame = player2.getPosition();
                continue;
            }
            else
            {
                player1.keyPressed(ch);
                player2.keyPressed(ch);
            }
        }

        // Update game state
        updateBomb();
        updatePlayers();
        
		player1LastPos = p1PosLastFrame;// update last positions after movement
		player2LastPos = p2PosLastFrame;

		// check if players moved
        bool p1Moved = player1.isActive() &&
            ((p1PosLastFrame.getX() != player1.getPosition().getX()) ||
                (p1PosLastFrame.getY() != player1.getPosition().getY()));
        bool p2Moved = player2.isActive() &&
            ((p2PosLastFrame.getX() != player2.getPosition().getX()) ||
                (p2PosLastFrame.getY() != player2.getPosition().getY()));

		// draw based on dark / torch state if players moved
        bool isDark = currentScreen.getRoomMeta().isDark();
        if (isDark && (p1Moved || p2Moved))
        {
			// check who has torch
            if (Torch::playerHasTorch(player1))
            {
                currentScreen.drawMapWithTorch(player1);
            }
            else if (Torch::playerHasTorch(player2))
            {
                currentScreen.drawMapWithTorch(player2);
            }
            else
            {
                currentScreen.drawDark();
            }
        }
		// draw bomb   
        drawActiveBomb();
        // Update players and status bar
        updateDisplay();


        // Check end conditions
        if (checkGameOver())
        {
            gameRunning = false;
        }

		// when a player goes through a door
        if ((!player1.isActive() || !player2.isActive()) && activeDoor != ' ')
        {
            if (player1.isActive()) {
                player1.rememberPosition();
                player1.erase();
            }
            if (player2.isActive()) {
                player2.rememberPosition();
                player2.erase();
            }
            redrawGame();
            p1PosLastFrame = player1.getPosition();
            p2PosLastFrame = player2.getPosition();
            player1LastPos = p1PosLastFrame;
            player2LastPos = p2PosLastFrame;
            activeDoor = ' ';
            currentScreen.setDark(currentScreen.getRoomMeta().isDark());
        }


		// remember last positions
        p1PosLastFrame = player1.getPosition();
        p2PosLastFrame = player2.getPosition();

        Sleep(GAME_DELAY);
    }
}

bool Game::handleTile(Player& player)// handle tile interaction for a player
{
    // get reference to the other player
    Point pos = player.getPosition();
    Point targetPos = pos;
    targetPos.move();
    char cell = currentScreen.getCharAt(pos);
    char targetCell = currentScreen.getCharAt(targetPos);
	Point& lastPos = (&player == &player1) ? player1LastPos : player2LastPos;

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
    case 'A': // light switch
        if (currentRoomMeta.hasLightSwitchAt(pos.getX(), pos.getY()))
        {
			if (lastPos.getX() != pos.getX() || lastPos.getY() != pos.getY()) // Check if switch position changed
            {
                // Get the room meta by reference and activate switch
                RoomMeta meta = currentScreen.getRoomMeta();
                meta.activateLightSwitch();
                currentScreen.setRoomMeta(meta);
                currentScreen.setDark(meta.isDark()); 
                drawCurrentScreen(); // Redraw the screen with new lighting

                // Get reference to other player
                Player& otherPlayer = (&player == &player1) ? player2 : player1;
                if (otherPlayer.isActive())
                    otherPlayer.draw();
            }
        }
        break;

    case '?':// riddle
        riddleBank.handleRiddle(player, currentScreen, currentLevelIdx);
        break;

    case 'K':// key
        if (player.getHeldItem() == ' ' || player.getHeldItem() == 0)
        {
            Key keyFromRoom = currentRoomMeta.getRoomKey();  // get the key info from room meta
            player.GrabItem('K', keyFromRoom.getDoorID());   
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
        bool doorOpened = Door::handleDoor(player, currentScreen, activeDoor);// try to handle door
        if (doorOpened)
        {
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
bool Game::checkLevel() // check if level is completed
{
    if (!player1.isActive() && !player2.isActive())
    {
		allLevels[currentLevelIdx] = currentScreen; // save current screen state

        int doorId = activeDoor - '0';
  Door* door = currentScreen.getDoorById(doorId); // get door object
  if (!door) return false;

  int nextLevelIdx = door->getDestinationLevel();

  // if next level index is valid, load next level
  if (nextLevelIdx >= 0 && nextLevelIdx < (int)allLevels.size())
  {
      char p1Item = player1.getHeldItem();
      int p1ItemId = player1.getItemId();
      char p2Item = player2.getHeldItem();
      int p2ItemId = player2.getItemId();

			// update current level index and screen
            currentLevelIdx = nextLevelIdx;
            currentScreen = allLevels[currentLevelIdx];
            currentScreen.setDark(currentScreen.getRoomMeta().isDark());
            initLevel(screenFileNames[currentLevelIdx], doorId);
            if (p1Item != ' ' && p1Item != 0)
                player1.GrabItem(p1Item, p1ItemId);
            if (p2Item != ' ' && p2Item != 0)
                player2.GrabItem(p2Item, p2ItemId);
            activeDoor = ' ';
            return false;
        }
        else
        {
			// you won the game
            showWinScreen();
            return true;
        }
    }
    return false;
}
void Game::placePlayersAtEntrance(int specificDoor)
{
    int targetDoorId = -1;
	// step 1 look for door that leads to previous level
    if (specificDoor != -1 && currentLevelIdx > 0)
    {
        for (int i = 1; i <= 9; i++)
        {
            int leadsTo = currentScreen.getRoomMeta().getDoorLeadsTo(i);
            if (leadsTo == currentLevelIdx - 1)
            {
                targetDoorId = i;
                break;
            }
        }
    }

	// step 2 : if not found, look for any door (lowest numbered)
    if (targetDoorId == -1)
    {
        int lowestDoor = 10;
        Point targetDoorPos;
        bool found = false;

        for (int y = 0; y < Screen::MAP_HEIGHT && !found; y++)
        {
            for (int x = 0; x < Screen::WIDTH && !found; x++)
            {
                char c = currentScreen.getCharAt(x, y);
                if (c >= '1' && c <= '9')
                {
                    int id = c - '0';

					// if there is a specific door to use, prioritize it
                    if (specificDoor != -1 && id == specificDoor)
                    {
                        targetDoorId = id;
                        found = true;
                    }
					// else find the lowest numbered door
                    else if (id < lowestDoor)
                    {
                        lowestDoor = id;
                        targetDoorId = id;
                    }
                }
            }
        }
    }

	// step 3: place players next to the target door
    if (targetDoorId != -1)
    {
        for (int y = 0; y < Screen::MAP_HEIGHT; y++)
        {
            for (int x = 0; x < Screen::WIDTH; x++)
            {
                if (currentScreen.getCharAt(x, y) == ('0' + targetDoorId))
                {
                    placeNextToDoor(Point(x, y, Direction::directions[Direction::STAY], ' '));
                    return;
                }
            }
        }
    }
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
    for (int i = 0; i < Door::MAX_DOOR_ID; i++)
        Door::openDoors[i] = false;

	//reset bombs
    if (activeBomb != nullptr)
    {
        delete activeBomb;
        activeBomb = nullptr;
	}
    // Reset timer
    gameTimer = 0;
    timerActive = false;
}

void Game::drawActiveBomb()
{
    if (activeBomb != nullptr && activeBomb->getRoomID() == currentLevelIdx)
    {
        Point pos = activeBomb->getPosition();

		// draw bomb at its position
        gotoxy(pos.getX(), pos.getY());
        if (ColorsEnabled) {
			setColor(COLOR_RED); // set bomb color
        }

        std::cout << '@';

        if (ColorsEnabled) {
            resetColor();
        }
    }
}