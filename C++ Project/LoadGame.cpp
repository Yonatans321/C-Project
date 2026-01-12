#include "LoadGame.h"
#include <iostream>
#include <fstream>
#include <conio.h>
#include <windows.h>
#include <filesystem>
#include "Torch.h"
#include "Key.h"
#include "Switch.h"
#include "Door.h"

bool LOAD_MODE = false;

void LoadGame::run()
{
    LOAD_MODE = true;
    if (!isSilentMode)
    {
        cls();
        std::cout << "\n========================================" << std::endl;
        std::cout << "       LOAD MODE - Game Replay" << std::endl;
        std::cout << "========================================\n" << std::endl;
        Sleep(1000);
    }

    // Load expected results file
    Results expectedResults;
    try
    {
        expectedResults.load("adv-world.result");
    }
    catch (const std::exception&)
    {
        if (!isSilentMode)
        {
            std::cout << "ERROR: Could not load adv-world.result!" << std::endl;
            std::cout << "Make sure you ran -save mode first." << std::endl;
        }
        else
        {
            std::cout << "ERROR: adv-world.result not found!" << std::endl;
        }
        return;
    }

    // Check if adv-world.steps exists
    loadedSteps = Steps::loadSteps("adv-world.steps");
    if (!loadedSteps.hasMoreSteps())
    {
        if (!isSilentMode)
        {
            std::cout << "ERROR: Could not load adv-world.steps!" << std::endl;
            std::cout << "Steps file not found. Make sure you ran -save mode first." << std::endl;
        }
        else
        {
            std::cout << "ERROR: adv-world.steps not found!" << std::endl;
        }
        return;
    }
    loadedSteps.resetReplay();

    // Load all screen files from directory
    getAllScreenFileNames(screenFileNames);

    if (screenFileNames.empty())
    {
        if (!isSilentMode)
        {
            std::cout << "ERROR: No screen files found!" << std::endl;
        }
        else
        {
            std::cout << "ERROR: No screen files found!" << std::endl;
        }
        return;
    }

    // Load all level screens
    allLevels.clear();
    for (const auto& fileName : screenFileNames)
    {
        Screen tempScreen;
        if (!tempScreen.loadMapFromFile(fileName))
        {
            if (!isSilentMode)
            {
                std::cout << "ERROR: Could not load screen file: " << fileName << std::endl;
            }
            else
            {
                std::cout << "ERROR: Could not load screen file: " << fileName << std::endl;
            }
            return;
        }
        allLevels.push_back(tempScreen);
    }

    // Initialize game state
    resetGame();
    currentLevelIdx = 0;
    currentScreen = allLevels[currentLevelIdx];
    gameResults = Results();
    std::string screensString = "";
    for (size_t i = 0; i < screenFileNames.size(); i++)
    {
        if (i > 0) screensString += "|";
        screensString += screenFileNames[i];
    }
    gameResults.setScreenFiles(screensString);
    gameResults.addScreenChange(eventTimer, screenFileNames[currentLevelIdx]);

    // Initialize level - only draw if NOT in silent mode
    if (!isSilentMode)
    {
        initLevel(screenFileNames[currentLevelIdx]);
    }
    else
    {
        initLevelSilent(screenFileNames[currentLevelIdx]);
    }

    riddleBank.attachSteps(&loadedSteps, true);

    // Tell RiddleBank if we're in SILENT mode
    if (isSilentMode)
    {
        riddleBank.setSilentMode(true);
    }

    // Run game loop
    replayGameLoop();

    // ===== NO FILE SAVING - Just compare in memory =====

    // Validation result output
    if (isSilentMode)
    {
        cls();
        gotoxy(0, 0);

        std::string failureReason;
        bool testPassed = gameResults.compareWith(expectedResults, failureReason);

        if (testPassed)
        {
            std::cout << "TEST PASSED: Game replay matches expected results!" << std::endl;
        }
        else
        {
            std::cout << "TEST FAILED: Game replay does NOT match expected results!" << std::endl;
            std::cout << failureReason << std::endl;
        }
    }
    else
    {
        cls();
        gotoxy(18, 10);
        std::cout << "\n========================================" << std::endl;
        std::cout << "       GAME REPLAY COMPLETED" << std::endl;
        std::cout << "========================================\n" << std::endl;
        waitForKey();
    }
}

void LoadGame::initLevelSilent(const std::string& filename)
{
    currentRoomMeta = currentScreen.getRoomMeta();
    currentScreen.resetTorchState();

    if (currentRoomMeta.hasKeyPosition())
    {
        int doorID = currentRoomMeta.getKeyDoorID();
        if (doorID != -1 && !Door::openDoors[doorID] &&
            player1.getItemId() != doorID &&
            player2.getItemId() != doorID)
        {
            Key::placeFromMetadata(currentScreen);
        }
    }

    currentRoomMeta.placeLightSwitchFromMetadata(currentScreen);

    if (currentLevelIdx == 0)
    {
        gameTimer = maxGameTime;
        timerActive = true;
    }

    riddleBank.attachPositionToRoom(currentScreen);
    riddleBank.attachResults(&gameResults, &eventTimer);
    player1.setScreen(currentScreen);
    player2.setScreen(currentScreen);

    if (!player1.isActive())
        player1.activate();
    if (!player2.isActive())
        player2.activate();

    player1.setPosition(Point(P1_START_X, P1_START_Y, Direction::directions[Direction::STAY], '&'));
    player2.setPosition(Point(P2_START_X, P2_START_Y, Direction::directions[Direction::STAY], '$'));

    if (!Switch::exists(currentScreen))
        Door::allSwitchesAreOn();
}

