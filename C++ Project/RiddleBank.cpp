#include "RiddleBank.h"
#include "Game.h"
#include <iostream>
#include <string>
#include <conio.h>
#include <Windows.h>
#include "Utils.h"
#include <fstream>

// Helper function to allocate and copy a string to const char*
static const char* allocateString(const std::string& str) {
    char* newStr = new char[str.length() + 1];
    strcpy_s(newStr, str.length() + 1, str.c_str());
    return newStr;
}

// Helper function to replace \n with actual newlines
static std::string processNewlines(const std::string& str) {
    std::string result;
    for (size_t i = 0; i < str.length(); ++i) {
        if (i + 1 < str.length() && str[i] == '\\' && str[i + 1] == 'n') {
            result += '\n';
            ++i; // Skip the 'n'
        }
        else {
            result += str[i];
        }
    }
    return result;
}

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
        // Read ID
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
void RiddleBank::addRiddle(const Riddle& r)
{
    if (riddleCount < MAX_RIDDLES) {
        riddles[riddleCount++] = r;
    }
}
Riddle* RiddleBank::getRiddleAt(int x, int y)
{
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
    const int W = Screen::WIDTH;
    const int H = Screen::MAP_HEIGHT;

    // for each '?' on the screen
    for (int y = 0; y < H; ++y)
    {
        for (int x = 0; x < W; ++x)
        {
            if (screen.getCharAt(x, y) != '?')
                continue;

            //check if a riddle is already assigned here
            if (getRiddleAt(x, y) != nullptr)
                continue;

            // find an unsolved riddle without a position
            for (size_t i = 0; i < riddleCount; ++i)
            {
                if (riddles[i].isSolved())
                    continue;

                Point p = riddles[i].getPosition();
                if (p.getX() != 0 || p.getY() != 0)
                    continue;


                riddles[i].setPosition(Point(x, y));
                break;
            }
        }
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
