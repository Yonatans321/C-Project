#include "RiddleBank.h"
#include "Game.h"
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

    if (!file.is_open()) {
        std::cout << "\n========================================" << std::endl;
        std::cout << "ERROR: Could not open Riddles.txt file!" << std::endl;
        std::cout << "Please make sure Riddles.txt is in the same folder as the game." << std::endl;
        std::cout << "========================================\n" << std::endl;
        loadedSuccessfully = false;
        return;
    }

    // Read riddles from file
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
            std::to_string(y) + ") is not empty or '?'! Current: '" +
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
        std::cout << "\n========================================" << std::endl;
        std::cout << "RIDDLE PLACEMENT ERRORS:" << std::endl;
        std::cout << "========================================" << std::endl;
        std::cout << errorMessages;
        std::cout << "========================================\n" << std::endl;
        std::cout << "Press any key to continue..." << std::endl;
        _getch();
    }
}

// AI generated function to check the answer for a given riddle ID
RiddleOutcome RiddleBank::checkAnswerFor(int riddleID, const std::string& answer)
{
    Riddle* r = getRiddleById(riddleID);
    if (!r) return RiddleOutcome::NotFound;

    if (r->isSolved()) return RiddleOutcome::AlreadySolved;

    if (r->checkAnswer(answer.c_str())) {
        return RiddleOutcome::Correct;
    }
    return RiddleOutcome::Incorrect;
}

// AI genertaed func to help handle the Riddle when player steps on '?'
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


    // YES/NO question
    screen.drawAnimatedBox(bx, by, bw, bh);
    gotoxy(bx + 2, by + 2); std::cout << "You stepped on a riddle.";
    gotoxy(bx + 2, by + 4); std::cout << "Answer it? (Y/N): ";

    while (true)
    {
        char choice = _getch();
        // ESC → pause
        if (choice == ESC)
        {
            Game::pauseRequestedFromRiddle = true;
            screen.closeAnimatedBox(bx, by, bw, bh);
            player.stepBack();
            return;
        }
        if (choice == 'N' || choice == 'n')
        {
            screen.closeAnimatedBox(bx, by, bw, bh);
            player.stepBack(); // move player back to avoid re-triggering
            //bring the '?' back
            screen.setCharAt(x, y, '?');

            // draw map and player again
            if (screen.isDark())
                screen.drawMapWithTorch(player);
            else
                screen.drawMap();
            player.draw();

            return;
        }

        if (choice == 'Y' || choice == 'y')
        {
            break;
        }
    }
    screen.closeAnimatedBox(bx, by, bw, bh);

    screen.drawAnimatedBox(bx, by, bw, bh);

    gotoxy(bx + 2, by + 1);
    std::cout << "Riddle:";

    // print the question
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

    int  answerLine = line + 1;
    int answerInputLine = line + 2;
    int hintorResultLine = line + 3;
    bool answered = false;

    while (!answered)
    {
        // Prompt
        gotoxy(bx + 2, answerLine);
        std::cout << "Answer (press H for hint): ";
        gotoxy(bx + 2, answerInputLine);
        std::cout << ">>> " << std::string(40, ' ');
        std::string ans = "";

        int promptX = bx + 2;
        int cursorX = promptX + 4;
        int cursorY = answerInputLine;

        gotoxy(cursorX, cursorY);

        while (true)
        {
            char c = _getch();
            // ESC → pause the game
            if (c == ESC)
            {
                Game::pauseRequestedFromRiddle = true;
                screen.closeAnimatedBox(bx, by, bw, bh);
                player.stepBack();
                return;
            }
            if (c == '\r')   // ENTER
            {
                break; // end riddle input
            }
            else if ((c == 'H' || c == 'h') && ans.empty())
            {
                // show hint
                gotoxy(bx + 2, hintorResultLine);
                std::cout << std::string(50, ' ');
                gotoxy(bx + 2, hintorResultLine);
                std::cout << "Hint: " << r->getHint();
                gotoxy(cursorX + (int)ans.length(), cursorY);
                continue;
            }
            if (c == '\b') // BACKSPACE
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

        // CHECK ANSWER
        bool correct = r->checkAnswer(ans.c_str());
        //Clear hint line
        gotoxy(bx + 2, hintorResultLine);
        std::cout << std::string(50, ' ');

        gotoxy(bx + 2, hintorResultLine);// Write feedback

        if (correct)
        {
            std::cout << "Correct! +100 points";
            player.addPoints(100);
            r->markAsSolved();

            // remove '?'
            screen.setCharAt(x, y, ' ');
        }
        else
        {
            std::cout << "Wrong! -1 life";
            player.loseLife();
          
            screen.setCharAt(x, y, '?'); // put the riddle symbol back
        }

        Sleep(700);
        screen.closeAnimatedBox(bx, by, bw, bh);

        //redraw map and player
        player.stepBack(); // move player back to avoid re-triggering
        if (screen.isDark())
            screen.drawMapWithTorch(player);
        else
            screen.drawMap();
        player.draw();

        answered = true;
    }
}
void RiddleBank::resetAllRiddles()
{
    for (size_t i = 0; i < riddleCount; ++i)
    {
        riddles[i].resetRiddle();
    }
}
