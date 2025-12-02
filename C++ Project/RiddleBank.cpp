#include "RiddleBank.h"
#include <iostream>
#include <string>

using namespace std;

RiddleBank::RiddleBank() : riddleCount(0)
{
    // Initialize with some riddles
    riddles[riddleCount++] = Riddle(1, "What's the output?\nint x = 3;\ncout << x + ++x;", "7");
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
void RiddleBank::Hints() const
{
    cout << "Riddle Hints:" << endl;
    for (int i = 0;i < riddleCount;i++)
    {
        cout << "Riddle ID: " << riddles[i].getRiddleID() << " - Hint: Think about basic C++ concepts!" << std::endl;
    }
}