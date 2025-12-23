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
            std::cout << " ^ added!!\n";
            vec_to_fill.push_back(filenameStr);
        }
    }
}

// Show main menu and handle input
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
    //Screen& currentScreen = gameScreens[currentLevel];

    cls();

    //currentScreen.loadMapFromFile(filename);

    if (filename.find("dark") != std::string::npos)
        currentScreen.setDark(true);
    else
        currentScreen.setDark(false); 

    if (currentScreen.isDark())
        currentScreen.drawMapWithTorch(player1);
    else
        currentScreen.drawMap();

    riddleBank.attachPositionToRoom(currentScreen); // attach riddles to the current screen

    // Assign screen to players
    player1.setScreen(currentScreen);
    player2.setScreen(currentScreen);

	// Reset players' positions
    player1.activate();
    player2.activate();

    if (specificDoor != -1)
   // if (activeDoor >= '1' && activeDoor <= '9')
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
    if (currentScreen.isDark())
        currentScreen.drawMapWithTorch(player1);
    else
        currentScreen.drawMap();;    player1.draw();
    player2.draw();
}
// Main game loop 
void Game::gameLoop()
{
    bool gameRunning = true;

    while (gameRunning)
    {
       // Screen& currentScreen = gameScreens[currentLevel];
        if (pauseRequestedFromRiddle) // stop in the middle of riddle
        {
            pauseRequestedFromRiddle = false;
            handlePause(currentScreen, gameRunning);
            clearInputBuffer();
            if (!gameRunning)
            {
                break;
            }
            if (currentScreen.isDark())
                currentScreen.drawMapWithTorch(player1);
            else
                currentScreen.drawMap();;

            player1.draw();
            player2.draw();
            currentScreen.drawStatusBar(
                player1.getHeldItem(), player1.getLives(), player1.getScore(),
				player2.getHeldItem(), player2.getLives(), player2.getScore());
        }
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

                // redraw after pause
                if (currentScreen.isDark())
                    currentScreen.drawMapWithTorch(player1);
                else
                    currentScreen.drawMap();

                player1.draw();
                player2.draw();
                currentScreen.drawStatusBar(
                    player1.getHeldItem(), player1.getLives(), player1.getScore(),
                    player2.getHeldItem(), player2.getLives(), player2.getScore());
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

        if (currentScreen.isDark())
            currentScreen.drawMapWithTorch(player1);
        

        player1.draw();
        player2.draw();

        currentScreen.drawStatusBar(
            player1.getHeldItem(), player1.getLives(), player1.getScore(),
            player2.getHeldItem(), player2.getLives(), player2.getScore());

        if (player1.isDead() || player2.isDead())
        {
            UIScreens::showGameOverMessage();
            resetGame();
            currStatus = GameModes::MENU;
            return;
        }

        // --- Check level ---
        if (checkLevel())
        {
            gameRunning = false;
            currStatus = GameModes::MENU;
            break;
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
            // 1. קבלת שמות כל הקבצים מהתיקייה
            getAllScreenFileNames(screenFileNames);

            // בדיקה אם קיימים שלבים בכלל לפני שממשיכים
            if (screenFileNames.empty()) {
                cls();
                std::cout << "No screen files found (adv-world*.screen) in the directory!" << std::endl;
                waitForKey();
                currStatus = GameModes::MENU;
                continue;
            }

            // 2. טעינת כל המפות לזיכרון (וקטור allLevels)
            allLevels.clear();
            for (const auto& fileName : screenFileNames) {
                Screen tempScreen;
                tempScreen.loadMapFromFile(fileName);
                allLevels.push_back(tempScreen);
            }

            // 3. איפוס נתוני המשחק (חיים, ניקוד וכו')
            resetGame();

            // 4. הגדרת השלב ההתחלתי (אינדקס 0)
            currentLevelIdx = 0;

            // 5. עדכון currentScreen מהזיכרון - זה מה שמונע מסך שחור!
            currentScreen = allLevels[currentLevelIdx];

            // 6. אתחול השלב (ציור המפה והצבת שחקנים)
            initLevel(screenFileNames[currentLevelIdx]);

            // 7. כניסה ללולאת המשחק
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

}

