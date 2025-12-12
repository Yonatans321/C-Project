#include "RiddleBank.h"
#include "Game.h" 
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
// AI generated function to attach positions to unsolved riddles based on '?' locations on the screen
void RiddleBank::attachPositionToRoom(Screen& screen)
{
    size_t nextRiddleIndex = 0;

    // get screen dimensions
    const int W = Screen::WIDTH;
    const int H = Screen::MAP_HEIGHT;

    // find the first unsolved riddle 
    for (size_t i = 0; i < riddleCount; ++i)
    {
        Point p = riddles[i].getPosition();
        if (p.getX() == 0 && p.getY() == 0 && !riddles[i].isSolved()) // not assigned yet
        {
            nextRiddleIndex = i;
            break;
        }
    }
    // scan the screen to get '?' positions
    for (int y = 0; y < H; ++y)
    {
        for (int x = 0; x < W; ++x)
        {
            char c = screen.getCharAt(x, y);

            if (c == '?')
            {
				while (nextRiddleIndex < riddleCount)// find the next unsolved riddle without position  
                {
                    Point p = riddles[nextRiddleIndex].getPosition();

                    if (p.getX() == 0 && p.getY() == 0 &&
                        !riddles[nextRiddleIndex].isSolved())
                    {
                        break;
                    }

                    nextRiddleIndex++;
                }

                if (nextRiddleIndex >= riddleCount)
                    return;

				// give this riddle the position
                riddles[nextRiddleIndex].setPosition(Point(x, y));
                nextRiddleIndex++;
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
                    gotoxy(cursorX + ans.length(), cursorY);
					continue;
                }
                if (c == '\b') // BACKSPACE
                {
                    if (!ans.empty())
                    {
						ans.pop_back();
						gotoxy(cursorX + ans.length(), cursorY);
						std::cout << ' ';
                        gotoxy(cursorX + ans.length(), cursorY);
                    }
                    continue;
				}
                if (ans.length()<40)
                {
                    ans.push_back(c);
					gotoxy(cursorX + ans.length() - 1, cursorY);
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
                if (player.getLives() <= 0)
                {
                    player.addLives(); 

					// special message for no lives left
                    std::string msg = "No lives left :( granting 1 bonus life :)";
                    gotoxy(34, Screen::MAP_HEIGHT + 1);
                    setColor(COLOR_LIGHT_RED);
                    std::cout << msg;
					Sleep(2000);
                    gotoxy(34, Screen::MAP_HEIGHT + 1);
                    resetColor();
					std::cout << std::string(msg.length(), ' ');
                }
                screen.setCharAt(x, y, '?'); // put the riddle symbol back
            }

            Sleep(700);
            screen.closeAnimatedBox(bx, by, bw, bh);

            //redraw map and player
            player.stepBack(); // move player back to avoid re-triggering
            screen.drawMap();
            player.draw();

            answered = true;
    }
}