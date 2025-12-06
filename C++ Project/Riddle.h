#pragma once
#include <iostream>
#include <cstring>
#include "Point.h"
class Riddle
{
private:
	int riddleID;
	const char* question;
	const char* answer;
	bool solved;
	const char* hint;
	Point pos;
public:
	Riddle() : riddleID(-1), question(nullptr), answer(nullptr), solved(false), hint(nullptr),pos(0,0) {}
	Riddle(int id, const char* q, const char* a,const char* h) :riddleID(id), question(q), answer(a), solved(false),hint(h),pos(0,0) {}
	// functions declaration
	bool checkAnswer(const char* userAnswer);
	bool isSolved() const;
	void markAsSolved();
	int getRiddleID() const;
	static bool isRiddle(char ch);	
	std::string getQuestion() const;
	const char* getHint() const;
	Point getPosition() const;
	void setPosition(const Point& p);
};

