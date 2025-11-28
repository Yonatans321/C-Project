#include "Riddle.h"
#include <iostream>
#include <cstring>	

void Riddle::askQuestion() const { // Display the riddle question
	std::cout << question << std::endl;
}

// Check if the provided answer is correct- if correct, mark the riddle as solved, helped by AI
bool Riddle::checkAnswer(const char* userAnswer) {
	if (strcmp(userAnswer, answer) == 0) {
		solved = true;
		return true;
	}
	return false;
}
bool Riddle::isSolved() const { // Check if the riddle has been solved
	return solved;
}

int Riddle::getRiddleID() const
{
	return riddleID;
}
bool Riddle::isRiddle(char ch) { // Check if the character represents a riddle
	return ch == '?';
}