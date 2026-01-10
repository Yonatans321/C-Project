#include "RiddleBank.h"
#include "Game.h"
#include "SaveGame.h"

#include <iostream>
#include <string>
#include <conio.h>
#include <Windows.h>
#include "Utils.h"
#include <fstream>

RiddleBank::RiddleBank() : riddleCount(0), loadedSuccessfully(false)
{
    // Try to load riddles from file
    std::ifstream file("Riddles.txt");

	if (!file.is_open()) {// File could not be opened
        std::cout << "\n========================================" << std::endl;
        std::cout << "ERROR: Could not open Riddles.txt file!" << std::endl;
        std::cout << "Please make sure Riddles.txt is in the same folder as the game." << std::endl;
        std::cout << "========================================\n" << std::endl;
        loadedSuccessfully = false;
        return;
    }

	// Read riddles from file helped by AI
    // Format: ID, Question, Answer, Hint (each on separate lines)
    std::string line;
    while (riddleCount < MAX_RIDDLES && std::getline(file, line)) {
        // Read id
        int id = std::stoi(line);

        // Read Question
        std::string question;
        if (!std::getline(file, question)) break;
        question = processNewlines(question);

        // Read Answer
        std::string answer;
        if (!std::getline(file, answer)) break;

        // Read Hint
        std::string hint;
        if (!std::getline(file, hint)) break;

        // Create riddle with allocated strings
        riddles[riddleCount++] = Riddle(id,
            allocateString(question),
            allocateString(answer),
            allocateString(hint));
    }

    file.close();

    if (riddleCount > 0) {
        std::cout << "Successfully loaded " << riddleCount << " riddles from Riddles.txt" << std::endl;
        loadedSuccessfully = true;
    }
    else {
        std::cout << "ERROR: No riddles were loaded from Riddles.txt!" << std::endl;
        loadedSuccessfully = false;
    }
}
// Retrieve a riddle by its ID (returns nullptr if not found)
Riddle* RiddleBank::getRiddleById(int riddleID)
{
    for (size_t i = 0; i < riddleCount; ++i) {

        if (riddles[i].getRiddleID() == riddleID) {
            return &riddles[i];
        }
    }
    return nullptr; // Not found
}
void RiddleBank::addRiddle(const Riddle& r)// Add a riddle to the bank 
{
    if (riddleCount < MAX_RIDDLES) {
        riddles[riddleCount++] = r;
    }
}
Riddle* RiddleBank::getRiddleAt(int x, int y)// Retrieve a riddle by its position (x, y) on the screen
{// Check each riddle's position
    for (size_t i = 0; i < riddleCount; ++i) {
        Point p = riddles[i].getPosition();
        if (p.getX() == x && p.getY() == y) {
            return &riddles[i];
        }
    }
    return nullptr; // Not found
}
// AI generated function to attach positions to unsolved riddles based on '?' locations on the screen
void RiddleBank::attachPositionToRoom(Screen& screen)
{
    const RoomMeta& meta = screen.getRoomMeta();
    bool hasError = false;
    std::string errorMessages = "";

    // First, assign riddles based on metadata
    for (int i = 0; i < meta.getRiddleCount(); i++)
    {
        int riddleID = meta.getRiddleID(i);
        int x = meta.getRiddleX(i);
        int y = meta.getRiddleY(i);

        // Find the riddle with this ID
        Riddle* r = getRiddleById(riddleID);
        if (!r)
        {
            hasError = true;
            errorMessages += "ERROR: Riddle ID " + std::to_string(riddleID) +
                " not found in Riddles.txt!\n";
            continue;
        }
		// Skip already solved riddles
        if (r->isSolved())
        {
            continue;
        }
		// Validate position
        if (x < 0 || x >= Screen::WIDTH || y < 0 || y >= Screen::MAP_HEIGHT)
        {
            hasError = true;
            errorMessages += "ERROR: Riddle ID " + std::to_string(riddleID) +
                " has invalid position (" + std::to_string(x) +
                ", " + std::to_string(y) + ")!\n";
            continue;
        }
		char cell = screen.getCharAt(x, y);
		if (cell == '?') // Correct placement
        {
            r->setPosition(Point(x, y));
            continue;
        }
		if (cell == ' ')// Empty cell - place the riddle here
        {
            r->setPosition(Point(x, y));
            screen.setCharAt(x, y, '?'); // Place '?' at the position
            continue;
        }
		hasError = true;// Cell is occupied
        errorMessages += "ERROR: Riddle ID " + std::to_string(riddleID) +
            " position (" + std::to_string(x) + ", " +
            std::to_string(y) + ") is not empty or '?'. Current: '" +
            cell + "'\n";
    }

    // Then handle any remaining '?' marks without metadata (backward compatibility)
    const int W = Screen::WIDTH;
    const int H = Screen::MAP_HEIGHT;

    for (int y = 0; y < H; ++y)
    {
        for (int x = 0; x < W; ++x)
        {
            if (screen.getCharAt(x, y) != '?')
                continue;

            // Check if a riddle is already assigned here
            if (getRiddleAt(x, y) != nullptr)
                continue;
            screen.setCharAt(x, y, ' ');
        }
    }
	// Display errors if any
    if (hasError) {
        gotoxy(0, 23);
        std::cout << errorMessages;
        std::cout << "Press any key to continue..." << std::endl;
        waitForKey();
        cls();
    }
}