void LoadGame::gameLoop()
{
    replayGameLoop();
}

void LoadGame::replayGameLoop()
{
    bool gameRunning = true;
    Point p1PosLastFrame = player1.getPosition();
    Point p2PosLastFrame = player2.getPosition();
    player1LastPos = p1PosLastFrame;
    player2LastPos = p2PosLastFrame;

    ULONGLONG lastTickTime = GetTickCount64();
    const DWORD timerInterval = 1000;

    if (!isSilentMode)
    {
        redrawGame();
    }

    while (gameRunning)
    {
        eventTimer++;

        // Update timer
        if (timerActive)
        {
            ULONGLONG currentTime = GetTickCount64();
            if (currentTime - lastTickTime >= timerInterval)
            {
                gameTimer--;
                lastTickTime = currentTime;

                if (gameTimer <= 0)
                {
                    gameResults.addGameOver(eventTimer, player1.getScore(), player2.getScore());
                    gameRunning = false;
                    break;
                }
            }
        }

        Steps::Step currentStep;
        if (loadedSteps.getNextStep(eventTimer, currentStep))
        {
            char key = currentStep.key;
            player1.keyPressed(key);
            player2.keyPressed(key);
        }

        // Ignore all user input in LOAD mode, but check for automatic steps
        if (_kbhit())
        {
            _getch();
        }

        // Update game state
        updateBomb();
        updatePlayers();

        player1LastPos = p1PosLastFrame;
        player2LastPos = p2PosLastFrame;

        bool p1Moved = player1.isActive() &&
            ((p1PosLastFrame.getX() != player1.getPosition().getX()) ||
                (p1PosLastFrame.getY() != player1.getPosition().getY()));
        bool p2Moved = player2.isActive() &&
            ((p2PosLastFrame.getX() != player2.getPosition().getX()) ||
                (p2PosLastFrame.getY() != player2.getPosition().getY()));

        // Render only if not in silent mode
        if (!isSilentMode)
        {
            bool isDark = currentScreen.getRoomMeta().isDark();
            if (isDark)
            {
                if (Torch::playerHasTorch(player1))
                    currentScreen.drawMapWithTorch(player1);
                else if (Torch::playerHasTorch(player2))
                    currentScreen.drawMapWithTorch(player2);
            }

            drawActiveBomb();
            updateDisplay();

            Sleep(20);
        }

        // Check end conditions
        if (checkGameOver())
        {
            gameRunning = false;
        }

        // Handle door transitions
        if ((!player1.isActive() || !player2.isActive()) && activeDoor != ' ')
        {
            if (player1.isActive())
            {
                player1.rememberPosition();
                if (!isSilentMode)
                    player1.erase();
            }
            if (player2.isActive())
            {
                player2.rememberPosition();
                if (!isSilentMode)
                    player2.erase();
            }

            if (!isSilentMode)
            {
                redrawGame();
            }

            p1PosLastFrame = player1.getPosition();
            p2PosLastFrame = player2.getPosition();
            player1LastPos = p1PosLastFrame;
            player2LastPos = p2PosLastFrame;
            activeDoor = ' ';
            currentScreen.setDark(currentScreen.getRoomMeta().isDark());
        }

        p1PosLastFrame = player1.getPosition();
        p2PosLastFrame = player2.getPosition();
    }
}

bool LoadGame::checkGameOver()
{
    if (player1.isDead() || player2.isDead())
    {
        if (!isSilentMode)
        {
            UIScreens::showGameOverMessage();
        }
        gameResults.addGameOver(eventTimer, player1.getScore(), player2.getScore());
        resetGame();
        return true;
    }

    if (checkLevel())
    {
        return true;
    }

    return false;
}

bool LoadGame::checkLevel()
{
    if (!player1.isActive() && !player2.isActive())
    {
        allLevels[currentLevelIdx] = currentScreen;

        int doorId = activeDoor - '0';
        Door* door = currentScreen.getDoorById(doorId);
        if (!door) return false;

        int nextLevelIdx = door->getDestinationLevel();

        if (nextLevelIdx >= 0 && nextLevelIdx < (int)allLevels.size())
        {
            char p1Item = player1.getHeldItem();
            int p1ItemId = player1.getItemId();
            char p2Item = player2.getHeldItem();
            int p2ItemId = player2.getItemId();

            currentLevelIdx = nextLevelIdx;
            currentScreen = allLevels[currentLevelIdx];
            currentScreen.setDark(currentScreen.getRoomMeta().isDark());

            if (!isSilentMode)
            {
                initLevel(screenFileNames[currentLevelIdx], doorId);
            }

            gameResults.addScreenChange(eventTimer, screenFileNames[currentLevelIdx]);

            if (p1Item != ' ' && p1Item != 0)
                player1.GrabItem(p1Item, p1ItemId);
            if (p2Item != ' ' && p2Item != 0)
                player2.GrabItem(p2Item, p2ItemId);
            activeDoor = ' ';
            return false;
        }
        else
        {
            // You won the game
            if (!isSilentMode)
            {
                showWinScreen();
            }

            gameResults.addGameFinished(eventTimer, player1.getScore(), player2.getScore());

            return true;
        }
    }
    return false;
}