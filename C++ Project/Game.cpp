#include "Game.h"
#include <conio.h>
#include <windows.h>
#include <iostream>
#include "Door.h"
#include "Obstacle.h"
#include "Switch.h"

bool Game::pauseRequestedFromRiddle = false;

Game::Game()
    : player1(Point(2, 2, Direction::directions[Direction::STAY], '&'),
        { 'W','D','X','A','S','E' }),
    player2(Point(77, 2, Direction::directions[Direction::STAY], '$'),
        { 'I','L','M','J','K','O' })
{
    hideCursor();
    currStatus = GameModes::MENU;
    for (int i = 0;i < MAX_LEVELS;i++)
    {
        gameScreens[i].loadMap(i);
    }
    player1.setScreen(gameScreens[0]);
    player2.setScreen(gameScreens[0]);
}

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


void Game::showInstructions()
{
    UIScreens::showInstructions();
    while (_kbhit()) _getch(); // פונקציית עזר CLEANBUFFER ב UTILLS להוסיףףףףףףף
    //[[maybe_unused]]
    auto i =_getch();  // Press any key לעשות פונקציית עזר WAIT FOR KEY שיבינו מה עושים
    while (_kbhit()) _getch();

    currStatus = GameModes::MENU;
}

void Game::initLevel()
{
    Screen& currentScreen = gameScreens[currentLevel];

    cls();
    currentScreen.drawMap();
    riddleBank.attachPositionToRoom(currentScreen);

    // Assign screen to players
    player1.setScreen(currentScreen);
    player2.setScreen(currentScreen);

	// Reset players' positions
    player1.activate();
    player2.activate();
    
    if (activeDoor >= '1' && activeDoor <= '9')
    {
        placePlayersAtEntrance();
    }
    else
    {
        player1.setPosition(Point(2, 2, Direction::directions[Direction::STAY], '&'));
        player2.setPosition(Point(77, 2, Direction::directions[Direction::STAY], '$'));
    }

    player1.draw();
    player2.draw();

    if (!Switch::exists(currentScreen))
    {
        Door::allSwitchesAreOn();
    }
}