// AI generated function to check the answer for a given riddle ID
RiddleOutcome RiddleBank::checkAnswerFor(int riddleID, const std::string& answer)
{
    Riddle* r = getRiddleById(riddleID);
	if (!r) return RiddleOutcome::NotFound; // Riddle ID not found

	if (r->isSolved()) return RiddleOutcome::AlreadySolved; // Already solved

    if (r->checkAnswer(answer.c_str())) {
        return RiddleOutcome::Correct;
    }
    return RiddleOutcome::Incorrect;
}
// AI generated function to display the riddle question inside a box
void RiddleBank::displayRiddleQuestion(Riddle* r, int bx, int by)
{
    gotoxy(bx + 2, by + 1);
    std::cout << "Riddle:";

    std::string q = r->getQuestion();
    int line = by + 3;
    std::string temp;

    for (char c : q)
    {
        if (c == '\n')
        {
            gotoxy(bx + 2, line++);
            std::cout << temp;
            temp.clear();
        }
        else temp += c;
    }
    if (!temp.empty())
    {
        gotoxy(bx + 2, line++);
        std::cout << temp;
    }
}

// AI generated function to get user answer with hint support
std::string RiddleBank::getUserAnswer(int bx, int answerLine, int answerInputLine,
    int hintorResultLine, Riddle* r)
{
    // Display prompt
    gotoxy(bx, answerLine);
    std::cout << "Answer (press H for hint): ";
    gotoxy(bx, answerInputLine);
    std::cout << ">>> " << std::string(40, ' ');

    std::string ans = "";
    int promptX = bx;
    int cursorX = promptX + 4;
    int cursorY = answerInputLine;

    gotoxy(cursorX, cursorY);
    // Input loop
    while (true)
    {
        char c = _getch(); // get character from user
		// Record the keystroke if in SAVE_MODE
        if (SAVE_MODE && recordedSteps != nullptr && eventTimerPtr != nullptr)
        {
            // Record ALL keystrokes during riddle input
            // We'll use player 0 to indicate it's a riddle input, not player movement
            recordedSteps->addStep(*eventTimerPtr, 0, c);
        }

        // ESC → pause the game
        if (c == ESC)
        {
            Game::pauseRequestedFromRiddle = true;
            return "";
        }

        if (c == '\r')   // ENTER
        {
            return ans;
        }
        else if ((c == HINT || c == tolower(HINT)) && ans.empty()) // HINT
        {
            // show hint
            gotoxy(bx, hintorResultLine);
            std::cout << std::string(50, ' ');
            gotoxy(bx, hintorResultLine);
            std::cout << "Hint: " << r->getHint();
            gotoxy(cursorX + (int)ans.length(), cursorY);
            continue;
        }
        if (c == BACKSPACE) // BACKSPACE
        {
            if (!ans.empty())
            {
                ans.pop_back();
                gotoxy(cursorX + (int)ans.length(), cursorY);
                std::cout << ' ';
                gotoxy(cursorX + (int)ans.length(), cursorY);
            }
            continue;
        }
        if (ans.length() < 40)
        {
            ans.push_back(c);
            gotoxy(cursorX + (int)ans.length() - 1, cursorY);
            std::cout << c;
        }
    }
}

