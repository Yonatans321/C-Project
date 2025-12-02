#pragma once
#include <iostream>
#include <cstring>
class Riddle
{
private:
	int riddleID;
	const char* question;
	const char* answer;
	bool solved;
public:
	Riddle() : riddleID(-1), question(nullptr), answer(nullptr), solved(false) {}
	Riddle(int id, const char* q, const char* a) :riddleID(id), question(q), answer(a), solved(false) {}
	// functions declaration
	void askQuestion() const;
	bool checkAnswer(const char* userAnswer);
	bool isSolved() const;
	void markAsSolved();
	int getRiddleID() const;
	static bool isRiddle(char ch);	
	std::string getQuestion() const;
};