void Game::handlePause(Screen& currentScreen, bool& gameRunning)
{
    UIScreens::showPauseScreen();
    while (_kbhit()) _getch();

    bool paused = true;
    while (paused)
    {
        if (_kbhit())
        {
            char c = _getch();
            if (c == 27)
                paused = false;
            else if (c == 'H' || c == 'h')
            {
                currStatus = GameModes::MENU;
                gameRunning = false;
                return;
            }
        }
        Sleep(50);
    }

    while (_kbhit()) _getch();
    currentScreen.drawMap();
    player1.draw();
    player2.draw();
}
void Game::gameLoop()
{
    bool gameRunning = true;

    while (gameRunning)
    {
        if (pauseRequestedFromRiddle)
        {
            pauseRequestedFromRiddle = false;
            Screen& cs = gameScreens[currentLevel];
            handlePause(cs, gameRunning);
            while (_kbhit()) _getch();
            if (!gameRunning)
            {
                break;
            }
            cls();
            cs.drawMap();
            player1.draw();
            player2.draw();

            cs.drawStatusBar(player1.getHeldItem(), player1.getLives(), player1.getScore(),player2.getHeldItem(), player2.getLives(), player2.getScore());
        }
        Screen& currentScreen = gameScreens[currentLevel];
        
        // Handle input
        if (_kbhit())
        {
            char ch = _getch();

            // PAUSE → Escape key
            if (ch == 27) // להחליף בקבוע ESC ( יותר קריא בשביל בודק התרגליים)
            {
                handlePause(currentScreen, gameRunning);
                 while (_kbhit()) _getch();

                if (!gameRunning)
                    break;

                // Redraw after pause
                currentScreen.drawMap();
                player1.draw();
                player2.draw();
                currentScreen.drawStatusBar(player1.getHeldItem(), player1.getLives(), player1.getScore(),player2.getHeldItem(), player2.getLives(), player2.getScore());
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

        currentScreen.drawStatusBar(player1.getHeldItem(), player1.getLives(), player1.getScore(), player2.getHeldItem(), player2.getLives(), player2.getScore());

        if (checkLevel() == true)
        {
            gameRunning = false;      // עוצר את הלולאה
            currStatus = GameModes::MENU; // מעדכן סטטוס לתפריט ראשי
            break;                    // יוצא מיידית מהלולאה
        }
    
        Sleep(80);
    }
}


bool Game::handleTile(Player& player)
{
    Screen& currentScreen = gameScreens[currentLevel];
    Point pos = player.getPosition();
    Point targetPos = pos;
    targetPos.move();
    char cell = currentScreen.getCharAt(pos);
    char targetCell = currentScreen.getCharAt(targetPos);

    switch (cell)
    {
    case '?':
        riddleBank.handleRiddle(player, currentScreen, currentLevel);
        break;

    case 'K':
        player.GrabItem('K', currentLevel+1);
        currentScreen.setCharAt(pos, ' ');
        break;

    case '\\':
    case '/':
        Switch::handleSwitch(player, currentScreen);
        return false;
    }

    switch (targetCell)
    {
    case '1': case '2': case '3': case '4': case '5':
    case '6': case '7': case '8': case '9':
    {
        bool doorOpened = Door::handleDoor(player, currentScreen, currentLevel, activeDoor);
        if (doorOpened)
        {

            player.setPosition(targetPos);
            return true;
        }
        break;

    }
    case '*':
    {
        Obstacle::handleObstacle(player1, player2, currentScreen);

        char afterPush = currentScreen.getCharAt(targetPos);

        if (afterPush == '*')
            return true;    // עדיין חסום → אל תזוז ואל תחזיר אחורה

        player.setPosition(targetPos);
        return true;
    }

    case 's':
        return true;
        break;

    }

    return false; // לא היה אירוע מיוחד
    
}

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
            resetGame();
            initLevel();
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
        if (activeDoor == '1')
        {
            if (currentLevel == 0)
                currentLevel = 1;
            else if (currentLevel == 1)
                currentLevel = 0;
        }
        else if (activeDoor == '2')
        {
            if (currentLevel == 1)
                currentLevel = 2;
            else if (currentLevel == 2)
                currentLevel = 1;
        }
        else if (activeDoor == '3')
        {
            showWinScreen();
            activeDoor = ' ';
            return true;
        }
        initLevel();
        activeDoor = ' ';
    }
    return false;
}

void Game::placePlayersAtEntrance()
{
    Screen& currentScreen = gameScreens[currentLevel];

    int lowestDoor = 10; 
    Point lowestDoorPos;
    bool found = false;

    for (int y = 0; y < Screen::MAP_HEIGHT; y++)
    {
        for (int x = 0; x < Screen::WIDTH; x++)
        {
            char c = currentScreen.getCharAt(x, y);
            if (c >= '1' && c <= '9')
            {
                int id = c - '0';
                if (id < lowestDoor)
                {
                    lowestDoor = id;
                    lowestDoorPos = Point(x, y, Direction::directions[Direction::STAY], ' ');
                    found = true;
                }
            }
        }
    }

    if (!found)
        return; // אין דלתות בכלל

    // מציבים את השחקנים בצמוד לדלת — מימין לה
    int px = lowestDoorPos.getX();
    int py = lowestDoorPos.getY();

    // שחקן 1
    Point p1(px + 1, py, Direction::directions[Direction::STAY], '$');
    player1.setPosition(p1);

    // שחקן 2 לידו
    Point p2(px + 2, py, Direction::directions[Direction::STAY], '&');
    player2.setPosition(p2);

    player1.draw();
    player2.draw();
}


void Game::resetGame()
{
    currentLevel = 0;
    activeDoor = ' ';
    Door::switchesAreOn = false;

    // לאפס מערך דלתות פתוחות
    for (int i = 0; i < 10; i++)
        Door::openDoors[i] = false;

    // לטעון מחדש את כל המסכים
    for (int i = 0; i < 3; i++)   // כמה מסכים שיש לך
        gameScreens[i].loadMap(i);

    // לאפס שחקנים
    player1.activate();
    player2.activate();

    player1.setPosition(Point(2, 2, Direction::directions[Direction::STAY], '&'));
    player2.setPosition(Point(77, 2, Direction::directions[Direction::STAY], '$'));

}