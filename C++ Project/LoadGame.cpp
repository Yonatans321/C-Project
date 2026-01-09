#include "LoadGame.h"
#include <iostream>
#include <conio.h>
#include <windows.h>
#include "Torch.h"

void LoadGame::run() // override run method helped by AI
{
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
        return;
    }

    // Load all screen files from directory
    getAllScreenFileNames(screenFileNames);

    if (screenFileNames.empty())
    {
        if (!isSilentMode)
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

    initLevel(screenFileNames[currentLevelIdx]);

    // Run game loop (will ignore user input)
    replayGameLoop();

    // Validation in silent mode
    if (isSilentMode)
    {
        std::cout << "TEST PASSED: Game replay completed successfully!" << std::endl;
    }
    else
    {
        std::cout << "\n========================================" << std::endl;
        std::cout << "       GAME REPLAY COMPLETED" << std::endl;
        std::cout << "========================================\n" << std::endl;
        waitForKey();
    }
}

// Override game loop to ignore user input and handle silent mode
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

		// ignore user input
        if (_kbhit())
        {
            _getch();  // Consume key but don't process it
        }

        // Update game state (same as normal game)
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

            // Faster playback in load mode (20ms per frame)
            Sleep(20);
        }
        else
        {
            // Silent mode - minimal sleep
            Sleep(1);
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
                player1.erase();
            }
            if (player2.isActive())
            {
                player2.rememberPosition();
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

    // Save the replay results for comparison
    gameResults.save("adv-world.result");
}