#pragma once
#include <iostream>
#include <cstring>
#include "Point.h"

class Riddle
{
private:
	int riddleID;           // riddle ID
	const char* question;   // riddle question
	const char* answer;     // riddle answer
	bool solved;            // is riddle solved
	const char* hint;       // riddle hint
	Point pos;              // position of the riddle in the screen

public:
	// Default constructor
	Riddle() : riddleID(-1), question(nullptr), answer(nullptr), solved(false), hint(nullptr), pos(0, 0) {}

	// Parameterized constructor
	Riddle(int id, const char* q, const char* a, const char* h)
		: riddleID(id), question(q), answer(a), solved(false), hint(h), pos(0, 0) {
	}

	// DESTRUCTOR - Prevents memory leak by deallocating dynamically allocated strings
	// This is required because question, answer, and hint are allocated with new[] in allocateString()
	~Riddle() {
		delete[] question;
		delete[] answer;
		delete[] hint;
	}

	// DELETED COPY OPERATIONS - Prevents shallow copying of pointers
	// Copying would cause two Riddle objects to point to the same memory, leading to double-delete and undefined behavior
	Riddle(const Riddle& other) = delete;           // Disable copy constructor
	Riddle& operator=(const Riddle& other) = delete; // Disable copy assignment operator

	// MOVE OPERATIONS - Allows safe transfer of ownership when assigning Riddle objects
	// This enables riddles[i] = Riddle(...) to work correctly without triggering the deleted copy operations

	// Move constructor - Transfers ownership of dynamically allocated members
	Riddle(Riddle&& other) noexcept
		: riddleID(other.riddleID),
		question(other.question),
		answer(other.answer),
		hint(other.hint),
		solved(other.solved),
		pos(other.pos) {
		// Set source pointers to nullptr so destructor doesn't delete them
		other.question = nullptr;
		other.answer = nullptr;
		other.hint = nullptr;
	}

	// Move assignment operator - Transfers ownership and cleans up existing resources
	Riddle& operator=(Riddle&& other) noexcept {
		if (this != &other) {
			// Deallocate old resources
			delete[] question;
			delete[] answer;
			delete[] hint;

			// Transfer ownership from other to this
			riddleID = other.riddleID;
			question = other.question;
			answer = other.answer;
			hint = other.hint;
			solved = other.solved;
			pos = other.pos;

			// Set source pointers to nullptr to prevent double-delete
			other.question = nullptr;
			other.answer = nullptr;
			other.hint = nullptr;
		}
		return *this;
	}
	bool checkAnswer(const char* userAnswer);  // check if the answer is correct
	bool isSolved() const;                      // check if the riddle is solved
	void markAsSolved();                        // mark the riddle as solved
	int getRiddleID() const;                    // get the riddle ID
	static bool isRiddle(char ch);              // check if a character represents a riddle
	std::string getQuestion() const;            // get the riddle question
	const char* getHint() const;                // get the riddle hint
	Point getPosition() const;                  // get the position of the riddle
	void setPosition(const Point& p);           // set the position of the riddle
	void resetRiddle();                         // reset the riddle to unsolved state
};