// AI generated func to help handle the Riddle when player steps on '?'
void RiddleBank::handleRiddle(Player& player, Screen& screen, int level)
{
    int x = player.getX();
    int y = player.getY();
    char cell = screen.getCharAt(x, y);

    if (cell != '?')
        return;

    Riddle* r = getRiddleAt(x, y);
    if (!r || r->isSolved())
        return;

    const int bx = 15, by = 5, bw = 50, bh = 12;
    
	screen.drawAnimatedBox(bx, by, bw, bh); // Draw initial box
    gotoxy(bx + 2, by + 2); std::cout << "You stepped on a riddle.";
    gotoxy(bx + 2, by + 4); std::cout << "Answer it? (Y/N): ";

    while (true)
    {
        char choice = _getch();
		// Record the choice if in SAVE_MODE
        if (SAVE_MODE && recordedSteps != nullptr && eventTimerPtr != nullptr)
        {
            recordedSteps->addStep(*eventTimerPtr, 0, choice);
        }
        // ESC → pause
        if (choice == ESC)
        {
            Game::pauseRequestedFromRiddle = true;
            screen.closeAnimatedBox(bx, by, bw, bh);
            player.stepBack();
            return;
        }
        if (tolower(choice) == tolower(NO))
        {
            screen.closeAnimatedBox(bx, by, bw, bh);
            player.stepBack();
            screen.setCharAt(x, y, '?');

            if (screen.isDark())
                screen.drawMapWithTorch(player);
            else
                screen.drawMap();
            player.draw();

            return;
        }

        if (tolower(choice) == tolower(YES))
        {
            break;
        }
    }
    screen.closeAnimatedBox(bx, by, bw, bh);

    screen.drawAnimatedBox(bx, by, bw, bh);
	displayRiddleQuestion(r, bx, by);  // help function to display question

    int answerLine = by + 7;
    int answerInputLine = by + 8;
    int hintorResultLine = by + 9;

    std::string answer = getUserAnswer(bx + 2, answerLine, answerInputLine,
		hintorResultLine, r);  // help function to get user answer

	//if paused during answer input
    if (Game::pauseRequestedFromRiddle)
    {
        screen.closeAnimatedBox(bx, by, bw, bh);
        player.stepBack();
        return;
    }

    bool correct = r->checkAnswer(answer.c_str());

    // Clear hint line
    gotoxy(bx + 2, hintorResultLine);
    std::cout << std::string(50, ' ');

    gotoxy(bx + 2, hintorResultLine);  // Write feedback

    if (correct)
    {
        std::cout << "Correct! +100 points";
        player.addPoints(100);
        r->markAsSolved();

        // remove '?'
        screen.setCharAt(x, y, ' ');
		if (SAVE_MODE && gameResults != nullptr)  //check if are writing results
        {
            gameResults->addRiddle(*eventTimerPtr, r->getRiddleID(), r->getQuestion(), answer, true);
        }
    }
    else
    {
        std::cout << "Wrong! -1 life";
        player.loseLife();

        screen.setCharAt(x, y, '?');  // put the riddle symbol back
        if (SAVE_MODE && gameResults != nullptr)  //check if are writing results
        {
            PlayerType playerType = (player.getChar() == '&') ? PlayerType::Player1 : PlayerType::Player2;
            gameResults->addRiddle(*eventTimerPtr, r->getRiddleID(), r->getQuestion(), answer, false);
            gameResults->addLifeLost(*eventTimerPtr, playerType);
        }
    }

    Sleep(700);
    screen.closeAnimatedBox(bx, by, bw, bh);

    player.stepBack();  // move player back to avoid re-triggering
    if (screen.isDark())
        screen.drawMapWithTorch(player);
    else
        screen.drawMap();
    player.draw();
}

void RiddleBank::resetAllRiddles() // Reset all riddles to unsolved state
{
    for (size_t i = 0; i < riddleCount; ++i)
    {
        riddles[i].resetRiddle();
    }
}

void RiddleBank::attachResults(Results* results, size_t* timerPtr)
{
    gameResults = results;
    eventTimerPtr = timerPtr;
};

void RiddleBank::attachSteps(Steps* steps)
{
    recordedSteps = steps;
};  
