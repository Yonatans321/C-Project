#include "SaveGame.h"
#include <iostream>

void SaveGame::run()
{
    std::cout << "\n========================================" << std::endl;
    std::cout << "       SAVE MODE - Game Recording" << std::endl;
    std::cout << "========================================\n" << std::endl;

    while (currStatus != GameModes::EXIT)
    {
        if (currStatus == GameModes::MENU)
        {
            cls();
            clearInputBuffer();
            showMenu();
        }
        else if (currStatus == GameModes::INSTRUCTIONS)
        {
            showInstructions();
        }
        else if (currStatus == GameModes::NEW_GAME)
        {
            getAllScreenFileNames(screenFileNames);

            if (screenFileNames.empty())
            {
                cls();
                std::cout << "ERROR: No screen files found (adv-world*.screen)!" << std::endl;
                waitForKey();
                currStatus = GameModes::MENU;
                continue;
            }

            allLevels.clear();
            bool successLoad = true;
            for (const auto& fileName : screenFileNames)
            {
                Screen tempScreen;
                if (!tempScreen.loadMapFromFile(fileName))
                {
                    successLoad = false;
                    break;
                }
                allLevels.push_back(tempScreen);
            }

            if (!successLoad)
            {
                std::cout << "\nERROR: Failed to load screen files!" << std::endl;
                std::cout << "Press any key to return to Menu..." << std::endl;
                waitForKey();
                currStatus = GameModes::MENU;
                continue;
            }

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
            gameLoop();

            // ===== SAVE FILES =====
            gameResults.save("adv-world.result");

            std::cout << "\n========================================" << std::endl;
            std::cout << "       GAME SAVED SUCCESSFULLY" << std::endl;
            std::cout << "  - adv-world.result" << std::endl;
            std::cout << "========================================\n" << std::endl;
            waitForKey();

            currStatus = GameModes::MENU;
        }
    }

    UIScreens::showExitMessage();
}