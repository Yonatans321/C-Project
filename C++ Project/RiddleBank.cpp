#include "RiddleBank.h"
#include <iostream>
#include <string>

using namespace std;

RiddleBank::RiddleBank() : riddleCount(0)
{
    // Initialize with some riddles
    riddles[riddleCount++] = Riddle(1, "What's the output?\nint x = 3;\ncout << x + ++x;", "7","Remember:++x increments BEFORE use");
    riddles[riddleCount++] = Riddle(2, "What does this print?\nfor(int i=0;i<3;i++) cout << i;", "012");
    riddles[riddleCount++] = Riddle(3, "What's the output?\nbool a = true;\nbool b = false;\ncout << (a && b);", "0");
    riddles[riddleCount++] = Riddle(4, "What is the output?\ncout << 7 / 2;", "3");
    riddles[riddleCount++] = Riddle(5, "What does this print?\nstring s = \"Hello\";\ncout << s.length();", "5");
    riddles[riddleCount++] = Riddle(6, "What's the output?\nint arr[] = {1,2,3};\ncout << arr[1];", "2");
    riddles[riddleCount++] = Riddle(7, "What's the output?\ncout << (5 > 3 ? 10 : 20);", "10");

    //fun riddlers
    riddles[riddleCount++] = Riddle(8,"what has keys but can't open locks?","Keyboard");
	riddles[riddleCount++] = Riddle(9, "what has a head and a tail but no body?", "Coin");
	riddles[riddleCount++] = Riddle(10, "who will win the world cup 2026?", "Argentina");
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
//Generated helper function to ask the riddle interactively
RiddleOutcome RiddleBank::askRiddleInteractive(int riddleID)
{
    Riddle* r = getRiddleById(riddleID);
    if (!r) return RiddleOutcome::NotFound;
    if (r->isSolved()) return RiddleOutcome::AlreadySolved;
    // Show question
    r->askQuestion();
    // Read user's full-line answer
    string userAnswer;
    cout << "\nYour answer: ";
    getline(cin, userAnswer);
    if (r->checkAnswer(userAnswer.c_str())) {
        return RiddleOutcome::Correct;
    }
    return RiddleOutcome::Incorrect;
}

RiddleOutcome RiddleBank::checkAnswerFor(int riddleID, const string& answer)
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
    cout << "Riddle Bank:" <<endl;
    for (int i = 0;i < riddleCount;i++) {
		const Riddle& r = riddles[i];
        cout << "Riddle ID: " << r.getRiddleID() << endl;
        r.askQuestion();
        cout << "Solved: " << (r.isSolved() ? "Yes" : "No") << endl;
        cout << "------------------------" << endl;
    }
}
char* RiddleBank::gethint(int riddleID) const
{
    return ri
}

void RiddleBank::handleRiddle(Player& player, Screen& screen, RoomScreenManager& ui, int level)
{
    int x = player.getX();
    int y = player.getY();
    char cell = screen.getCharAt(x, y);

    if (cell != '?')
        return;

    int id = level + 1;
    Riddle* r = getRiddleById(id);

    if (!r || r->isSolved())
        return;

    int bx = 15, by = 5, bw = 50, bh = 12;

    // שאלת Y/N
    ui.drawAnimatedBox(bx, by, bw, bh);
    gotoxy(bx + 2, by + 2); cout << "You stepped on a riddle.";
    gotoxy(bx + 2, by + 3); cout << "Answer it? (Y/N, or H for hint): ";

    char choice = _getch();

    if (choice == 'H' || choice == 'h')
    {
        ui.clearBox(bx, by, bw, bh);
        ui.drawAnimatedBox(bx, by, bw, bh);

        gotoxy(bx + 2, by + 2);
        cout << "Hint: " << r->getHint();
        Sleep(1500);
        ui.closeAnimatedBox(bx, by, bw, bh);
        return;
    }

    if (choice == 'N' || choice == 'n')
    {
        ui.closeAnimatedBox(bx, by, bw, bh);
        screen.setCharAt(x, y, '?');
        screen.draw();
        return;
    }

    ui.closeAnimatedBox(bx, by, bw, bh);

    // הצגת השאלה
    ui.drawAnimatedBox(bx, by, bw, bh);
    gotoxy(bx + 2, by + 1); cout << "Riddle:";

    string q = r->getQuestion();
    int line = by + 3;

    string temp;
    for (char c : q)
    {
        if (c == '\n')
        {
            gotoxy(bx + 2, line++);
            cout << temp;
            temp.clear();
        }
        else temp += c;
    }
    if (!temp.empty())
    {
        gotoxy(bx + 2, line++);
        cout << temp;
    }

    gotoxy(bx + 2, line + 1);
    cout << "Answer: ";

    string ans;
    getline(cin, ans);

    RiddleOutcome result = checkAnswerFor(id, ans);

    gotoxy(bx + 2, line + 3);

    if (result == RiddleOutcome::Correct)
    {
        cout << "Correct! +100 points";
        player.addPoints(100);
        r->markAsSolved();
        screen.setCharAt(x, y, ' ');
    }
    else
    {
        cout << "Wrong! -1 life";
        player.loseLife();
        screen.setCharAt(x, y, '?');
    }

    Sleep(1200);
    ui.closeAnimatedBox(bx, by, bw, bh);

    screen.draw();
}

char* RiddleBank::getHint(int riddleID) const
{
    Riddle* r = getRiddleById(riddleID);
    if (r) {
        return const_cast<char*>(r->getHint());
    }
    return nullptr;
}