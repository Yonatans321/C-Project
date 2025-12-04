#include "RiddleBank.h"
#include <iostream>
#include <string>
#include <conio.h>
#include <Windows.h>
#include "Utils.h"


RiddleBank::RiddleBank() : riddleCount(0)
{
    // Initialize with some riddles
    riddles[riddleCount++] = Riddle(1, "What's the output?\nint x = 3;\ncout << x + ++x;", "7","Remember:++x increments BEFORE use");
    riddles[riddleCount++] = Riddle(2, "What does this print?\nfor(int i=0;i<3;i++) cout << i;", "012", "Remember:Loop just like C");
    riddles[riddleCount++] = Riddle(3, "What's the output?\nbool a = true;\nbool b = false;\ncout << (a && b);", "0", "Remember:the outcome can be in numbers");
    riddles[riddleCount++] = Riddle(4, "What is the output?\ncout << 7 / 2;", "3", "Remember:how Divide works..?");
    riddles[riddleCount++] = Riddle(5, "What does this print?\nstring s = \"Hello\";\ncout << s.length();", "5", "Remember:just like in C");
    riddles[riddleCount++] = Riddle(6, "What's the output?\nint arr[] = {1,2,3};\ncout << arr[1];", "2", "Remember:you start counting from 0");
    riddles[riddleCount++] = Riddle(7, "What's the output?\ncout << (5 > 3 ? 10 : 20);", "10", "Remember:if/else outcome..");

    //fun riddlers
    riddles[riddleCount++] = Riddle(8,"what has keys but can't open locks?","Keyboard", "you are using it right now...");
	riddles[riddleCount++] = Riddle(9, "what has a head and a tail but no body?", "Coin", "possibility of 50% to guess it");
	riddles[riddleCount++] = Riddle(10, "who will win the world cup 2026?", "Argentina", "They've won in 2022...");
}
Riddle* RiddleBank::getRiddleById(int riddleID)
{
    for (size_t i = 0; i <riddleCount; ++i) {
        if (riddles[i].getRiddleID() == riddleID) {
            return &riddles[i];
        }
    }
	return nullptr; // Not found
}
void RiddleBank::addRiddle(const Riddle& r)
{
    if(riddleCount < MAX_RIDDLES) {
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

void RiddleBank::attachPositionToRoom(const char room[][Screen::WIDTH], int roomWidth, int roomHeight)
{
    int nextRiddleIndex = 0;

    // Find first unassigned riddle
    for (int i = 0; i < riddleCount; ++i)
    {
        Point p = riddles[i].getPosition();
        if (p.getX() == 0 && p.getY() == 0 && !riddles[i].isSolved())
        {
            nextRiddleIndex = i;
            break;
        }
    }

    for (int y = 0; y < roomHeight; ++y)
    {
        for (int x = 0; x < roomWidth; ++x)
        {
            if (room[y][x] == '?')
            {
                // Find next available riddle
                while (nextRiddleIndex < riddleCount)
                {
                    Point p = riddles[nextRiddleIndex].getPosition();

                    if (p.getX() == 0 && p.getY() == 0 &&
                        !riddles[nextRiddleIndex].isSolved())
                    {
                        break; // found one
                    }
                    nextRiddleIndex++;
                }

                if (nextRiddleIndex >= riddleCount)
                    return; // no more riddles left

                // Assign coordinates
                riddles[nextRiddleIndex].setPosition(Point(x, y));
                nextRiddleIndex++;
            }
        }
    }
}

//Generated helper function to ask the riddle interactively
RiddleOutcome RiddleBank::askRiddleInteractive(int riddleID)
{
    Riddle* r = getRiddleById(riddleID);
    if (!r) return RiddleOutcome::NotFound;
    if (r->isSolved()) return RiddleOutcome::AlreadySolved;
    // Show question
    r->askQuestion();
    // Read user's full-line answer
    std::string userAnswer;
    std::cout << "\nYour answer: ";
    std::getline(std::cin, userAnswer);
    if (r->checkAnswer(userAnswer.c_str())) {
        return RiddleOutcome::Correct;
    }
    return RiddleOutcome::Incorrect;
}

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
void RiddleBank::printAllRiddles() const
{
    std::cout << "Riddle Bank:" << std::endl;
    for (int i = 0;i < riddleCount;i++) {
		const Riddle& r = riddles[i];
        std::cout << "Riddle ID: " << r.getRiddleID() << std::endl;
        r.askQuestion();
        std::cout << "Solved: " << (r.isSolved() ? "Yes" : "No") << std::endl;
        std::cout << "------------------------" << std::endl;
    }
}
// AI genertaed func to help handle the Riddle when player steps on '?'
void RiddleBank::handleRiddle(Player& player, Screen& screen, int level)
{
    int x = player.getX();
    int y = player.getY();
    char cell = screen.getCharAt(x, y);

    // Only activate if stepped on '?'
    if (cell != '?')
        return;

    int id = level + 1;
    Riddle* r = getRiddleById(id);

    if (!r || r->isSolved())
        return;

    int bx = 15, by = 5, bw = 50, bh = 12;

    // ───────────────────────────────────────────────
    // STEP 1: Ask Y/N (with optional H for hint)
    // ───────────────────────────────────────────────

    screen.drawAnimatedBox(bx, by, bw, bh);
    gotoxy(bx + 2, by + 2); std::cout << "You stepped on a riddle.";
    gotoxy(bx + 2, by + 3); std::cout << "Answer it? (Y/N, H for hint): ";

    char choice = _getch();

    // ───────────────────────────────────────────────
    // HINT DISPLAY — shows hint in SAME place where
    // result (Correct/Wrong) will later appear.
    // ───────────────────────────────────────────────

    if (choice == 'H' || choice == 'h')
    {
        screen.closeAnimatedBox(bx, by, bw, bh);
        screen.drawAnimatedBox(bx, by, bw, bh);

        gotoxy(bx + 2, by + 1);
        std::cout << "Hint:";

        gotoxy(bx + 2, by + 3);
        std::cout << r->getHint();

        Sleep(1500);

        screen.closeAnimatedBox(bx, by, bw, bh);
        return; // does NOT continue riddle
    }

    // ───────────────────────────────────────────────
    // If player refuses to answer
    // ───────────────────────────────────────────────

    if (choice == 'N' || choice == 'n')
    {
        screen.closeAnimatedBox(bx, by, bw, bh);
        screen.setCharAt(x, y, '?');
        player.draw();
        return;
    }

    screen.closeAnimatedBox(bx, by, bw, bh);

    // ───────────────────────────────────────────────
    // STEP 2: Show the riddle question
    // ───────────────────────────────────────────────

    screen.drawAnimatedBox(bx, by, bw, bh);
    gotoxy(bx + 2, by + 1); std::cout << "Riddle:";

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

    // ───────────────────────────────────────────────
    // STEP 3: Get user answer
    // ───────────────────────────────────────────────

    gotoxy(bx + 2, line + 1);
    std::cout << "Answer: ";

    clearInputBuffer();

    std::string ans;
    std::getline(std::cin, ans);

    RiddleOutcome result = checkAnswerFor(id, ans);

    // ───────────────────────────────────────────────
    // STEP 4: Show result (IN SAME PLACE AS HINT!)
    // ───────────────────────────────────────────────

    gotoxy(bx + 2, line + 3);

    if (result == RiddleOutcome::Correct)
    {
        std::cout << "Correct! +100 points";
        player.addPoints(100);
        r->markAsSolved();
        screen.setCharAt(x, y, ' ');
    }
    else
    {
        std::cout << "Wrong! -1 life";
        player.loseLife();
        screen.setCharAt(x, y, '?');
    }

    Sleep(1200);

    screen.closeAnimatedBox(bx, by, bw, bh);

    // ───────────────────────────────────────────────
    // FINAL REDRAW
    // ─────────────────────────────────────────────── 
    screen.drawMap();
    player.draw();
}